import os
import gc
import torch
from torch.distributions.normal import Normal
from tqdm import tqdm
import neural_ode.models.utils as utils
from neural_ode.models.recurrent import RecurrentODE, RecurrentCDE
from neural_ode.models.seq2seq import Seq2Seq
from neural_ode.models.vae import VAE
import matplotlib
import matplotlib.pyplot as plt
import seaborn as sns

torch.set_default_tensor_type(torch.DoubleTensor)
# for headless use (write to files, don't show())
# matplotlib.use("Agg")

from typing import Literal, Optional, List, Tuple, Dict
import ubelt as ub
import numpy as np
import pandas as pd
from dataclasses import asdict, astuple, dataclass
from simple_parsing import ArgumentParser

import pytorch_lightning as pl
import pytorch_forecasting as pf


class UpdateKLCoef(pl.Callback):
    # for VAE, currently unused

    def __init__(self, kl_first, wait_until_kl=10):
        self.kl_first = kl_first
        self.wait_until_kl = wait_until_kl
        self.kl_coef = 0.

    def on_train_epoch_end(self, trainer: "pl.Trainer",
                           pl_module: "pl.LightningModule") -> None:
        epoch = pl_module.current_epoch
        if epoch < self.wait_until_kl:
            self.kl_coef = 0.
        else:
            self.kl_coef = self.kl_first * (1 -
                                            0.99**(epoch - self.wait_until_kl))


# https://pytorch-lightning.readthedocs.io/en/stable/common/debugging.html


class PlotCallback(pl.Callback):

    def __init__(self, every_n_batches=1, every_n_epochs=10):
        self.every_n_batches = every_n_batches
        self.every_n_epochs = every_n_epochs
        self.ready = True
        self.tr_data: List[Batch]  = []
        self.va_data: List[Batch]  = []

    def on_sanity_check_start(self, trainer, pl_module) -> None:
        self.ready = False

    def on_sanity_check_end(self, trainer, pl_module) -> None:
        self.ready = True

    @dataclass
    class Batch:  # TODO make x optional?
        y_pred: List[torch.Tensor]
        y_true: List[torch.Tensor]
        xs: List[torch.Tensor]
        yt: Optional[List[torch.Tensor]] = None
        xt: Optional[List[torch.Tensor]] = None

        def __post_init__(self):
            # stub out t if needed
            assert (self.yt is None) == (self.xt is None)
            if self.yt is None:
                # self.use_t_mins = False
                self.xt = [torch.arange(0, len(x)) for x in self.xs[0]]
                self.yt = [torch.arange(0 + len(x), len(y) + len(x))
                    for x, y in zip(self.xs[0], self.y_pred[0])]
            else:
                # self.use_t_mins = True
                pass

        @classmethod
        def from_net_out(cls, trainer, batch):
            with torch.no_grad():
                # batch = trainer.datamodule.transfer_batch_to_device(
                # batch, trainer.model.device, 0)
                x, y = batch
                y_pred, y_true, xs = (trainer.model(x)['prediction'], y[0],
                                      x['encoder_target'])

            def _detach_fn(tensor, lens=None):
                tensor = tensor.detach().cpu()
                if lens is not None:
                    tensor = [t[:l] for t, l in zip(tensor, lens)]
                return tensor

            try:
                # TODO where to put this for recurrent?
                xt, yt = (x['encoder_cont'][:, :, trainer.model.t_idx],
                          x['decoder_cont'][:, :, trainer.model.t_idx])

                return cls(
                    [_detach_fn(y, x['decoder_lengths']) for y in y_pred],
                    [_detach_fn(y, x['decoder_lengths']) for y in y_true],
                    [_detach_fn(_x, x['encoder_lengths']) for _x in xs],
                    [_detach_fn(yt, x['decoder_lengths'])],
                    [_detach_fn(xt, x['encoder_lengths'])],
                )

            except AttributeError:

                return cls(
                    [_detach_fn(y, x['decoder_lengths']) for y in y_pred],
                    [_detach_fn(y, x['decoder_lengths']) for y in y_true],
                    [_detach_fn(_x, x['encoder_lengths']) for _x in xs],
                )

    def on_train_batch_end(self, trainer, pl_module, outputs, batch,
                           batch_idx):
        if (self.ready and (batch_idx % self.every_n_batches == 0)
                and (trainer.current_epoch % self.every_n_epochs == 0)):
            self.tr_data.append(self.Batch.from_net_out(trainer, batch))

    def on_validation_batch_end(self, trainer, pl_module, outputs, batch,
                                batch_idx, dataloader_idx):
        if (self.ready and (batch_idx % self.every_n_batches == 0)
                and (trainer.current_epoch % self.every_n_epochs == 0)):
            self.va_data.append(self.Batch.from_net_out(trainer, batch))

    @staticmethod
    def make_plot(
        y_pred: List[torch.Tensor],
        y_true: List[torch.Tensor],
        yt: List[torch.Tensor],
        xt: List[torch.Tensor],
        use_t_mins: bool,  # TODO awk
        xs: Optional[torch.Tensor] = None,
    ) -> Tuple[np.ndarray, Dict[str, float]]:

        target_labels_to_facets = {
            'HeartRate(1/min)': 0,
            'BloodVolume(mL)': 1,
            'CardiacOutput(mL/min)': 2,
            'RespirationRate(1/min)': 0,
            'OxygenSaturation': 3,
            'MeanArterialPressure(mmHg)': 4,
            'SystolicArterialPressure(mmHg)': 4,
            'DiastolicArterialPressure(mmHg)': 4,
            'HemoglobinContent(g)': 5,
            'TotalHemorrhageRate(mL/s)': 3,
            'TotalHemorrhagedVolume(mL)': 1,
        }
        # add target names to title
        facets_to_target_labels = {
            v: [kk for kk, vv in target_labels_to_facets.items() if vv == v]
            for v in set(target_labels_to_facets.values())
        }
        target_labels_to_facets = {
            k: '\n'.join([str(v)] + facets_to_target_labels[v])
            for k, v in target_labels_to_facets.items()
        }

        # (target pt t 1)
        # (11 3 27 1)
        if xs is None:
            xs = [[torch.zeros((0, 1)) for y in y_pred[0]]] * len(y_pred)

        # 1 (b * n_batches) t -> t
        xt = np.concatenate(xt[0])
        yt = np.concatenate(yt[0])

        if use_t_mins:
            xt = xt / 60
            yt = yt / 60

        # TODO use group_id for this after adding data aug
        pts = np.concatenate(
            [np.repeat(i, len(y)) for i, y in enumerate(y_pred[0])])

        pred_df = pd.DataFrame(
            dict(zip(target_labels_to_facets.keys(),
                     [np.concatenate(y).squeeze() for y in y_pred]),
                 t=yt,
                 pt=pts))
        true_df = pd.DataFrame(
            dict(
                zip(target_labels_to_facets.keys(),
                    [np.concatenate(y).squeeze() for y in y_true])))

        df = pred_df.melt(id_vars=['t', 'pt'],
                          var_name='vital',
                          value_name='pred').join(
                              true_df.melt(value_name='true')['true'])

        # gets rid of start of THR/THV as well as padded timesteps
        df_error = df[df['true'] != 0]
        # https://stackoverflow.com/a/53954986
        with pd.option_context('mode.chained_assignment', None):
            df_error['rel_err'] = (df_error['pred'] -
                                   df_error['true']) / df_error['true']

        # add x timesteps and reshape long
        df['is_y'] = '1_y_predeath'
        if len(xt) > 0:
            pts_x = np.concatenate(
                [np.repeat(i, len(x)) for i, x in enumerate(xs[0])])
            df_x = pd.DataFrame(
                dict(zip(target_labels_to_facets.keys(),
                         [np.concatenate(x).squeeze() for x in xs]),
                     t=xt,
                     pt=pts_x)).melt(id_vars=['t', 'pt'], var_name='vital')
            df_x['pred'] = df_x['value']
            df_x['true'] = df_x['value']
            df_x.drop('value', axis=1, inplace=True)
            df_x['is_y'] = '0_x'
            df = pd.concat((df, df_x), ignore_index=True, copy=False)
        df = df.melt(id_vars=['t', 'pt', 'vital', 'is_y'])
        df['facet'] = df['vital'].replace(target_labels_to_facets)

        # units= for groupid (patient splits by death time etc) w/ estimator

        def to_array(grid):
            grid.fig.canvas.draw()
            buf = grid.fig.canvas.buffer_rgba()
            arr = np.array(buf)[:, :, :3]
            grid.fig.clf()
            plt.close()
            return arr

        # pred and true timeseries
        grid1 = sns.relplot(kind='line',
                            estimator=None,
                            x='t',
                            y='value',
                            row='variable',
                            col='facet',
                            hue='vital',
                            data=df,
                            alpha=0.5,
                            sort=False,
                            legend=False,
                            facet_kws=dict(sharey='col'),
                            units='pt',
                            style='is_y',
                            markers=False,  # ['o', '.', '.'],
                            style_order=['0_x', '1_y_predeath', '1_y_postdeath'],
                            linewidth=1,
                            # ms=1,
        )
        # add green and yellow points
        for ax, targets in zip(grid1.axes[1, :], facets_to_target_labels.values()):
            batch_size = len(ax.lines) // (2 * len(targets))
            for line in np.take(ax.lines, [i for i in range(len(ax.lines)) if (i // batch_size) % 2 == 0]):
                ax.plot(line.get_xdata()[0], line.get_ydata()[0], 'go', ms=2)
                ax.plot(line.get_xdata()[-1], line.get_ydata()[-1], 'yo', ms=2)
        for ax, targets in zip(grid1.axes[0, :], facets_to_target_labels.values()):
            batch_size = len(ax.lines) // (2 * len(targets))
            for line in np.take(ax.lines, [i for i in range(len(ax.lines)) if (i // batch_size) % 2 == 0]):
                ax.plot(line.get_xdata()[0], line.get_ydata()[0], 'go', ms=2)
                ax.plot(line.get_xdata()[-1], line.get_ydata()[-1], 'yo', ms=2)
        if use_t_mins:
            grid1.set_xlabels('t (min)')
        width, height = grid1.figure.get_size_inches()
        arr1 = to_array(grid1)

        # relative error
        grid2 = sns.relplot(kind='line',
                            estimator=None,
                            x='t',
                            y='rel_err',
                            row=None,
                            col='vital',
                            hue='vital',
                            data=df_error,
                            alpha=0.4,
                            facet_kws=dict(sharex=False, sharey=False),
                            sort=False,
                            units='pt')
        # add info to subtitles
        errs = []
        for ax in grid2.axes.flat:
            title = ax.get_title()
            err = np.concatenate([line.get_ydata()
                                  for line in ax.lines]).mean()
            errs.append(err)
            ax.set_title(f'{title}\nmean relative error = {err:.2f}')
        if use_t_mins:
            grid2.set_xlabels('t (min)')
        # resize for stackability
        cur_w, cur_h = grid2.figure.get_size_inches()
        grid2.figure.set_size_inches(width, (width * cur_h / cur_w) * 1.1)
        arr2 = to_array(grid2)

        return (
            np.vstack((arr1, arr2)),
            dict(zip(target_labels_to_facets.keys(), errs))
        )

    @staticmethod
    def _flatten(tups: List[Tuple]):
            # n_batches f b t 1 -> f (n_batches * b) t 1
            # this is a huge pain with ragged arrays, and unreadable...
            lsts = zip(*tups)
            return [
                [[item for items in batch for item in items]
                      for batch in zip(*lst)]
                for lst in lsts
            ]

    # @rank_zero_only
    def on_train_epoch_end(self, trainer: pl.Trainer,
                           pl_module: pl.LightningModule) -> None:
        if self.ready and self.tr_data:
            # predictions = trainer.predict(trainer.model, dataloaders=trainer.datamodule.train_dataloader())

            kwargs = dict(zip(asdict(self.tr_data[0]).keys(),
                              self._flatten(map(astuple, self.tr_data))))
            arr, errs = self.make_plot(**kwargs, use_t_mins=True)

            trainer.logger.experiment.add_image(
                'train',
                arr,
                global_step=trainer.global_step,
                dataformats='HWC')
            for k, v in errs.items():
                escaped_key = k  # .replace('/', '//')
                trainer.logger.experiment.add_scalar(
                    f'mpe/train/{escaped_key}', v, trainer.global_step)

            self.tr_data = []

    # @rank_zero_only
    def on_validation_epoch_end(self, trainer: pl.Trainer,
                                pl_module: pl.LightningModule) -> None:
        if self.ready and self.va_data:
            kwargs = dict(zip(asdict(self.va_data[0]).keys(),
                              self._flatten(map(astuple, self.va_data))))
            arr, errs = self.make_plot(**kwargs, use_t_mins=True)

            trainer.logger.experiment.add_image(
                'val', arr, global_step=trainer.global_step, dataformats='HWC')
            for k, v in errs.items():
                escaped_key = k  # .replace('/', '//')
                trainer.logger.experiment.add_scalar(f'mpe/val/{escaped_key}',
                                                     v, trainer.global_step)

            self.va_data = []


from pytorch_lightning.utilities.cli import LightningCLI
'''
could try this if problems with loading ckpts
https://github.com/Lightning-AI/lightning/issues/12302#issuecomment-1110425635
class NBCLI(LightningCLI):
    def fit(self, *args, **kwargs):
        pass
'''


class MyLightningCLI(LightningCLI):
    '''
    Currently identical to LightningCLI. Reserved for .from_dataset() shenanigans.
    '''

    # def _get(self, config: Dict[str, Any], key: str, default: Optional[Any] = None) -> Any:
    # """Utility to get a config value which might be inside a subcommand."""
    # return config.get(str(self.subcommand), config).get(key, default)

    # subclass this fn to couple model to dm and delay its instantiation until
    # after dm is setup.
    def instantiate_classes(self) -> None:
        """Instantiates the classes and sets their attributes."""
        # import xdev; xdev.embed()
        self.config_init = self.parser.instantiate_classes(self.config)
        self.datamodule = self._get(self.config_init, "data")
        self.model = self._get(self.config_init, "model")

        # self.model = self.model.from_datamodule(self.datamodule, **self.model.hparams)
        # import xdev; xdev.embed()

        self._add_configure_optimizers_method_to_model(self.subcommand)
        self.trainer = self.instantiate_trainer()

    def before_fit(self):
        pass

    def before_validate(self):
        pass

    def before_test(self):
        pass

    def before_predict(self):
        pass

    def before_tune(self):
        pass


# TODO curriculum learning https://arxiv.org/abs/2101.10382
# train on easy examples before hard examples
# with missingness, this amounted to removing missingness from 10 more points
# per batch every epoch
# this was done only for enc-dec arch, related to GoogleStock(start_reverse)?
# https://gitlab.kitware.com/physiology/engine/-/blob/study/ode/src/python/pulse/study/neural_ode/neural_ode/run_models.py#L263
def trainer_kwargs(kl_coef=1,
                   max_epochs=301,
                   test_for_epochs=1,
                   load_ckpt=None,
                   patience_for_no_better_epochs=30):
    kl_getter = UpdateKLCoef(kl_coef)
    top_va_callback = pl.callbacks.ModelCheckpoint(dirpath='checkpoints',
                                                   save_top_k=1,
                                                   monitor='val_loss')
    top_tr_callback = pl.callbacks.ModelCheckpoint(dirpath='checkpoints',
                                                   save_top_k=1,
                                                   monitor='train_loss_epoch')
    plot_callback = PlotCallback(every_n_epochs=50)
    early_stopping_callback = pl.callbacks.EarlyStopping(
                monitor='train_loss_epoch',
                # monitor='tr_mse',
                # monitor='va_mse',
                mode='min',
                patience=patience_for_no_better_epochs)
    lr_monitor_callback = pl.callbacks.LearningRateMonitor()
    logger = pl.loggers.TensorBoardLogger(
        save_dir='lightning_logs/',
        name=None,  # can set this in cli
        default_hp_metric=False,
    )
    return dict(
        max_epochs=max_epochs,
        check_val_every_n_epoch=test_for_epochs,
        resume_from_checkpoint=load_ckpt,
        callbacks=[
            kl_getter,
            plot_callback,
            early_stopping_callback,
            lr_monitor_callback,
            # top_va_callback,
            # top_tr_callback,
        ],
        gradient_clip_val=0.01,
        logger=logger,
        gpus=1)


if __name__ == "__main__":

    torch.set_default_dtype(torch.float32)
    '''
    alternate invocation:
        model_class=RecurrentODE.from_dataset or
        --model RecurrentODE.from_dataset
    combine with https://jsonargparse.readthedocs.io/en/stable/index.html#argument-linking for model.dset == dm.dset_tr
    https://github.com/omni-us/jsonargparse/issues/146
    '''

    # for mild dset
    # --data.root_path=/data/pulse/hemorrhage/patient_variability/hemorrhage/test

    # model = pf.DeepAR.from_dataset(dm.dset_tr, learning_rate=1e-4)

    cli = MyLightningCLI(
        model_class=utils.PFMixin,
        datamodule_class=utils.BaseData,
        trainer_defaults=trainer_kwargs(),
        seed_everything_default=47,
        run=False,
        subclass_mode_model=True,
        subclass_mode_data=True,
        auto_registry=False)

    print('reloading model')
    cli.model = cli.model.from_datamodule(cli.datamodule,
                                          **cli.model.hparams)
    if cli.trainer.overfit_batches:
        cli.datamodule.shuffle = False   # HACK
        # TODO change lr scheduler from val_loss to train_loss_epoch here

    # batch = next(iter(cli.datamodule.train_dataloader()))
    # x, y = batch
    # y_pred = cli.model(x)

    # DUMP HPARAMS
    # from pytorch_lightning.core.saving import save_hparams_to_yaml
    # save_hparams_to_yaml('cli_model.yaml', cli.model.hparams)
    # save_hparams_to_yaml('cli_dm.yaml', cli.datamodule.hparams)

    # # fit == train + validate
    # import xdev
    # with xdev.embed_on_exception_context():
    cli.trainer.fit(cli.model, datamodule=cli.datamodule)

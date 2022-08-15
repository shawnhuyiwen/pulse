import os
import gc
from random import SystemRandom
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
from dataclasses import asdict
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
    plot_callback = PlotCallback()
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
            pl.callbacks.EarlyStopping(
                monitor='train_loss_epoch',
                # monitor='tr_mse',
                # monitor='va_mse',
                mode='min',
                patience=patience_for_no_better_epochs),
            kl_getter,
            plot_callback,
            # top_va_callback,
            # top_tr_callback,
        ],
        gradient_clip_val=0.01,
        logger=logger,
        gpus=1)


# https://pytorch-lightning.readthedocs.io/en/stable/common/debugging.html


class PlotCallback(pl.Callback):

    def __init__(self, every_n_batches=1, every_n_epochs=50):
        self.every_n_batches = every_n_batches
        self.every_n_epochs = every_n_epochs
        self.ready = True

    def on_sanity_check_start(self, trainer, pl_module) -> None:
        self.ready = False

    def on_sanity_check_end(self, trainer, pl_module) -> None:
        self.ready = True

    @staticmethod
    def make_plot(
        y_pred: List[torch.Tensor],
        y_true: List[torch.Tensor],
        yt: Optional[torch.Tensor] = None,
        xs: Optional[torch.Tensor] = None,
        xt: Optional[torch.Tensor] = None
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
            xs = [[torch.zeros((0, 1)) for y in y_pred[0]]] * 11

        if xt is None:
            xt = [torch.arange(0, len(x)) for x in xs[0]]
        xt = np.concatenate(xt)

        if yt is None:
            yt = [
                torch.arange(0 + len(x),
                             len(y) + len(x))
                for x, y in zip(xs[0], y_pred[0])
            ]
        yt = np.concatenate(yt)

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
        df['is_y'] = True
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
            df_x['is_y'] = False
            df = pd.concat((df, df_x), ignore_index=True, copy=False)
        df = df.melt(id_vars=['t', 'pt', 'vital', 'is_y'])
        df['facet'] = df['vital'].replace(target_labels_to_facets)

        # have to put each facetgrid in a separate figure
        # https://stackoverflow.com/a/44159496
        # fig, axs = plt.subplots(2, 1, figsize=(9, 5), gridspec_kw=dict(height_ratios=[2, 1]))
        # units= for groupid (patient splits by death time etc) w/ estimator

        def to_array(grid):
            grid.fig.canvas.draw()
            buf = grid.fig.canvas.buffer_rgba()
            arr = np.array(buf)[:, :, :3]
            grid.fig.clf()
            plt.close()
            return arr

        grid1 = sns.relplot(kind='line',
                            estimator=None,
                            x='t',
                            y='value',
                            row='variable',
                            col='facet',
                            hue='vital',
                            data=df,
                            alpha=0.4,
                            sort=False,
                            legend=False,
                            facet_kws=dict(sharey='col'),
                            units='pt',
                            style='is_y',
                            linewidth=1)
        width, height = grid1.figure.get_size_inches()
        arr1 = to_array(grid1)

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
        # resize for stackability
        cur_w, cur_h = grid2.figure.get_size_inches()
        grid2.figure.set_size_inches(width, width * cur_h / cur_w)
        arr2 = to_array(grid2)

        return np.vstack(
            (arr1, arr2)), dict(zip(target_labels_to_facets.keys(), errs))

    def make_log(self, trainer, dataloader, prefix_str):

        with torch.no_grad():
            _b = trainer.datamodule.batch_size
            trainer.datamodule.batch_size = 10 * _b
            n_targets = len(trainer.datamodule.continue_params)

            # this is a huge pain with ragged arrays...
            # TODO strip right-padding using decoder_target, {de|en}coder_lengths
            # TODO handle t

            def _process(batch):
                batch = trainer.datamodule.transfer_batch_to_device(
                    batch, trainer.model.device, 0)
                x, y = batch
                y_pred, y_true, xs = (trainer.model(x)['prediction'], y[0],
                                      x['encoder_target'])
                detach_fn = lambda t: t.detach().cpu()
                return (
                    list(map(detach_fn, y_pred)),
                    list(map(detach_fn, y_true)),
                    list(map(detach_fn, xs)),
                )

            # n_batches f b t 1
            filtered_dl = (batch for idx, batch in enumerate(dataloader)
                           if idx % self.every_n_batches == 0)
            y_pred, y_true, xs = zip(*map(_process, filtered_dl))

        # f (n_batches * b) t 1
        y_pred = [[item for items in batch for item in items]
                  for batch in zip(*y_pred)]
        y_true = [[item for items in batch for item in items]
                  for batch in zip(*y_true)]
        xs = [[item for items in batch for item in items]
              for batch in zip(*xs)]
        arr, errs = self.make_plot(y_pred, y_true, xs=xs)

        # predictions = trainer.predict(trainer.model, dataloaders=trainer.datamodule.train_dataloader())

        trainer.logger.experiment.add_image(prefix_str,
                                            arr,
                                            global_step=trainer.global_step,
                                            dataformats='HWC')
        for k, v in errs.items():
            escaped_key = k  # .replace('/', '//')
            trainer.logger.experiment.add_scalar(
                f'mpe/{prefix_str}/{escaped_key}', v, trainer.global_step)

    # @rank_zero_only
    def on_train_epoch_end(self, trainer: pl.Trainer,
                           pl_module: pl.LightningModule) -> None:
        if self.ready and trainer.current_epoch % self.every_n_epochs == 0:
            self.make_log(trainer, trainer.datamodule.train_dataloader(),
                          'train')
                          # f'train/{pl_module.__class__.__name__}')

    # @rank_zero_only
    def on_validation_epoch_end(self, trainer: pl.Trainer,
                                pl_module: pl.LightningModule) -> None:
        if self.ready and trainer.current_epoch % self.every_n_epochs == 0:
            self.make_log(trainer, trainer.datamodule.val_dataloader(),
                          'val')
                          # f'val/{pl_module.__class__.__name__}')


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


if __name__ == "__main__":

    torch.set_default_dtype(torch.float32)
    if 0:

        from neural_ode.models.recurrent import RecurrentODEParams
        from neural_ode.models.diff_func import ODEFuncParams
        from neural_ode.models.utils import BaseModel
        dm = utils.HemorrhageVitals(
            stride=100,
            max_pts=200,
            batch_size=2,
        )
        trainer = pl.Trainer(**trainer_kwargs(),
                             overfit_batches=False,
                             log_every_n_steps=1,
                             gpus=1)

        dm.prepare_data()
        dm.setup()
        # model = RecurrentODE.from_datamodule(dm, recurrentodeparams=RecurrentODEParams(), odefuncparams=ODEFuncParams(), learning_rate=1e-4)
        loss = pf.MultiLoss([pf.MAE()] * 11,
                            weights=BaseModel.target_weights(dm.dset_tr))

        model = pf.RecurrentNetwork.from_dataset(dm.dset_tr,
                                                 hidden_size=100,
                                                 learning_rate=1e-3,
                                                 loss=loss)
        # model = pf.DeepAR.from_dataset(dm.dset_tr, learning_rate=1e-4)

        batch = next(iter(dm.train_dataloader()))
        x, y = batch
        y_pred = model(x)
        # import xdev; xdev.embed()

        trainer.fit(model, datamodule=dm)

    else:
        '''
        alternate invocation:
            model_class=RecurrentODE.from_dataset or
            --model RecurrentODE.from_dataset
        combine with https://jsonargparse.readthedocs.io/en/stable/index.html#argument-linking for model.dset == dm.dset_tr
        https://github.com/omni-us/jsonargparse/issues/146
        '''

        cli = MyLightningCLI(
            model_class=utils.StubBaseModel,
            # cli = LightningCLI(model_class=utils.BaseModel,
            datamodule_class=utils.BaseData,
            trainer_defaults=trainer_kwargs(),
            seed_everything_default=47,
            # run=True,
            run=False,
            subclass_mode_model=True,
            subclass_mode_data=True,
            auto_registry=False)
        cli.model = cli.model.from_datamodule(cli.datamodule,
                                              **cli.model.hparams)
        # # fit == train + validate
        # import xdev
        # with xdev.embed_on_exception_context():
        cli.trainer.fit(cli.model, datamodule=cli.datamodule)

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

torch.set_default_tensor_type(torch.DoubleTensor)
# for headless use (write to files, don't show())
# matplotlib.use("Agg")

from typing import Literal, Optional
import ubelt as ub
import numpy as np
from dataclasses import asdict
from simple_parsing import ArgumentParser

import pytorch_lightning as pl


def add_model_args_to_parser(parser: ArgumentParser) -> ArgumentParser:
    from neural_ode.models.diff_func import ODEFuncParams, CDEFuncParams
    from neural_ode.models.recurrent import RecurrentODEParams, RecurrentCDEParams
    from neural_ode.models.seq2seq import Seq2SeqParams

    tmp_args, _ = parser.parse_known_args()

    if tmp_args.using == 'ODE':
        SolverParams = ODEFuncParams
    elif tmp_args.using == 'CDE':
        SolverParams = CDEFuncParams
    else:
        raise NotImplementedError

    if tmp_args.arch == 'Recurrent':
        parser.add_arguments(SolverParams, dest='solver')
        if tmp_args.using == 'ODE':
            parser.add_arguments(RecurrentODEParams, dest='recurrent')
        else:
            parser.add_arguments(RecurrentCDEParams, dest='recurrent')
    elif tmp_args.arch == 'Seq2Seq':
        parser.add_arguments(SolverParams, dest='enc_solver')
        parser.add_arguments(SolverParams, dest='dec_solver')
        parser.add_arguments(Seq2SeqParams, dest='seq2seq')
    elif tmp_args.arch == 'VAE':
        if tmp_args.using == 'ODE':
            parser.add_arguments(SolverParams(h_dims=20, method='euler'),
                                 dest='enc_solver')
        else:
            parser.add_arguments(SolverParams(h_dims=20), dest='enc_solver')
        parser.add_arguments(SolverParams, dest='dec_solver')
        parser.add_arguments(VAEParams, dest='VAE')
    else:
        raise NotImplementedError

    return parser


def get_model(args, x_dims, y_dims):

    if args.using == 'ODE':
        de_func = ODEFunc(x_dims=x_dims, **asdict(args.ode))
    elif args.using == 'CDE':
        de_func = CDEFunc(x_dims=x_dims, **asdict(args.cde))

    if args.arch == 'Recurrent':
        if args.using == 'ODE':
            model = RecurrentODE(x_dims=x_dims,
                                 y_dims=y_dims,
                                 **asdict(args.recurrent))
        elif args.using == 'CDE':
            model = RecurrentCDE(x_dims=x_dims,
                                 y_dims=y_dims,
                                 **asdict(args.recurrent))

    elif args.arch == 'Seq2Seq':
        model = Seq2Seq(x_dims=x_dims, y_dims=y_dims, **asdict(args.seq2seq))

    elif args.arch == 'VAE':
        # uses two DEs with different settings
        pass

    else:
        raise NotImplementedError

    return model


def test_and_plot(model,
                  test_dataloader,
                  fig_saveat,
                  produce_intercoeffs: bool,
                  arch: Literal['Recurrent', 'Seq2Seq', 'VAE'],
                  save_fig_per_test: int = 0,
                  y_data_color='gray',
                  x_data_color='dodgerblue',
                  y_pred_color='orange'):
    model.eval()
    with torch.no_grad():
        #TRANS: visualization
        if not produce_intercoeffs and fig_saveat is not None:

            def to_np(x):
                return x.detach().cpu().numpy() if x.is_cuda else x.detach(
                ).numpy()

            batch = next(iter(test_dataloader))
            batch_dict = utils.time_sync_batch_dict(batch, ode_time=ode_time)
            if arch == "Recurrent" or arch == "Seq2Seq":
                y_pred = model(batch_dict["y_time"], batch_dict["x_data"],
                               batch_dict["x_time"], batch_dict["x_mask"])
            else:
                _, info = model(batch_dict["y_time"], batch_dict["x_data"],
                                batch_dict["x_time"], batch_dict["x_mask"])
                y_pred = info["y_pred_mean"]
            # y_pred shape: [batch_size, time_points, data_dims]
            #TRANS: TODO: one-dimensional, regression, and cannot be used for interpolation
            for k in range(min(save_fig_per_test, len(batch))):
                plt.clf()
                plt.plot(to_np(batch_dict["y_time"]),
                         to_np(batch_dict["y_data"][k, :, 0]),
                         color=y_data_color,
                         linestyle="--")
                plt.scatter(to_np(batch_dict["x_time"]),
                            to_np(batch_dict["x_data"][k, :, 0]),
                            color=x_data_color,
                            marker="s")
                plt.plot(to_np(batch_dict["y_time"]),
                         to_np(y_pred[k, :, 0]),
                         color=y_pred_color)
                plt.savefig(fig_saveat + "/" + str(epoch) + "_" + str(k) +
                            ".jpg")

        #TRANS: Save the output fields
        output_str = "Test at Epoch: %4d | Loss: %f | MSE: %f" % (
            epoch, test_res["loss"].item(), test_res["mse"].item())
        logger.info(output_str)
        logger.info(res_dict)

    return output_str


class UpdateKLCoef(pl.Callback):

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
            self.kl_coef = self.kl_first * (1 - 0.99**(epoch - self.wait_until_kl))


def train_one_epoch(model):
    #TRANS: Prepared using BatchNorm1d (for time series, temporarily don't need)
    model.train()

    #TRANS: For each batch
    for train_batch_list in train_dataloader:
        optimizer.zero_grad()
        batch_dict = utils.time_sync_batch_dict(train_batch_list,
                                                ode_time=ode_time)
        train_res = model.compute_loss_one_batch(batch_dict, kl_coef)
        #TRANS: Back propagation
        train_res["loss"].backward()

    #TRANS: Update the superparametric
    if val_dataloader is None:  #TRANS: Updated using the training set
        scheduler.step(train_res["loss"].item())
    else:  #TRANS: Using a validation set to update
        val_res = utils.compute_loss_all_batches(model,
                                                 val_dataloader,
                                                 kl_coef,
                                                 ode_time=ode_time)


parser = ArgumentParser()

# main
parser.add_argument("--test_for_epochs", type=int, default=1)
parser.add_argument("--max_epochs", type=int, default=300)
parser.add_argument("--kl_coef", type=float, default=1)
parser.add_argument("--patience_for_no_better_epochs", type=int, default=30)
parser.add_argument("--load_ckpt", default=None, type=Optional[str])
# parser.add_argument("--lr", type=float, default=1e-2)

import pytorch_forecasting as pf
pf.models.BaseModel
pf.models.baseline.Baseline
# TODO curriculum learning https://arxiv.org/abs/2101.10382
# train on easy examples before hard examples
# with missingness, this amounted to removing missingness from 10 more points
# per batch every epoch 
# this was done only for enc-dec arch, related to GoogleStock(start_reverse)?
# https://gitlab.kitware.com/physiology/engine/-/blob/study/ode/src/python/pulse/study/neural_ode/neural_ode/run_models.py#L263
def trainer_kwargs(kl_coef=1, max_epochs=300, test_for_epochs=1, load_ckpt=None,
                   patience_for_no_better_epochs=30):
    kl_getter = UpdateKLCoef(kl_coef)
    top_va_callback = pl.callbacks.ModelCheckpoint(dirpath='checkpoints',
                                                   save_top_k=1,
                                                   monitor='va_loss')
    top_tr_callback = pl.callbacks.ModelCheckpoint(dirpath='checkpoints',
                                                   save_top_k=1,
                                                   monitor='tr_loss')
    return dict(max_epochs=max_epochs,
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
                    # top_va_callback,
                    # top_tr_callback,
                ],
               gradient_clip_val=0.01,)


# model
parser.add_argument('--arch',
                    choices=['Recurrent', 'Seq2Seq', 'VAE'],
                    default='Recurrent')
parser.add_argument('--using', choices=['ODE', 'CDE'], default='ODE')


# https://pytorch-lightning.readthedocs.io/en/stable/common/debugging.html
def main(args):

    pl.seed_everything(47)
    dm = args.datamodule
    model = get_model(x_dims=dm.x_dims, y_dims=dm.y_dims)
    kl_getter = UpdateKLCoef(args.kl_coef)
    top_va_callback = pl.callbacks.ModelCheckpoint(dirpath='checkpoints',
                                                   save_top_k=1,
                                                   monitor='va loss')
    top_tr_callback = pl.callbacks.ModelCheckpoint(dirpath='checkpoints',
                                                   save_top_k=1,
                                                   monitor='tr loss')
    trainer = pl.Trainer(max_epochs=args.max_epochs,
                         check_val_every_n_epoch=test_for_epochs,
                         resume_from_checkpoint=args.load_ckpt,
                         callbacks=[
                             pl.callbacks.EarlyStopping(
                                 monitor='va mse',
                                 mode='min',
                                 patience=args.patience_for_no_better_epochs),
                             kl_getter,
                             top_va_callback,
                             top_tr_callback,
                         ])

    #TRANS: Start training
    #TRANS: Sampling part of the point, progress_training forecast more gradually
    # TODO masking
    # train_dict, val_dict, test_dict, x_dims, y_dims = dsetter.get_data(
    # arch=cf.get('arch', get_model))
    # train_dataloader = dsetter.masked_dataloader(train_dict,
    # 'train',
    # pre_points=pre_points)
    # val_dataloader = dsetter.masked_dataloader(val_dict,
    # 'val',
    # missing_rate=None)
    # test_dataloader = dsetter.masked_dataloader(test_dict,
    # 'test',
    # missing_rate=None)
    if 1:
        trainer.fit(model, datamodule=dm)
    else:
        train_dataloader = dm.train_dataloader()
        val_dataloader = dm.val_dataloader()
        test_dataloader = dm.test_dataloader()

        stop_training = False
        for epoch in range(1, max_epochs + 1):

            train_one_epoch(model, kl_coef=kl_getter.kl_coef)

            #TRANS: The test set
            if epoch % test_for_epochs == 0 or epoch == max_epochs or stop_training:
                output_str = test_and_plot(model,
                                           test_dataloader,
                                           kl_coef,
                                           fig_saveat,
                                           arch=args.arch)


from pytorch_lightning.utilities.cli import LightningCLI
class MyLightningCLI(LightningCLI):

    # subclass this fn to couple model to dm and delay its instantiation until
    # after dm is setup.
    def instantiate_classes(self) -> None:
        """Instantiates the classes and sets their attributes."""
        # import xdev; xdev.embed()
        self.config_init = self.parser.instantiate_classes(self.config)
        self.datamodule = self._get(self.config_init, "data")
        self.model = self._get(self.config_init, "model")
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
    if 1:

        from neural_ode.models.recurrent import RecurrentODEParams
        from neural_ode.models.diff_func import ODEFuncParams
        dm = utils.HemorrhageVitals(stride=1000, max_pts=10, batch_size=1)
        model = RecurrentODE.from_datamodule(dm, recurrentodeparams=RecurrentODEParams(), odefuncparams=ODEFuncParams(), learning_rate=1e-4)
        trainer = pl.Trainer(**trainer_kwargs(), overfit_batches=1)

        # dm.prepare_data()
        # dm.setup()
        # batch = next(iter(dm.train_dataloader()))
        # x,y = batch
        # y_pred = model(x)
        # import xdev; xdev.embed()

        trainer.fit(model, datamodule=dm)

    elif 0:


        # cli = MyLightningCLI(model_class=utils.BaseModel,
        cli = LightningCLI(model_class=utils.BaseModel,
                           datamodule_class=utils.BaseData,
                           trainer_defaults=trainer_kwargs(),
                           seed_everything_default=47,
                           run=True,
                           # run=False,
                           subclass_mode_model=True,
                           subclass_mode_data=True,
                           auto_registry=False)
        # # cli.model.init_from_datamodule(cli.datamodule)
        # cli.model = cli.model.from_datamodule(cli.datamodule)
        # # fit == train + validate
        # cli.trainer.fit(cli.model, datamodule=cli.datamodule)

    else:

        parser = add_model_args_to_parser(parser)

        # dataset logic
        tmp_args, _ = parser.parse_known_args()
        if tmp_args.dset == 'google_stock':
            parser.add_arguments(utils.GoogleStock, dest='datamodule')
        elif tmp_args.dset == 'default':
            parser.add_arguments(utils.ProcessDataset2, dest='datamodule')

        args = parser.parse_args()
        main(args)

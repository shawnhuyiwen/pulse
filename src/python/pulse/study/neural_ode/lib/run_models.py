import os
import gc
from random import SystemRandom
import torch
from torch.distributions.normal import Normal
from tqdm import tqdm
import models.utils as utils
from models.recurrent import Recurrent
from models.seq2seq import Seq2Seq
from models.vae import VAE
from models.diff_func import ODEFunc, CDEFunc
from models.diffeq_solver import DiffeqSolver
import matplotlib
import matplotlib.pyplot as plt

torch.set_default_tensor_type(torch.DoubleTensor)
matplotlib.use("Agg")

from typing import Literal, Optional
import config as cf
import ubelt as ub
import numpy as np


def get_model(arch: Literal['Recurrent', 'Seq2Seq',
                            'VAE'], n_gru_units: int, n_out_units: int,
              gaussian_likelihood_std: float, x_dims: int, y_dims: int):
    common = dict(x_dims=x_dims,
                  y_dims=y_dims,
                  n_gru_units=n_gru_units,
                  n_out_units=n_out_units,
                  gaussian_likelihood_std=gaussian_likelihood_std)
    diffeq_solver = get_diffeq_solver(x_dims, cf.get())
    if arch == "Recurrent":
        model = Recurrent(diffeq_solver=diffeq_solver, **common)
    elif arch == "Seq2Seq":
        model = Seq2Seq(enc_diffeq_solver=diffeq_solver,
                        dec_diffeq_solver=diffeq_solver,
                        **common)
    elif arch == "VAE":
        kwargs = cf.get(None, get_diffeq_solver)
        kwargs['method'] = 'euler'
        enc_diffeq_solver = get_diffeq_solver(x_dims, h_dims=20, **kwargs)
        model = VAE(h_prior=Normal(torch.Tensor([0.0]), torch.Tensor([1.])),
                    enc_diffeq_solver=enc_diffeq_solver,
                    dec_diffeq_solver=diffeq_solver,
                    **common)
    else:
        raise NotImplementedError

    return model


def get_diffeq_solver(using: Literal['ODE_RNN', 'CDE'],
                      x_dims,
                      h_dims: Optional[int] = None,
                      h_trans_layers: int = 1):
    h_dims = h_dims if h_dims is not None else x_dims * 2
    if using == "ODE_RNN":
        return DiffeqSolver(ODEFunc(h_dims, h_trans_layers=h_trans_layers),
                            cf.get())
    elif using == "CDE":
        return DiffeqSolver(
            CDEFunc(x_dims,
                    h_dims,
                    h_trans_dims=h_dims,
                    h_trans_layers=h_trans_layers), cf.get())
    else:
        raise NotImplementedError


def prepare_to_train(dsetter: utils.ProcessDataset,
                     dataset: str,
                     load_ckpt: Optional[ub.Path],
                     device: Optional[str],
                     lr: float,
                     progress_train: bool = False):
    if device is None:
        device = torch.device(
            "cpu") if not torch.cuda.is_available() else torch.device("cuda:0")
    #TRANS: The data set
    train_dict, val_dict, test_dict, x_dims, y_dims = dsetter.get_data(
        dataset=cf.get())
    #TRANS: State is read
    if load_ckpt:
        model, optimizer, scheduler, pre_points, experimentID = utils.load_checkpoint(
            load_ckpt)
        model.to(device)
    else:
        #TRANS: The value of the model, the optimizer, LR changes
        model = get_model(cf.get(), x_dims=x_dims, y_dims=y_dims).to(device)
        optimizer = torch.optim.Adamax(model.parameters(), lr=lr)
        scheduler = torch.optim.lr_scheduler.ReduceLROnPlateau(optimizer,
                                                               patience=10)
        if progress_train:  # TODO: curriculum learning for encoder-decoder
            pre_points = 10
        else:
            pre_points = None
        experimentID = str(int(SystemRandom().random() * 1000000))
    return model, optimizer, scheduler, pre_points, experimentID, train_dict, val_dict, test_dict


def test_and_plot(model,
                  test_dataloader,
                  kl_coef,
                  experimentID,
                  res_files,
                  fig_saveat,
                  logger,
                  epoch,
                  produce_intercoeffs: bool,
                  arch: Literal['Recurrent', 'Seq2Seq', 'VAE'],
                  save_fig_per_test: int = 0,
                  y_data_color='gray',
                  x_data_color='dodgerblue',
                  y_pred_color='orange'):
    model.eval()
    with torch.no_grad():
        #TRANS: Save the best results
        test_res, res_dict = utils.store_best_results(model, test_dataloader,
                                                      kl_coef, experimentID,
                                                      res_files, cf.get())
        #TRANS: visualization
        if not produce_intercoeffs and fig_saveat is not None:

            def to_np(x):
                return x.detach().cpu().numpy() if x.is_cuda else x.detach(
                ).numpy()  #TRANS: equipment

            batch_list = next(iter(test_dataloader))  #TRANS: Draw the largest number of not more than batch_size
            batch_dict = utils.time_sync_batch_dict(batch_list,
                                                    ode_time=cf.get())
            if arch == "Recurrent" or arch == "Seq2Seq":
                y_pred = model(batch_dict["y_time"], batch_dict["x_data"],
                               batch_dict["x_time"], batch_dict["x_mask"])
            else:
                _, info = model(batch_dict["y_time"], batch_dict["x_data"],
                                batch_dict["x_time"], batch_dict["x_mask"])
                y_pred = info["y_pred_mean"]
            # y_pred shape: [batch_size, time_points, data_dims]
            #TRANS: TODO: one-dimensional, regression, and cannot be used for interpolation
            for k in range(save_fig_per_test):
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


def main(test_for_epochs: int,
         max_epochs: int,
         kl_coef: float,
         patience_for_no_better_epochs: int,
         save_res_for_epochs=np.inf,
         device=None):
    dset = utils.ProcessDataset(cf.get())
    (model, optimizer, scheduler, pre_points, experimentID, train_dict,
     val_dict, test_dict) = prepare_to_train(cf.get())
    (logger, res_files, train_res_csv, val_res_csv, fig_saveat,
     ckpt_saveat) = utils.get_logger_and_save(
         model,
         experimentID,
         cf.get(),
         save_res_for_epochs=save_res_for_epochs)

    if device is None:
        device = torch.device(
            "cpu") if not torch.cuda.is_available() else torch.device("cuda:0")
    #TRANS: Start training
    #TRANS: Sampling part of the point, progress_training forecast more gradually
    train_dataloader = dset.masked_dataloader(train_dict,
                                              'train',
                                              pre_points=pre_points)
    val_dataloader = dset.masked_dataloader(val_dict, 'val', missing_rate=None)
    test_dataloader = dset.masked_dataloader(test_dict,
                                             'test',
                                             missing_rate=None)

    test_for_epochs = max(1, test_for_epochs)
    pbar = tqdm(total=test_for_epochs)

    #TRANS: Used to hold the best model and decide whether to quit the training
    best_metric = torch.tensor([torch.inf], device=device)
    best_metric_epoch = 0
    stop_training = False

    for epoch in range(1, max_epochs + 1):
        if stop_training:
            break
        model.train()  #TRANS: Prepared using BatchNorm1d (for time series, temporarily don't need)
        kl_coef = utils.update_kl_coef(kl_coef, epoch)
        pbar.set_description("Epoch [%4d / %4d]" % (epoch, max_epochs))

        #TRANS: For each batch
        for train_batch_list in train_dataloader:
            optimizer.zero_grad()
            batch_dict = utils.time_sync_batch_dict(train_batch_list,
                                                    ode_time=cf.get(
                                                        'ode_time',
                                                        utils.ProcessDataset))
            train_res = model.compute_loss_one_batch(batch_dict, kl_coef)
            gc.collect()
            #TRANS: Back propagation
            train_res["loss"].backward()
            optimizer.step()
            gc.collect()
            pbar.set_postfix(Loss=train_res["loss"].item(),
                             MSE=train_res["mse"].item())

        #TRANS: To reach an epoch
        pbar.update(1)
        if train_res_csv is not None and epoch % save_res_for_epochs == 0:
            train_res_csv.write(train_res, epoch)
        #TRANS: Update the superparametric
        if val_dataloader is None:  #TRANS: Updated using the training set
            scheduler.step(train_res["loss"].item())
            if train_res["mse"] * 1.0001 < best_metric:
                best_metric = train_res["mse"]
                best_metric_epoch = epoch
                #TRANS: checkpoint
                checkpoint = (model, optimizer, scheduler, pre_points,
                              experimentID)
                utils.save_checkpoint(ckpt_saveat,
                                      checkpoint,
                                      name="best_for_train")
        else:  #TRANS: Using a validation set to update
            val_res = utils.compute_loss_all_batches(model,
                                                     val_dataloader,
                                                     kl_coef,
                                                     ode_time=cf.get(
                                                         'ode_time',
                                                         utils.ProcessDataset))
            scheduler.step(val_res["loss"].item())
            if val_res_csv is not None and epoch % save_res_for_epochs == 0:
                val_res_csv.write(val_res, epoch)
            if val_res["mse"] * 1.0001 < best_metric:
                best_metric = val_res["mse"]
                best_metric_epoch = epoch
                #TRANS: checkpoint
                checkpoint = (model, optimizer, scheduler, pre_points,
                              experimentID)
                utils.save_checkpoint(ckpt_saveat,
                                      checkpoint,
                                      name="best_for_val")

        # curriculum learning
        if pre_points is not None:
            pre_points += 10
            train_dataloader = utils.masked_dataloader(train_dict, pre_points,
                                                       "train", cf.get())
        #TRANS: Beyond the corresponding epoch, stop training
        if patience_for_no_better_epochs is not None:
            if epoch > best_metric_epoch + patience_for_no_better_epochs:
                pbar.close()
                tqdm.write(
                    "No better metrics than %f for %d epochs. Stop training." %
                    (best_metric.item(), patience_for_no_better_epochs))
                output_str = test_and_plot(model, test_dataloader, kl_coef,
                                           experimentID, res_files, fig_saveat,
                                           logger, epoch, cf.get())  #TRANS: At this time for a test
                tqdm.write(output_str)
                stop_training = True

        #TRANS: The test set
        if epoch % test_for_epochs == 0 or epoch == max_epochs:
            output_str = test_and_plot(model, test_dataloader, kl_coef,
                                       experimentID, res_files, fig_saveat,
                                       logger, epoch, cf.get())
            pbar.close()
            if epoch != max_epochs:  #TRANS: Open new progress bar
                pbar = tqdm(total=test_for_epochs)
            tqdm.write(output_str)  #TRANS: Show the last test results


if __name__ == "__main__":
    main()

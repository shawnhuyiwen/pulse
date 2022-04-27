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
    # 数据集
    train_dict, val_dict, test_dict, x_dims, y_dims = dsetter.get_data(
        dataset=cf.get())
    # 状态读取
    if load_ckpt:
        model, optimizer, scheduler, pre_points, experimentID = utils.load_checkpoint(
            load_ckpt)
        model.to(device)
    else:
        # 模型、优化器、LR的值改动
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
        # 保存最佳结果
        test_res, res_dict = utils.store_best_results(model, test_dataloader,
                                                      kl_coef, experimentID,
                                                      res_files, cf.get())
        # 可视化
        if not produce_intercoeffs and fig_saveat is not None:

            def to_np(x):
                return x.detach().cpu().numpy() if x.is_cuda else x.detach(
                ).numpy()  # 设备

            batch_list = next(iter(test_dataloader))  # 最大画图数量不超过batch_size
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
            # TODO: 一维，自回归情况，且不能用于插值
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

        # 保存输出字段
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
    # 开始训练
    # 采样部分的点，progress_training逐渐预测多点
    train_dataloader = dset.masked_dataloader(train_dict,
                                              'train',
                                              pre_points=pre_points)
    val_dataloader = dset.masked_dataloader(val_dict, 'val', missing_rate=None)
    test_dataloader = dset.masked_dataloader(test_dict,
                                             'test',
                                             missing_rate=None)

    test_for_epochs = max(1, test_for_epochs)
    pbar = tqdm(total=test_for_epochs)

    # 用于保存最佳模型和决定是否退出训练
    best_metric = torch.tensor([torch.inf], device=device)
    best_metric_epoch = 0
    stop_training = False

    for epoch in range(1, max_epochs + 1):
        if stop_training:
            break
        model.train()  # 预备使用BatchNorm1d(对时间序列，暂时不需要)
        kl_coef = utils.update_kl_coef(kl_coef, epoch)
        pbar.set_description("Epoch [%4d / %4d]" % (epoch, max_epochs))

        # 对每个batch
        for train_batch_list in train_dataloader:
            optimizer.zero_grad()
            batch_dict = utils.time_sync_batch_dict(train_batch_list,
                                                    ode_time=cf.get(
                                                        'ode_time',
                                                        utils.ProcessDataset))
            train_res = model.compute_loss_one_batch(batch_dict, kl_coef)
            gc.collect()
            # 反向传播
            train_res["loss"].backward()
            optimizer.step()
            gc.collect()
            pbar.set_postfix(Loss=train_res["loss"].item(),
                             MSE=train_res["mse"].item())

        # 到达一个epoch
        pbar.update(1)
        if train_res_csv is not None and epoch % save_res_for_epochs == 0:
            train_res_csv.write(train_res, epoch)
        # 更新超参数
        if val_dataloader is None:  # 使用训练集更新
            scheduler.step(train_res["loss"].item())
            if train_res["mse"] * 1.0001 < best_metric:
                best_metric = train_res["mse"]
                best_metric_epoch = epoch
                # 检查点
                checkpoint = (model, optimizer, scheduler, pre_points,
                              experimentID)
                utils.save_checkpoint(ckpt_saveat,
                                      checkpoint,
                                      name="best_for_train")
        else:  # 使用验证集更新
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
                # 检查点
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
        # 超出对应epoch，停止训练
        if patience_for_no_better_epochs is not None:
            if epoch > best_metric_epoch + patience_for_no_better_epochs:
                pbar.close()
                tqdm.write(
                    "No better metrics than %f for %d epochs. Stop training." %
                    (best_metric.item(), patience_for_no_better_epochs))
                output_str = test_and_plot(model, test_dataloader, kl_coef,
                                           experimentID, res_files, fig_saveat,
                                           logger, epoch, cf.get())  # 此时进行一次测试
                tqdm.write(output_str)
                stop_training = True

        # 测试集
        if epoch % test_for_epochs == 0 or epoch == max_epochs:
            output_str = test_and_plot(model, test_dataloader, kl_coef,
                                       experimentID, res_files, fig_saveat,
                                       logger, epoch, cf.get())
            pbar.close()
            if epoch != max_epochs:  # 开新的进度条
                pbar = tqdm(total=test_for_epochs)
            tqdm.write(output_str)  # 展示上一次测试结果


if __name__ == "__main__":
    main()

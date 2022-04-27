###########################
# Modified from: https://github.com/YuliaRubanova/latent_ode by Kirin Ciao
###########################
import os
import sys
import csv
import time
import logging
import pickle
import math
import torch
import torch.nn as nn
import torchcde
import numpy as np
import pandas as pd
import ubelt as ub
import config as cf
from dataclasses import dataclass
from typing import Optional, List, Literal


def get_device(tensor):
    device = torch.device('cpu')
    if tensor.is_cuda:
        device = tensor.get_device()
    return device


def set_seed(seed: int = 47):
    np.random.seed(seed)
    # random.seed(seed)
    torch.manual_seed(seed)
    # GPU
    # torch.cuda.manual_seed(seed)
    torch.cuda.manual_seed_all(seed)
    torch.random.manual_seed(seed)


@dataclass
class ProcessDataset():

    # common
    load_data: bool = True
    ode_time: Literal['Concat', 'Mean', 'No'] = 'No'
    produce_intercoeffs: bool = False
    x_data_with_time: bool = False
    x_y_points = None
    x_points = None
    y_delay = 0
    y_dim_list = None
    dataset: str

    # split_train_val_test
    csv_list: Optional[List[str]] = None
    continue_params: Optional[List[str]] = None
    parse_dates: Optional[List[str]] = None
    glob_params: Optional[List[str]] = None

    # cut_and_split_data
    stride: int = 1
    train_fraq: float = 0.8
    val_fraq: Optional[float] = None
    shuffle: bool = True
    n_start: int = 0

    # dataloader
    batch_size: int
    missing_rate: Optional[float]

    # ENTRYPOINT
    def get_data(self):
        data_pkl_file = ub.Path('data') / dataset / 'All_Data.pkl'
        # 不是插值和Concat的情况时，才可以直接读取
        if self.load_data and os.path.isfile(
                data_pkl_file
        ) and self.ode_time != 'Concat' and not self.produce_intercoeffs:
            return load_pickle(data_pkl_file)
        else:
            data_dict = self.split_train_val_test(dataset)
            train_dict, x_dims, y_dims = self.split_x_y(data_dict, 'train')
            val_dict, _, _ = self.split_x_y(data_dict, 'val')
            test_dict, _, _ = self.plit_x_y(data_dict, 'test')

            # 不是插值和Concat的情况时，才可以在这里存储
            if self.ode_time != 'Concat' and not self.produce_intercoeffs:
                data = (train_dict, val_dict, test_dict, x_dims, y_dims)
                save_items_to_pickle(data, data_pkl_file)
            # TODO: Concat读取到对应的值后设置batch_size = 1
        return train_dict, val_dict, test_dict, x_dims, y_dims

    def _split_train_val_test(self, dataset):
        # 从原始文件中提取数据并分成训练集和测试集（验证集），注意新建的tensor.device
        if 'csv_list' is None:  # 从train.pt和test.pt中直接提取成批数据，不会重新排序
            train = torch.load('data/' + dataset + '/train.pt')
            test = torch.load('data/' + dataset + '/test.pt')
            train_time = train[:, :, :1]
            test_time = test[:, :, :1]
            train_data = train[:, :, 1:]
            test_data = test[:, :, 1:]
            data_dict = {
                'train_data': train_data,
                'test_data': test_data,
                'train_time': train_time,
                'test_time': test_time
            }
            if os.path.isfile('data/' + dataset + '/val.pt'):  # 验证集，用于调整lr等超参数
                val = torch.load('data/' + dataset + '/val.pt')
                val_time = val[:, :, :1]
                val_data = val[:, :, 1:]
                data_dict['val_data'] = val_data
                data_dict['val_time'] = val_time
            else:
                data_dict['val_data'] = None
                data_dict['val_time'] = None
            return data_dict
        else:  # 从csv中读取数据，进行切分和划分训练集/测试集（要求训练集、测试集在同一个文件中）
            train_data = []
            val_data = []
            test_data = []
            train_time = []
            val_time = []
            test_time = []

            data_min = []
            data_max = []

            # 由于Numpy对字符串的支持不是很好，所以直接使用嵌套列表——必须使用2个切片
            train_glob_list = []
            val_glob_list = []
            test_glob_list = []
            for csv_file in csv_list:
                csv_path = 'data/' + dataset + '/' + csv_file + '.csv'
                # 替换缺失值：https://blog.csdn.net/qq_18351157/article/details/104993254，这里去掉了缺失值对应的点
                origin_pd_data = pd.read_csv(csv_path,
                                             header=0).dropna(how='any')

                if continue_params is not None:
                    pd_continue_data = origin_pd_data[continue_params]
                else:
                    pd_continue_data = origin_pd_data  # 不分标签，全部读取，适用于csv文件所有变量都需要的情况

                if parse_dates is not None:  # 将时间变量转换为浮点数
                    pd_timestamp = pd.to_datetime(origin_pd_data[parse_dates],
                                                  infer_datetime_format=True)
                    pd_timestamp = ((pd_timestamp - pd_timestamp.iloc[0]) /
                                    pd.Timedelta(1, 'd')).fillna(0).astype(
                                        float)  # 每个文件的时间都从0开始，1天作为单位1
                    pd_timestamp = pd_timestamp.rename('time')
                    pd_continue_data = pd.concat(
                        [pd_timestamp, pd_continue_data],
                        axis=1)  # continue_data要保证以时间作为第一列

                # TODO: 处理标签数据，将其加入scalar？另外保存一个encoder
                # https://blog.csdn.net/qq_32863549/article/details/106972347
                # https://scikit-learn.org/stable/modules/generated/sklearn.preprocessing.LabelEncoder.html?highlight=labelencoder#sklearn.preprocessing.LabelEncoder
                # TODO label_dict?
                if 'discrete_params' is not None:  # 用于处理csv中的标签数据，比如风向，最后要合并到tensor_data中，另外再保存一个标签映射？
                    pd_discrete_data = origin_pd_data[discrete_params]

                tensor_data = torch.tensor(data=pd_continue_data.values)

                # 验证集为空的情况下，返回的data和time都是None
                (sw_train_data, sw_val_data, sw_test_data, sw_train_time,
                 sw_val_time, sw_test_time,
                 csv_data_min, csv_data_max) = cut_and_split_data(
                     tensor_data, cf.get())  # 在获取训练集和验证集时按照shuffle打乱
                train_data.append(sw_train_data)
                test_data.append(sw_test_data)
                train_time.append(sw_train_time)
                test_time.append(sw_test_time)
                val_data.append(sw_val_data)
                val_time.append(sw_val_time)
                data_min.append(csv_data_min)
                data_max.append(csv_data_max)

                if 'glob_params' is not None:  # 用于处理整个时间序列（包括训练集和测试集）都不变的数据，如采样地点
                    glob_item = origin_pd_data[glob_params].iloc[
                        0].values.tolist()  # 这batch份的值都是一样的
                    train_glob_list.extend([glob_item] *
                                           sw_train_time.shape[0])
                    if sw_val_time is not None:
                        val_glob_list.extend([glob_item] *
                                             sw_val_time.shape[0])
                    else:
                        val_glob_list = None
                    test_glob_list.extend([glob_item] * sw_test_time.shape[0])

            train_data = torch.cat(train_data, dim=0)
            test_data = torch.cat(test_data, dim=0)
            train_time = torch.cat(train_time, dim=0)
            test_time = torch.cat(test_time, dim=0)
            data_dict = {
                'train_data': train_data,
                'test_data': test_data,
                'train_time': train_time,
                'test_time': test_time
            }

            if 'discrete_params' is not None:  # 追加标签数据
                raise NotImplementedError

            if val_data[0] is not None:  # 追加验证集
                val_data = torch.cat(val_data, dim=0)
                val_time = torch.cat(val_time, dim=0)
                data_dict['val_data'] = val_data
                data_dict['val_time'] = val_time
            else:
                data_dict['val_data'] = val_data
                data_dict['val_time'] = val_time

            if 'glob_params' is not None:  # 追加全局数据
                data_dict['train_glob_list'] = train_glob_list
                data_dict['val_glob_list'] = val_glob_list
                data_dict['test_glob_list'] = test_glob_list

            pickle_data = (
                x_data_with_time, pd_continue_data.columns.tolist()
                if x_data_with_time else pd_continue_data.columns.tolist()[1:],
                data_min, data_max)  # TODO: 保存映射信息
            save_items_to_pickle(pickle_data,
                                 'data/' + dataset + '/DataInfo.pkl')

            return data_dict

    def _cut_and_split_data(self,
                            data,
                            x_y_points=None,
                            stride=1,
                            train_fraq=0.8,
                            val_fraq=None,
                            shuffle=True,
                            n_start=0,
                            x_data_with_time=False):
        # 切分滑动窗口
        if x_y_points is None:
            x_y_points = data.shape[0] // 100
        device = get_device(data)

        if x_data_with_time:
            data, data_min, data_max = normalize_data_minmax(data)
        else:
            data[:, 1:], data_min, data_max = normalize_data_minmax(
                data[:, 1:])  # normalize设为min-max
        assert not torch.isnan(data).any()
        assert not torch.isnan(data_min).any()
        assert not torch.isnan(data_max).any()

        data_new = data[n_start:].clone().detach().unsqueeze(0)
        assert x_y_points < data_new.shape[1] - 1
        for i in range(1, x_y_points):
            zero_tensor = torch.zeros((i, data.shape[1]), device=device)
            tmp_tensor = torch.vstack(
                (data[i + n_start:], zero_tensor)).unsqueeze(0)
            data_new = torch.cat((data_new, tmp_tensor))
        data_new = data_new.permute(1, 0, 2)
        data_new = data_new[:-(x_y_points - 1)]  # 切掉末尾带有0分量的几个切片
        if stride > 1 and data_new.shape[0] > stride:
            data_select = data_new[0].unsqueeze(0)
            for j in range(stride, data_new.shape[0], stride):
                data_select = torch.cat(
                    (data_select, data_new[j].unsqueeze(0)))
            data_new = data_select
        if shuffle:  # 对滑动窗口重排序
            idx = torch.randperm(data_new.shape[0])
            data_new = data_new[idx]

        time = data_new[:, :, :1]
        data = data_new[:, :, 1:]

        train_data, val_data, test_data, train_time, val_time, test_time = split_train_test_data_and_time(
            data, time, train_fraq, val_fraq)
        return train_data, val_data, test_data, train_time, val_time, test_time, data_min, data_max

    def _split_train_test_data_and_time(self,
                                        data,
                                        time,
                                        train_fraq=0.8,
                                        val_fraq=None):
        # 划分训练集和测试集，要求第1维为划分的时间段
        # 自回归时，测试集应该整体未知，第一步从训练集中取最后几点作为输入，然后逐渐替代，切分出的data_test和test_time
        data_size = data.shape[0]
        assert len(time.shape) == 3

        if val_fraq is None:
            train_point = int(data_size * train_fraq)
            train_data = data[:train_point]
            train_time = time[:train_point]
            test_data = data[train_point:]
            test_time = time[train_point:]
            val_data = None
            val_time = None
        else:
            train_point = int(data_size * train_fraq)
            val_point = int(data_size * (val_fraq + train_fraq))
            train_data = data[:train_point]
            train_time = time[:train_point]
            val_data = data[train_point:val_point]
            val_time = time[train_point:val_point]
            test_data = data[val_point:]
            test_time = time[val_point:]
        return train_data, val_data, test_data, train_time, val_time, test_time

    def _split_x_y(self,
                   data_dict,
                   data_type: Literal['train', 'val', 'test'],
                   x_data_with_time: bool,
                   x_y_points,
                   x_points=None,
                   y_delay=0,
                   y_dim_list=None,
                   arch):  # TODO
        if data_type + '_time' not in data_dict or data_dict[data_type +
                                                             '_time'] is None:
            return None
        if x_points is not None:
            x_points = min(x_points, x_y_points - 1)
        # 非自回归预测时，Autoencoder先反向求解到h0，再正向求解到最后一个点
        if y_dim_list is not None:
            y_dims = len(y_dim_list)
            all_dim_list = [
                i for i in range(data_dict[data_type + '_data'].shape[-1])
            ]
            x_dim_list = list(set(all_dim_list).difference(set(y_dim_list)))
            x_dims = len(x_dim_list)
            split_dict = {
                'x_data': data_dict[data_type + '_data'][:, :,
                                                         x_dim_list].clone(),
                'x_time': data_dict[data_type + '_time'].clone(),
                'y_data': data_dict[data_type + '_data'][:, :,
                                                         y_dim_list].clone(),
                'y_time': data_dict[data_type + '_time'].clone()
            }
            if arch in {'Seq2Seq', 'VAE'}:  # 对采用AutoEncoder的两种架构，需要x反向求解到t0之前的一个小量
                # 线性小量
                x_data_until = split_dict['x_data'][:, :1, :] - 1 / 10 * (
                    split_dict['x_data'][:, 1:2, :] -
                    split_dict['x_data'][:, :1, :])
                x_time_until = split_dict['x_time'][:, :1, :] - 1 / 10 * (
                    split_dict['x_time'][:, 1:2, :] -
                    split_dict['x_time'][:, :1, :])
                split_dict['x_data'] = torch.cat(
                    (split_dict['x_data'].flip(dims=[1]), x_data_until), dim=1)
                split_dict['x_time'] = torch.cat(
                    (split_dict['x_time'].flip(dims=[1]), x_time_until), dim=1)
        else:
            if x_points is None:
                # 自回归：默认按照1/2的关系划分观察点（输入）和非观察点（输出）
                x_points = data_dict[data_type + '_data'].shape[1] // 2
            # y_delay用于超前预测，将总时间长度中的y向后移动
            split_dict = {
                'x_data':
                data_dict[data_type + '_data'][:, :x_points, :].clone(),
                'x_time':
                data_dict[data_type + '_time'][:, :x_points, :].clone(),
                'y_data':
                data_dict[data_type + '_data'][:,
                                               x_points + y_delay:, :].clone(),
                'y_time':
                data_dict[data_type + '_time'][:,
                                               x_points + y_delay:, :].clone()
            }
            x_dims = split_dict['x_data'].shape[-1]
            y_dims = split_dict['y_data'].shape[-1]

        if x_data_with_time:  # 将时间加入x序列
            split_dict['x_data'] = torch.cat(
                (split_dict['x_time'], split_dict['x_data']), dim=2)
            x_dims += 1

        return split_dict, x_dims, y_dims

    # 训练中处理数据
    # ENTRYPOINT
    def masked_dataloader(self,
                          data_dict,
                          data_name='train',
                          missing_rate=None,
                          pre_points=None):
        if missing_rate is None:
            missing_rate = self.missing_rate
        if data_dict is None:
            return None
        if produce_intercoeffs:
            file_detail = data_name + '_' + str(int(missing_rate * 100) if missing_rate is not None else 0) + '_' + \
                str(pre_points if pre_points is not None else 0) + '_' + \
                (ode_time if ode_time is not None else 'Default') + '_' + \
                ('with_time' if x_data_with_time else 'without_time')
            coeffs_save_file = ub.Path('data') / dataset / 'coeffs_' + file_detail + '.pkl'
            if coeffs_save_file.exists and load_data:
                return load_pickle(coeffs_save_file)
        # TODO: curriculum learning for train data
        # 1.从已划分的输入中进行下采样，并将mask加入新的dict，默认50个点
        data = data_dict['x_data'].clone()
        device = get_device(data)
        mask = torch.ones_like(data, dtype=torch.bool,
                               device=device)  # 先用bool类型，到生成batch时再换float值

        if missing_rate is not None:
            time_points = len(data_dict['x_time'][0, :, 0])
            missing_points = min(int(time_points * 0.9),
                                 int(time_points * missing_rate))
            # 对每个切片，随机使time_points * missing_rate个点失活
            for i in range(data.shape[0]):
                missing_idx = sorted(
                    np.random.choice(np.arange(time_points),
                                     missing_points,
                                     replace=False))
                mask[i, missing_idx, :] = False

        # 更新其data和mask
        new_data_dict = {}
        new_data_dict['x_data'] = data.clone()
        new_data_dict['x_time'] = data_dict['x_time'].clone()
        new_data_dict['x_mask'] = mask.clone()

        # 2.使用pre_points截断输出
        data = data_dict['y_data'].clone()
        mask = torch.ones_like(data, dtype=torch.bool,
                               device=device)  # 换用bool类型，直接判断

        if pre_points is not None:
            pre_points = min(pre_points, len(data_dict['y_time'][0, :, 0]))
            mask[:, pre_points:, :] = False

        new_data_dict['y_data'] = data.clone()
        new_data_dict['y_time'] = data_dict['y_time'].clone()
        new_data_dict['y_mask'] = mask.clone()

        if produce_intercoeffs:
            produce_and_save_intercoeffs(new_data_dict, coeffs_save_file)

        return _wrap_dataloader(new_data_dict, min(batch_size, new_data_dict['x_data'].shape[0]))

    def _produce_and_save_intercoeffs(self, data_dict, coeffs_save_file):
        # 计算和保存插值，对使用DataLoader的情况保存DataLoader
        data_dict['x_data'][~data_dict['x_mask']] = torch.nan
        data_dict['x_data'] = torchcde.natural_cubic_coeffs(
            data_dict['x_data'])
        dataloader = _wrap_dataloader(data_dict,
                                     batch_size=min(self.batch_size, data_dict['x_data'].shape[0]),
                                     shuffle=self.shuffle)
        save_items_to_pickle(dataloader, coeffs_save_file)

    def _wrap_dataloader(self, data_dict, **kwargs):
        # 使用Dataloader
        dataset = torch.utils.data.TensorDataset(
            data_dict['x_time'], data_dict['x_data'], data_dict['x_mask'],
            data_dict['y_time'], data_dict['y_data'], data_dict['y_mask'])
        assert 'batch_size' in kwargs
        if 'shuffle' not in kwargs:
            kwargs['shuffle'] = True
        if 'drop_last' not in kwargs:
            kwargs['drop_last'] = True
        if 'num_workers' not in kwargs:
            kwargs['num_workers'] = 0
        kwargs['batch_size'] = min(kwargs['batch_size'], len(dataset))
        return torch.utils.data.DataLoader(dataset, **kwargs)


# 定义和度量模型
def get_params_num(model):
    # filter接收两个参数，第一个为函数，第二个为序列，序列的每个元素作为参数传递给函数进行判断，然后返回 True 或 False，最后将返回 True 的元素放到新列表中——计算参数量
    model_parameters = filter(lambda p: p.requires_grad, model.parameters())
    n_params = sum([np.prod(p.shape) for p in model_parameters])
    kilo_params = n_params // 1000
    mod_params = n_params % 1000
    return str(kilo_params) + ' k ' + str(mod_params)


def create_net(input_dims,
               output_dims=None,
               n_units=100,
               hidden_layers=0,
               nonlinear=nn.Tanh,
               device=torch.device('cpu'),
               return_dim=False):
    if output_dims is None:
        output_dims = input_dims
    layers = [nn.Linear(input_dims, n_units)]

    for _ in range(hidden_layers):
        layers.append(nonlinear())
        layers.append(nn.Linear(n_units, n_units))

    layers.append(nonlinear())
    layers.append(nn.Linear(n_units, output_dims))
    # 用在函数传递当中，*尝试将对象转为元组（可变参数），**尝试将对象转为字典（关键字参数），两者均可传入0个或任意个参数
    if return_dim:
        return nn.Sequential(*layers).to(device), input_dims, output_dims
    else:
        return nn.Sequential(*layers).to(device)


def split_last_dim(data):
    # 二等分tensor最后一个维度，以区分mean和std
    last_dim = data.shape[-1]
    last_dim = last_dim // 2
    return data[..., :last_dim], data[..., last_dim:]


def init_network_weights(net, std=0.1):
    # 高斯分布初始化
    # TODO: 如果用xavier初始化呢？
    for m in net.modules():
        if isinstance(m, nn.Linear):
            nn.init.normal_(m.weight, mean=0, std=std)
            nn.init.constant_(m.bias, val=0)


def sample_standard_gaussian(mu, sigma):
    device = get_device(mu)

    d = torch.distributions.normal.Normal(
        torch.Tensor([0.]).to(device),
        torch.Tensor([1.]).to(device))
    r = d.sample(mu.shape).squeeze(-1)
    return r * sigma.float() + mu.float()


def linspace_vector(start, end, n_points, device=torch.device('cpu')):
    # start is either one value or a vector
    size = np.prod(start.shape)

    assert start.shape == end.shape
    if size == 1:
        # start and end are 1d-tensors
        res = torch.linspace(start, end, n_points, device=device)  # 包括end端点
    else:  # 支持多维度的linspace，对每个维度单独作向量空间
        # start and end are vectors
        res = torch.Tensor(device=device)
        for i in range(0, start.shape[0]):
            res = torch.cat((res, torch.linspace(start[i], end[i], n_points)),
                            0)
        res = torch.t(res.reshape(start.shape[0], n_points))
    return res


def time_sync_batch_dict(batch_list, ode_time=None):
    batch_dict = {}
    (batch_dict['x_time'], batch_dict['x_data'], batch_dict['x_mask'],
     batch_dict['y_time'], batch_dict['y_data'],
     batch_dict['y_mask']) = batch_list

    # batch中进行时间同步
    if ode_time is not None:
        if ode_time == 'Concat':
            concat_batch(batch_dict)
        elif ode_time == 'Mean':
            # 直接平均，简化成1维形式
            batch_dict['x_time'] = torch.mean(batch_dict['x_time'],
                                              dim=0).squeeze()
            batch_dict['y_time'] = torch.mean(batch_dict['y_time'],
                                              dim=0).squeeze()
        elif ode_time == 'No':
            # 取点数，从0开始
            device = get_device(batch_dict['x_time'])
            dtype = batch_dict['x_time'].dtype
            batch_dict['x_time'] = torch.arange(
                start=0,
                end=batch_dict['x_time'].shape[1],
                dtype=dtype,
                device=device)
            batch_dict['y_time'] = torch.arange(
                start=0,
                end=batch_dict['y_time'].shape[1],
                dtype=dtype,
                device=device)
        else:
            raise NotImplementedError
    else:
        batch_dict['x_time'] = batch_dict['x_time'][0].squeeze()
        batch_dict['y_time'] = batch_dict['y_time'][0].squeeze()
    return batch_dict


def concat_batch(batch_dict):
    # 是否需要 batch_dict['x_time'][0, 0, 0] != batch_dict['x_time'][-1, 0, 0]？
    batch_size = batch_dict['x_data'].shape[0]
    # 拼接方案1，生成batch_size个扩展tensor，极度耗费时间 ######## TODO: 修改拼接方案
    _, time_points, x_dims = batch_dict['x_data'].shape
    device = get_device(batch_dict['x_data'])
    x_sync_time = torch.unique(batch_dict['x_time'][:, :, 0],
                               sorted=True).flatten()
    x_sync_time_points = x_sync_time.shape[0]
    x_sync_data = torch.zeros((batch_size, x_sync_time_points, x_dims),
                              device=device)
    x_sync_mask = torch.zeros_like(x_sync_data, dtype=torch.bool)
    for i in range(batch_size):
        cur = 0
        for j in range(x_sync_time_points):
            if cur >= time_points:
                break
            if x_sync_time[j] == batch_dict['x_time'][i, cur, 0]:
                x_sync_data[i, j, :] = batch_dict['x_data'][i, cur, :]
                x_sync_mask[i, j, :] = batch_dict['x_mask'][i, cur, :]
                cur += 1
    batch_dict['x_data'] = x_sync_data
    batch_dict['x_mask'] = x_sync_mask
    batch_dict['x_time'] = x_sync_time
    # 是否需要 batch_dict['y_time'][0, 0, 0] != batch_dict['y_time'][-1, 0, 0]？借助batch首末第1条判断是否需要时间同步
    _, time_points, y_dims = batch_dict['y_data'].shape
    y_sync_time = torch.unique(batch_dict['y_time'][:, :, 0],
                               sorted=True).flatten()
    y_sync_time_points = y_sync_time.shape[0]
    y_sync_data = torch.zeros((batch_size, y_sync_time_points, y_dims),
                              device=device)
    y_sync_mask = torch.zeros_like(y_sync_data,
                                   dtype=torch.bool)  # 对y值，它的mask是bool值
    for i in range(batch_size):
        cur = 0
        for j in range(y_sync_time_points):
            if cur >= time_points:
                break
            if y_sync_time[j] == batch_dict['y_time'][i, cur, 0]:
                y_sync_data[i, j, :] = batch_dict['y_data'][i, cur, :]
                y_sync_mask[i, j, :] = batch_dict['y_mask'][i, cur, :]
                cur += 1
    batch_dict['y_data'] = y_sync_data
    batch_dict['y_mask'] = y_sync_mask
    batch_dict['y_time'] = y_sync_time
    if batch_size <= 1:
        return  # 无需拼接


# 更新超参数
def update_learning_rate(optimizer, decay_rate=0.999, lowest=1e-3):
    for param_group in optimizer.param_groups:
        lr = param_group['lr']
        lr = max(lr * decay_rate, lowest)
        param_group['lr'] = lr


def update_kl_coef(kl_first, epoch, wait_until_kl=10):
    if epoch < wait_until_kl:
        kl_coef = 0.
    else:
        kl_coef = kl_first * (1 - 0.99**(epoch - wait_until_kl))
    return kl_coef


# 用于测试
def store_best_results(model, test_dataloader, kl_coef, experimentID,
                       res_files, ode_time, model_name):
    test_res = compute_loss_all_batches(model,
                                        test_dataloader,
                                        kl_coef,
                                        ode_time=ode_time)

    # 检查对应值是否更新，注意每组度量值metric_name都需要单独的文件
    res_dict, value_updated = update_metric_if_larger(
        res_files['best_results'], model_name, 'negative_MSE',
        -test_res['mse'].item())
    # 如果更新后，更改最佳ID
    if value_updated:
        res_dict = update_value(res_files['best_experimentID'], model_name,
                                'negative_MSE', experimentID)

    return test_res, res_dict


def compute_loss_all_batches(model,
                             metric_dataloader,
                             kl_coef=1.,
                             ode_time='No'):
    res_dict_mean = {}
    res_dict_mean['loss'] = 0
    res_dict_mean['likelihood'] = 0
    res_dict_mean['mse'] = 0
    res_dict_mean['kl_coef'] = 0
    res_dict_mean['C_kl'] = 0
    res_dict_mean['C_std'] = 0

    n_test_batches = 0

    for metric_data_list in metric_dataloader:

        batch_dict = time_sync_batch_dict(metric_data_list, ode_time)
        results = model.compute_loss_one_batch(batch_dict, kl_coef=kl_coef)

        for key in res_dict_mean.keys():
            if key in results:
                var = results[key]
                if isinstance(var, torch.Tensor):
                    var = var.detach()
                res_dict_mean[key] += var

        n_test_batches += 1

    # 取平均
    if n_test_batches > 1:
        for key, _ in res_dict_mean.items():
            res_dict_mean[key] = res_dict_mean[key] / n_test_batches

    return res_dict_mean


def update_metric_if_larger(results_file_name, model_name, metric_name, value):
    if not os.path.isfile(results_file_name) or os.path.getsize(
            results_file_name) == 0:  # 不存在或文件为空
        with open(results_file_name, 'w') as f:
            f.write('model,' + metric_name)

    res_dict = pd.read_csv(results_file_name, sep=',')
    if metric_name not in res_dict.columns:
        res_dict[metric_name] = None

    value_updated = False

    if isinstance(value, torch.Tensor):
        # value = value.cpu().numpy() # 对要比较的量转换为数值类型
        value = value.item()

    if (res_dict['model'] == model_name).any():
        prev_value = res_dict.loc[res_dict['model'] == model_name, metric_name]
        larger_than_prev = False
        if prev_value is not None and prev_value.item() is not None:
            larger_than_prev = float(prev_value.item()) < value
        if (prev_value.item() is
                None) or math.isnan(prev_value) or larger_than_prev:
            res_dict.loc[res_dict['model'] == model_name,
                         metric_name] = float(value)  # 更新对应表格
            value_updated = True
    else:  # 将对应模型结果加入
        res_dict = pd.concat([
            res_dict,
            pd.DataFrame({
                'model': [model_name],
                metric_name: [value]
            })
        ],
                             ignore_index=True)
        value_updated = True

    with open(results_file_name, 'w', newline='') as csvfile:  # 改写对应文件
        res_dict.to_csv(csvfile, index=False)
    return res_dict, value_updated


def update_value(results_file_name, model_name, metric_name, value):
    if not os.path.isfile(results_file_name) or os.path.getsize(
            results_file_name) == 0:
        with open(results_file_name, 'w') as f:
            f.write('model,' + metric_name)

    res_dict = pd.read_csv(results_file_name, sep=',')
    if metric_name not in res_dict.columns:
        res_dict[metric_name] = None

    if (res_dict['model'] == model_name).any():  # 存在就直接更新，不需要比大小
        res_dict.loc[res_dict['model'] == model_name, metric_name] = value
    else:
        res_dict = pd.concat([
            res_dict,
            pd.DataFrame({
                'model': [model_name],
                metric_name: [value]
            })
        ],
                             ignore_index=True)

    with open(results_file_name, 'w', newline='') as csvfile:
        res_dict.to_csv(csvfile, index=False)
    return res_dict


def normalize_data_minmax(data):
    # 辅助处理数据
    data_min = torch.min(data, 0).values
    data_max = torch.max(data, 0).values
    numerator = data - data_min
    denominator = data_max - data_min
    norm_data = numerator / (denominator + 1e-7)  # 加入小数避免除以0
    return norm_data, data_min, data_max


# 保存检查点和结果
def get_logger_and_save(model,
                        experimentID,
                        dataset_str,
                        experiment_str,
                        save_log=False,
                        save_fig_per_test=0,
                        save_ckpt=False,
                        save_res_for_epochs=None):

    if save_log:
        log_file = ub.Path('log').ensuredir() / experiment_str + '.log'
    else:
        log_file = None
    res_files = {
        'best_results': 'log/best_results_' + dataset_str + '.csv',
        'best_experimentID': 'log/best_experimentID_' + dataset_str + '.csv'
    }

    if save_fig_per_test > 0:
        fig_saveat = ub.Path('fig').ensuredir() / experiment_str
    else:
        fig_saveat = None

    if save_ckpt:
        ckpt_saveat = ub.Path('ckpt').ensuredir() / experiment_str
    else:
        ckpt_saveat = None

    print('ExperimentID: ' + experimentID)  # 看到这个print的实验已经产生了结果文件
    # 提示信息，warning会被输出到控制台并记录到文件中，info级别不会输出到控制台
    logger = get_logger(log_file)
    note = ub.paragraph('''

                        arch, solver, dset, x_pts, y_pts, missing_pct, time
                        {experiment_str}

        ''')
    logger.warning(note)
    logger.warning('All args are: ' + cf.log())
    # 训练结果记录器
    if save_res_for_epochs is not None and save_log:
        train_res_csv = save_csv(log_saveat, 'train')
        val_res_csv = save_csv(log_saveat, 'val')
    else:
        train_res_csv = None
        val_res_csv = None
    return logger, res_files, train_res_csv, val_res_csv, fig_saveat, ckpt_saveat


def get_logger(log_file=None):
    logger = logging.getLogger()
    logger.setLevel(logging.INFO)
    if log_file is not None:
        info_file_handler = logging.FileHandler(log_file)
        info_file_handler.setLevel(logging.INFO)
        logger.addHandler(info_file_handler)
    console_handler = logging.StreamHandler()
    console_handler.setLevel(logging.WARNING)
    logger.addHandler(console_handler)

    return logger


def save_checkpoint(ckpt_saveat, state, **kwargs):
    if ckpt_saveat is None or not os.path.isdir(ckpt_saveat):
        return
    if 'epoch' in kwargs:
        filename = os.path.join(ckpt_saveat,
                                'CheckPoint-%04d.pth' % kwargs['epoch'])
    elif 'name' in kwargs:
        filename = os.path.join(ckpt_saveat, '%s.pth' % kwargs['name'])
    else:
        raise ValueError
    torch.save(state, filename)


def load_checkpoint(ckpt_saveat, model=None):
    if not os.path.exists(ckpt_saveat):
        raise Exception('Checkpoint ' + ckpt_saveat + ' does not exist.')
    if os.path.isdir(ckpt_saveat):
        ckpt_saveat = os.path.join(ckpt_saveat, 'SaveAll.pth')
    checkpoint = torch.load(ckpt_saveat)
    if model is not None:  # 只获取model对应状态
        state_dict = checkpoint
        model_dict = model.state_dict()
        # 1. filter out unnecessary keys
        state_dict = {k: v for k, v in state_dict.items() if k in model_dict}
        # 2. overwrite entries in the existing state dict
        model_dict.update(state_dict)
        # 3. load the new state dict
        model.load_state_dict(state_dict)
        # model.to(device)
        return model
    else:
        # model = checkpoint['model'].to(device)
        # optimizer = checkpoint['optimizer']
        # scheduler = checkpoint['scheduler']
        # pre_points = checkpoint['pre_points']
        # experimentID = checkpoint['ID']
        return checkpoint


class save_csv(object):
    '''write results into csv'''

    def __init__(self, csv_saveat, data_type='train'):
        super(save_csv, self).__init__()
        self.csv_saveat = csv_saveat  # 和log存在一个文件夹里
        self.data_type = data_type
        self.filepath = self.csv_saveat + '/' + self.data_type + '_' + str(
            0) + '.csv'
        self.no_head = True

    def write(self, res, epoch):
        row = [
            str(metric.item())
            if isinstance(metric, torch.Tensor) else str(metric)
            for metric in res.values()
        ]
        row.append(str(epoch))
        with open(self.filepath, 'a', newline='') as f:
            csv_writer = csv.writer(f)
            if self.no_head:
                head = list(res.keys())
                head.append('epoch')
                csv_writer.writerow(head)
                self.no_head = False  # 只写入一次表头
            csv_writer.writerow(row)
        new_filepath = self.csv_saveat + '/' + self.data_type + '_' + str(
            epoch) + '.csv'
        os.rename(self.filepath, new_filepath)
        self.filepath = new_filepath  # 更新epoch数信息


# pickle版本
def save_items_to_pickle(data, filename):
    with open(filename, 'wb') as pkl_file:
        pickle.dump(data, pkl_file)


def load_pickle(filename):
    with open(filename, 'rb') as pkl_file:
        filecontent = pickle.load(pkl_file)
    return filecontent


def get_items_from_pickle(filename, item_name_list):
    pickle_items = load_pickle(filename)
    items = {}
    for item_name in item_name_list:
        if item_name in pickle_items:
            items[item_name] = pickle_items[item_name]
    return items

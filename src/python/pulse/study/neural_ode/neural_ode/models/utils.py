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
import json
import einops
import ubelt as ub
from copy import deepcopy
from dataclasses import dataclass, field, asdict
from typing import Optional, List, Literal, Any, Dict, Tuple
from simple_parsing import ArgumentParser

import pytorch_lightning as pl
import pytorch_forecasting as pf
from pytorch_forecasting.data.timeseries import (TimeSeriesDataSet,
                                                 GroupNormalizer)
import torchmetrics

from neural_ode.models.evaluation import get_log_likelihood, get_mse


# https://mail.python.org/archives/list/python-dev@python.org/message/JBYXQH3NV3YBF7P2HLHB5CD6V3GVTY55/
# for dataset-dependent model params, to get around lack of dataclass(kw_only=True)
class Sentinel:

    def __new__(cls, *args, **kwargs):
        raise TypeError(f'{cls.__qualname__} cannot be instantiated')


class MISSING(Sentinel):
    pass


# TODO death prediction
# https://pytorch-forecasting.readthedocs.io/en/stable/tutorials/building.html#Classification

@dataclass
class BaseModelWithCovariatesKwargs:
    static_categoricals: List[str] = field(default_factory=list)
    static_reals: List[str] = field(default_factory=list)
    time_varying_categoricals_encoder: List[str] = field(default_factory=list)
    time_varying_categoricals_decoder: List[str] = field(default_factory=list)
    categorical_groups: Dict[str, List[str]] = field(default_factory=dict)
    time_varying_reals_encoder: List[str] = field(default_factory=list)
    time_varying_reals_decoder: List[str] = field(default_factory=list)
    embedding_sizes: Dict[str, Tuple[int, int]] = field(default_factory=dict)
    embedding_paddings: List[str] = field(default_factory=list)
    embedding_labels: Dict[str, np.ndarray] = field(default_factory=dict)
    x_reals: List[str] = field(default_factory=list)
    x_categoricals: List[str] = field(default_factory=list)


class BaseModel(pf.BaseModelWithCovariates):
    '''

    Model with additional methods using covariates.

    Assumes the following hyperparameters:

    Args:
        static_categoricals (List[str]): names of static categorical variables
        static_reals (List[str]): names of static continuous variables
        time_varying_categoricals_encoder (List[str]): names of categorical variables for encoder
        time_varying_categoricals_decoder (List[str]): names of categorical variables for decoder
        time_varying_reals_encoder (List[str]): names of continuous variables for encoder
        time_varying_reals_decoder (List[str]): names of continuous variables for decoder
        x_reals (List[str]): order of continuous variables in tensor passed to forward function
        x_categoricals (List[str]): order of categorical variables in tensor passed to forward function
        embedding_sizes (Dict[str, Tuple[int, int]]): dictionary mapping categorical variables to tuple of integers
            where the first integer denotes the number of categorical classes and the second the embedding size
        embedding_labels (Dict[str, List[str]]): dictionary mapping (string) indices to list of categorical labels
        embedding_paddings (List[str]): names of categorical variables for which label 0 is always mapped to an
             embedding vector filled with zeros
        categorical_groups (Dict[str, List[str]]): dictionary of categorical variables that are grouped together and
            can also take multiple values simultaneously (e.g. holiday during octoberfest). They should be implemented
            as bag of embeddings
    '''

    @staticmethod
    # TODO un-hardcode
    def _coerce_loss(loss, n_targets=11):
        if isinstance(loss, str):
            _loss =getattr(pf.metrics, loss)()
        elif isinstance(loss, (pf.metrics.Metric, pf.metrics.MultiLoss)):
            _loss = loss
        else:
            raise TypeError(loss)

        if n_targets > 1 and not isinstance(_loss, pf.metrics.MultiLoss):
            _loss = pf.metrics.MultiLoss([_loss] * n_targets)

        return _loss


    def __init__(
        self,
        learning_rate=1e-2,
        # loss=pf.metrics.RMSE(),
        loss='RMSE',  # coerce later to avoid yaml bug in LightningCLI(run=True)
        # loss=pf.metrics.SMAPE(),
        # logging_metrics=[pf.metrics.RMSE()],
        logging_metrics=['RMSE'],
        # logging_metrics=torch.nn.ModuleList([pf.metrics.RMSE()]),
        optimizer='Adamax',  # 'ranger'
        reduce_on_plateau_patience=10,
        reduce_on_plateau_reduction=10,
        reduce_on_plateau_min_lr=1e-6,
        weight_decay=0.,
        optimizer_params={},
        monotone_constaints={},  # sp
        **kwargs
    ):
        loss = self._coerce_loss(loss)

        logging_metrics = nn.ModuleList(
            [self._coerce_loss(l) for l in logging_metrics])
        self.save_hyperparameters()
        # self.save_hyperparameters(dict(loss=loss, logging_metrics=logging_metrics), logger=False)
        # TODO ensure the cov kwargs are saved in hparams
        super().__init__(loss=loss, logging_metrics=logging_metrics, **ub.dict_diff(kwargs, asdict(BaseModelWithCovariatesKwargs())))
        self.gaussian_likelihood_std = torch.tensor([0.01])

        self.x_dims: int = MISSING
        self.y_dims: int = MISSING
        # https://pytorch-lightning.readthedocs.io/en/stable/api/pytorch_lightning.core.LightningModule.html#pytorch_lightning.core.LightningModule.example_input_array
        # shouldn't declare this before it can be assigned to
        # self.example_input_array: Any = MISSING
        self.t_param: str = MISSING

        # need a param with requires_grad=True before configure_optimizers()
        # TODO ensure params in setup() are added to optimizer and training
        self.dummy_param = torch.nn.Parameter(torch.ones(1))

        # TODO self.input_embeddings for categorical
        # # create embedder - can be fed with x["encoder_cat"] or x["decoder_cat"] and will return
        # # dictionary of category names mapped to embeddings
        # self.input_embeddings = MultiEmbedding(
            # embedding_sizes=self.hparams.embedding_sizes,
            # categorical_groups=self.hparams.categorical_groups,
            # embedding_paddings=self.hparams.embedding_paddings,
            # x_categoricals=self.hparams.x_categoricals,
            # max_embedding_size=self.hparams.hidden_size,
        # )


    def forward(self, x: Dict[str, torch.Tensor]) -> Dict[str, torch.Tensor]:
        '''
        # x is a batch generated based on the TimeSeriesDataset
        batch_size = x["encoder_lengths"].size(0)
        embeddings = self.input_embeddings(x["encoder_cat"])  # returns dictionary with embedding tensors
        network_input = torch.cat(
            [x["encoder_cont"]]
            + [
                emb
                for name, emb in embeddings.items()
                if name in self.encoder_variables or name in self.static_variables
            ],
            dim=-1,
        )
        prediction = self.network(network_input.view(batch_size, -1))

        # rescale predictions into target space
        prediction = self.transform_output(prediction, target_scale=x["target_scale"])

        # We need to return a dictionary that at least contains the prediction.
        # The parameter can be directly forwarded from the input.
        # The conversion to a named tuple can be directly achieved with the `to_network_output` function.
        return self.to_network_output(prediction=prediction)
        '''
        # TODO x_time and y_time as known covariate (encoder_cont, decoder_cont)
        # TODO use packed_sequence
        # 'b t f'
        x_data = x['encoder_cont']
        assert torch.unique(x['encoder_lengths']).shape == (1, )
        enc_len = x['encoder_lengths'][0]
        assert torch.unique(x['decoder_time_idx'][:, 0]).shape == (1, )
        dec_idx = x['decoder_time_idx'][0, 0]
        dec_len = torch.max(x['decoder_lengths'])
        x_time = torch.arange(dec_idx - enc_len, dec_idx)
        y_time = torch.arange(dec_idx, dec_idx + dec_len)

        y_pred = self.old_forward(y_time, x_data, x_time)
        # TODO update if any target is multichannel (categorical)
        # has to be a list of tensors...
        y_pred = [*einops.rearrange(y_pred, 'b t f -> f b t 1')]

        y_pred = self.transform_output(y_pred, target_scale=x["target_scale"])
        return self.to_network_output(prediction=y_pred)

    def old_forward(self, y_time, x, x_time):
        raise NotImplementedError

    def init_xy(self):
        pass

    # TODO check this when implementing categorical target in from_dataset
    # https://pytorch-forecasting.readthedocs.io/en/stable/tutorials/building.html#Predicting-multiple-targets-at-the-same-time
    @classmethod
    def from_datamodule(cls, dm, **kwargs):
        dm.prepare_data()
        dm.setup(stage='train')
        # TODO keep track of deduce_default_output_parameters output_size
        # output_size (Union[int, List[int]], optional): number of outputs
        # (e.g. number of quantiles for QuantileLoss and one target or list of
        # output sizes).
        # [n_features, min(dec_lens)??]
        self = cls.from_dataset(dm.dset_tr, **ub.dict_diff(kwargs, ['output_transformer']))#, output_transformer=dm.dset_tr.target_normalizer)
        batch = next(iter(dm.train_dataloader()))
        x, y = batch
        self.example_input_array = (x,)
        self.x_dims = len(self.encoder_variables)  # + len(self.static_variables)
        self.y_dims = len(self.decoder_variables) + len(self.target_names)
        self.batch_size = -1
        self.t_param = dm.t_param
        self.init_xy()
        return self

    def _setup(self, stage=None):
        # TODO parameterize dset_tr and train_dataloader by stage
        dm = self.trainer.datamodule
        # import xdev; xdev.embed()
        self = self.from_dataset(dm.dset_tr, **self.hparams)
        batch = next(iter(dm.train_dataloader()))
        self.example_input_array = batch
        self.x_dims = len(self.encoder_variables)  # + len(self.static_variables)
        self.y_dims = len(self.decoder_variables) + len(self.target_names)
        # self.batch_size = len(batch[0]['encoder_lengths'])
        self.t_param = dm.t_param
        self.init_xy()
        super().setup(stage)


# no dice due to https://github.com/Lightning-AI/lightning/issues/12506
# datamodule is still ok though
# @dataclass(eq=False)
class BaseModelOld(pl.LightningModule):

    # lr: float = 1e-2

    # def __post_init__(self):
    def __init__(self, lr: float = 1e-2):
        super().__init__()
        self.lr = lr
        self.gaussian_likelihood_std = torch.tensor([0.01])

        self.x_dims: int = MISSING
        self.y_dims: int = MISSING
        # https://pytorch-lightning.readthedocs.io/en/stable/api/pytorch_lightning.core.LightningModule.html#pytorch_lightning.core.LightningModule.example_input_array
        self.example_input_array: Any = MISSING

        # TODO try SMAPE too
        # self.mse_loss = torchmetrics.MeanSquaredError()
        # self.val_mse_loss = torchmetrics.MeanSquaredError()
        # https://becominghuman.ai/pytorch-lightning-tutorial-2-using-torchmetrics-and-lightning-flash-901a979534e2
        # probably should use pf metrics instead of these

    # TODO add to cli
    # https://pytorch-lightning.readthedocs.io/en/stable/common/lightning_cli.html#optimizers-and-learning-rate-schedulers
    def configure_optimizers(self):
        optimizer = torch.optim.Adamax(self.parameters(), lr=self.lr)
        # this has no effect if patience_for_no_better_epochs <= 10 (default 30)
        scheduler = torch.optim.lr_scheduler.ReduceLROnPlateau(optimizer,
                                                               patience=10)
        return {
            'optimizer': optimizer,
            'lr_scheduler': scheduler,
            'monitor': 'tr_mse',
            # 'monitor': 'va_mse',
        }

    def forward(self, y_time, x, x_time):
        raise NotImplementedError

    def compute_loss_one_batch(self, batch, batch_idx):
        #TRANS: Predict the y series, including the observed and was not observed
        y_pred = self.forward(batch["y_time"], batch["x_data"],
                              batch["x_time"])

        # TRANS: To calculate indicators, batch_dict "y_mask" if is True, it should
        # enter None
        likelihood = get_log_likelihood(y_pred, batch["y_data"],
                                        self.gaussian_likelihood_std)
        mse_loss = get_mse(y_pred, batch["y_data"])
        # self.mse_loss(y_pred, batch['y_data'])
        loss = -torch.mean(
            likelihood
        )  #TRANS: To maximize the average log_likelihood back propagation

        return loss, mse_loss

    '''
    def training_step(self, batch, batch_idx):
        self.log("my_metric", x)

    The log() method has a few options:

        on_step (logs the metric at that step in training)

        on_epoch (automatically accumulates and logs at the end of the epoch)

        prog_bar (logs to the progress bar)

        logger (logs to the logger like Tensorboard)

    Depending on where the log is called from, Lightning auto-determines the correct mode for you. But of course you can override the default behavior by manually setting the flags.
    '''

    def training_step(self, batch, batch_idx):
        loss, mse_loss = self.compute_loss_one_batch(batch, batch_idx)
        # mse_loss = torchmetrics.functional.mean_squared_error(
        # self.forward(batch[...]),
        # batch['y_data'])
        self.log('tr_loss', loss, batch_size=self.batch_size)
        self.log('tr_mse', mse_loss, batch_size=self.batch_size)
        # self.log('tr_mse', self.mse_loss, batch_size=self.batch_size)
        # loss is calculated in a strange way, can't verify that it's
        # equivalent to log-likelihood, and not needed for seq2seq anyway.
        # Switch to MSE for now.
        # return loss
        return mse_loss

    def validation_step(self, batch, batch_idx):
        loss, mse_loss = self.compute_loss_one_batch(batch, batch_idx)
        self.log('va_loss', loss, batch_size=self.batch_size)
        self.log('va_mse', mse_loss, batch_size=self.batch_size)
        # return loss
        return mse_loss

    # By default, the predict_step() method runs the forward() method.
    # TODO so dm.predict_dataloader should be different from the others

    def init_xy(self):
        pass

    def init_from_datamodule(self, dm):
        # def init_from_datamodule(self):
        # assume this is a pytorch_forecasting.TimeSeriesDataset under the hood
        # https://pytorch-forecasting.readthedocs.io/en/stable/api/pytorch_forecasting.data.timeseries.TimeSeriesDataSet.html
        # dm = self.trainer.datamodule

        # When using forward, you are responsible to call eval() and use the
        # no_grad() context manager.

        # > use prepare_data() to download and process the dataset.
        #   > Lightning ensures this method is called only within a single process,
        #   > so you can safely add your downloading logic within.
        # > use setup() to do splits, and build your model internals
        #   > set model/dm state here, it's called on every device
        # lightningmodule also has a setup(), move this there so it's autoinvoked

        # def setup(self, stage: Optional[str] = None):
        # # stage is either 'fit', 'validate', 'test', or 'predict'. 90% of the
        # # time not relevant
        # data = load_data()
        # num_classes = data.classes
        # self.l1 = nn.Linear(..., num_classes)

        dm.prepare_data()
        dm.setup(stage='train')
        batch = next(iter(dm.train_dataloader()))

        # x, y = batch
        # self.example_input_array = x
        # _, _, self.x_dims = x['encoder_target'].shape
        # _, _, self.y_dims = x['decoder_target'].shape
        self.example_input_array = (batch['y_time'], batch['x_data'],
                                    batch['x_time'])
        _, _, self.x_dims = batch['x_data'].shape
        _, _, self.y_dims = batch['y_data'].shape
        # import xdev; xdev.embed()

        # UserWarning: Trying to infer the `batch_size` from an ambiguous
        # collection. The batch size we found is 2. To avoid any
        # miscalculations, use `self.log(..., batch_size=batch_size)`.
        self.batch_size = dm.batch_size

        self.init_xy()


class DummyModel(BaseModel):

    def init_xy(self):
        self.net = create_net(self.x_dims, self.y_dims,
                              nonlinear=nn.ReLU).float()

    def old_forward(self, y_time, x, x_time):
        # import xdev; xdev.embed()
        y0 = self.net(x[:, -1, :])
        y = einops.repeat(y0, 'b f -> b t f', t=len(y_time))
        return y


class BaseData(pl.LightningDataModule):
    pass


def wrap_pf_dataloader(dl):
    '''
    Turn pytorch_forecasting's convoluted dataloader output into something more
    reasonable by overloading its collate_fn.
    '''

    # default_collate for comparison; TODO worker handling

    # elem = batch[0]
    # elem_type = type(elem)
    # if isinstance(elem, torch.Tensor):
    # out = None
    # if torch.utils.data.get_worker_info() is not None:
    # # If we're in a background process, concatenate directly into a
    # # shared memory tensor to avoid an extra copy
    # numel = sum(x.numel() for x in batch)
    # storage = elem.storage()._new_shared(numel, device=elem.device)
    # out = elem.new(storage).resize_(len(batch), *list(elem.size()))
    # return torch.stack(batch, 0, out=out)
    # # etc...

    def _wrap_collate(x, y):

        x_data = einops.rearrange(x['encoder_target'], 'f b t -> b t f')
        y_data = einops.rearrange(x['decoder_target'], 'f b t -> b t f')

        assert torch.unique(x['encoder_lengths']).shape == (1, )
        enc_len = x['encoder_lengths'][0]
        assert torch.unique(x['decoder_time_idx'][:, 0]).shape == (1, )
        dec_idx = x['decoder_time_idx'][0, 0]
        dec_len = min(x['decoder_lengths'])
        y_data = y_data[:, :dec_len, :]

        # 'decoder_lengths': tensor([10,  7]),
        # 'decoder_time_idx': tensor([[ 6,  7,  8,  9, 10, 11, 12, 13, 14, 15],
        # [ 6,  7,  8,  9, 10, 11, 12, 13, 14, 15]]),
        # These are irregular. Should they be truncated to the shortest in the
        # batch or is padding with zeros ok?
        # Now I see why orig codebase had a custom sampler to try and pick
        # batch entries with similar durations...
        # Going to truncate for now and try to learn static time_remaining
        # separately. TODO survival...

        # these are regular ints from pf, though nets could take irregular floats
        x_time = torch.arange(dec_idx - enc_len, dec_idx)
        y_time = torch.arange(dec_idx, dec_idx + dec_len)

        # (enc|dec)oder_target
        # float tensor with unscaled continous target or encoded
        # categorical target,
        # target_scale
        # (batch_size x scale_size or list thereof with each entry for a
        # different target):
        # scale_size == 2 regardless of enc/dec len, why? one for enc & dec?
        # docs say center & scale

        # normalize data
        # TODO check if this is correct and if BaseModel does it
        # TODO softplus?
        center, scale = einops.rearrange(x['target_scale'],
                                         'f b cs -> cs b 1 f',
                                         cs=2)
        x_data = (x_data - center) / scale
        y_data = (y_data - center) / scale

        return {
            'x_data': x_data,
            'y_data': y_data,
            'x_time': x_time,
            'y_time': y_time,
        }

    orig_collate = deepcopy(dl.collate_fn)

    def _collate(*args, **kwargs):
        return _wrap_collate(*orig_collate(*args, **kwargs))

    dl.collate_fn = _collate

    return dl


@dataclass
class HemorrhageVitals(BaseData):

    use_cache: bool = True

    # dataloader
    batch_size: int = 2  #200
    missing_rate: Optional[float] = None  # 0.7

    # common
    root_path: str = '/data/pulse/hemorrhage/hemorrhage'
    x_time_minutes: int = 20
    total_time_minutes_max: int = 120

    # split_train_val_test
    continue_params: List[str] = field(default_factory=lambda: [
        'HeartRate(1/min)', 'BloodVolume(mL)', 'CardiacOutput(mL/min)',
        'RespirationRate(1/min)', 'OxygenSaturation',
        'MeanArterialPressure(mmHg)', 'SystolicArterialPressure(mmHg)',
        'DiastolicArterialPressure(mmHg)', 'HemoglobinContent(g)',
        'TotalHemorrhageRate(mL/s)', 'TotalHemorrhagedVolume(mL)'
    ])
    # another meaningful difference from google stock: t_param is
    # per-timeseries (per group/pt), so don't have to avoid peeking ahead for val
    t_param: str = 'Time(s)'  # assume seconds

    # cut_and_split_data
    stride: int = 1
    train_fraq: float = 0.7
    val_fraq: Optional[float] = 0.15
    n_start: int = 0
    max_pts: Optional[int] = None

    # TRANS: Two kinds of architecture, using AutoEncoder need a small amount
    # of x reverse solving until t0
    # if arch in { 'Seq2Seq', 'VAE' }
    start_reverse: bool = False

    static_behavior: Literal['ignore', 'propagate', 'augment'] = 'ignore'
    use_pf_format: bool = True
    time_augmentation: bool = False
    num_workers: int = 4

    def __post_init__(self):
        super().__init__()
        self._prepare_data()
        self._setup()

    def _cache_dir(self):
        if not self.hparams:
            self.save_hyperparameters()
        hparam_str = ub.hash_data(dict(self.hparams))
        return (ub.Path(self.root_path) / 'cache' / hparam_str)

    def _prepare_data(self):
        cache_dir = self._cache_dir()
        if not cache_dir.exists():

            df, (dset_tr, dset_va, dset_te) = self._build_dsets()

            # save
            cache_dir.ensuredir()
            with open(cache_dir / 'hparams.json', 'w') as f:
                json.dump(self.hparams, f)
            dset_tr.save(cache_dir / 'dset_tr.pt')
            dset_va.save(cache_dir / 'dset_va.pt')
            dset_te.save(cache_dir / 'dset_te.pt')
            df.to_pickle(cache_dir / 'df.pkl')

    def _setup(self, stage: Optional[str] = None):
        cache_dir = self._cache_dir()
        assert cache_dir.exists()

        # load
        dset_tr = pf.TimeSeriesDataSet.load(cache_dir / f'dset_tr.pt')
        dset_va = pf.TimeSeriesDataSet.load(cache_dir / f'dset_va.pt')
        dset_te = pf.TimeSeriesDataSet.load(cache_dir / f'dset_te.pt')

        if stage == 'predict':
            self.dset_pr = pf.TimeSeriesDataSet.from_dataset(
                dset_te,
                # stop_randomization=True,
                # predict=True,
            )
        else:
            self.dset_tr = dset_tr
            self.dset_va = dset_va
            self.dset_te = dset_te

    def _build_dsets(
        self
    ) -> Tuple[pd.DataFrame, Tuple[pf.TimeSeriesDataSet, pf.TimeSeriesDataSet,
                                   pf.TimeSeriesDataSet]]:
        # does not assign to self.

        #
        # get patient entries
        #

        # TODO use patient_results.json manifest for id and init params
        csvs = sorted(ub.Path(self.root_path).glob('**/HemorrhageResults.csv'))
        if self.max_pts is not None:
            csvs = np.random.choice(csvs, (self.max_pts, ), replace=False)
        # multithread this
        print('loading patients...')
        exc = ub.Executor('thread', max_workers=32)

        def _job(csv):
            _id = json.load(csv.with_name('patient.json').open())['Name']
            _df = pd.read_csv(csv).assign(id=_id)[self.n_start::self.stride]
            return _df

        jobs = [exc.submit(_job, csv) for csv in csvs]
        dfs = [j.result() for j in jobs]
        df = pd.concat(dfs, ignore_index=True)
        print(f'found {len(dfs)} patients with {len(df)} timesteps')

        if self.static_behavior != 'ignore':
            raise NotImplementedError

        #
        # narrow down to long enough ones
        #

        # TODO variable x_time, survival analysis approach (right-censoring)
        # TODO see if pf can do this filtering https://stackoverflow.com/a/71874176
        df = df.groupby('id').filter(lambda x: (x[self.t_param].max() - x[
            self.t_param].min()) / 60 > self.x_time_minutes)
        n_pts = len(df.groupby('id'))
        print(
            f'filtered to tmax > {self.x_time_minutes}; {n_pts} patients remaining'
        )

        # TODO if not builtin to pf, use
        # https://scikit-learn.org/stable/modules/generated/sklearn.preprocessing.LabelEncoder.html
        # for static_unknown (end time w/o survival analysis)

        # TRANS: The autoregressive prediction, Autoencoder reverse
        # solution to h0 first, then forward solution to the last point
        if self.start_reverse:
            raise NotImplementedError

        #
        # handle sampling
        #

        # TODO add ASSUMED_FREQ_S as a static feature to enable training across
        # resolutions, or return it from the DL and fudge it some other way
        # don't know why dropna() is necessary here...
        periods = df.groupby(
            ['id'])[self.t_param].diff()[1:-1].dropna().round(6).unique()
        assert len(periods) == 1, periods
        ASSUMED_FREQ_S = periods[0]  # 0.05s
        x_points = int(60 * self.x_time_minutes / ASSUMED_FREQ_S)
        y_points = int(60 *
                       (self.total_time_minutes_max - self.x_time_minutes) /
                       ASSUMED_FREQ_S)
        print(f'{x_points} in timesteps, [1, {y_points}] out timesteps')
        # make t_param integer
        df[self.t_param] = (df[self.t_param] / ASSUMED_FREQ_S).astype(int)
        # make continue_params float
        df[self.continue_params] = df[self.continue_params].astype(float)

        #
        # train-test split
        #

        uids = df['id'].unique()
        idx = np.random.randn(len(uids))
        tr_idx = idx < self.train_fraq
        if self.val_fraq:
            va_idx = ((self.train_fraq <= idx) &
                      (idx < (self.train_fraq + self.val_fraq)))
            te_idx = idx >= (self.train_fraq + self.val_fraq)
        else:
            va_idx = idx >= self.train_fraq
            te_idx = va_idx
        # ensure each contains at least a full batch
        assert sum(tr_idx) >= self.batch_size
        if not sum(va_idx) >= self.batch_size:
            entries = np.where(tr_idx)[0][:(self.batch_size - sum(va_idx))]
            tr_idx[entries] = False
            va_idx[entries] = True
        if not sum(te_idx) >= self.batch_size:
            entries = np.where(tr_idx)[0][:(self.batch_size - sum(te_idx))]
            tr_idx[entries] = False
            te_idx[entries] = True

        df_tr = df[df['id'].isin(uids[tr_idx])]
        df_va = df[df['id'].isin(uids[va_idx])]
        df_te = df[df['id'].isin(uids[te_idx])]
        print('train, val, test timesteps: ', df_tr.shape, df_va.shape,
              df_te.shape)

        #
        # build datasets
        #

        # TODO add lag if it's not already here - is onset or measurement t=0?

        # change this to GroupNormalizer (on encoder values only?) when
        # adding sliding-time-window data augmentation ie multiple
        # samples per group
        # what does: (on which overfitting tests will fail) mean?

        # have to fit normalizers outside of dset
        # https://github.com/jdb78/pytorch-forecasting/issues/605
        # categorical_encoders={
                # 'product_id': NaNLabelEncoder().fit(df.product_id),
                # 'shop_id': NaNLabelEncoder().fit(df.shop_id),
                # 'product_type': NaNLabelEncoder().fit(df.product_type)
            # },

        target_normalizer = pf.MultiNormalizer(
            [pf.EncoderNormalizer(transformation='softmax')] *
            len(self.continue_params))
        kwargs = dict(
            time_idx=self.t_param,
            # mutitarget returns a list because the targets may have different dtypes. How to get around this besides fixing in dataloader? Multiindex? pd.Series of vectors?
            target=self.continue_params,
            group_ids=['id'],
            min_encoder_length=x_points,
            min_prediction_length=1,
            max_prediction_length=
            y_points,  # does this have to be masked or truncated?
            # this is duped w/ different normalizer between target and cont
            # worth doing because target doesn't come normalized in dataloader
            time_varying_unknown_reals=self.continue_params,
            time_varying_known_reals=[],
            time_varying_unknown_categoricals=[],
            time_varying_known_categoricals=[],
            static_categoricals=[],
            static_reals=[],
            add_relative_time_idx=False,
            # this means add to static features; already an entry in x
            # add_target_scales=True,  # TODO figure out how to add static feats
            add_target_scales=False,
            add_encoder_length=False,
        )
        if not self.time_augmentation:
            kwargs.update(
                max_encoder_length=x_points,  # increase w/ min_prediction_idx
                # how to predict_mode but only 1st sample instead of last?
                # should be ok as is because max_prediction_length is greedy
                # turn this off for sliding-time-window
                predict_mode=True,
                # turn this off for sliding-time-window?
                min_prediction_idx=x_points,
                # target_normalizer=target_normalizer,
                target_normalizer='auto',
                # if not randomized, this happens; dec_len = minimax over batch samples
                # decoder_length = self.calculate_decoder_length(time[-1], sequence_length)
                # encoder_length = sequence_length - decoder_length
                # turn this on for sliding-time-window
                randomize_length=False,
            )
        else:
            raise NotImplementedError

        dset_tr = TimeSeriesDataSet(df_tr, **kwargs)
        dset_va = TimeSeriesDataSet(
            df_va,
            **kwargs,
        )
        # predict=True,
        # stop_randomization=True)
        dset_te = TimeSeriesDataSet(
            df_te,
            **kwargs,
        )
        # predict=True,
        # stop_randomization=True)

        # this would be the approach if there were new entries from the
        # existing groups (patients) at val/test time. But since group id is
        # used for normalization, can't extend existing dset to new groups
        # without treating them as NaN.
        # Instead, create va/te independently.

        # dset_va = TimeSeriesDataSet.from_dataset(
        # dset_tr,
        # data=df_va,
        # predict=True,
        # stop_randomization=True)

        # dset_te = TimeSeriesDataSet.from_dataset(
        # dset_tr,
        # data=df_te,
        # predict=True,
        # stop_randomization=True)

        # TODO missingness

        # TODO y_delay (per batch), y_dim_list (?)

        return df, (dset_tr, dset_va, dset_te)

    def train_dataloader(self):
        # can remove batch_sampler='synchronized' with irregular/masking support
        # (and should write a custom sampler - "TimeBounded"? - to ensure there
        # is some temporal overlap between batch entries)
        # (and that lengths are similar so that not too much is cut off)
        dl = self.dset_tr.to_dataloader(train=True,
                                        batch_size=self.batch_size,
                                        batch_sampler='synchronized',
                                        num_workers=self.num_workers)
        if self.use_pf_format:
            return dl
        else:
            return wrap_pf_dataloader(dl)

    def val_dataloader(self):
        dl = self.dset_va.to_dataloader(train=False,
                                        batch_size=self.batch_size * 10,
                                        batch_sampler='synchronized',
                                        num_workers=self.num_workers)
        if self.use_pf_format:
            return dl
        else:
            return wrap_pf_dataloader(dl)

    def test_dataloader(self):
        dl = self.dset_te.to_dataloader(train=False,
                                        batch_size=self.batch_size * 10,
                                        batch_sampler='synchronized',
                                        num_workers=self.num_workers)
        if self.use_pf_format:
            return dl
        else:
            return wrap_pf_dataloader(dl)


    def predict_dataloader(self):

        def wrap_predict_dl(dl):

            def _wrap_collate(x, y):
                return x

            orig_collate = deepcopy(dl.collate_fn)

            def _collate(*args, **kwargs):
                return _wrap_collate(*orig_collate(*args, **kwargs))

            dl.collate_fn = _collate

            return dl

        dl = self.dset_te.to_dataloader(train=False,
                                        batch_size=self.batch_size * 10,
                                        batch_sampler='synchronized',
                                        num_workers=self.num_workers)
        if self.use_pf_format:
            return wrap_predict_dataloader(dl)
        else:
            return wrap_pf_dataloader(dl)


@dataclass
class GoogleStock(BaseData):
    '''
    pytorch-forecasting version.
    '''

    # dataloader
    batch_size: int = 2  #200
    missing_rate: Optional[float] = 0.7

    # common
    csv_path: str = 'data/google_stock/data.csv'
    x_y_points: Optional[int] = 4  #25
    x_points: Optional[int] = 3  #24

    # split_train_val_test
    continue_params: List[str] = field(
        default_factory=lambda:
        ['Open', 'High', 'Low', 'Close', 'Adj Close', 'Volume'])
    parse_dates: List[str] = field(default_factory=lambda: ['Date'])
    t_param: Optional[str] = 'Date'

    # cut_and_split_data
    csv_rel_fpath: Optional[str] = 'data.csv'
    stride: int = 1
    train_fraq: float = 0.7
    val_fraq: Optional[float] = 0.15
    shuffle: bool = True
    n_start: int = 0

    # TRANS: Two kinds of architecture, using AutoEncoder need a small amount
    # of x reverse solving until t0
    # if arch in { 'Seq2Seq', 'VAE' }
    start_reverse: bool = False

    def __post_init__(self):
        self.csv_path = ub.Path(self.csv_path)

    # ENTRYPOINT
    def setup(self, stage: Optional[str] = None):

        # TRANS: Replace the missing value:
        # https://blog.csdn.net/qq_18351157/article/details/104993254,
        # here to get rid of the missing value corresponding to the point
        df = pd.read_csv(self.csv_path, header=0).dropna(how='any')

        # TRANS: Converts time variable to a floating point number
        if self.parse_dates:

            pd_timestamp = df[self.parse_dates].apply(
                lambda x: pd.to_datetime(x, infer_datetime_format=True))
            pd_timestamp = ((pd_timestamp - pd_timestamp.iloc[0]) //
                            pd.Timedelta(1, 'd')).fillna(0).astype(int)
            df[self.parse_dates] = pd_timestamp

        #TRANS: Add the time sequence x
        # for col in self.parse_dates:
        # df[col + '2'] = df[col]
        # time_varying_unknown_reals.append(col + '2')

        #TRANS: TODO: data processing tag, add it to the scalar?
        # https://blog.csdn.net/qq_32863549/article/details/106972347
        # https://scikit-learn.org/stable/modules/generated/sklearn.preprocessing.LabelEncoder.html?highlight=labelencoder#sklearn.preprocessing.LabelEncoder
        # TODO label_dict?
        # if self.discrete_params is not None:  #TRANS: To deal with tags inside the CSV data, such as wind, finally to merge into the tensor_data, in addition to save a label map?
        # pd_discrete_data = origin_pd_data[self.discrete_params]

        if self.x_y_points is None:
            self.x_y_points = len(df) // 100
        # df['group'] = np.repeat(range(np.ceil(len(df) / x_y_points)), x_y_points)
        df['group'] = 1

        df = df[self.n_start::self.stride]

        # we don't want to impute missing timesteps, so ignore pf's time_idx
        # support and just pretend it's a feature
        df['time_idx'] = df.index

        # TRANS: The autoregressive prediction, Autoencoder reverse
        # solution to h0 first, then forward solution to the last point
        if self.start_reverse:
            raise NotImplementedError
            #TRANS: The linear small
            # x_data_until = split_dict['x_data'][:, :1, :] - 1 / 10 * (
            # split_dict['x_data'][:, 1:2, :] -
            # split_dict['x_data'][:, :1, :])
            # x_time_until = split_dict['x_time'][:, :1, :] - 1 / 10 * (
            # split_dict['x_time'][:, 1:2, :] -
            # split_dict['x_time'][:, :1, :])
            # split_dict['x_data'] = torch.cat(
            # (split_dict['x_data'].flip(dims=[1]), x_data_until), dim=1)
            # split_dict['x_time'] = torch.cat(
            # (split_dict['x_time'].flip(dims=[1]), x_time_until), dim=1)

        # unused
        # train_point = int(len(df) * self.train_fraq)
        # val_point = int(len(df) * (self.val_fraq + self.train_fraq))

        tr_cutoff = int((df[self.t_param].max() - df[self.t_param].min()) *
                        self.train_fraq)
        va_cutoff = int((df[self.t_param].max() - df[self.t_param].min()) *
                        (self.train_fraq + self.val_fraq))
        df_tr = df[lambda x: x[self.t_param] <= tr_cutoff]
        df_va = df[lambda x: x[self.t_param] <= va_cutoff]

        if self.x_points is not None:
            x_points = min(self.x_points, self.x_y_points - 1)
            x_points_tr, x_points_va, x_points_te = [x_points] * 3
        else:
            # TRANS: Regression: 1/2 relationship of default in accordance
            # with the observation point (input) and the observation point
            # (output)
            x_points_tr = len(df_tr) // 2
            x_points_va = len(df_va) // 2
            x_points_te = len(df) // 2

        y_points = self.x_y_points - x_points

        self.dset_tr = TimeSeriesDataSet(
            df_tr,
            # time_idx=self.t_param,
            time_idx='time_idx',
            target=self.continue_params,
            group_ids=['group'],
            min_encoder_length=x_points_tr,
            max_encoder_length=x_points_tr,
            min_prediction_length=y_points,
            max_prediction_length=y_points,
            time_varying_unknown_reals=self.continue_params,
            time_varying_known_reals=self.parse_dates,
            time_varying_unknown_categoricals=[],
            time_varying_known_categoricals=[],
            static_categoricals=[],
            static_reals=[],
            add_relative_time_idx=True,
            # add_target_scales=True,  # TODO figure out how to add static feats
            add_target_scales=False,
            add_encoder_length=False,
            target_normalizer=pf.data.MultiNormalizer(
                [pf.data.EncoderNormalizer(transformation=None)] *
                len(self.continue_params)))

        self.dset_va = TimeSeriesDataSet.from_dataset(
            self.dset_tr,
            data=df_va,
            predict=True,
            stop_randomization=True,
            min_encoder_length=x_points_va,
            max_encoder_length=x_points_va)

        self.dset_te = TimeSeriesDataSet.from_dataset(
            self.dset_tr,
            data=df,
            predict=True,
            stop_randomization=True,
            min_encoder_length=x_points_te,
            max_encoder_length=x_points_te)

        # TODO missingness

        # TODO y_delay (per batch), y_dim_list (?)

    def train_dataloader(self):
        return self.dset_tr.to_dataloader(
            train=True,
            batch_size=self.batch_size,
            # batch_sampler='synchronized',
            num_workers=4)

    def val_dataloader(self):
        return self.dset_va.to_dataloader(
            train=False,
            batch_size=self.batch_size * 10,
            # batch_sampler='synchronized',
            num_workers=4)

    def test_dataloader(self):
        return self.dset_te.to_dataloader(
            train=False,
            batch_size=self.batch_size * 10,
            # batch_sampler='synchronized',
            num_workers=4)

    def predict_dataloader(self):
        return self.dset_te.to_dataloader(train=False,
                                          batch_size=self.batch_size * 10,
                                          batch_sampler='synchronized',
                                          num_workers=4)


@dataclass
class ProcessDataset():

    # dataloader
    batch_size: int
    missing_rate: Optional[float]

    # common
    dataset: str
    load_data: bool = True
    ode_time: Literal['Concat', 'Mean', 'No'] = 'No'
    produce_intercoeffs: bool = False
    x_data_with_time: bool = False
    x_y_points: Optional[int] = None
    x_points: Optional[int] = None
    y_delay: int = 0
    y_dim_list: Optional[List[int]] = None

    # split_train_val_test
    csv_list: Optional[List[str]] = None
    continue_params: Optional[List[str]] = None
    discrete_params: Optional[List[str]] = None
    parse_dates: Optional[List[str]] = None
    glob_params: Optional[List[str]] = None

    # cut_and_split_data
    stride: int = 1
    train_fraq: float = 0.8
    val_fraq: Optional[float] = None
    shuffle: bool = True
    n_start: int = 0

    # ENTRYPOINT
    def get_data(self, arch: str):
        data_pkl_file = ub.Path('data') / self.dataset / 'All_Data.pkl'
        #TRANS: Not the interpolation and Concat, only can be read directly
        if self.load_data and os.path.isfile(
                data_pkl_file
        ) and self.ode_time != 'Concat' and not self.produce_intercoeffs:
            return load_pickle(data_pkl_file)
        else:
            data_dict = self._split_train_val_test(self.dataset)
            train_dict, x_dims, y_dims = self._split_x_y(
                data_dict, 'train', arch)
            val_dict, _, _ = self._split_x_y(data_dict, 'val', arch)
            test_dict, _, _ = self._split_x_y(data_dict, 'test', arch)

            #TRANS: Not the interpolation and Concat, can be stored here
            if self.ode_time != 'Concat' and not self.produce_intercoeffs:
                data = (train_dict, val_dict, test_dict, x_dims, y_dims)
                save_items_to_pickle(data, data_pkl_file)
            #TRANS: TODO: Concat set batch_size after reading to the corresponding value = 1
        return train_dict, val_dict, test_dict, x_dims, y_dims

    def _split_train_val_test(self, dataset):
        #TRANS: To extract data from the original file and is divided into training set and test set (verification), pay attention to the new tensor. The device
        if self.csv_list is None:  #TRANS: From the train. The pt and test. Pt in batch data extracted directly, not reorder
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
            if os.path.isfile(
                    'data/' + dataset + '/val.pt'
            ):  #TRANS: Validation set, used to adjust the lr and other parameters
                val = torch.load('data/' + dataset + '/val.pt')
                val_time = val[:, :, :1]
                val_data = val[:, :, 1:]
                data_dict['val_data'] = val_data
                data_dict['val_time'] = val_time
            else:
                data_dict['val_data'] = None
                data_dict['val_time'] = None
            return data_dict
        else:  #TRANS: Read data from CSV, segmentation and classification training set/test set (training set and testing set in the same file)
            train_data = []
            val_data = []
            test_data = []
            train_time = []
            val_time = []
            test_time = []

            data_min = []
            data_max = []

            #TRANS: Because the Numpy support for string is not very good, so use nested list directly, must use 2 slices
            train_glob_list = []
            val_glob_list = []
            test_glob_list = []
            for csv_file in self.csv_list:
                csv_path = 'data/' + dataset + '/' + csv_file + '.csv'
                #TRANS: Replace the missing value: https://blog.csdn.net/qq_18351157/article/details/104993254, here to get rid of the missing value corresponding to the point
                origin_pd_data = pd.read_csv(csv_path,
                                             header=0).dropna(how='any')

                if self.continue_params is not None:
                    pd_continue_data = origin_pd_data[self.continue_params]
                else:
                    pd_continue_data = origin_pd_data  #TRANS: Read all labels, all, applicable to CSV file all variables are needed

                if self.parse_dates is not None:  #TRANS: Converts time variable to a floating point number
                    pd_timestamp = pd.to_datetime(
                        origin_pd_data[self.parse_dates],
                        infer_datetime_format=True)
                    pd_timestamp = (
                        (pd_timestamp - pd_timestamp.iloc[0]) /
                        pd.Timedelta(1, 'd')
                    ).fillna(0).astype(
                        float
                    )  #TRANS: Each file time starting from 0, 1 day as unit 1
                    pd_timestamp = pd_timestamp.rename('time')
                    pd_continue_data = pd.concat(
                        [pd_timestamp, pd_continue_data], axis=1
                    )  #TRANS: Continue_data to guarantee on time as the first column

                #TRANS: TODO: data processing tag, add it to the scalar?
                # https://blog.csdn.net/qq_32863549/article/details/106972347
                # https://scikit-learn.org/stable/modules/generated/sklearn.preprocessing.LabelEncoder.html?highlight=labelencoder#sklearn.preprocessing.LabelEncoder
                # TODO label_dict?
                if self.discrete_params is not None:  #TRANS: To deal with tags inside the CSV data, such as wind, finally to merge into the tensor_data, in addition to save a label map?
                    pd_discrete_data = origin_pd_data[self.discrete_params]

                tensor_data = torch.tensor(data=pd_continue_data.values)

                #TRANS: Validation sets empty cases, the returned data and time are None
                (
                    sw_train_data, sw_val_data, sw_test_data, sw_train_time,
                    sw_val_time, sw_test_time, csv_data_min, csv_data_max
                ) = self._cut_and_split_data(
                    tensor_data
                )  #TRANS: In the training set and validation set according to shuffle
                train_data.append(sw_train_data)
                test_data.append(sw_test_data)
                train_time.append(sw_train_time)
                test_time.append(sw_test_time)
                val_data.append(sw_val_data)
                val_time.append(sw_val_time)
                data_min.append(csv_data_min)
                data_max.append(csv_data_max)

                if self.glob_params is not None:  #TRANS: Used to handle the whole time series (including training and test sets) are the same data, such as sampling sites
                    glob_item = origin_pd_data[
                        self.glob_params].iloc[0].values.tolist(
                        )  #TRANS: This batch of the values are the same
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

            if self.discrete_params is not None:  #TRANS: Additional label data
                raise NotImplementedError

            if val_data[0] is not None:  #TRANS: Additional validation set
                val_data = torch.cat(val_data, dim=0)
                val_time = torch.cat(val_time, dim=0)
                data_dict['val_data'] = val_data
                data_dict['val_time'] = val_time
            else:
                data_dict['val_data'] = val_data
                data_dict['val_time'] = val_time

            if self.glob_params is not None:  #TRANS: Additional global data
                data_dict['train_glob_list'] = train_glob_list
                data_dict['val_glob_list'] = val_glob_list
                data_dict['test_glob_list'] = test_glob_list

            pickle_data = (
                self.x_data_with_time,
                pd_continue_data.columns.tolist() if self.x_data_with_time else
                pd_continue_data.columns.tolist()[1:], data_min, data_max
            )  #TRANS: TODO: save the mapping information
            save_items_to_pickle(pickle_data,
                                 'data/' + dataset + '/DataInfo.pkl')

            return data_dict

    def _cut_and_split_data(self, data):
        #TRANS: Dividing a sliding window
        if self.x_y_points is None:
            self.x_y_points = data.shape[0] // 100
        device = get_device(data)

        if self.x_data_with_time:
            data, data_min, data_max = normalize_data_minmax(data)
        else:
            data[:, 1:], data_min, data_max = normalize_data_minmax(
                data[:, 1:])  #TRANS: The normalize set to min - Max
        assert not torch.isnan(data).any()
        assert not torch.isnan(data_min).any()
        assert not torch.isnan(data_max).any()

        data_new = data[self.n_start:].clone().detach().unsqueeze(0)
        assert self.x_y_points < data_new.shape[1] - 1
        for i in range(1, self.x_y_points):
            zero_tensor = torch.zeros((i, data.shape[1]), device=device)
            tmp_tensor = torch.vstack(
                (data[i + self.n_start:], zero_tensor)).unsqueeze(0)
            data_new = torch.cat((data_new, tmp_tensor))
        data_new = data_new.permute(1, 0, 2)
        data_new = data_new[:-(
            self.x_y_points -
            1)]  #TRANS: Cut off the end of a few slices with zero component
        if self.stride > 1 and data_new.shape[0] > self.stride:
            data_select = data_new[0].unsqueeze(0)
            for j in range(self.stride, data_new.shape[0], self.stride):
                data_select = torch.cat(
                    (data_select, data_new[j].unsqueeze(0)))
            data_new = data_select
        if self.shuffle:  #TRANS: Reordering of sliding window
            idx = torch.randperm(data_new.shape[0])
            data_new = data_new[idx]

        time = data_new[:, :, :1]
        data = data_new[:, :, 1:]

        (train_data, val_data, test_data, train_time, val_time,
         test_time) = self._split_train_test_data_and_time(data, time)
        return (train_data, val_data, test_data, train_time, val_time,
                test_time, data_min, data_max)

    def _split_train_test_data_and_time(self, data, time):
        #TRANS: Divided into training set and test set, 1 d to divided the time period
        #TRANS: Regression test suite should overall is unknown, the first step from the training last time as input, and then gradually replace data_test and test_time segmentation
        data_size = data.shape[0]
        assert len(time.shape) == 3

        if self.val_fraq is None:
            train_point = int(data_size * self.train_fraq)
            train_data = data[:train_point]
            train_time = time[:train_point]
            test_data = data[train_point:]
            test_time = time[train_point:]
            val_data = None
            val_time = None
        else:
            train_point = int(data_size * self.train_fraq)
            val_point = int(data_size * (self.val_fraq + self.train_fraq))
            train_data = data[:train_point]
            train_time = time[:train_point]
            val_data = data[train_point:val_point]
            val_time = time[train_point:val_point]
            test_data = data[val_point:]
            test_time = time[val_point:]
        return train_data, val_data, test_data, train_time, val_time, test_time

    def _split_x_y(self, data_dict, data_type: Literal['train', 'val', 'test'],
                   arch: str):
        if (data_type + '_time' not in data_dict
                or data_dict[data_type + '_time'] is None):
            return None
        if self.x_points is not None:
            x_points = min(self.x_points, self.x_y_points - 1)
        else:
            x_points = self.x_points
        #TRANS: The autoregressive prediction, Autoencoder reverse solution to h0 first, then forward solution to the last point
        if self.y_dim_list is not None:
            y_dims = len(self.y_dim_list)
            all_dim_list = list(range(data_dict[data_type +
                                                '_data'].shape[-1]))
            x_dim_list = list(
                set(all_dim_list).difference(set(self.y_dim_list)))
            x_dims = len(x_dim_list)
            split_dict = {
                'x_data':
                data_dict[data_type + '_data'][:, :, x_dim_list].clone(),
                'x_time':
                data_dict[data_type + '_time'].clone(),
                'y_data':
                data_dict[data_type + '_data'][:, :, self.y_dim_list].clone(),
                'y_time':
                data_dict[data_type + '_time'].clone()
            }
            if arch in {
                    'Seq2Seq', 'VAE'
            }:  #TRANS: Two kinds of architecture, using AutoEncoder need a small amount of x reverse solving until t0
                #TRANS: The linear small
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
                #TRANS: Regression: 1/2 relationship of default in accordance with the observation point (input) and the observation point (output)
                x_points = data_dict[data_type + '_data'].shape[1] // 2
            #TRANS: Y_delay is used to predict in advance, the y move back from the total length of time
            split_dict = {
                'x_data':
                data_dict[data_type + '_data'][:, :x_points, :].clone(),
                'x_time':
                data_dict[data_type + '_time'][:, :x_points, :].clone(),
                'y_data':
                data_dict[data_type + '_data'][:, x_points +
                                               self.y_delay:, :].clone(),
                'y_time':
                data_dict[data_type + '_time'][:, x_points +
                                               self.y_delay:, :].clone()
            }
            x_dims = split_dict['x_data'].shape[-1]
            y_dims = split_dict['y_data'].shape[-1]

        if self.x_data_with_time:  #TRANS: Add the time sequence x
            split_dict['x_data'] = torch.cat(
                (split_dict['x_time'], split_dict['x_data']), dim=2)
            x_dims += 1

        return split_dict, x_dims, y_dims

    #TRANS: In the training process the data
    # ENTRYPOINT
    def masked_dataloader(self,
                          data_dict,
                          data_name='train',
                          missing_rate=None,
                          pre_points=None) -> torch.utils.data.DataLoader:
        if missing_rate is None:
            missing_rate = self.missing_rate
        if data_dict is None:
            return None
        if self.produce_intercoeffs:
            file_detail = data_name + '_' + str(int(missing_rate * 100) if missing_rate is not None else 0) + '_' + \
                str(pre_points if pre_points is not None else 0) + '_' + \
                (ode_time if ode_time is not None else 'Default') + '_' + \
                ('with_time' if self.x_data_with_time else 'without_time')
            coeffs_save_file = ub.Path(
                'data') / dataset / 'coeffs_' + file_detail + '.pkl'
            if coeffs_save_file.exists and load_data:
                return load_pickle(coeffs_save_file)
        # TODO: curriculum learning for train data
        #TRANS: 1. From the partition of the input for the sampling, and will join the new dict mask, 50 points by default
        data = data_dict['x_data'].clone()
        device = get_device(data)
        mask = torch.ones_like(
            data, dtype=torch.bool, device=device
        )  #TRANS: With bool type, the first to generate batch change again when the float value

        if missing_rate is not None:
            time_points = len(data_dict['x_time'][0, :, 0])
            missing_points = min(int(time_points * 0.9),
                                 int(time_points * missing_rate))
            #TRANS: For each slice, random time_points * missing_rate inactivation
            for i in range(data.shape[0]):
                missing_idx = sorted(
                    np.random.choice(np.arange(time_points),
                                     missing_points,
                                     replace=False))
                mask[i, missing_idx, :] = False

        #TRANS: Update the data and mask
        new_data_dict = {}
        new_data_dict['x_data'] = data.clone()
        new_data_dict['x_time'] = data_dict['x_time'].clone()
        new_data_dict['x_mask'] = mask.clone()

        #TRANS: 2. Use pre_points truncate the output
        data = data_dict['y_data'].clone()
        mask = torch.ones_like(
            data, dtype=torch.bool,
            device=device)  #TRANS: Switch to a bool type, direct judgment

        if pre_points is not None:
            pre_points = min(pre_points, len(data_dict['y_time'][0, :, 0]))
            mask[:, pre_points:, :] = False

        new_data_dict['y_data'] = data.clone()
        new_data_dict['y_time'] = data_dict['y_time'].clone()
        new_data_dict['y_mask'] = mask.clone()

        if self.produce_intercoeffs:
            self._produce_and_save_intercoeffs(new_data_dict, coeffs_save_file)

        return self._wrap_dataloader(new_data_dict,
                                     batch_size=min(
                                         self.batch_size,
                                         new_data_dict['x_data'].shape[0]))

    def _produce_and_save_intercoeffs(self, data_dict, coeffs_save_file):
        #TRANS: Calculation and save the interpolation, the use of DataLoader save DataLoader
        data_dict['x_data'][~data_dict['x_mask']] = torch.nan
        data_dict['x_data'] = torchcde.natural_cubic_coeffs(
            data_dict['x_data'])
        dataloader = self._wrap_dataloader(data_dict,
                                           batch_size=min(
                                               self.batch_size,
                                               data_dict['x_data'].shape[0]),
                                           shuffle=self.shuffle)
        save_items_to_pickle(dataloader, coeffs_save_file)

    def _wrap_dataloader(self, data_dict, **kwargs):
        #TRANS: Use the Dataloader
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


#TRANS: The definition and measurement model
def get_params_num(model):
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
               return_dim=False,
               init=True):
    if output_dims is None:
        output_dims = input_dims

    net = nn.Sequential(
        nn.Linear(input_dims, n_units),
        *([nonlinear(), nn.Linear(n_units, n_units)] * hidden_layers),
        nonlinear(), nn.Linear(n_units, output_dims))

    if init:
        init_network_weights(net)

    if return_dim:
        return net, input_dims, output_dims
    else:
        return net


def split_last_dim(data):
    # TRANS: Bisect tensor last dimension, in order to distinguish between the
    # mean and STD
    last_dim = data.shape[-1]
    last_dim = last_dim // 2
    return data[..., :last_dim], data[..., last_dim:]


def init_network_weights(net, std=0.1):
    #TRANS: Initialize the gaussian distribution
    #TRANS: TODO: if use Xavier initialization?
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
    assert start.shape == end.shape

    if len(start.shape) == 0:
        # start and end are scalars
        return torch.linspace(start, end, n_points, device=device)
    if len(start.shape) == 1:
        # start and end are vectors
        return torch.stack([
            torch.linspace(s, e, n_points, device=device)
            for s, e in zip(start, end)
        ],
                           dim=0).T
    raise NotImplementedError


def time_sync_batch_dict(batch_list, ode_time=None, from_pf_format=False):
    batch_dict = {}
    if from_pf_format:
        x, (y, _) = batch_list
        batch_dict['x_time'] = x['encoder_cont'][..., -1]
        batch_dict['x_data'] = x['encoder_cont'][..., :-1]
        batch_dict['x_mask'] = torch.ones_like(batch_dict['x_data'],
                                               dtype=torch.bool)
        batch_dict['y_time'] = x['decoder_time_idx']
        batch_dict['y_data'] = y
        batch_dict['y_mask'] = torch.ones_like(y, dtype=torch.bool)
    else:
        (batch_dict['x_time'], batch_dict['x_data'], batch_dict['x_mask'],
         batch_dict['y_time'], batch_dict['y_data'],
         batch_dict['y_mask']) = batch_list

        #TRANS: Time synchronization in the batch
        if ode_time is not None:
            if ode_time == 'Concat':
                concat_batch(batch_dict)
            elif ode_time == 'Mean':
                #TRANS: The average directly, reduced to 1 d form
                batch_dict['x_time'] = torch.mean(batch_dict['x_time'],
                                                  dim=0).squeeze()
                batch_dict['y_time'] = torch.mean(batch_dict['y_time'],
                                                  dim=0).squeeze()
            elif ode_time == 'No':
                #TRANS: Take points, starting from 0
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
    #TRANS: Whether you need batch_dict [' x_time] [0, 0, 0]!
    batch_size = batch_dict['x_data'].shape[0]
    #TRANS: Stitching plan 1, generated batch_size extension tensor, extremely time-consuming # # # # # # # # TODO: modify splicing
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
    #TRANS: Whether you need batch_dict [' y_time] [0, 0, 0]!
    _, time_points, y_dims = batch_dict['y_data'].shape
    y_sync_time = torch.unique(batch_dict['y_time'][:, :, 0],
                               sorted=True).flatten()
    y_sync_time_points = y_sync_time.shape[0]
    y_sync_data = torch.zeros((batch_size, y_sync_time_points, y_dims),
                              device=device)
    y_sync_mask = torch.zeros_like(
        y_sync_data,
        dtype=torch.bool)  #TRANS: The y value, its mask is a Boolean value
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
        return  #TRANS: Without stitching


#TRANS: Update the superparametric
def update_learning_rate(optimizer, decay_rate=0.999, lowest=1e-3):
    for param_group in optimizer.param_groups:
        lr = param_group['lr']
        lr = max(lr * decay_rate, lowest)
        param_group['lr'] = lr


#TRANS: Used for testing
def store_best_results(model, test_dataloader, kl_coef, experimentID,
                       res_files, ode_time, model_name):
    test_res = compute_loss_all_batches(model,
                                        test_dataloader,
                                        kl_coef,
                                        ode_time=ode_time)

    #TRANS: Check whether the corresponding value is updated, pay attention to each set of measurements metric_name requires a separate file
    res_dict, value_updated = update_metric_if_larger(
        res_files['best_results'], model_name, 'negative_MSE',
        -test_res['mse'].item())
    #TRANS: If after the update, change the best ID
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

    #TRANS: Take the average
    if n_test_batches > 1:
        for key, _ in res_dict_mean.items():
            res_dict_mean[key] = res_dict_mean[key] / n_test_batches

    return res_dict_mean


def update_metric_if_larger(results_file_name, model_name, metric_name, value):
    if not os.path.isfile(results_file_name) or os.path.getsize(
            results_file_name
    ) == 0:  #TRANS: Does not exist or the file is empty
        with open(results_file_name, 'w') as f:
            f.write('model,' + metric_name)

    res_dict = pd.read_csv(results_file_name, sep=',')
    if metric_name not in res_dict.columns:
        res_dict[metric_name] = None

    value_updated = False

    if isinstance(value, torch.Tensor):
        #TRANS: Value = value. (CPU). Numpy # () to convert the amount to compare value type
        value = value.item()

    if (res_dict['model'] == model_name).any():
        prev_value = res_dict.loc[res_dict['model'] == model_name, metric_name]
        larger_than_prev = False
        if prev_value is not None and prev_value.item() is not None:
            larger_than_prev = float(prev_value.item()) < value
        if (prev_value.item() is
                None) or math.isnan(prev_value) or larger_than_prev:
            res_dict.loc[res_dict['model'] == model_name, metric_name] = float(
                value)  #TRANS: Update the corresponding form
            value_updated = True
    else:  #TRANS: Add the corresponding model results
        res_dict = pd.concat([
            res_dict,
            pd.DataFrame({
                'model': [model_name],
                metric_name: [value]
            })
        ],
                             ignore_index=True)
        value_updated = True

    with open(results_file_name, 'w',
              newline='') as csvfile:  #TRANS: Rewrite the corresponding file
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

    if (res_dict['model'] == model_name
        ).any():  #TRANS: Is updated directly, do not need to size
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
    #TRANS: Auxiliary processing data
    data_min = torch.min(data, 0).values
    data_max = torch.max(data, 0).values
    numerator = data - data_min
    denominator = data_max - data_min
    norm_data = numerator / (denominator + 1e-7
                             )  #TRANS: Join the decimal avoid divided by zero
    return norm_data, data_min, data_max


#TRANS: Save the checkpoint and results
def get_logger_and_save(model,
                        experimentID,
                        dataset_str,
                        experiment_str,
                        save_log=False,
                        save_fig_per_test=0,
                        save_ckpt=False,
                        save_res_for_epochs=None):

    experiment_str += f'_{int(experimentID):06}'
    if save_log:
        log_saveat = (ub.Path('log') / experiment_str).ensuredir()
        log_file = (log_saveat / (experiment_str + '.log')).touch()
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

    print('ExperimentID: ' + experimentID
          )  #TRANS: See this print experiment has produced results file
    #TRANS: Message, warning will be output to the console and record to a file, info not output to the console
    logger = get_logger(log_file)
    note = ub.paragraph('''

                        arch, solver, dset, x_pts, y_pts, missing_pct, time
                        {experiment_str}

        ''')
    logger.warning(note)
    logger.warning('All args are: ' + ub.repr2(cf.log()))
    #TRANS: training result recorder
    if save_res_for_epochs is not None and save_log:
        train_res_csv = save_csv(str(log_saveat), 'train')
        val_res_csv = save_csv(str(log_saveat), 'val')
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
    #TRANS: Only get the corresponding state of the model
    if model is not None:
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
        self.csv_saveat = csv_saveat  #TRANS: And the log in a folder
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
                self.no_head = False  #TRANS: Just write a header
            csv_writer.writerow(row)
        new_filepath = self.csv_saveat + '/' + self.data_type + '_' + str(
            epoch) + '.csv'
        os.rename(self.filepath, new_filepath)
        self.filepath = new_filepath  #TRANS: Update the epoch number information


#TRANS: Pickled version
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

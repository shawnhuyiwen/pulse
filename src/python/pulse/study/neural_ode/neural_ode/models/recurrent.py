import torch
import torch.nn as nn
from torchcde import CubicSpline, hermite_cubic_coefficients_with_backward_differences
import neural_ode.models.utils as utils
from neural_ode.models.evaluation import get_log_likelihood, get_mse
from neural_ode.models.diff_func import ODEFunc, CDEFunc, ODEFuncParams, CDEFuncParams
from neural_ode.models.utils import MISSING, BaseModel
import einops
import pytorch_forecasting as pf
import pytorch_lightning as pl
from dataclasses import dataclass, InitVar, fields, asdict
from typing import Optional, Union, Literal
import ubelt as ub


class GRU_with_std(nn.Module):

    def __init__(self,
                 h_dims,
                 x_dims,
                 update_gate=None,
                 reset_gate=None,
                 new_state_net=None,
                 n_units=100):

        super(GRU_with_std, self).__init__()

        if update_gate is None:
            self.update_gate = nn.Sequential(
                nn.Linear(h_dims * 2 + x_dims, n_units), nn.Tanh(),
                nn.Linear(n_units, h_dims), nn.Sigmoid())
            utils.init_network_weights(self.update_gate)
        else:
            self.update_gate = update_gate

        if reset_gate is None:
            self.reset_gate = nn.Sequential(
                nn.Linear(h_dims * 2 + x_dims, n_units), nn.Tanh(),
                nn.Linear(n_units, h_dims), nn.Sigmoid())
            utils.init_network_weights(self.reset_gate)
        else:
            self.reset_gate = reset_gate

        if new_state_net is None:
            self.new_state_net = nn.Sequential(
                nn.Linear(h_dims * 2 + x_dims, n_units), nn.Tanh(),
                nn.Linear(n_units, h_dims * 2))
            utils.init_network_weights(self.new_state_net)
        else:
            self.new_state_net = new_state_net

    def forward(self, y_mean, y_std, x):
        y_concat = torch.cat((y_mean, y_std, x), -1)

        update_gate = self.update_gate(y_concat)
        reset_gate = self.reset_gate(y_concat)
        concat = torch.cat((y_mean * reset_gate, y_std * reset_gate, x), -1)

        new_state, new_state_std = utils.split_last_dim(
            self.new_state_net(concat))
        new_state_std = new_state_std.abs()

        new_y = (1 - update_gate) * new_state + update_gate * y_mean
        new_y_std = (1 - update_gate) * new_state_std + update_gate * y_std

        new_y_std = new_y_std.abs()
        return new_y, new_y_std


# another implementation:
# https://github.com/ashysheya/ODE-RNN/blob/master/lib/ode_rnn.py
class ODE_RNN(nn.Module):

    def __init__(self, diffeq_solver, RNN_net):

        super(ODE_RNN, self).__init__()
        self.h_dims = diffeq_solver.h_dims
        self.diffeq_solver = diffeq_solver  # TODO could use CDE?
        self.RNN_net = RNN_net  # a GRU, probably

    # labeled x->h instead of x->y bc it'll usually be the hidden layer inside
    # another model
    def forward(self,
                x,
                x_time,
                return_latents=False,
                initial_dt=0.01,
                min_step_frac=50):
        batch_size = x.shape[0]

        prev_hi = torch.zeros((batch_size, self.h_dims))
        prev_hi_std = torch.zeros((batch_size, self.h_dims))

        # TODO why going backwards?
        prev_ti, ti = x_time[-1] + 0.01, x_time[-1]
        #TRANS: Internal grid size
        interval_length = x_time[-1] - x_time[0]
        minimum_step = interval_length / min_step_frac

        hs = []

        for i in range(x_time.numel()):
            #TRANS: If it can not meet the minimum step direct linear increase
            if (prev_ti - ti) < minimum_step:

                time_points = torch.stack((prev_ti, ti))
                #TRANS: The linear increment
                inc = self.diffeq_solver(prev_ti, prev_hi) * (ti - prev_ti)
                hi_ode = prev_hi + inc

            else:
                n_intermediate_tp = max(2,
                                        ((prev_ti - ti) / minimum_step).int())
                time_points = utils.linspace_vector(prev_ti, ti,
                                                    n_intermediate_tp)
                #TRANS: Calculate the ODE, work out corresponding latent value
                # to (1) finally can
                hi_ode = self.diffeq_solver.solve(prev_hi, time_points)[:, -1, :]

            #TRANS: Calculate the GRU helped
            hi, hi_std = self.RNN_net(hi_ode, prev_hi_std, x[:, i, :])

            if return_latents:
                hs.append(hi)

            # prev_hi-(odesolver)->hi_ode-(GRU)->hi->prev_hi
            prev_hi, prev_hi_std = hi, hi_std
            prev_ti, ti = x_time[i], x_time[i - 1]

        if return_latents:
            return torch.stack(hs, dim=1)
        else:
            return hi, hi_std


@dataclass
class RecurrentODEParams:
    encoder_out_dims: Union[int, Literal['y']] = 'y'
    h_dims: Optional[int] = None
    n_gru_units: int = 100
    n_out_units: int = 100
    # gaussian_likelihood_std: float = 0.01


# @pl.utilities.cli.MODEL_REGISTRY
class RecurrentODE(BaseModel):

    def __init__(self,
                 recurrentodeparams: RecurrentODEParams,
                 odefuncparams: ODEFuncParams,
                 RNN_net: Optional[nn.Module] = None,
                 **kwargs
                ):
        super().__init__(**kwargs)

        vars(self).update(asdict(recurrentodeparams))  # hack?
        self.odefuncparams = odefuncparams
        self.RNN_net = RNN_net

        # self.save_hyperparameters(ignore=list(kwargs.keys()))
        # self.save_hyperparameters()

        if self.x_dims is not MISSING:
            self.init_xy()

    def init_xy(self):

        # self.gaussian_likelihood_std = torch.tensor([self.gaussian_likelihood_std])

        diffeq_solver = ODEFunc(self.x_dims, self.odefuncparams)

        if self.h_dims is None:
            self.h_dims = diffeq_solver.h_dims

        if self.RNN_net is None:
            # TODO must h_dims == solver.h_dims?
            self.RNN_net = GRU_with_std(self.h_dims,
                                        self.x_dims,
                                        n_units=self.n_gru_units)

        self.ODE_RNN = ODE_RNN(diffeq_solver, self.RNN_net)

        self.RNN_net = None  # so it doesn't show up in self.parameters()

        # RecurrentODE
        if self.encoder_out_dims == 'y':
            self.encoder_out_dims = self.y_dims
            self.output_net = utils.create_net(self.h_dims, self.y_dims,
                                               self.n_out_units)
        # ODE_GRU_Encoder
        else:
            self.output_net = utils.create_net(self.h_dims * 2,
                                               self.encoder_out_dims * 2,
                                               self.n_out_units)

    def setup(self, stage=None):
        super().setup(stage)

    # RecurrentODE
    def old_forward(self, y_time, x, x_time):
        #TRANS: Complete sequence prediction
        if len(y_time.shape) < 1:
            y_time = y_time.unsqueeze(0)
        #TRANS: Stitching x_time and y_time
        # TODO replace this with something less funky
        # TODO enforce min(y_time) > max(x_time) in dm and remove t := tx & ty
        x_len_orig = len(x_time)  # x_time becomes union(x_time, y_time)
        x_time, time_index = torch.unique(torch.cat((x_time, y_time)),
                                          return_inverse=True)
        x_time_idx, y_time_idx = time_index[:x_len_orig], time_index[
            x_len_orig:]

        batch_size, x_dims = x.shape[0], x.shape[2]
        #TRANS: To expand the x
        x_merged = torch.zeros((batch_size, len(x_time), x_dims),
                               dtype=x.dtype,)
                               # device=utils.get_device(x))
        # import xdev; xdev.embed()
        x_merged[:, x_time_idx, :] = x
        hs = self.ODE_RNN(x_merged, x_time, return_latents=True)
        # this is like having t as an extra batch dim for output_net
        y_pred = self.output_net(hs)

        # y_pred shape [batch_size, y_time_points, y_dims]
        return y_pred.index_select(1, y_time_idx)

    # ODE_GRU_Encoder
    def run_to_prior(self, x_data, x_time):
        batch_size = x_data.shape[0]

        if x_time.numel() == 1:
            prev_hi = torch.zeros((batch_size, self.h_dims))
            prev_hi_std = torch.zeros((batch_size, self.h_dims))
            hi_last_mu, hi_last_sigma = self.GRU_update(
                prev_hi, prev_hi_std, x_data[:, 0, :])
        else:
            hi_last_mu, hi_last_sigma = self.run_to_last_point(
                x_data, x_time, return_latents=False)

        hi_last_mu = hi_last_mu.reshape(1, batch_size, self.h_dims)
        hi_last_sigma = hi_last_sigma.reshape(1, batch_size, self.h_dims)

        # TRANS: VAE2: through the encoder (x, hi_last_mu, hi_last_sigma) to
        # produce the corresponding mu and the sigma
        hi_last_mu, hi_last_sigma = utils.split_last_dim(
            self.output_net(torch.cat((hi_last_mu, hi_last_sigma), -1)))
        hi_last_sigma = hi_last_sigma.abs()

        return hi_last_mu, hi_last_sigma

    # ODE_GRU_Encoder
    # TRANS: VAE1: at the same time to generate the final hi average and
    # standard deviation of the observation point hi_std
    def run_to_last_point(self, x_data, x_time, return_latents=False):
        return self.ODE_RNN(x_data, x_time, return_latents=return_latents)


@dataclass
class RecurrentCDEParams:
    gaussian_likelihood_std: float = 0.01


@dataclass
class RecurrentCDE(nn.Module, RecurrentCDEParams):

    x_dims: int = MISSING
    y_dims: int = MISSING
    diffeq_solver: CDEFunc = MISSING

    def __post_init__(self):

        super(RecurrentCDE, self).__init__()
        self.h_dims = diffeq_solver.h_dims
        self.gaussian_likelihood_std = torch.tensor([gaussian_likelihood_std])

        self.using = 'CDE'

        self.initial_net = nn.Linear(self.x_dims, self.h_dims)
        utils.init_network_weights(self.initial_net)
        self.output_net = nn.Linear(self.h_dims, self.y_dims)
        utils.init_network_weights(self.output_net)


    def forward(self, y_time, x, x_time):
        # import xdev; xdev.embed()
        #TRANS: Complete sequence prediction
        if len(y_time.shape) < 1:
            y_time = y_time.unsqueeze(0)

        # TODO do this in dataloader
        x_time, x_ix = x_time.sort()
        x_data = x_data[..., x_ix, :]
        y_time, y_ix = y_time.sort()

        # need rectilinear interpolation instead if doing online prediction
        # https://github.com/patrick-kidger/torchcde#different-interpolation-methods

        # TODO https://github.com/patrick-kidger/torchcde#the-log-ode-method

        # standard preprocessing step for cdes: include time as a channel,
        # since they're invariant to time. have to make sure seqs are
        # temporally aligned from start? eg seconds since hemorrhage onset
        TIME_INV = 0
        if TIME_INV:
            # combine x and t - do this in dataloader
            # https://github.com/arogozhnikov/einops/issues/56#issuecomment-962584525
            common_dims = ' '.join(map(str, x_data.shape[:-2]))
            x = torch.cat(
                (x_data, einops.repeat(x_time, f't -> {common_dims} t 1')),
                dim=-1)

            coeffs = hermite_cubic_coefficients_with_backward_differences(
                x)
            X = CubicSpline(coeffs)
            X0 = X.evaluate(0)
            h0 = self.initial_net(X0)

            # TODO need to do this for yt instead of xt
            # or both, using predicted xs for another loss
            GET_ALL_TIME_POINTS = 1
            if GET_ALL_TIME_POINTS:
                ts = torch.linspace(*X.interval, len(x_time))
            else:
                # interval is (start, end)
                ts = X.interval

            hs = self.diffeq_solver.solve(h0, ts, x)

        else:
            # import xdev; xdev.embed()
            coeffs = hermite_cubic_coefficients_with_backward_differences(
                x_data, x_time)
            X = CubicSpline(coeffs, x_time)

            # need at least 2 points to take a solver step, so augment with
            # closest x point to start of y
            # assumes xt, yt sorted
            assert y_time[0] > x_time[0]
            x_time_last = x_time[torch.argwhere(x_time < y_time)[-1]]

            X0 = X.evaluate(x_time[-1])
            h0 = self.initial_net(X0)  # TODO should parameterize this by time
            hs = self.diffeq_solver.solve(h0, torch.cat((x_time_last, y_time)), X)[:, 1:, :]

        y_pred = self.output_net(hs)

        # put in the orig time order
        # https://stackoverflow.com/a/64455575
        # y_pred = y_pred.gather(-2, y_ix.argsort())

        # y_pred shape [batch_size, y_time_points, y_dims]
        return y_pred

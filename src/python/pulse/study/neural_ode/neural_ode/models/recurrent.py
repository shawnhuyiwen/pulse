import torch
import torch.nn as nn
from torchcde import CubicSpline, hermite_cubic_coefficients_with_backward_differences
import neural_ode.models.utils as utils
from neural_ode.models.evaluation import get_log_likelihood, get_mse
from neural_ode.models.diff_func import ODEFunc, CDEFunc, ODEFuncParams, CDEFuncParams
from neural_ode.models.utils import BaseModel
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
    # this is setup for irregular time steps, though not needed
    # TRANS: VAE1: at the same time to generate the final hi average and
    # standard deviation of the observation point hi_std
    def forward(
            self,
            x,
            x_time,
            return_latents=False,
            initial_dt=0.01,  # TODO adjust this
            min_step_frac=None):  # 50
        batch_size = x.shape[0]

        hi = torch.zeros((batch_size, self.h_dims), device=x.device)
        hi_std = torch.zeros((batch_size, self.h_dims), device=x.device)

        ti = x_time[0] - initial_dt
        if min_step_frac:
            #TRANS: Internal grid size
            interval_length = x_time[-1] - x_time[0]
            # ODE is never solved unless a timestep is >1/50 of the total time
            minimum_step = interval_length / min_step_frac
        else:
            minimum_step = x_time.diff().min()

        hs = []
        # print(f'ODE_RNN solving {x_time[0]} -> {x_time[-1]}')

        for i in range(len(x_time)):
            # prev_hi-(odesolver)->hi_ode-(GRU)->hi->prev_hi
            prev_hi, prev_hi_std = hi, hi_std
            prev_ti, ti = ti, x_time[i]
            dt = ti - prev_ti

            n_tp = (dt / minimum_step).int()
            hi, hi_std = self.step(x[:, i, :], prev_hi, prev_hi_std, ti,
                                   prev_ti, n_tp)

            if return_latents:
                hs.append(hi)

        if return_latents:
            return torch.stack(hs, dim=1)
        else:
            return hi, hi_std

    def step(self, xi, prev_hi, prev_hi_std, ti, prev_ti, n_tp):
        #TRANS: If it can not meet the minimum step direct linear increase
        if n_tp < 1:
            time_points = torch.stack((prev_ti, ti))
            #TRANS: The linear increment
            inc = self.diffeq_solver(prev_ti, prev_hi) * (ti - prev_ti)
            hi_ode = prev_hi + inc
            # print(f'{dt=} < {minimum_step=}, linear')

        else:
            time_points = utils.linspace_vector(prev_ti,
                                                ti,
                                                max(2, n_tp),
                                                device=xi.device)
            #TRANS: Calculate the ODE, work out corresponding latent value
            # to (1) finally can
            hi_ode = self.diffeq_solver.solve(prev_hi, time_points)[:, -1, :]
            # print(f'{dt=} >= {minimum_step=}, solving {time_points=}')

        #TRANS: Calculate the GRU helped
        import xdev
        with xdev.embed_on_exception_context():
            hi, hi_std = self.RNN_net(hi_ode, prev_hi_std, xi)
        # print(f'x(t)={x[:, i, 0]}')
        return hi, hi_std

    # ODE_GRU_Encoder
    def run_to_prior(self, x_data, x_time):
        batch_size = x_data.shape[0]

        if x_time.numel() == 1:
            prev_hi = torch.zeros((batch_size, self.h_dims))
            prev_hi_std = torch.zeros((batch_size, self.h_dims))
            hi_last_mu, hi_last_sigma = self.RNN_net(prev_hi, prev_hi_std,
                                                     x_data[:, 0, :])
        else:
            hi_last_mu, hi_last_sigma = self(
                x_data, x_time, return_latents=False)

        hi_last_mu = hi_last_mu.reshape(1, batch_size, self.h_dims)
        hi_last_sigma = hi_last_sigma.reshape(1, batch_size, self.h_dims)

        # TRANS: VAE2: through the encoder (x, hi_last_mu, hi_last_sigma) to
        # produce the corresponding mu and the sigma
        hi_last_mu, hi_last_sigma = utils.split_last_dim(
            self.output_net(torch.cat((hi_last_mu, hi_last_sigma), -1)))
        hi_last_sigma = hi_last_sigma.abs()

        return hi_last_mu, hi_last_sigma


@dataclass
class RecurrentODEParams:
    encoder_out_dims: Union[int, Literal['y']] = 'y'
    h_dims: Optional[int] = None
    n_gru_units: int = 100
    n_out_units: int = 100
    # gaussian_likelihood_std: float = 0.01


class RecurrentODE(BaseModel):

    def __init__(self,
                 recurrentodeparams: RecurrentODEParams,
                 odefuncparams: ODEFuncParams,
                 RNN_net: Optional[nn.Module] = None,
                 **kwargs):
        kwargs['hidden_size'] = recurrentodeparams.h_dims
        super().__init__(**kwargs)

        # # bad
        # self.t = torch.rand(2, 2, device=self.device)
        # # good
        # self.register_buffer("t", torch.rand(2, 2))

        vars(self).update(asdict(recurrentodeparams))  # hack?
        self.odefuncparams = odefuncparams
        self.RNN_net = RNN_net

        # self.save_hyperparameters(ignore=list(kwargs.keys()))
        # self.save_hyperparameters()

        if not self.STUB:
            self.x_dims = kwargs['x_dims']
            self.y_dims = kwargs['y_dims']

            if self.odefuncparams.h_dims is None:
                self.odefuncparams.h_dims = 2 * self.x_dims

            diffeq_solver = ODEFunc(self.odefuncparams)

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

    # RecurrentODE
    def old_forward(self, y_time, x, x_time, y_cov):
        #TRANS: Complete sequence prediction
        if len(y_time.shape) < 1:
            y_time = y_time.unsqueeze(0)
        if 0:
            xy_time = torch.cat((x_time, y_time))
            batch_size, x_extent, x_dims = x.shape
            y_extent = len(y_time)
            xy = torch.cat((x,
                            torch.cat((
                                y_cov,
                                torch.zeros((batch_size, y_extent, x_dims),
                                            dtype=x.dtype,
                                            device=self.device)),
                                dim=-1)),
                           dim=1)
            # print(f'xt={x_time.shape} {x_time[0]}..{x_time[-1]}')
            # print(f'yt={y_time.shape} {y_time[0]}..{y_time[-1]}')
            # print(f'feeding x={x.shape} + y to ODE_RNN')
            hs = self.ODE_RNN(xy, xy_time, return_latents=True)
            # this is like having t as an extra batch dim for output_net
            y_pred = self.output_net(hs)

            # y_pred shape [batch_size, y_time_points, y_dims]
            return y_pred[:, -y_extent:, :]
        else:
            hi, hi_std = self.ODE_RNN(x, x_time, return_latents=False)
            yi = x[:, -1, -len(self.target_positions):]
            ti = x_time[-1]
            y = []
            for i in range(len(y_time)):
                prev_hi, prev_hi_std = hi, hi_std
                prev_ti, ti = ti, y_time[i]
                yi_aug = torch.cat(
                    (y_cov[:, i, :], yi), dim=-1)
                hi, hi_std = self.ODE_RNN.step(yi_aug,
                                               prev_hi,
                                               prev_hi_std,
                                               ti,
                                               prev_ti,
                                               n_tp=2)
                yi = self.output_net(hi)
                y.append(yi)
            return torch.stack(y, dim=1)


@dataclass
class MLPParams:
    n_units: int = 100
    hidden_layers: int = 0
    nonlinear: type = nn.Tanh


@dataclass
class CDEEncoderParams:
    cdefuncparams: CDEFuncParams
    in_dims: Union[int, Literal['x']] = 'x'
    out_dims: Union[int, Literal['y'], Literal['h']] = 'h'
    in_net_params: Optional[MLPParams] = None
    out_net_params: Optional[MLPParams] = None


class CDEEncoder(nn.Module):

    def __init__(self, x_dims: int, y_dims: int,
                 cdeencoderparams: CDEEncoderParams):
        super().__init__()

        vars(self).update(asdict(cdeencoderparams))  # hack?
        self.cdefuncparams = cdefuncparams

        if not self.STUB:
            self.h_dims = self.cdefuncparams.h_dims

            if in_dims == 'x':
                self.in_dims = x_dims
            else:
                self.in_dims = in_dims

            if out_dims == 'y':
                self.out_dims = y_dims
            elif out_dims == 'h':
                self.out_dims = self.h_dims
            else:
                self.out_dims = out_dims

            if in_net_params is not None:
                self.in_net = utils.create_net(self.x_dims,
                                               self.h_dims,
                                               **asdict(in_net_params),
                                               init=True)
                self.diffeq_solver = CDEFunc(self.h_dims, self.cdefuncparams)
            else:
                self.in_net = nn.Identity()
                self.diffeq_solver = CDEFunc(self.x_dims, self.cdefuncparams)

            if out_net_params is not None:
                self.out_net = utils.create_net(self.h_dims,
                                                self.y_dims,
                                                **asdict(out_net_params),
                                                init=True)
                self.diffeq_solver = CDEFunc(self.h_dims, self.cdefuncparams)
            else:
                self.out_net = nn.Identity()
                assert self.out_dims == self.h_dims

    def forward(self, x, x_time):
        x = self.in_net(x)

        # need rectilinear interpolation instead if doing online prediction
        # https://github.com/patrick-kidger/torchcde#different-interpolation-methods

        # TODO https://github.com/patrick-kidger/torchcde#the-log-ode-method

        # assumes seqs are temporally aligned, t is included as an x feature,
        # and y_time does not overlap x_time

        # can move this to dataloader if using fixed normalization
        coeffs = hermite_cubic_coefficients_with_backward_differences(x)
        X = CubicSpline(coeffs)

        X0 = X.evaluate(0)
        h0 = self.initial_net(X0)

        # TODO need to do this for yt instead of xt
        # or both, using predicted xs for another loss
        GET_ALL_TIME_POINTS = 0
        if GET_ALL_TIME_POINTS:
            ts = torch.linspace(*X.interval, len(x_time))
        else:
            # interval is (start, end)
            ts = X.interval

        hs = self.diffeq_solver.solve(h0, ts, x)

        y_pred = self.out_net(hs)

        # y_pred shape [batch_size, y_time_points, y_dims]
        return y_pred

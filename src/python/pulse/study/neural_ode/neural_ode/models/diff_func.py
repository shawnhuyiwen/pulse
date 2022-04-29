import torch
import torch.nn as nn
import neural_ode.models.utils as utils

from torchdiffeq import odeint
from torchcde import cdeint

import einops
import ubelt as ub
import config as cf


class ODEFunc(nn.Module):

    def __init__(self,
                 h_dims,
                 h_trans_dims=100,
                 h_trans_layers=1,
                 nonlinear=nn.Tanh,
                 final_nonlinear=None):
        super(ODEFunc, self).__init__()
        self.h_dims = h_dims
        self.h_trans_dims = h_trans_dims
        self.h_trans_layers = h_trans_layers

        self.ode_func = utils.create_net(input_dims=h_dims,
                                         output_dims=h_dims,
                                         n_units=h_trans_dims,
                                         hidden_layers=h_trans_layers,
                                         nonlinear=nonlinear)

        if final_nonlinear is None:
            final_nonlinear = nn.Identity
        self.final_nonlinear = final_nonlinear()

    def forward(self, t, h, backwards=False):
        hs = self.ode_func(h)
        hs = self.final_nonlinear(hs)
        # TRANS: It needs to be the same equation, the reverse solving backwards
        if backwards:
            hs = -hs
        return hs


class CDEFunc(nn.Module):

    def __init__(self,
                 x_dims,
                 h_dims,
                 h_trans_dims=100,
                 h_trans_layers=1,
                 nonlinear=nn.ReLU,
                 final_nonlinear=nn.Tanh):
        super(CDEFunc, self).__init__()
        self.x_dims = x_dims
        self.h_dims = h_dims
        self.h_trans_dims = h_trans_dims
        self.h_trans_layers = h_trans_layers

        self.cde_func = utils.create_net(input_dims=h_dims,
                                         output_dims=(h_dims * x_dims),
                                         n_units=h_trans_dims,
                                         hidden_layers=h_trans_layers,
                                         nonlinear=nonlinear)

        if final_nonlinear is None:
            final_nonlinear = nn.Identity
        self.final_nonlinear = final_nonlinear()

    def forward(self, t, h):
        hs = self.cde_func(h)
        hs = einops.rearrange(hs,
                              '... (h x) -> ... h x',
                              h=self.h_dims,
                              x=self.x_dims)
        # TRANS: Finally using tanh explain output in (0, 1)
        hs = self.final_nonlinear(hs)
        return hs


class DiffeqSolver(nn.Module):

    def __init__(self, diff_func, method, odeint_rtol, odeint_atol):
        super(DiffeqSolver, self).__init__()

        self.diff_func = diff_func
        self.odeint_rtol = odeint_rtol
        self.odeint_atol = odeint_atol
        self.ode_method = method

    def forward(self, h0, t, X=None):
        if isinstance(self.diff_func, ODEFunc):
            hs_pred = odeint(self.diff_func,
                             h0,
                             t,
                             rtol=self.odeint_rtol,
                             atol=self.odeint_atol,
                             method=self.ode_method)
            #TRANS: Second time_points corresponding dimensions will be
            # moved to the bottom
            hs_pred = einops.rearrange(hs_pred, 't ... c -> ... t c')
            # hs_pred.shape is ([sample_hs] batch_size time_points h_dims)

        elif isinstance(self.diff_func, CDEFunc):
            # kwargs["rtol"] = self.odeint_rtol
            # kwargs['atol'] = self.odeint_atol
            # kwargs["method"] = self.ode_method
            # this overwrites kwargs from ODE
            steps = (t[1:] - t[:-1])
            assert len(steps) > 0
            step_size = steps.min().item()
            kwargs = {
                "method": "rk4",
                "options": {
                    "step_size": step_size
                }
            }
            hs_pred = cdeint(X,
                             self.diff_func,
                             h0,
                             t,
                             adjoint=True,
                             backend="torchdiffeq",
                             **kwargs)
            # hs_pred.shape is ([sample_hs] batch_size time_points h_dims)

        else:
            raise NotImplementedError

        return hs_pred

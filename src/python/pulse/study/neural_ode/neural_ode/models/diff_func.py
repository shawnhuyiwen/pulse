import torch
import torch.nn as nn
import neural_ode.models.utils as utils

from torchdiffeq import odeint
from torchcde import cdeint

import einops
import ubelt as ub
import config as cf


class ODEFunc(nn.Module, ub.NiceRepr):
    def __init__(self, h_dims, h_trans_dims=100, h_trans_layers=1, nonlinear=nn.Tanh, final_nonlinear=None):
        super(ODEFunc, self).__init__()
        self.h_dims = h_dims
        self.h_trans_dims = h_trans_dims
        self.h_trans_layers = h_trans_layers
        self.device = 'cpu'

        layers = [nn.Linear(h_dims, h_trans_dims)]
        for _ in range(h_trans_layers):
            layers.append(nonlinear())
            layers.append(nn.Linear(h_trans_dims, h_trans_dims))
        layers.append(nonlinear())
        layers.append(nn.Linear(h_trans_dims, h_dims))
        self.ode_func = nn.Sequential(*layers)
        utils.init_network_weights(self.ode_func)
        if final_nonlinear is not None:
            self.final_nonlinear = final_nonlinear()
        else:
            self.final_nonlinear = None

    def __nice__(self):
        return f"ODEFunc {self.h_dims=}, {self.h_trans_dims=}, {self.h_trans_layers=}"

    def forward(self, t, h, backwards=False):
        func = self.ode_func(h)
        if self.final_nonlinear is not None:
            func = self.final_nonlinear(func)
        if backwards:  #TRANS: It needs to be the same equation, the reverse solving backwards
            func = -func
        return func


class CDEFunc(nn.Module, ub.NiceRepr):
    def __init__(self, x_dims, h_dims, h_trans_dims=100, h_trans_layers=1, nonlinear=nn.ReLU, final_nonlinear=nn.Tanh):
        super(CDEFunc, self).__init__()

        layers = [nn.Linear(h_dims, h_trans_dims)]
        for _ in range(h_trans_layers):
            layers.append(nonlinear())
            layers.append(nn.Linear(h_trans_dims, h_trans_dims))
        layers.append(nonlinear())
        layers.append(nn.Linear(h_trans_dims, h_dims * x_dims))
        self.cde_func = nn.Sequential(*layers)
        utils.init_network_weights(self.cde_func)
        if final_nonlinear is not None:
            self.final_nonlinear = final_nonlinear()
        else:
            self.final_nonlinear = None
        self.device = 'cpu'

    def __nice__(self):
        return f"CDEFunc {self.x_dims=}, {self.h_dims=}, {self.h_trans_dims=}, {self.h_trans_layers=}"

    def forward(self, t, h):
        hs = self.cde_func(h)
        hs = hs.view(*h.shape[:-1], self.h_dims, self.x_dims)
        if self.final_nonlinear is not None:
            hs = self.final_nonlinear(hs)  #TRANS: Finally using tanh explain output in (0, 1)
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
            hs_pred = odeint(self.diff_func, h0, t, rtol=self.odeint_rtol, atol=self.odeint_atol, method=self.ode_method)
            #TRANS: Second time_points corresponding dimensions will be
            # moved to the bottom
            hs_pred = einops.rearrange(hs_pred, 't ... c -> ... t c')
            # hs_pred.shape is ([sample_hs] batch_size time_points h_dims)

        elif isinstance(self.diff_func, CDE):
            # kwargs["rtol"] = self.odeint_rtol
            # kwargs['atol'] = self.odeint_atol
            # kwargs["method"] = self.ode_method
            # this overwrites kwargs from ODE
            kwargs = {
                "method": "rk4",
                "options": {
                    "step_size": (t[1:] - t[:-1]).min().item()
                }
            }
            hs_pred = cdeint(X, self.diff_func, h0, t, adjoint=True, backend="torchdiffeq", **kwargs)
            #TRANS: Do not need to transpose, output [batch_size, time_points, h_dims] or [sample_hs batch_size, time_points, h_dims]

        else:
            raise NotImplementedError

        return hs_pred  #TRANS: 输出[batch_size time_points h_dims]或[sample_hs、batch_size time_points, h_dims]

import torch
import torch.nn as nn
import neural_ode.models.utils as utils
from neural_ode.models.utils import MISSING

from torchdiffeq import odeint
from torchcde import cdeint

import einops
import ubelt as ub
from typing import Optional, Union
from dataclasses import dataclass, asdict


# for argparse compatibility
Layer = Optional[Union[str, nn.Module]]

def _coerce_layer_type(layer: Layer, default=nn.Identity) -> nn.Module:
    if layer is None:
        return default
    elif isinstance(layer, str):
        return vars(nn)[layer]()  # HACK
    elif isinstance(layer, nn.Module):
        return layer
    else:
        raise TypeError(layer)


# fixed parameters
# TODO integrate with pl hparams
@dataclass
class ODEFuncParams:
    # network
    h_dims: Optional[int] = None
    h_trans_dims: int = 100
    h_trans_layers: int = 2
    nonlinear: Layer = nn.Tanh()  # TODO fix create_net for ()
    final_nonlinear: Layer = nn.Tanh()
    # solver
    odeint_rtol: float = 1e-3
    odeint_atol: float = 1e-4
    ode_method: str = 'dopri15'


class ODEFunc(nn.Module):

    def __init__(self, x_dims: int, odefuncparams: ODEFuncParams):
        super(ODEFunc, self).__init__()

        self.x_dims = x_dims
        vars(self).update(asdict(odefuncparams))  # hack?

        if self.h_dims is None:
            self.h_dims = 2 * self.x_dims

        self.nonlinear = _coerce_layer_type(self.nonlinear)
        self.final_nonlinear = _coerce_layer_type(self.final_nonlinear)

        self.ode_func = utils.create_net(input_dims=self.h_dims,
                                         output_dims=self.h_dims,
                                         n_units=self.h_trans_dims,
                                         hidden_layers=self.h_trans_layers,
                                         nonlinear=type(self.nonlinear))

    def forward(self, t, h, backwards=False):
        hs = self.ode_func(h)
        hs = self.final_nonlinear(hs)
        # TRANS: It needs to be the same equation, the reverse solving backwards
        if backwards:
            hs = -hs
        return hs

    def solve(self, h0, t):
        hs_pred = odeint(self, h0, t,
                         rtol=self.odeint_rtol,
                         atol=self.odeint_atol,
                         method=self.ode_method)
        #TRANS: Second time_points corresponding dimensions will be
        # moved to the bottom
        hs_pred = einops.rearrange(hs_pred, 't ... c -> ... t c')
        # hs_pred.shape is ([sample_hs] batch_size time_points h_dims)
        return hs_pred

import pytorch_lightning as pl
pl.core.saving.save_hparams_to_yaml

# fixed parameters
# TODO integrate with pl hparams
@dataclass
class CDEFuncParams:
    # network
    h_dims: Optional[int] = None
    h_trans_dims: int = 100
    h_trans_layers: int = 2
    nonlinear: Layer = nn.ReLU()  # TODO awk
    final_nonlinear: Layer = nn.Tanh()

class CDEFunc(nn.Module):

    def __init__(self, x_dims: int, cdefuncparams: CDEFuncParams):
        super(CDEFunc, self).__init__()

        self.x_dims = x_dims
        vars(self).update(asdict(cdefuncparams))  # hack?

        if self.h_dims is None:
            self.h_dims = 2 * self.x_dims

        self.nonlinear = _coerce_layer_type(self.nonlinear)
        self.final_nonlinear = _coerce_layer_type(self.final_nonlinear)

        self.cde_func = utils.create_net(input_dims=self.h_dims,
                                         output_dims=(self.h_dims * self.x_dims),
                                         n_units=self.h_trans_dims,
                                         hidden_layers=self.h_trans_layers,
                                         nonlinear=type(self.nonlinear))

        if self.final_nonlinear is None:
            self.final_nonlinear = nn.Identity()

    def forward(self, t, h):
        hs = self.cde_func(h)
        hs = einops.rearrange(hs,
                              '... (h x) -> ... h x',
                              h=self.h_dims,
                              x=self.x_dims)
        # TRANS: Finally using tanh explain output in (0, 1)
        hs = self.final_nonlinear(hs)
        return hs

    def solve(self, h0, t, X):
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
        hs_pred = cdeint(X, self, h0, t,
                         adjoint=True,
                         backend="torchdiffeq",
                         **kwargs)
        # hs_pred.shape is ([sample_hs] batch_size time_points h_dims)
        return hs_pred

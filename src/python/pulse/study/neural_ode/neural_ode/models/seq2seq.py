import torch
import torch.nn as nn
from neural_ode.models.diff_func import ODEFunc, CDEFunc, ODEFuncParams, CDEFuncParams
from neural_ode.models.evaluation import get_log_likelihood, get_mse
from neural_ode.models.recurrent import ODE_RNN, GRU_with_std
from neural_ode.models.utils import BaseModel
from dataclasses import dataclass
from typing import Union, Optional


class ODEDecoder(nn.Module):
    #TRANS: The ODE as decoder
    def __init__(self, y_dims, h_dims, n_out_units, dec_diffeq_solver):
        super(ODEDecoder, self).__init__()
        if h_dims is None:
            h_dims = dec_diffeq_solver.h_dims

        self.output_net = utils.create_net(h_dims, y_dims, n_out_units)

        self.dec_diffeq_solver = dec_diffeq_solver

    def forward(self, h0, t_span):
        ode_sol = self.dec_diffeq_solver(h0, t_span)
        outputs = self.output_net(ode_sol)
        return outputs


@dataclass
class Seq2SeqParams:
    h_dims: int = 100
    enc_n_gru_units: int = 100
    enc_n_out_units: int = 100
    enc_h_dims: Optional[int] = None
    dec_n_out_units: int = 100
    dec_h_dims: Optional[int] = None


# TODO tie enc-dec weights? https://stackoverflow.com/q/36889732
class Seq2Seq(BaseModel):

    def __init__(self, h_dims: int = 100, **kwargs):

        super().__init__(**kwargs)

        vars(self).update(asdict(seq2seqparams))  # hack?
        self.enc_diffeq_params = enc_diffeq_params
        self.dec_diffeq_params = dec_diffeq_params

        self.save_hyperparameters()

        if not self.STUB:
            pass


    def forward(self, y_time, x_data, x_time):

        if len(y_time.shape) < 1:
            y_time = y_time.unsqueeze(0)

        # encoder
        hs = self.encoder(x, x_time, return_latents=True)
        # TRANS: The decoder initial point set as x_time finally
        decoder_begin_hi = hs[:, -1, :]
        y_time = torch.cat((x_time[-1:], y_time))

        # decoder
        # TRANS: Continue to solve the ode down, using the decoder work out
        # prediction
        y_pred = self.decoder(decoder_begin_hi, y_time)[:, 1:, :]

        return y_pred


def odernn2ode(h_dims: int = 100, n_gru_units: int = 100, odefuncparams: ODEFuncParams = None):
    diffeq_solver = ODEFunc(odefuncparams)
    encoder = ODE_RNN(diffeq_solver, GRU_with_std(h_dims, x_dims, n_units=n_gru_units))
    decoder = diffeq_solver

    decoder = ODEDecoder(self.y_dims, self.dec_h_dims,
                              self.dec_n_out_units,
                         diffeq_solver)


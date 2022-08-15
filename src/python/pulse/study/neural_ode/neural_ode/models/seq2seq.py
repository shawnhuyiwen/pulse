import torch
import torch.nn as nn
from neural_ode.models.diff_func import ODEFunc, CDEFunc, ODEFuncParams
from neural_ode.models.evaluation import get_log_likelihood, get_mse
from neural_ode.models.recurrent import RecurrentODE, RecurrentODEParams
from neural_ode.models.utils import BaseModel
from dataclasses import dataclass, InitVar
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
    h_dims: int
    enc_n_gru_units: int = 100
    enc_n_out_units: int = 100
    enc_h_dims: Optional[int] = None
    dec_n_out_units: int = 100
    dec_h_dims: Optional[int] = None
    # gaussian_likelihood_std: float = 0.01


# TODO tie enc-dec weights? https://stackoverflow.com/q/36889732
# @pl.utilities.cli.MODEL_REGISTRY
class Seq2Seq(BaseModel):

    # TODO make CDE work here, replace ode_gru_encoder
    # ode_rnn:ode_gru_encoder::cde:?
    def __init__(self, seq2seqparams: Seq2SeqParams,
                 enc_diffeq_params: ODEFuncParams,
                 dec_diffeq_params: ODEFuncParams):

        super(Seq2Seq, self).__init__()

        vars(self).update(asdict(seq2seqparams))  # hack?
        self.enc_diffeq_params = enc_diffeq_params
        self.dec_diffeq_params = dec_diffeq_params

        self.save_hyperparameters()

        if self.x_dims is not None:
            self.init_xy()

    def init_xy(self):

        # self.gaussian_likelihood_std = torch.tensor([self.gaussian_likelihood_std])

        self.encoder = RecurrentODE(
            RecurrentODEParams(encoder_out_dims=self.h_dims,
                               h_dims=self.enc_h_dims,
                               n_gru_units=self.enc_n_gru_units,
                               n_out_units=self.enc_n_out_units),
            self.enc_diffeq_params)

        self.decoder = ODEDecoder(self.y_dims, self.dec_h_dims,
                                  self.dec_n_out_units,
                                  ODEFunc(self.h_dims, self.dec_diffeq_params))

    def forward(self, y_time, x_data, x_time, x_mask=None):
        #TRANS: Complete sequence prediction
        if x_mask is not None:
            x = x_data * x_mask
        else:
            x = x_data

        if len(y_time.shape) < 1:
            y_time = y_time.unsqueeze(0)

        # encoder
        hs = self.encoder.run_to_last_point(x, x_time, return_latents=True)
        # TRANS: The decoder initial point set as x_time finally
        decoder_begin_hi = hs[:, -1, :]
        y_time = torch.cat((x_time[-1:], y_time))

        # decoder
        # TRANS: Continue to solve the ode down, using the decoder work out
        # prediction
        y_pred = self.decoder(decoder_begin_hi, y_time)[:, 1:, :]

        return y_pred

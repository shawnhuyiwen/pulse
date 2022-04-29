import torch
import torch.nn as nn
import neural_ode.models.utils as utils
from neural_ode.models.recurrent import GRU_with_std, ODE_RNN


class ODE_GRU_Encoder(nn.Module):

    def __init__(self,
                 x_dims,
                 h_dims,
                 enc_diffeq_solver,
                 n_gru_units=100,
                 h_last_dim=None,
                 GRU_update=None):

        super(ODE_GRU_Encoder, self).__init__()

        if h_last_dim is None:
            self.h_last_dim = h_dims
        else:
            self.h_last_dim = h_last_dim

        if GRU_update is None:
            GRU_update = GRU_with_std(h_dims, x_dims, n_units=n_gru_units)
        self.ODE_RNN = ODE_RNN(enc_diffeq_solver, GRU_update)

        self.h_dims = h_dims
        self.x_dims = x_dims

        self.transform_h_last = utils.create_net(self.h_dims * 2,
                                                 self.h_last_dim * 2)

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
            self.transform_h_last(torch.cat((hi_last_mu, hi_last_sigma), -1)))
        hi_last_sigma = hi_last_sigma.abs()

        return hi_last_mu, hi_last_sigma

    # TRANS: VAE1: at the same time to generate the final hi average and
    # standard deviation of the observation point hi_std
    def run_to_last_point(self, x_data, x_time, return_latents=False):
        return self.ODE_RNN(x_data, x_time, return_latents=return_latents)


class ODE_Decoder(nn.Module):
    #TRANS: The ODE as decoder
    def __init__(self, y_dims, h_dims, n_out_units, dec_diffeq_solver):
        super(ODE_Decoder, self).__init__()
        self.output_net = utils.create_net(h_dims, y_dims, n_out_units)

        self.dec_diffeq_solver = dec_diffeq_solver

    def forward(self, h0, t_span):
        ode_sol = self.dec_diffeq_solver(h0,
                                         t_span)  #TRANS: Continue down the ode
        outputs = self.output_net(ode_sol)
        return outputs

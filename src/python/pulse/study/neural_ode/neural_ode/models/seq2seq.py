import torch
import torch.nn as nn
from neural_ode.models.encoder_decoder import ODE_GRU_Encoder, ODE_Decoder
from neural_ode.models.evaluation import get_log_likelihood, get_mse


class Seq2Seq(nn.Module):

    # TODO make CDE work her
    def __init__(self,
                 x_dims,
                 y_dims,
                 n_gru_units=100,
                 n_out_units=100,
                 enc_diffeq_solver=None,
                 dec_diffeq_solver=None,
                 gaussian_likelihood_std=0.02):

        super(Seq2Seq, self).__init__()
        enc_h_dims = enc_diffeq_solver.diff_func.h_dims
        dec_h_dims = dec_diffeq_solver.diff_func.h_dims
        self.gaussian_likelihood_std = torch.tensor([gaussian_likelihood_std])

        self.encoder = ODE_GRU_Encoder(x_dims=x_dims,
                                       h_dims=enc_h_dims,
                                       n_gru_units=n_gru_units,
                                       h_last_dim=dec_h_dims,
                                       enc_diffeq_solver=enc_diffeq_solver)

        self.decoder = ODE_Decoder(y_dims, dec_h_dims, n_out_units,
                                   dec_diffeq_solver)

    def compute_loss_one_batch(self, batch_dict, kl_coef=1.):
        #TRANS: Predict the y series, including the observed and was not observed
        y_pred = self.forward(batch_dict["y_time"], batch_dict["x_data"],
                              batch_dict["x_time"], batch_dict["x_mask"])

        # TRANS: To calculate indicators, batch_dict "y_mask" if is True, it
        # should enter None
        log_likelihood = get_log_likelihood(
            y_pred, batch_dict["y_data"], self.gaussian_likelihood_std, None
            if batch_dict["y_mask"].all() else batch_dict["y_mask"]).squeeze()
        mse = get_mse(
            y_pred, batch_dict["y_data"],
            None if batch_dict["y_mask"].all() else batch_dict["y_mask"])
        loss = -log_likelihood.squeeze(
        )  #TRANS: - log_likelihood back propagation

        #TRANS: Take the average in batch
        results = {}
        results["loss"] = loss
        results["likelihood"] = log_likelihood.detach()
        results["mse"] = mse.detach()
        results["kl_coef"] = kl_coef
        results["C_kl"] = 0.
        results["C_std"] = 0.

        return results

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

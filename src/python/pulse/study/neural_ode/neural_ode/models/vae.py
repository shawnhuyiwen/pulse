import gc
import torch
import torch.nn as nn
from torch.distributions import kl_divergence
from torch.distributions.normal import Normal
import neural_ode.models.utils as utils
# TODO move param classes here to share among seq2seq and vae
# from neural_ode.models.encoder_decoder import ODE_GRU_Encoder, ODE_Decoder
from neural_ode.models.diff_func import ODEFunc, CDEFunc
from neural_ode.models.evaluation import get_log_likelihood, get_mse
from neural_ode.models.utils import MISSING
from dataclasses import dataclass
from typing import Union


@dataclass
class VAEParams:
    h_prior: torch.distributions.Distribution = Normal(torch.tensor([0.]), torch.tensor([1.]))
    n_gru_units: int = 100
    n_out_units: int = 100
    gaussian_likelihood_std: float = 0.01

@dataclass
class VAE(nn.Module, VAEParams):
    x_dims: int = MISSING
    y_dims: int = MISSING
    enc_diffeq_solver: Union[ODEFunc, CDEFunc] = MISSING
    dec_diffeq_solver: Union[ODEFunc, CDEFunc] = MISSING

    def __post_init__(self):

        super(VAE, self).__init__()
        enc_h_dims = enc_diffeq_solver.h_dims
        dec_h_dims = dec_diffeq_solver.h_dims

        self.gaussian_likelihood_std = torch.tensor([gaussian_likelihood_std], device=self.device)
        self.h_prior = h_prior


        self.encoder = ODE_GRU_Encoder(
            x_dims=x_dims,  #TRANS: Mask directly ACTS on the data
            h_dims=enc_h_dims,
            n_gru_units=n_gru_units,
            h_last_dim=dec_h_dims,
            enc_diffeq_solver=enc_diffeq_solver,
            device=self.device)

        self.decoder = ODE_Decoder(y_dims, dec_h_dims, n_out_units, dec_diffeq_solver, device=self.device)

    def compute_loss_one_batch(self, batch_dict, kl_coef=1., sample_hs=3):
        #TRANS: Predict the y series, including the observed and was not observed
        y_pred, info = self.forward(batch_dict["y_time"], batch_dict["x_data"], batch_dict["x_time"], batch_dict["x_mask"], sample_hs=sample_hs)

        C_mu, C_std, _ = info["C"]
        C_std = C_std.abs()
        C_distr = Normal(C_mu, C_std)

        assert torch.sum(C_std < 0) == 0.

        C_kl = kl_divergence(C_distr, self.h_prior)

        if torch.isnan(C_kl).any():
            print(C_mu)
            print(C_std)
            raise Exception("C_kldiv is Nan!")

        # C_kldiv shape: [sample_hs, batch_size, h_dims] if prior is a mixture of gaussians (KL is estimated)
        # C_kldiv shape: [1, batch_size, h_dims] if prior is a standard gaussian (KL is computed exactly)
        # shape after: [sample_hs]
        #TRANS: Associated with IWAE: https://zhuanlan.zhihu.com/p/74556487, sample_h time sampling in order to obtain better refactoring capability

        C_kl = torch.mean(C_kl, (1, 2)).squeeze()

        #TRANS: Calculate indicators
        if not batch_dict["y_mask"].all():  #TRANS: All are not True, if that work
            y_mask = batch_dict["y_mask"].repeat(sample_hs, 1, 1, 1)  #TRANS: Copy the mask
        else:
            y_mask = None
        log_likelihood = get_log_likelihood(y_pred, batch_dict["y_data"], self.gaussian_likelihood_std, y_mask)
        mse = get_mse(y_pred, batch_dict["y_data"], y_mask)
        # IWAE loss
        loss = - torch.logsumexp(log_likelihood - kl_coef * C_kl, 0)
        if torch.isnan(loss):
            loss = - torch.mean(log_likelihood - kl_coef * C_kl, 0)

        #TRANS: Take the average in batch
        results = {}
        results["loss"] = loss
        results["likelihood"] = torch.mean(log_likelihood).detach()
        results["mse"] = mse.detach()
        results["kl_coef"] = kl_coef
        results["C_kl"] = C_kl.detach()
        results["C_std"] = torch.mean(C_std).detach()

        return results

    def forward(self, y_time, x_data, x_time, x_mask=None, sample_hs=1):
        #TRANS: Complete sequence prediction
        if x_mask is not None:
            x = x_data * x_mask
        else:
            x = x_data
        if len(y_time.shape) < 1:
            y_time = y_time.unsqueeze(0)

        # encoder
        enc_h_last_mu, enc_h_last_sigma = self.encoder.run_to_prior(x, x_time)

        #TRANS: VAE3: through normal distribution generated decoder of h (here with the first sampling sample_hs)
        enc_h_last_mean = enc_h_last_mu.repeat(sample_hs, 1, 1)
        enc_h_last_std = enc_h_last_sigma.repeat(sample_hs, 1, 1)
        dec_h_first = utils.sample_standard_gaussian(enc_h_last_mean, enc_h_last_std)
        enc_h_last_sigma = enc_h_last_sigma.abs()
        assert torch.sum(enc_h_last_sigma < 0) == 0.
        gc.collect()
        assert not torch.isnan(y_time).any()
        assert not torch.isnan(dec_h_first).any()
        #TRANS: The decoder initial point set as x_time finally
        y_time = torch.cat((x_time[-1:], y_time))

        # decoder
        y_pred = self.decoder(dec_h_first, y_time)[:, :, 1:, :]  #TRANS: Using the decoder work out forecast for training and testing
        y_pred_mean = self.decoder(enc_h_last_mean, y_time)[:, :, 1:, :]  #TRANS: Before using sampling average value to work out 1 group, used for drawing
        assert not torch.isnan(y_pred).any()
        assert not torch.isnan(y_pred_mean).any()

        info = {
            "C": (enc_h_last_mu, enc_h_last_sigma, dec_h_first),
            "y_pred_mean": torch.mean(y_pred_mean, 0)
        }

        return y_pred, info

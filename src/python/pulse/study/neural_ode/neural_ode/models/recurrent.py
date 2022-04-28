import torch
import torch.nn as nn
from torchcde import CubicSpline
import neural_ode.models.utils as utils
from neural_ode.models.evaluation import get_log_likelihood, get_mse
import einops
import pytorch_forecasting as pf


class GRU_with_std(nn.Module):
    def __init__(self, h_dims, x_dims,
                 update_gate=None,
                 reset_gate=None,
                 new_state_net=None,
                 n_units=100):

        super(GRU_with_std, self).__init__()

        if update_gate is None:
            self.update_gate = nn.Sequential(
                nn.Linear(h_dims * 2 + x_dims, n_units),
                nn.Tanh(),
                nn.Linear(n_units, h_dims),
                nn.Sigmoid())
            utils.init_network_weights(self.update_gate)
        else:
            self.update_gate = update_gate

        if reset_gate is None:
            self.reset_gate = nn.Sequential(
                nn.Linear(h_dims * 2 + x_dims, n_units),
                nn.Tanh(),
                nn.Linear(n_units, h_dims),
                nn.Sigmoid())
            utils.init_network_weights(self.reset_gate)
        else:
            self.reset_gate = reset_gate

        if new_state_net is None:
            self.new_state_net = nn.Sequential(
                nn.Linear(h_dims * 2 + x_dims, n_units),
                nn.Tanh(),
                nn.Linear(n_units, h_dims * 2))
            utils.init_network_weights(self.new_state_net)
        else:
            self.new_state_net = new_state_net

    def forward(self, y_mean, y_std, x):
        y_concat = torch.cat((y_mean, y_std, x), -1)

        update_gate = self.update_gate(y_concat)
        reset_gate = self.reset_gate(y_concat)
        concat = torch.cat((y_mean * reset_gate, y_std * reset_gate, x), -1)

        new_state, new_state_std = utils.split_last_dim(self.new_state_net(concat))
        new_state_std = new_state_std.abs()

        new_y = (1 - update_gate) * new_state + update_gate * y_mean
        new_y_std = (1 - update_gate) * new_state_std + update_gate * y_std

        new_y_std = new_y_std.abs()
        return new_y, new_y_std


class Recurrent(nn.Module):
    def __init__(self, x_dims, y_dims,
                 n_gru_units=100, n_out_units=100,
                 diffeq_solver=None, RNN_net=None, gaussian_likelihood_std=0.02):

        super(Recurrent, self).__init__()
        self.h_dims = diffeq_solver.diff_func.h_dims
        from neural_ode.models.diff_func import CDEFunc
        self.using = ('CDE' if isinstance(diffeq_solver.diff_func, CDEFunc) else 'ODE_RNN')
        self.diffeq_solver = diffeq_solver
        self.gaussian_likelihood_std = torch.tensor([gaussian_likelihood_std])

        if self.using == "CDE":
            self.x_dims = x_dims
            self.initial_net = nn.Linear(x_dims, self.h_dims)
            utils.init_network_weights(self.initial_net)
            self.output_net = nn.Linear(self.h_dims, y_dims)

        elif self.using == "ODE_RNN":
            if RNN_net is None:
                self.RNN_net = GRU_with_std(self.h_dims, x_dims, n_units=n_gru_units)
            else:
                self.RNN_net = RNN_net
            self.output_net = utils.create_net(self.h_dims, y_dims, n_out_units)

        else:
            raise NotImplementedError

    def compute_loss_one_batch(self, batch_dict, kl_coef=1.):
        #TRANS: Predict the y series, including the observed and was not observed
        y_pred = self.forward(batch_dict["y_time"], batch_dict["x_data"], batch_dict["x_time"], batch_dict["x_mask"])

        #TRANS: To calculate indicators, batch_dict "y_mask" if is True, it should enter None
        likelihood = get_log_likelihood(y_pred, batch_dict["y_data"], self.gaussian_likelihood_std, None if batch_dict["y_mask"].all() else batch_dict["y_mask"])
        mse_loss = get_mse(y_pred, batch_dict["y_data"], None if batch_dict["y_mask"].all() else batch_dict["y_mask"])
        loss = - torch.mean(likelihood)  #TRANS: To maximize the average log_likelihood back propagation

        #TRANS: Take out the corresponding value
        results = {}
        results["loss"] = loss  #TRANS: Can't detach, otherwise can not backward
        results["likelihood"] = likelihood.detach()
        results["mse"] = mse_loss.detach()
        results["kl_coef"] = kl_coef
        results["C_kl"] = 0.
        results["C_std"] = 0.

        return results

    def run_ODE_RNN(self, y_time_index, x, x_time):
        batch_size = x.shape[0]

        prev_hi = torch.zeros((batch_size, self.h_dims))
        prev_hi_std = torch.zeros((batch_size, self.h_dims))

        prev_ti, ti = x_time[-1] + 0.01, x_time[-1]
        #TRANS: Internal grid size
        interval_length = x_time[-1] - x_time[0]
        minimum_step = interval_length / 50

        time_points_iter = range(0, x_time.numel())
        outputs = []

        for i in time_points_iter:
            if (prev_ti - ti) < minimum_step:  #TRANS: If it can not meet the minimum step direct linear increase
                time_points = torch.stack((prev_ti, ti))
                inc = self.diffeq_solver.diff_func(
                    prev_ti, prev_hi) * (ti - prev_ti)  #TRANS: The linear increment


                hi_ode = prev_hi + inc

            else:
                n_intermediate_tp = max(2, ((prev_ti - ti) / minimum_step).int())
                time_points = utils.linspace_vector(prev_ti, ti, n_intermediate_tp)

                hi_ode = self.diffeq_solver(prev_hi, time_points)[:, -1, :]  #TRANS: Calculate the ODE, work out corresponding latent value to (1) finally can

            xi = x[:, i, :]

            hi, hi_std = self.RNN_net(hi_ode, prev_hi_std, xi)  #TRANS: Calculate the GRU helped

            outputs.append(self.output_net(hi))
            # prev_hi-(odesolver)->hi_ode-(GRU)->hi->prev_hi
            prev_hi, prev_hi_std = hi, hi_std
            prev_ti, ti = x_time[i], x_time[i - 1]

        outputs = torch.stack(outputs, dim=1)
        return outputs.index_select(1, y_time_index)

    def forward(self, y_time, x_data, x_time, x_mask=None):
        # import xdev; xdev.embed()
        #TRANS: Complete sequence prediction
        if len(y_time.shape) < 1:
            y_time = y_time.unsqueeze(0)
        #TRANS: Stitching x_time and y_time
        # TODO replace this with something less funky
        x_len_orig = len(x_time)
        x_time, time_index = torch.unique(torch.cat((x_time, y_time)), return_inverse=True)
        x_time_idx, y_time_idx = time_index[:x_len_orig], time_index[x_len_orig:]

        if self.using == "CDE":
            X = CubicSpline(x_data, x_time)
            X0 = X.evaluate(x_time[0])
            h0 = self.initial_net(X0)
            hs = self.diffeq_solver(h0, x_time, X)
            outputs = self.output_net(hs)
            y_pred = outputs.index_select(1, y_time_idx)

        elif self.using == "ODE_RNN":
            if x_mask is not None:
                x = x_data * x_mask  # [batch_size, x_time_points, x_dims]
            else:
                x = x_data
            batch_size, x_dims = x.shape[0], x.shape[2]
            #TRANS: To expand the x
            x_merged = torch.zeros((batch_size, len(x_time), x_dims), dtype=x.dtype, device=utils.get_device(x))
            x_merged[:, x_time_idx, :] = x
            y_pred = self.run_ODE_RNN(y_time_idx, x_merged, x_time)

        else:
            raise NotImplementedError
        return y_pred  # y_pred shape [batch_size, y_time_points, y_dims]

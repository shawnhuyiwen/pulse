# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.


import keras.backend
import matplotlib.pyplot as plt
import multiprocessing
import numpy as np
import os
import pandas as pd
import random
import scipy
import sys
import tensorflow as tf

from keras.callbacks import EarlyStopping
from keras.layers import Dense
from keras.models import Sequential
from sklearn.ensemble import RandomForestRegressor
from sklearn.model_selection import RandomizedSearchCV
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import MinMaxScaler
from sklearn.svm import SVR

import pulse.study.sensitivity_analysis.analysis_utils as analysis


def run(param_values_df, parsed_results_df, results_dir, model_type, quantity_of_interest):
    """
    Run models.
    :param param_values_df: DataFrame - features
    :param parsed_results_df: DataFrame - results
    :param results_dir: string - results directory
    :param model_type: string - model type
    :param quantity_of_interest: string - quantity of interest
    :return: None
    """

    if not os.path.exists(os.path.join(results_dir, "machine_learning")):
        os.mkdir(os.path.join(results_dir, "machine_learning"))
    if not os.path.exists(os.path.join(results_dir, "machine_learning", model_type)):
        os.mkdir(os.path.join(results_dir, "machine_learning", model_type))

    print("Using {} model ...".format(model_type))
    print("\nTraining for quantity of interest: {}".format(quantity_of_interest))
    num_sims = param_values_df.shape[0]

    # set the various training sizes
    trainsizes = np.arange(1000, round(int(0.8 * num_sims), -3), 10000)

    # create variables for multiprocessing
    manager = multiprocessing.Manager()
    metrics = manager.list()
    args_list = [(param_values_df, parsed_results_df, trainsize, metrics, model_type, quantity_of_interest,
                  results_dir) for trainsize in trainsizes]

    # set the number of processors
    num_procs = multiprocessing.cpu_count() - 1
    if len(trainsizes) < num_procs:
        num_procs = len(trainsizes)
    print("Using {} of {} available processors ...".format(num_procs, multiprocessing.cpu_count()))

    a_pool = multiprocessing.Pool(processes=num_procs)
    a_pool.starmap(train_test_model, args_list)

    metrics_np = np.asarray(metrics)
    np.savetxt(os.path.join(results_dir, "machine_learning", model_type, "correlation_coefficients.csv"),
               metrics_np, header="Training size,R^2,MSE,S1 MSE, ST MSE", delimiter=",", comments='')
    plot_ml_outcome_metrics(metrics_np, results_dir, model_type)


def train_test_model(param_values_df, parsed_results_df, trainsize, metrics, model_type, quantity_of_interest,
                     results_dir):
    """
    Train and test the model.
    :param param_values_df: DataFrame - features
    :param parsed_results_df: DataFrame - results
    :param results_dir: string - results directory
    :param metrics: - manager.list() - shared list among processors holding results of model fits
    :param quantity_of_interest: string - quantity of interest
    :param trainsize: int - size of training set
    :param model_type: string - model type
    :return: None
    """

    # split the data into training and test sets
    train_indices = random.sample(list(param_values_df.index.values), trainsize)
    test_indices = list(set(list(param_values_df.index.values)) - set(train_indices))
    param_values_df_train = param_values_df.loc[train_indices]
    parsed_results_df_train = parsed_results_df.loc[train_indices]
    param_values_df_test = param_values_df.loc[test_indices]
    parsed_results_df_test = parsed_results_df.loc[test_indices]

    # use a CPU instead of a GPU here because the neural network is small and runs faster on a GPU
    with tf.device("cpu:0"):
        print("tf.keras code in this scope will run on CPU")
        # training the neural network
        print("Training the {} with trainsize: {} ...\n".format(model_type, trainsize))
        if model_type == "NeuralNetwork":
            x_train, x_val, y_train, y_val = train_test_split(
                param_values_df_train.to_numpy(), parsed_results_df_train[quantity_of_interest].to_numpy(),
                test_size=0.2)
            model = neural_network_model(x_train, y_train, x_val, y_val)
        elif model_type == "SupportVectorMachines":
            model = support_vector_machine_model(
                param_values_df_train.to_numpy(), parsed_results_df_train[quantity_of_interest].to_numpy())
        elif model_type == "RandomForest":
            model = random_forest_model(
                param_values_df_train.to_numpy(), parsed_results_df_train[quantity_of_interest].to_numpy())
        else:
            raise ValueError("Model type not supported.")

        prediction_test_set = np.squeeze(model.predict(param_values_df_test.to_numpy()))
        r_squared, mse = plot_ml_vs_simulation(parsed_results_df_test[quantity_of_interest], prediction_test_set,
                                               "{} - Training Size - {} ".format(quantity_of_interest, trainsize),
                                               model_type, results_dir)

        # predict results for the rest of the parameter space
        prediction_test_set_df = pd.DataFrame(data=prediction_test_set, index=list(test_indices),
                                              columns=[quantity_of_interest])
        all_results_df = prediction_test_set_df.append(parsed_results_df_train[quantity_of_interest].to_frame())
        predicted_sobol_indices_df = analysis.compute_sobol_indices(all_results_df.sort_index(axis=0),
                                                                    quantity_of_interest, results_dir)
        predicted_sobol_indices_df = predicted_sobol_indices_df.rename(
            columns={quantity_of_interest: "Predicted {}".format(quantity_of_interest)})

        # compute the true global sensitivity indices for the entire parameter space
        true_sobol_indices_df = analysis.compute_sobol_indices(
            parsed_results_df[quantity_of_interest].to_frame(), quantity_of_interest, results_dir)
        true_sobol_indices_df = true_sobol_indices_df.rename(
            columns={quantity_of_interest: "True {}".format(quantity_of_interest)})

        concat_df = pd.concat([predicted_sobol_indices_df, true_sobol_indices_df], axis=1)
        # compute errors in global sensitivity indices
        s1_errors = np.abs(concat_df["Predicted {}".format(quantity_of_interest), "S1"] - concat_df[
            "True {}".format(quantity_of_interest), "S1"])
        st_errors = np.abs(concat_df["Predicted {}".format(quantity_of_interest), "ST"] - concat_df[
            "True {}".format(quantity_of_interest), "ST"])
        concat_df["Errors", "S1"] = s1_errors
        concat_df["Errors", "ST"] = st_errors
        s1_mse = np.sum(s1_errors**2) / concat_df.shape[0]
        st_mse = np.sum(st_errors ** 2) / concat_df.shape[0]
        metrics.append([trainsize, r_squared, mse, s1_mse, st_mse])

        concat_df.to_csv(os.path.join(results_dir, "machine_learning", model_type,
                                      "predicted_sobol_indices_training_size_{}.csv".format(trainsize)))

        if model_type == "NeuralNetwork":
            keras.backend.clear_session()


def neural_network_model(x_train, y_train, x_val, y_val):
    """
    Build and train neural network model.
    :param x_train: NumPy array - x training values
    :param y_train: NumPy array - y training values
    :return: Keras neural network model
    """

    # build the neural network
    model = Sequential()
    model.add(Dense(256, input_dim=x_train.shape[1], kernel_initializer='normal', activation='relu'))
    model.add(Dense(64, kernel_initializer='normal', activation='relu'))
    model.add(Dense(16, kernel_initializer='normal', activation='relu'))
    model.add(Dense(4, kernel_initializer='normal', activation='relu'))
    model.add(Dense(1, kernel_initializer='normal'))
    # model.summary()
    # Compile model
    model.compile(loss='mean_squared_error', optimizer='adam')

    # train the neural network
    call_backs = [EarlyStopping(monitor='val_loss', mode='min', verbose=0, patience=50)]
    model.fit(x_train, y_train, validation_data=(x_val, y_val), epochs=1000, batch_size=32, verbose=0,
              callbacks=call_backs)

    return model


def random_forest_model(x_train, y_train):
    """
    Build and train random forest model.
    :param x_train: NumPy array - x training values
    :param y_train: NumPy array - y training values
    :return: random forest model
    """

    param_grid = {
        'n_estimators': np.arange(100, 2000, 100).astype(int),
        'bootstrap': [True, False]
    }

    estimator = RandomForestRegressor(n_estimators=1000, random_state=42)
    rf = RandomizedSearchCV(estimator, param_grid, verbose=1)

    rf.fit(x_train, y_train)
    best_params = rf.best_estimator_

    # Get numerical feature importances
    importance = list(best_params.feature_importances_)
    # List of tuples with variable and importance
    feature_importance = [(feature, round(importance, 2)) for feature, importance in
                          zip(param_values_df.columns.to_list(), importance)]
    # Sort the feature importance by most important first
    feature_importance = sorted(feature_importance, key=lambda x: x[1], reverse=True)

    # Print out the feature and importance
    print("Variable importance:")
    [print('Variable: {:20} Importance: {}'.format(*pair)) for pair in feature_importance]

    print("\nOptimal regression tree parameters:")
    print(rf.best_params_)

    return best_params


def support_vector_machine_model(x_train, y_train):
    """
    Build and train support vector machine model.
    :param x_train: NumPy array - x training values
    :param y_train: NumPy array - y training values
    :return: support vector machine model
    """

    param_grid = {
        'C': np.linspace(1, 50, 100),
        'gamma': np.linspace(0.00001, 0.05, 100)
    }

    regressor = SVR(kernel='rbf')
    rf = RandomizedSearchCV(regressor, param_grid, verbose=1)

    rf.fit(x_train, y_train)
    best_params = rf.best_estimator_

    print("\nOptimal support vector machine parameters:")
    print(rf.best_params_)

    return best_params


def plot_ml_vs_simulation(y_test, prediction, quantity_of_interest, ml_model, results_dir):
    """
    Plot predicted vs. simulated results.
    :param y_test: NumPy array - simulated (True) values
    :param prediction: NumPy array - predicted values
    :param quantity_of_interest: string - quantity of interest
    :param ml_model: string - model type
    :param results_dir: string - results directory
    :return: floats - R^2, mean squared error
    """

    fig, ax = plt.subplots()
    plt.scatter(y_test, prediction)
    slope, intercept, r_value, p_value, std_err = scipy.stats.linregress(y_test, prediction)
    mse = np.sum((y_test - prediction)**2) / len(y_test)

    plt.text(0.5, 0.93, "R^2 = {}".format(round(r_value ** 2, 2)), horizontalalignment='center',
             verticalalignment='center', transform=ax.transAxes)
    line = gen_best_fit_line(y_test, prediction)

    # create best fit line
    plt.plot(y_test, line, 'r--')
    plt.xlabel("Simulated")
    plt.ylabel("Surrogate")
    plt.title(quantity_of_interest)

    plt.savefig(os.path.join(results_dir, "machine_learning", ml_model,
                             "{}_{}.png".format(quantity_of_interest, ml_model)))

    bland_altman_plot(y_test, prediction, quantity_of_interest, ml_model, results_dir)

    return r_value ** 2, mse


def bland_altman_plot(y_test, prediction, quantity_of_interest, ml_model, results_dir):
    """
    Create Bland-Altman plot.
    :param y_test: NumPy array - simulated results
    :param prediction: NumPy array - predicted results
    :param quantity_of_interest: string - quantity of interest
    :param ml_model: string - machine learning model
    :param results_dir: string - results directory
    :return: None
    """

    mean = (y_test + prediction) / 2.0
    diff = y_test - prediction
    md = np.mean(diff)
    sd = np.std(diff, axis=0)
    plt.figure()
    plt.scatter(mean, diff)
    plt.axhline(md, color='gray', linestyle='-')
    plt.axhline(md + 1.96 * sd, color='r', linestyle='--')
    plt.axhline(md - 1.96 * sd, color='r', linestyle='--')
    plt.xlabel("Mean of Simulation and Prediction")
    plt.ylabel("Simulation - Prediction")
    plt.title(quantity_of_interest)

    plt.savefig(os.path.join(results_dir, "machine_learning", ml_model,
                             "bland_altma`n_{}_{}.png".format(quantity_of_interest, ml_model)))


def plot_ml_outcome_metrics(metrics_np, results_dir, ml_model):
    """
    Plot outcome metrics (R^2, MSE).
    :param metrics_np: NumPy array - outcome metrics
    :param results_dir: string - results directory
    :param ml_model: string - machine learning model
    :return: None
    """

    metrics_names = ["R^2", "MSE", "S1 MSE", "ST MSE"]
    for index, i in enumerate(metrics_names):
        plt.figure()
        plt.scatter(metrics_np[:, 0], metrics_np[:, index + 1])
        plt.xlabel("Training set size")
        plt.ylabel(i)
        plt.title("{} vs. Training Set Size".format(i))
        plt.tight_layout()

        plt.savefig(os.path.join(results_dir, "machine_learning", ml_model,
                                 "{}_{}_{}.png".format(quantity_of_interest, ml_model, i)))


def gen_best_fit_line(x, y):
    """
    Fit a line to plot.
    :param x: list - x values
    :param y: list - y values
    :return: list - best fit line
    """

    w, b = np.polyfit(x, y, deg=1)
    line = w * x + b
    return line


def scale_parameters(param_values_df):
    """
    Scale features.
    :param param_values_df: DataFrame - features
    :return: DataFrame - scaled features
    """

    scaler = MinMaxScaler()
    param_values_df[:] = scaler.fit_transform(param_values_df)

    return param_values_df


if __name__ == '__main__':
    """
    To run, enter:
    python3 ml_surrogate.py arg1 arg2
    arg1: model_type (NeuralNetwork, SupportVectorMachines, RandomForest)
    arg2: quantity of interest (ex: MeanArterialPressure_mmHg)
    """

    results_dir = "./test_results/ntivity_analysis/"
    if len(sys.argv) != 3:
        raise ValueError("Please enter the machine learning model type and the quantity of interest as command line "
                         "arguments (ex: python3 ml_surrogate.py NeuralNetwork MeanArterialPressure_mmHg)")
    model_type = sys.argv[1]
    quantity_of_interest = sys.argv[2]

    # load and parse the simulation results
    phys_systems = "combined"
    parsed_results_df = analysis.load_and_parse_results(results_dir, phys_systems)

    # create our input parameter space
    # this should precisely match the input parameter space we used to run our simulations
    param_values_df = scale_parameters(analysis.sample_parameters(results_dir))

    run(param_values_df, parsed_results_df, results_dir, model_type, quantity_of_interest)

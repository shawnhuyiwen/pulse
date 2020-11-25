# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.


import matplotlib.pyplot as plt
import multiprocessing
import numpy as np
import os
import pandas as pd
import random
import scipy
import sys

# import tensorflow as tf
import keras.backend
from keras.callbacks import EarlyStopping
from keras.callbacks import ModelCheckpoint
from keras.layers import Dense
from keras.models import load_model
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

    param_values_df_scaled = scale_parameters(param_values_df)

    # set the various training sizes
    trainsizes = np.arange(1000, round(int(0.8 * num_sims), -3), 10000)

    # create variables for multiprocessing
    manager = multiprocessing.Manager()
    metrics = manager.list()
    args_list = [(param_values_df_scaled, parsed_results_df, trainsize, metrics, model_type, quantity_of_interest,
                  results_dir) for trainsize in trainsizes]

    # set the number of processors
    num_procs = multiprocessing.cpu_count() - 1
    if len(trainsizes) < num_procs:
        num_procs = len(trainsizes)
    print("Using {} of {} available processors ...".format(num_procs, multiprocessing.cpu_count()))

    with multiprocessing.Pool(processes=num_procs) as a_pool:
        a_pool.starmap(train_test_model, args_list)

    metrics_np = np.asarray(metrics)
    metrics_np = metrics_np[metrics_np[:, 0].argsort()]
    np.savetxt(os.path.join(results_dir, "machine_learning", model_type, "correlation_coefficients.csv"),
               metrics_np, header="Training size,R^2,MSE,S1 MSE,ST MSE,S1 Maximum Error,S2 Maximum Error",
               delimiter=",",
               comments='')
    plot_ml_outcome_metrics(metrics_np, results_dir, model_type)


def train_test_model(param_values_df, parsed_results_df, trainsize, metrics, model_type, quantity_of_interest,
                     results_dir):
    """
    Train and test the model.
    :param param_values_df: DataFrame - features
    :param parsed_results_df: DataFrame - results
    :param trainsize: int - size of training set
    :param metrics: - manager.list() - shared list among processors holding results of model fits
    :param model_type: string - model type
    :param quantity_of_interest: string - quantity of interest
    :param results_dir: string - results directory
    :return: None
    """

    # for some reason, using keras with multiprocessing requires importing the packages separately for each processor
    import tensorflow as tf
    import keras.backend

    # compute the true global sensitivity indices for the entire parameter space
    true_sobol_indices_df = analysis.compute_sobol_indices(
        parsed_results_df[quantity_of_interest].to_frame(), quantity_of_interest, results_dir)
    predicted_sobol_indices_sum_df = pd.DataFrame(0.0, columns=true_sobol_indices_df.columns,
                                                  index=list(true_sobol_indices_df.index.values))
    true_sobol_indices_df = true_sobol_indices_df.rename(
        columns={quantity_of_interest: "True {}".format(quantity_of_interest)})

    # set cross_validation_folds, use 1 if you don't want cross-validation
    cross_validation_folds = 10

    # split the data into training and test sets
    index_list = list(param_values_df.index.values)
    random.shuffle(index_list)
    if cross_validation_folds == 1:
        overflow_range = 0
    else:
        overflow_range = (len(index_list) - trainsize) // (cross_validation_folds - 1)

    r_squared_sum = 0.0
    mse_sum = 0.0
    # perform cross-validation
    i = 0
    while i < cross_validation_folds:
        train_indices = index_list[i * overflow_range: i * overflow_range + trainsize]
        test_indices = list(set(list(param_values_df.index.values)) - set(train_indices))
        param_values_df_train = param_values_df.loc[train_indices]
        parsed_results_df_train = parsed_results_df.loc[train_indices]
        param_values_df_test = param_values_df.loc[test_indices]
        parsed_results_df_test = parsed_results_df.loc[test_indices]

        # use a CPU instead of a GPU here because the neural network is small and runs faster on a CPU
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

        # create some regression plots but for all cross-validation runs
        r_squared, mse = plot_ml_vs_simulation(parsed_results_df_test[quantity_of_interest],
                                               prediction_test_set,
                                               "{} - Training Size - {} - cv - {}".format(quantity_of_interest,
                                                                                          trainsize, i),
                                               model_type, results_dir)
        print("r_squared: {}".format(r_squared))
        print("mse: {}".format(mse))

        # sometimes the initial weights cause the network to get stuck in bad local minima producing very poor results
        if r_squared < 0.8:
            continue

        # predict results for the rest of the parameter space
        prediction_test_set_df = pd.DataFrame(data=prediction_test_set, index=list(test_indices),
                                              columns=[quantity_of_interest])
        all_results_df = prediction_test_set_df.append(parsed_results_df_train[quantity_of_interest].to_frame())
        predicted_sobol_indices_df = analysis.compute_sobol_indices(all_results_df.sort_index(axis=0),
                                                                    quantity_of_interest, results_dir)
        predicted_sobol_indices_sum_df = predicted_sobol_indices_sum_df.add(predicted_sobol_indices_df)

        r_squared_sum += r_squared
        mse_sum += mse

    # compute errors in global sensitivity indices
    print("count: {}".format(cross_validation_folds))
    predicted_sobol_indices_sum_df = predicted_sobol_indices_sum_df / cross_validation_folds
    s1_errors = np.abs(predicted_sobol_indices_sum_df[quantity_of_interest, "S1"] - true_sobol_indices_df[
        "True {}".format(quantity_of_interest), "S1"])
    st_errors = np.abs(predicted_sobol_indices_sum_df[quantity_of_interest, "ST"] - true_sobol_indices_df[
        "True {}".format(quantity_of_interest), "ST"])

    s1_mse = np.sum(s1_errors ** 2) / s1_errors.shape[0]
    st_mse = np.sum(st_errors ** 2) / s1_errors.shape[0]

    predicted_sobol_indices_sum_df = predicted_sobol_indices_sum_df.rename(
        columns={quantity_of_interest: "Predicted {}".format(quantity_of_interest)})
    concat_df = pd.concat([predicted_sobol_indices_sum_df, true_sobol_indices_df], axis=1)

    plot_ml_vs_simulation(concat_df["True {}".format(quantity_of_interest), "ST"],
                          concat_df["Predicted {}".format(quantity_of_interest), "ST"],
                          "{} - Training Size - {} - cv - {} - ST".format(quantity_of_interest,
                                                                          trainsize, i),
                          model_type, results_dir)
    plot_ml_vs_simulation(concat_df["True {}".format(quantity_of_interest), "S1"],
                          concat_df["Predicted {}".format(quantity_of_interest), "S1"],
                          "{} - Training Size - {} - cv - {} - S1".format(quantity_of_interest,
                                                                          trainsize, i),
                          model_type, results_dir)

    concat_df["Errors", "S1"] = s1_errors
    concat_df["Errors", "ST"] = st_errors
    metrics.append(
        [trainsize,
         r_squared_sum / cross_validation_folds,
         mse_sum / cross_validation_folds,
         s1_mse,
         st_mse,
         np.max(s1_errors),
         np.max(st_errors)])

    concat_df.to_csv(os.path.join(results_dir, "machine_learning", model_type,
                                  "predicted_sobol_indices_training_size_{}.csv".format(trainsize)))

    if model_type == "NeuralNetwork":
        keras.backend.clear_session()


def train_test_backwards_model(param_values_df, parsed_results_df, results_dir):
    """
    Run the backwards model that predicts circuit elements from quantities of interest
    :param param_values_df:: DataFrame - features
    :param parsed_results_df: DataFrame - results
    :param results_dir: String - output directory
    :return: None
    """

    parsed_results_df_scaled = scale_parameters(parsed_results_df)

    if not os.path.exists(os.path.join(results_dir, "machine_learning/")):
        os.mkdir(os.path.join(results_dir, "machine_learning"))
    if not os.path.exists(os.path.join(results_dir, "machine_learning", model_type)):
        os.mkdir(os.path.join(results_dir, "machine_learning", model_type))
    if not os.path.exists(os.path.join(results_dir, "machine_learning", model_type, "saved_models")):
        os.mkdir(os.path.join(results_dir, "machine_learning", model_type, "saved_models"))

    x_train, x_val, y_train, y_val = train_test_split(parsed_results_df_scaled.to_numpy(),
                                                      param_values_df.to_numpy(), test_size=0.2, random_state=42)

    if os.path.exists(os.path.join(results_dir, "machine_learning", model_type, "saved_models",
                                   "backwards_model.hdf5")):
        model = load_model(
            os.path.join(results_dir, "machine_learning/NeuralNetwork/saved_models/backwards_model.hdf5"))
        model.summary()
    else:
        model = neural_network_model_backwards(x_train.shape[1], y_val.shape[1])

        # train the neural network
        call_backs = [EarlyStopping(monitor='val_loss', mode='min', verbose=0, patience=50),
                      ModelCheckpoint(os.path.join(results_dir, "machine_learning", model_type, "saved_models",
                                                   "backwards_model.hdf5"), monitor='val_loss', verbose=0,
                                      save_best_only=True, mode='min')
                      ]
        model.fit(x_train, y_train, validation_data=(x_val, y_val), epochs=1000, batch_size=32, verbose=2,
                  callbacks=call_backs)

    prediction_test_set = np.squeeze(model.predict(x_val))

    # create some regression plots
    capacitors = [param_values_df.columns.get_loc(c) for c in param_values_df.columns if
                  'ToGround' in c or 'Aorta1ToAorta4' in c]
    resistors = [param_values_df.columns.get_loc(c) for c in param_values_df.columns if
                 not ('ToGround' in c) and not ('Aorta1ToAorta4' in c)]
    r_squared_cap, mse_cap = plot_ml_vs_simulation(y_val[:, capacitors].flatten(),
                                                   prediction_test_set[:, capacitors].flatten(),
                                                   "Capacitance (mL/mmHg)",
                                                   "NeuralNetwork", results_dir)
    r_squared_res, mse_res = plot_ml_vs_simulation(y_val[:, resistors].flatten(),
                                                   prediction_test_set[:, resistors].flatten(),
                                                   "Resistance (mmHg s/mL)",
                                                   "NeuralNetwork", results_dir)
    print("Capacitors: R^2 - {}, MSE - {}".format(r_squared_cap, mse_cap))
    print("Resistors: R^2 - {}, MSE - {}".format(r_squared_res, mse_res))


def neural_network_model_backwards(num_quantities_of_interest, num_features):
    """
    Build and train neural network model from outputs to inputs.
    :param x_train: NumPy array - x training values (num_sims x num_quantities_of_interest)
    :param y_train: NumPy array - y training values (num_sims x num_circuit_parameters)
    :param x_val: NumPy array - y validation/test values
    :param y_val: NumPy array - y validation/test values
    :return: Keras neural network model
    """

    # build the neural network
    model = Sequential()
    model.add(Dense(512, input_dim=num_quantities_of_interest, kernel_initializer='normal', activation='relu'))
    model.add(Dense(512, kernel_initializer='normal', activation='relu'))
    model.add(Dense(512, kernel_initializer='normal', activation='relu'))
    model.add(Dense(512, kernel_initializer='normal', activation='relu'))
    model.add(Dense(512, kernel_initializer='normal', activation='relu'))
    model.add(Dense(512,  kernel_initializer='normal', activation='relu'))
    model.add(Dense(256, kernel_initializer='normal', activation='relu'))
    model.add(Dense(128, kernel_initializer='normal', activation='relu'))
    model.add(Dense(num_features, kernel_initializer='normal'))
    model.summary()
    # Compile model
    optimizer = keras.optimizers.Adam(lr=0.001)
    model.compile(loss='mean_squared_error', optimizer=optimizer)

    return model


def neural_network_model(x_train, y_train, x_val, y_val):
    """
    Build and train neural network model.
    :param x_train: NumPy array - x training values
    :param y_train: NumPy array - y training values
    :return: Keras neural network model
    """

    # build the neural network
    print("x_train size: {}".format(x_train.shape))
    print("y_train size: {}".format(y_train.shape))
    print("x_val size: {}".format(x_val.shape))
    print("x_val size: {}".format(y_val.shape))

    model = Sequential()
    model.add(Dense(256, input_dim=x_train.shape[1], kernel_initializer='normal', activation='relu'))
    model.add(Dense(64, kernel_initializer='normal', activation='relu'))
    model.add(Dense(16, kernel_initializer='normal', activation='relu'))
    model.add(Dense(4, kernel_initializer='normal', activation='relu'))
    model.add(Dense(1, kernel_initializer='normal'))
    # model.summary()
    # Compile model
    optimizer = keras.optimizers.Adam(lr=0.001)
    model.compile(loss='mean_squared_error', optimizer=optimizer)

    # train the neural network
    call_backs = [EarlyStopping(monitor='val_loss', mode='min', verbose=0, patience=50)]
    model.fit(x_train, y_train, validation_data=(x_val, y_val), epochs=1000, batch_size=32, verbose=2,
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

    font_size = 16
    plt.subplots()
    plt.scatter(y_test, prediction)
    slope, intercept, r_value, p_value, std_err = scipy.stats.linregress(y_test, prediction)
    mse = np.sum((y_test - prediction)**2) / len(y_test)

    line_slope_one = np.linspace(min(y_test), max(y_test), 2)
    plt.plot(line_slope_one, line_slope_one, 'r--')

    plt.xlabel("True", fontsize=font_size)
    plt.ylabel("Predicted", fontsize=font_size)
    plt.xticks(fontsize=font_size)
    plt.yticks(fontsize=font_size)
    plt.title(quantity_of_interest, fontsize=font_size, y=1.02)

    plt.savefig(os.path.join(results_dir, "machine_learning", ml_model,
                             "{}_{}.png".format(quantity_of_interest.replace("/", "_"), ml_model)),
                bbox_inches='tight')
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

    font_size = 16
    mean = (y_test + prediction) / 2.0
    diff = y_test - prediction
    md = np.mean(diff)
    sd = np.std(diff, axis=0)
    plt.figure()
    plt.scatter(mean, diff)
    plt.axhline(md, color='gray', linestyle='-')
    plt.axhline(md + 1.96 * sd, color='r', linestyle='--')
    plt.axhline(md - 1.96 * sd, color='r', linestyle='--')
    plt.xlabel("Mean of Simulation and Prediction", fontsize=font_size)
    plt.ylabel("Simulation - Prediction", fontsize=font_size)
    plt.title(quantity_of_interest, fontsize=font_size, y=1.02)

    plt.savefig(os.path.join(results_dir, "machine_learning", ml_model,
                             "bland_altman_{}_{}.png".format(quantity_of_interest.replace("/", "_"), ml_model)),
                bbox_inches='tight')


def plot_ml_outcome_metrics(metrics_np, results_dir, ml_model):
    """
    Plot outcome metrics (R^2, MSE).
    :param metrics_np: NumPy array - outcome metrics
    :param results_dir: string - results directory
    :param ml_model: string - machine learning model
    :return: None
    """

    font_size = 16
    metrics_names = ["R^2", "MSE", "S1 MSE", "ST MSE"]
    for index, i in enumerate(metrics_names):
        plt.figure()
        plt.scatter(metrics_np[:, 0], metrics_np[:, index + 1])
        plt.xlabel("Training set size", fontsize=font_size)
        plt.ylabel(i, fontsize=font_size)
        plt.title("{} vs. Training Set Size".format(i), fontsize=font_size)
        plt.tight_layout()

        plt.savefig(os.path.join(results_dir, "machine_learning", ml_model,
                                 "{}_{}_{}.png".format(quantity_of_interest, ml_model, i)), bbox_inches='tight')


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
    phys_systems = "cv"
    parsed_results_df = analysis.load_and_parse_results(results_dir, phys_systems)

    # create our input parameter space
    # this should precisely match the input parameter space we used to run our simulations
    param_values_df = analysis.sample_parameters(results_dir)

    run(param_values_df, parsed_results_df, results_dir, model_type, quantity_of_interest)
    # train_test_backwards_model(param_values_df, parsed_results_df, results_dir)

# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.


import json
import matplotlib.pyplot as plt
import numpy as np
import os
import pandas as pd
import seaborn as sns
import sys

from sklearn.model_selection import train_test_split

import pulse.study.sensitivity_analysis.ml_surrogate as ml_surrogate
import pulse.study.sensitivity_analysis.analysis_utils as analysis


def gen_heatmaps(sobol_indices_df, quantities_of_interest, results_dir, sobol_index):
    """
    Generate heatmaps.
    :param sobol_indices_df: Pandas DataFrame - holds Sobol indices
    :param quantities_of_interest: List - holds quantities of interest
    :param results_dir: String - output directory
    :param sobol_index: String - "S1" or "ST"
    :return: None
    """

    print("Generating heat maps ...")
    df = pd.DataFrame()
    for col in quantities_of_interest:
        df[col] = sobol_indices_df[col, sobol_index]

    bins = ["Brain", "Myocardium", "VenaCava", "RightArm", "LeftArm", "RightLeg", "LeftLeg", "Fat", "Muscle", "Skin",
            "Liver", "Spleen", "SmallIntestine", "LargeIntestine", "Splanchnic", "Kidney", "Bone", "LeftPulmonary",
            "RightPulmonary", "Pericardium"]

    save_heatmap(df, results_dir, sobol_index)
    found_bin_bool = True
    if sobol_index == "S1" and len(quantities_of_interest) > 1:
        binned_df = pd.DataFrame(0, columns=quantities_of_interest, index=bins)
        for col in df:
            for index, value in df[col].items():
                found_bin = False
                for part in bins:
                    if part in index:
                        binned_df.loc[part, col] = value
                        found_bin = True
                        break
                if not found_bin and found_bin_bool:
                    print("Could not place {} into a bin.".format(index))
            found_bin_bool = False

        binned_df.to_csv(os.path.join(results_dir, "global_sensitivity_indices", "sobol_indices_binned_s1.csv"))
        save_heatmap(binned_df, results_dir, "{}_binned".format(sobol_index))


def save_heatmap(df, results_dir, sobol_index):
    """
    Save heatmaps.
    :param df: Pandas DataFrame - holds sobol indices
    :param results_dir: String - output directory
    :param sobol_index: String - "S1", "ST", or "S1_binned"
    :return: None
    """

    plt.figure(figsize=(16, 16))
    sns.heatmap(df)
    plt.tight_layout()
    plt.title(sobol_index)

    plt.savefig(os.path.join(results_dir, "global_sensitivity_indices", "heatmap_{}".format(sobol_index)))


def compute_local_sensitivity(parsed_results_df, results_dir):
    """
    Compute local sensitivity indices.
    :param parsed_results_df: DataFrame - simulation results
    :param results_dir: string - results directory
    :return: None
    """

    file = open(os.path.join(results_dir, "local_sa_problem.json"))
    sa_problem = json.load(file)
    file.close()

    values = np.zeros((sa_problem["problem_size"], 1))
    std_dev_df = pd.DataFrame(np.nan, columns=parsed_results_df.columns, index=sa_problem["paths"])
    counter = 0
    for col in parsed_results_df:
        for index, value in enumerate(parsed_results_df[col]):
            if (index + 1) % 100 == 0 and index > 0:
                path = sa_problem["paths"][(index + 1) // 100 - 1]
                std_dev_df.at[path, col] = np.std(values)
                counter = 0
            values[counter] = value
            counter += 1

    for col in std_dev_df:
        std_dev_df[col] = std_dev_df[col] / std_dev_df[col].max()
    print(std_dev_df.head())

    std_dev_df.to_csv(os.path.join(results_dir, "local_sensitivity_indices", "normal_std_dev.csv"))

    plt.figure(figsize=(16, 16))
    sns.heatmap(std_dev_df)
    plt.tight_layout()
    plt.title("STD Local Sensitivity")

    plt.savefig(os.path.join(results_dir, "local_sensitivity_indices", "heatmap_normal_std_dev.png"))


def compute_sobol_indices_without_ml(parsed_results_df, results_dir):
    """
    Compute global sensitivity indices without machine learning.
    :param parsed_results_df: DataFrame - simulation results
    :param results_dir: string - results directory
    :return: None
    """

    print("Computing global sensitivity indices without machine learning ...")
    sobol_indices_df = analysis.compute_sobol_indices(parsed_results_df, "all", results_dir)
    sobol_indices_df.to_csv(os.path.join(results_dir, "global_sensitivity_indices", "sobol_indices.csv"))

    gen_heatmaps(sobol_indices_df, parsed_results_df.columns.to_list(), results_dir, "S1")
    gen_heatmaps(sobol_indices_df, parsed_results_df.columns.to_list(), results_dir, "ST")


def compute_sobol_indices_with_ml(parsed_results_df, model_type, quantity_of_interest, results_dir):
    """
    Compute global sensitivity indices with machine learning
    :param parsed_results_df: DataFrame - simulation results
    :param model_type: string - machine learning model type
    :param quantity_of_interest: string - quantity of interest
    :param results_dir: string - results directory
    :return: None
    """

    print("Computing global sensitivity indices with machine learning ...")

    # create all the results folders we need
    if not os.path.exists(os.path.join(results_dir, "machine_learning")):
        os.mkdir(os.path.join(results_dir, "machine_learning"))
    if not os.path.exists(os.path.join(results_dir, "machine_learning", model_type)):
        os.mkdir(os.path.join(results_dir, "machine_learning", model_type))

    param_values_df = ml_surrogate.scale_parameters(analysis.sample_parameters(results_dir))
    param_values_df_used = param_values_df.loc[list(parsed_results_df.index.values)]
    param_values_df_unused = param_values_df.loc[~param_values_df.index.isin(list(parsed_results_df.index.values))]

    x_train, x_val, y_train, y_val = train_test_split(
        param_values_df_used.to_numpy(), parsed_results_df[quantity_of_interest].to_numpy(), test_size=0.2)

    if model_type == "NeuralNetwork":
        model = ml_surrogate.neural_network_model(x_train, y_train, x_val, y_val)
    elif model_type == "RandomForest":
        model = ml_surrogate.random_forest_model(x_train, y_train)
    elif model_type == "SupportVectorMachine":
        model = ml_surrogate.support_vector_machine_model(x_train, y_train)
    else:
        raise ValueError("Machine learning model type not supported.")

    # test model on the test set
    val_prediction = np.squeeze(model.predict(x_val))
    ml_surrogate.plot_ml_vs_simulation(y_val, val_prediction, quantity_of_interest, model_type, results_dir)

    # predict the remaining simulations
    prediction = np.squeeze(model.predict(param_values_df_unused.to_numpy()))
    total_samples_df = pd.DataFrame(data=prediction, index=list(param_values_df_unused.index.values),
                                    columns=[quantity_of_interest])

    # combine the simulation results with the predicted remaining simulation results
    total_samples_df = total_samples_df.append(parsed_results_df[quantity_of_interest].to_frame())

    # compute the sensitivity indices
    sobol_indices_df = analysis.compute_sobol_indices(total_samples_df.sort_index(axis=0),
                                                      quantity_of_interest, results_dir)

    # output metrics
    sobol_indices_df.to_csv(os.path.join(results_dir, "global_sensitivity_indices", "sobol_indices.csv"))
    gen_heatmaps(sobol_indices_df, [quantity_of_interest], results_dir, "S1")
    gen_heatmaps(sobol_indices_df, [quantity_of_interest], results_dir, "ST")


if __name__ == '__main__':
    results_dir = "./test_results/sensitivity_analysis/"

    # cv or combined
    phys_systems = "cv"
    sensitivity_analysis = "local"

    parsed_results_df = analysis.load_and_parse_results(results_dir, phys_systems)

    if sensitivity_analysis == "local":
        if not os.path.exists(os.path.join(results_dir, "local_sensitivity_indices")):
            os.mkdir(os.path.join(results_dir, "local_sensitivity_indices"))
    elif sensitivity_analysis == "global":
        if not os.path.exists(os.path.join(results_dir, "global_sensitivity_indices")):
            os.mkdir(os.path.join(results_dir, "global_sensitivity_indices"))
    else:
        raise ValueError("Sensitivity analysis type must be either 'local' or 'global'.")

    if len(sys.argv) == 1:
        """
        Compute global sensitivity indices if you were able to run all of the simulations that you need.
        Often, this requires a very large number of simulations. To use this method, run this script 
        as follows: python3 analysis.py
        """
        if sensitivity_analysis == 'local':
            print("Computing local sensitivity parameters ...")
            compute_local_sensitivity(parsed_results_df, results_dir)
        elif sensitivity_analysis == "global":
            print("Computing global sensitivity indices ...")
            compute_sobol_indices_without_ml(parsed_results_df, results_dir)
    elif len(sys.argv) == 3:
        """
        If you ran a subset of the simulations that you need, you can build a surrogate machine learning
        model to predict the remaining simulations. Model type options are NeuralNetwork, SupportVectorMachines,
        and RegressionTrees. I found that SupportVectorMachines worked best with a smaller data set
        (<2,000 simulations), but NeuralNetworks worked best with larger data sets (>2,000 simulations).
        RegressionTrees generally had worse performance, but they are more explainable because you can determine
        the importance of each parameter in your model. You also need to specify the quantity of interest. Because
        machine learning models can take a long time to train, we only want to use this method for a single
        quantity of interest. To use this method, run this script as follows:
        python3 analysis.py arg1 arg2
        arg1: either NeuralNetwork, SupportVectorMachines, or RegressionTrees
        arg2: quantity of interest (ex: MeanArterialPressure_mmHg)
        """
        model_type = sys.argv[1]
        quantity_of_interest = sys.argv[2]
        compute_sobol_indices_with_ml(parsed_results_df, model_type, quantity_of_interest, results_dir)
    else:
        raise ValueError("Please provide either no arguments or the machine learning model type"
                         "('NeuralNetwork', 'SupportVectorMachines', 'RegressionTrees') followed"
                         "by the quantity of interest.")

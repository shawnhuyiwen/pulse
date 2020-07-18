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

    for col in df.columns:
        df.rename({col: col.replace("Mean ", "")}, axis=1, inplace=True)

    df_max = df.max(axis=1)
    indices_to_keep = sorted(range(len(df_max)), key=lambda sub: df_max[sub])[-20:]
    df_keep = df.iloc[indices_to_keep].sort_index()
    # df_keep = df_keep.sort_index()

    abbreviations = {"AbdominalCavity1ToGround": "AA1-G",
                     "Aorta1ToAorta4": "AA1-AA4",
                     "Aorta1ToBone1": "AA1-Bone",
                     "Aorta1ToBrain1": "AA1-Brain",
                     "Aorta1ToFat1": "AA1-Fat",
                     "Aorta1ToLargeIntestine": "AA1-LI",
                     "Aorta1ToLeftArm1": "AA1-LA",
                     "Aorta1ToLeftLeg1": "AA1-LL",
                     "Aorta1ToLiver1": "AA1-Liver",
                     "Aorta1ToMuscle1": "AA1-Muscle",
                     "Aorta1ToMyocardium1": "AA1-MC",
                     "Aorta1ToRightArm1": "AA1-RA",
                     "Aorta1ToRightLeg1": "AA1-RL",
                     "Aorta1ToSkin1": "AA1-Skin",
                     "Aorta1ToSmallIntestine": "AA1-SI",
                     "Aorta1ToSplanchnic": "AA1-SP",
                     "Aorta1ToSpleen": "AA1-Spleen",
                     "Aorta3ToAorta1": "AA3-AA1",
                     "Bone1ToBone2": "Bone-VC",
                     "Bone1ToGround": "Bone-G",
                     "Brain1ToBrain2": "Brain-VC",
                     "Brain1ToGround": "Brain-G",
                     "Fat1ToFat2": "Fat-VC",
                     "Fat1ToGround": "Fat-G",
                     "LargeIntestineToGround": "LI-G",
                     "LargeIntestineToPortalVein": "LI-PV",
                     "LeftArm1ToGround": "LA-G",
                     "LeftArm1ToLeftArm2": "LA-VC",
                     "LeftIntermediatePulmonaryArteriesToLeftPulmonaryArteries": "LIPA-LPA",
                     "LeftLeg1ToGround": "LL-G",
                     "LeftLeg1ToLeftLeg2": "LL-VC",
                     "LeftPulmonaryArteriesToGround": "LPA-G",
                     "LeftPulmonaryArteriesToLeftPulmonaryCapillaries": "LPA-LPC",
                     "LeftPulmonaryArteriesToLeftPulmonaryVeins": "LPA-LPV",
                     "LeftPulmonaryCapillariesToGround": "LPC-G",
                     "LeftPulmonaryCapillariesToLeftPulmonaryVeins": "LPC-LPV",
                     "LeftPulmonaryVeinsToGround": "LPV-G",
                     "LeftPulmonaryVeinsToLeftIntermediatePulmonaryVeins": "LPV-LIPV",
                     "Liver1ToGround": "Liver-G",
                     "Liver1ToLiver2": "Liver-VC",
                     "Muscle1ToGround": "Muscle-G",
                     "Muscle1ToMuscle2": "Muscle-VC",
                     "Myocardium1ToGround": "MC-G",
                     "Myocardium1ToMyocardium2": "MC-VC",
                     "Pericardium1ToGround": "PC-G",
                     "PortalVeinToLiver1": "PV-Liver",
                     "RightArm1ToGround": "RA-G",
                     "RightArm1ToRightArm2": "RA-VC",
                     "RightIntermediatePulmonaryArteriesToRightPulmonaryArteries": "RIPA-RPA",
                     "RightLeg1ToGround": "RL-G",
                     "RightLeg1ToRightLeg2": "RL-VC",
                     "RightPulmonaryArteriesToGround": "RPA-G",
                     "RightPulmonaryArteriesToRightPulmonaryCapillaries": "RPA-RPC",
                     "RightPulmonaryArteriesToRightPulmonaryVeins": "RPA-RPV",
                     "RightPulmonaryCapillariesToGround": "RPC-G",
                     "RightPulmonaryCapillariesToRightPulmonaryVeins": "RPC-RPV",
                     "RightPulmonaryVeinsToGround": "RPV-G",
                     "RightPulmonaryVeinsToRightIntermediatePulmonaryVeins": "RPV-RIPV",
                     "Skin1ToGround": "Skin-G",
                     "Skin1ToSkin2": "Skin-VC",
                     "SmallIntestineToGround": "SI-G",
                     "SmallIntestineToPortalVein": "SI-PV",
                     "SplanchnicToGround": "SP-G",
                     "SplanchnicToPortalVein": "SP-PV",
                     "SpleenToGround": "Spleen-G",
                     "SpleenToPortalVein": "Spleen-PV",
                     "VenaCavaToGround": "VC-G",
                     "VenaCavaToRightHeart2": "VC-RH",
                     "LeftAlveoliToLeftPleuralConnection": "LAL-LPC",
                     "LeftPleuralToRespiratoryMuscle": "LPL-RM",
                     "RightAlveoliToRightPleuralConnection": "RAL-RPC",
                     "RightPleuralToRespiratoryMuscle": "RPL-RM",
                     "CarinaToLeftAnatomicDeadSpace": "Carina-LANDS",
                     "CarinaToRightAnatomicDeadSpace": "Carina-RANDS",
                     "EnvironmentToLeftChestLeak": "ENV-LCL",
                     "EnvironmentToRightChestLeak": "ENV-RCL",
                     "LeftAlveoliLeakToLeftPleural": "LAL-LPL",
                     "RightAlveoliLeakToRightPleural": "RAL-RPL",
                     "LeftAnatomicDeadSpaceToLeftAlveolarDeadSpace": "LANDS-LALDS",
                     "RightAnatomicDeadSpaceToRightAlveolarDeadSpace": "RANDS-RALDS",
                     "LeftPleuralToEnvironment": "LPL-ENV",
                     "RightPleuralToEnvironment": "RPL-ENV",
                     "MouthToCarina": "Mouth-Carina",
                     "MouthToStomach": "Mouth-Stomach",
                     "StomachToEnvironment": "Stomach-ENV"}

    for path in df.index:
        df.rename(index={path: abbreviations[path]}, inplace=True)
    for path in df_keep.index:
        df_keep.rename(index={path: abbreviations[path]}, inplace=True)

    save_heatmap(df, results_dir, sobol_index)

    df_transpose = df_keep.T

    save_heatmap(df_transpose, results_dir, "{}_cleaned".format(sobol_index))


def save_heatmap(df, results_dir, sobol_index):
    """
    Save heatmaps.
    :param df: Pandas DataFrame - holds sobol indices
    :param results_dir: String - output directory
    :param sobol_index: String - "S1", "ST", or "S1_binned"
    :return: None
    """

    plt.figure(figsize=(22, 22))
    sns.set(font_scale=2.8)
    if "Arterial Oxygen Partial Pressure" in df.index:
        plt.figure(figsize=(50, 10))
        sns.set(font_scale=4.0)

    sns.heatmap(df)
    plt.tight_layout()
    plt.title("Global Sensitivity Index {}".format(sobol_index))

    plt.savefig(os.path.join(results_dir, "global_sensitivity_indices", "heatmap_{}".format(sobol_index)),
                bbox_inches='tight')


def compute_local_sensitivity(parsed_results_df, results_dir):
    """
    Compute local sensitivity indices.
    :param parsed_results_df: DataFrame - simulation results
    :param results_dir: string - results directory
    :return: None
    """

    file = open(os.path.join(results_dir, "sobol_problem.json"))
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

    std_dev_df.to_csv(os.path.join(results_dir, "local_sensitivity_indices", "normal_std_dev.csv"))

    indices_to_drop = []
    for index, row in std_dev_df.iterrows():
        if not any(ele > 0.1 for ele in row):
            indices_to_drop.append(index)
    std_dev_df.drop(indices_to_drop, inplace=True)

    plt.figure(figsize=(16, 16))
    sns.set(font_scale=2.0)
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
    sensitivity_analysis = "global"

    parsed_results_df = analysis.load_and_parse_results(results_dir, phys_systems)
    parsed_results_df.columns = [analysis.add_spaces(i) for i in parsed_results_df.columns]

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
        Compute global sensitivity indices if you ran all of the simulations that you need.
        Often, this requires a very large number of simulations. To use this method, run this script 
        as follows: python3 analysis.py
        """
        if sensitivity_analysis == 'local':
            print("Computing local sensitivity parameters ...")
            compute_local_sensitivity(parsed_results_df, results_dir)
        elif sensitivity_analysis == "global":
            print("Computing global sensitivity indices ...")
            compute_sobol_indices_without_ml(parsed_results_df, results_dir)
            # compute_parameter_correlations(parsed_results_df, phys_systems, results_dir)
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

# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import json
import numpy as np
import os
import pandas as pd
import re

from SALib.analyze import sobol
from SALib.sample import saltelli
from tqdm import tqdm


def add_spaces(input_str):
    input_str = input_str.replace("InFlow", "Flow")
    input_str_with_spaces = re.sub(r"(\w)([A-Z])", r"\1 \2", input_str)
    rest = input_str_with_spaces.split("_", 1)[0]

    return rest


def between(start_char, end_char, input_str):
    """
    Get substring between two characters.
    :param start_char: char - initial char
    :param end_char: char - end char
    :param input_str: string
    :return: string - substring between two chars
    """

    result = re.search('{}(.*){}'.format(start_char, end_char), input_str)
    return result.group(1)


def sample_parameters(results_dir):
    """
    Compute Sobol indices (https://salib.readthedocs.io/en/latest/).
    :param results_dir: String - output directory
    :param parsed_results_df: Pandas DataFrame - holds simulation results
    :return: None
    """

    # load the Sobol problem
    file = open(os.path.join(results_dir, "sobol_problem.json"))
    sobol_problem = json.load(file)
    file.close()

    problem_size = sobol_problem["problem_size"]
    del sobol_problem["problem_size"]
    param_values = saltelli.sample(sobol_problem, problem_size)

    param_values_df = pd.DataFrame(data=param_values, columns=sobol_problem["names"])

    return param_values_df


def load_and_parse_results(results_dir, phys_systems):
    """
    Parse JSON results file.
    :param results_dir: String - output directory
    :return: Pandas DataFrame - holds results
    """

    # for now, we are assuming results are separated into many results files - this may change in the future once
    # we settle on a serialization method
    file_nums = []
    for file in os.listdir(os.path.join(results_dir, "simulations")):
        if file.endswith(".json") and file.startswith("simlist_results"):
            file_nums.append(int(between("simlist_results_", ".json", file)))

    if not file_nums:
        raise ValueError("No results files found.")

    file_nums.sort()
    num_sims = file_nums[-1]
    print("Counted {} total simulations.".format(num_sims))

    # store results in DataFrame
    if os.path.exists(os.path.join(results_dir, "simulations/simlist_results_500.json")):
        file = open(os.path.join(results_dir, "simulations/simlist_results_500.json"))
        results_file = json.load(file)
        file.close()
    elif os.path.exists(os.path.join(results_dir, "simulations/simlist_results_0.json")):
        file = open(os.path.join(results_dir, "simulations/simlist_results_0.json"))
        results_file = json.load(file)
        file.close()
        num_sims = 1
    else:
        raise ValueError("No results file found!")

    cols_of_interest = []
    if phys_systems == "cv":
        cols_of_interest = ["MeanAortaInFlow_mL_Per_s",
                            "MeanBoneVasculatureInFlow_mL_Per_s",
                            "MeanBrainVasculatureInFlow_mL_Per_s",
                            "MeanFatVasculatureInFlow_mL_Per_s",
                            "MeanKidneyVasculatureInFlow_mL_Per_s",
                            "MeanLargeIntestineVasculatureInFlow_mL_Per_s",
                            "MeanLeftArmVasculatureInFlow_mL_Per_s",
                            "MeanLeftHeartInFlow_mL_Per_s",
                            "MeanLeftKidneyVasculatureInFlow_mL_Per_s",
                            "MeanLeftLegVasculatureInFlow_mL_Per_s",
                            "MeanLeftPulmonaryArteriesInFlow_mL_Per_s",
                            "MeanLeftPulmonaryCapillariesInFlow_mL_Per_s",
                            "MeanLeftPulmonaryVeinsInFlow_mL_Per_s",
                            "MeanLiverVasculatureInFlow_mL_Per_s",
                            "MeanMuscleVasculatureInFlow_mL_Per_s",
                            "MeanMyocardiumVasculatureInFlow_mL_Per_s",
                            "MeanPulmonaryArteriesInFlow_mL_Per_s",
                            "MeanPulmonaryCapillariesInFlow_mL_Per_s",
                            "MeanPulmonaryVeinsInFlow_mL_Per_s",
                            "MeanRightArmVasculatureInFlow_mL_Per_s",
                            "MeanRightHeartInFlow_mL_Per_s",
                            "MeanRightKidneyVasculatureInFlow_mL_Per_s",
                            "MeanRightLegVasculatureInFlow_mL_Per_s",
                            "MeanRightPulmonaryArteriesInFlow_mL_Per_s",
                            "MeanRightPulmonaryCapillariesInFlow_mL_Per_s",
                            "MeanRightPulmonaryVeinsInFlow_mL_Per_s",
                            "MeanSkinVasculatureInFlow_mL_Per_s",
                            "MeanSmallIntestineVasculatureInFlow_mL_Per_s",
                            "MeanSplanchnicVasculatureInFlow_mL_Per_s",
                            "MeanSpleenVasculatureInFlow_mL_Per_s",
                            "MeanVenaCavaInFlow_mL_Per_s"]
    elif phys_systems == "combined":
        cols_of_interest = ["MeanArterialCarbonDioxidePartialPressure_mmHg",
                            "MeanArterialOxygenPartialPressure_mmHg",
                            "MeanVenousCarbonDioxidePartialPressure_mmHg",
                            "MeanVenousOxygenPartialPressure_mmHg"]

    col_list = [i for i in results_file["Simulation"][0] if i in cols_of_interest]

    count_unstable = 0
    total_sim_time = 0
    df = pd.DataFrame(np.nan, columns=col_list, index=list(range(num_sims)))
    for num in file_nums:
        file = open(os.path.join(results_dir, "simulations/simlist_results_{}.json".format(num)))
        results_file = json.load(file)
        file.close()
        for index, sim in enumerate(results_file["Simulation"]):
            for key in sim:
                if key in cols_of_interest:
                    df.at[sim["ID"], key] = sim[key]
            if not sim["AchievedStabilization"]:
                count_unstable += 1
            total_sim_time += sim["TotalSimulationTime_s"]

    print("Number of unstable simulations: {}".format(count_unstable))
    print("Average physiological simulation time: {}.".format(total_sim_time / num_sims))

    return df


def compute_sobol_indices(parsed_results_df, quantity_of_interest, results_dir):
    """
    Compute Sobol indices (https://salib.readthedocs.io/en/latest/).
    :param parsed_results_df: DataFrame - holds simulation results
    :param quantity_of_interest: String - quantity of interest
    :param results_dir: string - output directory
    :return: None
    """

    print("Computing Sobol indices ...")
    # load the Sobol problem
    file = open(os.path.join(results_dir, "sobol_problem.json"))
    sobol_problem = json.load(file)
    file.close()

    cols = pd.MultiIndex.from_product([parsed_results_df.columns, ["S1", "ST"]])
    sobol_indices_df = pd.DataFrame(np.nan, columns=cols, index=sobol_problem["names"])

    if quantity_of_interest == "all":
        for col in tqdm(parsed_results_df.columns):
            compute_global_index(sobol_indices_df, sobol_problem, parsed_results_df[col].to_numpy(), col)
    elif quantity_of_interest in parsed_results_df.columns:
        compute_global_index(
            sobol_indices_df, sobol_problem, parsed_results_df[quantity_of_interest].to_numpy(), quantity_of_interest)
    else:
        raise ValueError("Quantity of interest not found.")

    return sobol_indices_df


def compute_global_index(sobol_indices_df, sobol_problem, parsed_results, quantity_of_interest):
    """
    Compute global sensitivity index
    :param sobol_indices_df: DataFrame - Sobol indices
    :param sobol_problem: Dictionary - Sobol problem
    :param parsed_results: NumPy array - results
    :param quantity_of_interest: string - quantity of interest
    :return: None
    """

    s_i = sobol.analyze(sobol_problem, parsed_results)
    sobol_indices_df[quantity_of_interest, "S1"] = s_i["S1"]
    sobol_indices_df[quantity_of_interest, "ST"] = s_i["ST"]

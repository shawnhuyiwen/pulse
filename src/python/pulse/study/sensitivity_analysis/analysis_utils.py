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


def load_and_parse_results(results_dir):
    """
    Parse JSON results file.
    :param results_dir: String - output directory
    :return: Pandas DataFrame - holds results
    """

    # for now, we are assuming results are separated into many results files - this may change in the future.
    file_nums = []
    for file in os.listdir(os.path.join(results_dir, "simulations")):
        if file.endswith(".json") and file.startswith("simlist_results"):
            file_nums.append(int(between("sim_list_testing_", ".json", file)))

    if not file_nums:
        raise ValueError("No results files found.")

    file_nums.sort()
    num_sims = file_nums[-1]
    print("Counted {} total simulations.".format(num_sims))

    # store results in DataFrame
    file = open(os.path.join(results_dir, "simulations/simlist_results_sim_list_testing_500.json"))
    results_file = json.load(file)
    file.close()

    col_list = []
    for key in results_file["Simulation"][0]:
        if key not in ["ID", "Name", "Overrides", "AchievedStabilization", "StabilizationTime_s",
                       "TotalSimulationTime_s"]:
            col_list.append(key)

    count_unstable = 0
    total_sim_time = 0
    df = pd.DataFrame(np.nan, columns=col_list, index=list(range(num_sims)))
    for num in file_nums:
        file = open(os.path.join(results_dir, "simulations/simlist_results_sim_list_testing_{}.json".format(num)))
        results_file = json.load(file)
        file.close()
        for index, sim in enumerate(results_file["Simulation"]):
            for key in sim:
                if key not in ["ID", "Name", "Overrides", "AchievedStabilization", "StabilizationTime_s",
                               "TotalSimulationTime_s"]:
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

# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import logging
import os
import shutil
import sys
import pandas as pd
from typing import Optional

from pulse.cdm.utils.csv_utils import read_csv_into_df
from pulse.cdm.utils.file_utils import get_verification_dir
from pulse.cdm.utils.math_utils import percent_difference
from pulse.cdm.plots import SEComparePlotter, SEPlotConfig, SEPlotSource
from pulse.cdm.utils.plotter import compare_plotter
from pulse.cdm.testing import SETestReport

class CSVComparison(SETestReport):
    __slots__ = ["error_limit"]

    def __init__(self):
        super().__init__()

    def compare(self, expected_file_path: str, computed_file_path):
        if not os.path.isfile(expected_file_path):
            logging.error(f"Expected file does not exist {expected_csv}")
            logging.info(f"I am going to try to plot the computed.")
            # TODO: check for zip file
        if not os.path.isfile(computed_file_path):
            logging.error(f"Computed file does not exist {computed_file_path}")

        report = f"{computed_file_path[:-4]}/{os.path.basename(computed_file_path)[:-4]}Report.json"
        self.set_full_report_path(report)

        # Remove and recreate directory
        try:
            shutil.rmtree(self.report_dir)
        except OSError as e:
            logging.warning(f"Could not remove old report directory: {self.report_dir}")
        os.makedirs(self.report_dir)

        # Create the test case
        suite = self.create_test_suite()
        suite.set_name(self.name)
        suite.start_case(self.name)

        # Use log file and directory from computed source
        log_file = computed_file_path[:-len("Results.csv")] + ".log"
        if not os.path.isfile(log_file):
            log_file = None
            logging.warning(f"Could not locate log file: {computed_file_path}")

        total_errors = 0

        # Prepare plot sources so we can open csvs just once
        expected = SEPlotSource(
            csv_data=expected_file_path,
            line_format="-k",
            line_width=4.0,
            label="Expected",
            log_file=log_file)
        computed = SEPlotSource(csv_data=computed_file_path, line_format="-r", label="Computed")

        expected_df = expected.get_data_frame()
        computed_df = computed.get_data_frame()
        if len(expected_df.columns) != len(computed_df.columns):
            logging.warning(f"Number of results is difference, expected ({expected_file_path}) " \
                  f"{len(expected_df.columns)} but computed ({computed_file_path}) is {len(computed_df.columns)}")

        for y_header in expected_df.columns:
            if y_header not in computed_df.columns:
                total_errors += 1
                logging.error(f'Computed results did not provide expected result "{y_header}"')

        # Actually compare CSVs
        error_summary = compare_dfs(expected_df, computed_df, self.error_limit)
        failures = set([column for column in error_summary.columns if error_summary.loc['num errors'][column] > 0])

        # TODO: Generate report

        # Create plots
        config = SEPlotConfig(set_defaults=True)
        config.set_values(
            gridlines=True,
            output_path_override=self.report_dir,
            plot_actions=True,
            plot_events=True,
        )
        plotter = SEComparePlotter(
            config=config,
            expected_source=expected,
            computed_source=computed,
            failures=failures,
        )
        compare_plotter(plotter)


def series_percent_difference(x: pd.Series, y: pd.Series, epsilon: float):
    return pd.Series([percent_difference(xi, yi, epsilon, verbose=False) for xi, yi in zip(x,y)])


def compare_dfs(df1: pd.DataFrame, df2: pd.DataFrame, error_limit: float):
    df_compare = df1.combine(df2, lambda x, y: series_percent_difference(x, y, epsilon=1e-10))
    error_summary = df_compare.apply(lambda x: get_error_info(x, error_limit))

    return error_summary


def get_error_info(x: pd.Series, error_limit: float=2.0):
    filtered = x[x > error_limit]
    min = filtered.min()
    max = filtered.max()
    num_errors = filtered.count()

    return pd.Series([min, max, num_errors], ["min error", "max error", "num errors"])


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')

    expected_csv = None
    computed_csv = None
    output_directory = None
    error_limit = 2.0

    if len(sys.argv) < 2:
        logging.error("Expected inputs : [expected results file path] [computed results file path] [output directory] [percent tolerance]")
        sys.exit(1)
    else:
        if sys.argv[1].endswith(".csv"):
            expected_csv = sys.argv[1]
            computed_csv = sys.argv[2]
            next_arg = 3
        else:
            # Treat this as a scenario name and attempt to locate it
            expected_csv = f"{get_verification_dir()}/scenarios/{sys.argv[1]}Results.csv"
            computed_csv = f"./test_results/scenarios/{sys.argv[1]}Results.csv"
            next_arg = 2

    if len(sys.argv) > next_arg:
        output_directory = sys.argv[next_arg]
        next_arg += 1

    if len(sys.argv) > next_arg:
        error_limit = float(sys.argv[next_arg])
        next_arg += 1

    logging.info(f"Comparing {expected_csv} to {computed_csv}")
    c = CSVComparison()
    c.error_limit = error_limit
    c.compare(expected_csv, computed_csv)

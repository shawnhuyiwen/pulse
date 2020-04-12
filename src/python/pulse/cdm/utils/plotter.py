# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import numpy as np
import pandas as pd
from pathlib import Path
import matplotlib.pyplot as plt

def read_csv_into_df(csv1):
    df = pd.read_csv(csv1)
    return df

def save_compare_plots(out_dir: str,
                       expected_data: pd.DataFrame, computed_data: pd.DataFrame,
                       expected_label_override: str=None, computed_label_override: str=None):
    # Create our output directory
    Path(out_dir).mkdir(parents=True, exist_ok=True)
    # Files we create
    generated_files = []
    # Plot with X always being time
    time_col = expected_data.columns[0]
    if expected_label_override is None:
        expected_label = 'Expected'
    else:
        expected_label = expected_label_override
    if computed_label_override is None:
        computed_label = 'Computed'
    else:
        computed_label = computed_label_override
    for column in expected_data.columns[1:]:
        outfile_name = out_dir + "/" + column.replace("/", "_Per_") + ".png"
        print("Generating "+outfile_name)
        # First Lables
        plt.xlabel("Time(s)")
        plt.ylabel(column)
        plt.title(column+"vs Time")
        plt.ticklabel_format(axis="y", style="sci", scilimits=(0, 0))
        # Math to set Y axis "buffer" zone
        e = expected_data[column]
        c = computed_data[column]
        min_val = min(e[np.isfinite(e)].min(), c[np.isfinite(c)].min())
        max_val = max(e[np.isfinite(e)].max(), c[np.isfinite(c)].max())
        # Set Y axis range and data
        plt.ylim([ min_val - (min_val * .15) , max_val + (abs(max_val) * .05)])
        plt.plot(time_col, column, data=computed_data, color="red", label=computed_label)
        plt.plot(time_col, column, data=expected_data, linestyle="dashed", color="black", label=expected_label)
        # create legend
        plt.legend()
        plt.grid(True)
        # Set ticks on both sides of the Y axis
        plt.tick_params(labelright=True)
        # Set output image size to 1600 x 800
        # Coupled with 100DPI below, should create the right image size
        figure = plt.gcf()
        figure.set_size_inches(16, 8)
        figure.savefig(outfile_name, dpi=100)
        generated_files.append(outfile_name)
        plt.close()
        plt.clf()
    return generated_files

# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

class PlotSource():
    __slots__ = ["label", "df", "style", "color"]
    def __init__(self):
        self.label = ""
        self.df = None
        self.color = "black"
        self.style = "solid"

def read_csv_into_df(csv1):
    df = pd.read_csv(csv1)
    for column in df.columns[1:]:
        # Convert any strings to NaN
        df[column] = pd.to_numeric(df[column], errors='coerce')
        # Replace slashes in units string
        df.rename(columns={column: column.replace("/", "_Per_")}, inplace=True)
    return df

def create_plot(column: str, plot_sources: [PlotSource]):
    plot_source0 = plot_sources[0]
    # Time is always the first column
    time_col = plot_source0.df.columns[0]
    min_val = sys.float_info.max
    max_val = -sys.float_info.max
    for ps in plot_sources:
        if ps.df.shape != plot_source0.df.shape or ps.df.size != plot_source0.df.size:
            print("Data frames are not equal for plotting")
            return False
        d = ps.df[column]
        min_val = min(min_val, d[np.isfinite(d)].min())
        max_val = max(max_val, d[np.isfinite(d)].max())
    plt.figure()
    plt.xlabel("Time(s)")
    plt.ylabel(column)
    plt.title(column + "vs Time")
    plt.ticklabel_format(axis="y", style="sci", scilimits=(0, 0))
    # Set Y axis range and data
    plt.ylim([min_val-(abs(min_val)*.15), max_val+(abs(max_val)*.05)])
    for ps in plot_sources:
        plt.plot(time_col, column, data=ps.df, linestyle=ps.style, color=ps.color, label=ps.label)
    # create legend
    plt.legend()
    plt.grid(True)
    # Set ticks on both sides of the Y axis
    plt.tick_params(labelright=True)
    return True

def save_current_plot(filename: str, x:float=16, y:float=8, dpi:int=100):
    figure = plt.gcf()
    figure.set_size_inches(x, y)
    figure.savefig(filename, dpi=dpi)

def clear_current_plot():
    plt.close()
    plt.clf()


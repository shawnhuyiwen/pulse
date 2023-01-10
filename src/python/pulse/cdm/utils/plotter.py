# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

class PlotSettings():
    __slots__ = ["title", "xlabel", "ylabel", "legend", "grid", "right_ticks",
                 "ticklabel_style", "scilimits"]
    def __init__(self):
        self.title = None
        self.xlabel = None
        self.ylabel = None
        self.legend = False
        self.grid = True
        self.right_ticks = True
        self.ticklabel_style = "sci"
        self.scilimits = (0,0)

class PlotSource():
    __slots__ = ["label", "df", "style", "color", "fill"]
    def __init__(self):
        self.label = ""
        self.df = None
        self.color = "black"
        self.style = "solid"
        self.fill = False

def read_csv_into_df(csv1, replace_slashes=True):
    df = pd.read_csv(csv1)
    for column in df.columns[1:]:
        # Convert any strings to NaN
        df[column] = pd.to_numeric(df[column], errors='coerce')
        # Replace slashes in units string
        if replace_slashes:
            df.rename(columns={column: column.replace("/", "_Per_")}, inplace=True)
    return df

def create_plot(column: str, plot_sources: [PlotSource], plot_settings : PlotSettings=None):
    # Time as x axis (time is always first column)
    return create_plot(plot_sources[0].df.columns[0], column, plot_sources, plot_settings)

def create_plot(column_x: str, column_y: str, plot_sources: [PlotSource], plot_settings : PlotSettings=None):
    if plot_settings is None:
        plot_settings = PlotSettings()

    plot_source0 = plot_sources[0]

    # Determine y axis range
    min_val_y = sys.float_info.max
    max_val_y = -sys.float_info.max
    for ps in plot_sources:
        if ps.df.shape != plot_source0.df.shape or ps.df.size != plot_source0.df.size:
            print("Data frames are not equal for plotting")
            return False
        d_x = ps.df[column_x]
        d_y = ps.df[column_y]
        min_val_y = min(min_val_y, d_y[np.isfinite(d_y)].min())
        max_val_y = max(max_val_y, d_y[np.isfinite(d_y)].max())

    plt.figure()
    plt.xlabel(plot_settings.xlabel if plot_settings.xlabel is not None else column_x)
    plt.ylabel(plot_settings.ylabel if plot_settings.ylabel is not None else column_y)
    plt.title(plot_settings.title if plot_settings.title is not None else (column_y + " vs "+ column_x))
    plt.ticklabel_format(axis="y", style=plot_settings.ticklabel_style, scilimits=plot_settings.scilimits)

    # Set y axis range and data
    plt.ylim([min_val_y-(abs(min_val_y)*.15), max_val_y+(abs(max_val_y)*.05)])
    for ps in plot_sources:
        plt.plot(column_x, column_y, data=ps.df, linestyle=ps.style, color=ps.color, label=ps.label)
        if ps.fill:
            plt.fill_between(column_x, column_y, data=ps.df, facecolor=ps.color)

    # Create legend
    if plot_settings.legend:
        plt.legend()
    if plot_settings.grid:
        plt.grid(True)
    # Set ticks on both sides of the Y axis
    plt.tick_params(labelright=plot_settings.right_ticks)

    return True

def save_current_plot(filename: str, x:float=16, y:float=8, bbox_inches:str="tight", dpi:int=100):
    figure = plt.gcf()
    figure.set_size_inches(x, y)
    figure.savefig(filename, bbox_inches=bbox_inches, dpi=dpi)

def clear_current_plot():
    plt.close()
    plt.clf()

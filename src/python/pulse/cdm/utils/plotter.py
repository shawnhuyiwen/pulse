# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import os
import sys
import numpy as np
import matplotlib.pyplot as plt
from copy import deepcopy

from pulse.cdm.plots import *
from pulse.cdm.io.plots import serialize_plotter_list_from_file

def create_plots(plots_file: str):
    plotters = []
    serialize_plotter_list_from_file(plots_file, plotters)
    for p in plotters:
        if isinstance(p, SEMultiHeaderSeriesPlotter):
            multi_header_series_plotter(p)
        else:
            raise Exception("Unknown plotter type")

def multi_header_series_plotter(plotter: SEMultiHeaderSeriesPlotter):
    if not plotter.has_plot_sources():
        raise Exception("No plot source provided")
    sources = plotter.get_plot_sources()

    # Create output directory if it does not exist
    output_path = plotter.get_plot_settings().get_output_path_override()
    if not os.path.exists(output_path):
        os.makedirs(output_path)

    for series in plotter.get_series():
        settings = deepcopy(series.get_plot_settings()) if series.has_plot_settings() else deepcopy(plotter.get_plot_settings())

        # Default x header is time (first column)
        x_header = series.get_x_header() if series.has_x_header() else sources[0].get_data_frame().columns[0]

        settings.set_output_filename(series.get_output_filename())
        settings.set_x_bounds(series.get_x_bounds())
        settings.set_y_bounds(series.get_y_bounds())
        settings.set_y2_bounds(series.get_y2_bounds())
        if series.has_title():
            settings.set_title(series.get_title())
        else:
            settings.invalidate_title()
        if series.has_x_label():
            settings.set_x_label(series.get_x_label())
        else:
            settings.invalidate_x_label()
        if series.has_y_label():
            settings.set_y_label(series.get_y_label())
        else:
            settings.invalidate_y_label()
        if series.has_y2_label():
            settings.set_y2_label(series.get_y2_label())
        else:
            settings.invalidate_y2_label()

        if not settings.has_title():
            settings.set_title(generate_title(x_header, series.get_y_header()))
        if not settings.get_output_filename():
            settings.set_output_filename(settings.get_title().replace(" ", "_").replace("/", "_Per_"))

        output_filename = settings.get_output_filename()
        if not os.path.splitext(output_filename)[1]:
            output_filename += settings.get_image_properties().get_file_format()
        output_filepath = os.path.join(output_path, output_filename)

        if create_plot(sources, settings, x_header, series.get_y_header(), series.get_y2_header()):
            save_current_plot(output_filepath, settings.get_image_properties())
        clear_current_plot()

def generate_title(x_header, y_header):
    return y_header + " vs "+ x_header

def create_plot(plot_sources: [SEPlotSource],
                plot_settings : SEPlotSettings,
                x_header: str,
                y_header: str,
                y2_header: str = None):

    plot_source0 = plot_sources[0]
    plot_source0_df = plot_source0.get_data_frame()
    for ps in plot_sources:
        ps_df = ps.get_data_frame()
        if ps_df.shape != plot_source0_df.shape or ps_df.size != plot_source0_df.size:
            print("Data frames are not equal for plotting")
            return False

    fig, ax1 = plt.subplots()
    ax1.set_xlabel(plot_settings.get_x_label() if plot_settings.has_x_label() else x_header, fontsize=plot_settings.get_font_size())
    ax1.set_ylabel(plot_settings.get_y_label() if plot_settings.has_y_label() else y_header, fontsize=plot_settings.get_font_size())
    ax1.set_title(plot_settings.get_title() if plot_settings.has_title() else generate_title(x_header, y_header), fontsize=plot_settings.get_font_size())
    ax1.ticklabel_format(axis="y", style=plot_settings.get_tick_style().name, scilimits=plot_settings.get_sci_limits())

    fig.tight_layout()

    # Set up axes
    if plot_settings.get_log_axis():
        ax1.set_yscale("log")
    x_bounds = plot_settings.get_x_bounds()
    if x_bounds.has_lower_bound():
        ax1.set_xlim(left=x_bounds.get_lower_bound())
    if x_bounds.has_upper_bound():
        ax1.set_xlim(right=x_bounds.get_upper_bound())
    y_bounds = plot_settings.get_y_bounds()
    if y_bounds.has_lower_bound():
        ax1.set_ylim(bottom=y_bounds.get_lower_bound())
    if y_bounds.has_upper_bound():
        ax1.set_ylim(top=y_bounds.get_upper_bound())

    if y2_header:
        ax2 = ax1.twinx()
        ax2.set_ylabel(plot_settings.get_y2_label() if plot_settings.has_y2_label() else y2_header, fontsize=plot_settings.get_font_size())
        ax2.ticklabel_format(axis="y", style=plot_settings.get_tick_style().name, scilimits=plot_settings.get_sci_limits())
        if plot_settings.get_log_axis():
            ax2.set_yscale("log")
        y2_bounds = plot_settings.get_y2_bounds()
        if y2_bounds.has_lower_bound():
            ax2.set_ylim(bottom=y2_bounds.get_lower_bound())
        if y2_bounds.has_upper_bound():
            ax2.set_ylim(top=y2_bounds.get_upper_bound())

    # Plot data
    for ps in plot_sources:
        baseline_mode = plot_settings.get_percent_of_baseline_mode()
        if baseline_mode == ePercentageOfBaselineMode.Off:
            cols = []
        elif baseline_mode == ePercentageOfBaselineMode.All:
            cols = [x_header, y_header]
            if y2_header:
                cols.append(y2_header)
        elif baseline_mode == ePercentageOfBaselineMode.OnlyX:
            cols = [x_header]
        elif baseline_mode == ePercentageOfBaselineMode.OnlyY:
            cols = [y_header]
            if y2_header:
                cols.append(y2_header)
        else:
            print(f"Unknown percentage of baseline mode: {baseline_mode}")
            return False
        df = ps.get_data_frame()
        df[cols] = df[cols].apply(lambda x: x / x[0] * 100.0)

        ax1.plot(x_header, y_header, ps.get_line_format(), data=df, label=ps.get_label())
        if plot_settings.get_fill_area():
            ax1.fill_between(x_header, y_header, data=df, facecolor=ps.get_line_format()[-1])

        if y2_header:
            ax2.plot(x_header, y2_header, ps.get_line_format(), data=df, label=ps.get_label())
            if plot_settings.get_fill_area():
                ax1.fill_between(x_header, y2_header, data=df, facecolor=ps.get_line_format()[-1])

    # Create legend
    if not plot_settings.get_remove_legends():
        plt.legend(fontsize=plot_settings.get_legend_font_size())
    plt.grid(plot_settings.get_gridlines())

    return True

def save_current_plot(filename: str, image_props: SEImageProperties):
    figure = plt.gcf()
    figure.set_size_inches(image_props.get_width_inch(), image_props.get_height_inch())
    figure.savefig(filename, bbox_inches='tight', dpi=image_props.get_dpi())

def clear_current_plot():
    plt.close()
    plt.clf()

def get_config_dir():
    with open("run.config") as file:
        for line in file:
            if line.startswith("root_dir"):
                line = line.replace('\n','')
                root = line.split("=")
                return root[1]+"data/config/"
    raise ValueError('Could not find study directory.')

if __name__ == "__main__":
    create_plots(get_config_dir() + "PlotRun.json")

# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import os
import sys
import numpy as np
import matplotlib.pyplot as plt
from copy import deepcopy

from pulse.cdm.plots import *
from pulse.cdm.io.plots import serialize_plot_list_from_file

def create_plots(plots_file: str):
    plots = []
    serialize_plot_list_from_file(plots_file, plots)
    for p in plots:
        if isinstance(p, SERelationalPlotter):
            relational_plotter(p)
        elif isinstance(p, SETimeSeriesPlotter):
            time_series_plotter(p)
        else:
            raise Exception("Unkown plotter type")

def relational_plotter(plotter: SERelationalPlotter):
    if not plotter.has_plot_source():
        raise Exception("No plot source provided")
    source = plotter.get_plot_source()

    output_path = plotter.get_plot_settings().get_output_path_override()
    if not os.path.exists(output_path):
        os.makedirs(output_path)

    for relation in plotter.get_relationships():
        settings = deepcopy(plotter.get_plot_settings())
        settings.set_output_filename(relation.get_output_filename() if relation.has_output_filename() else "")
        settings.set_title(relation.get_title() if relation.has_title() else None)
        settings.set_x1_label(relation.get_label_x() if relation.has_label_x() else None)
        settings.set_y1_label(relation.get_label_y() if relation.has_label_y() else None)
        if not settings.has_title():
            settings.set_title(generate_title(relation.get_header_x(), relation.get_header_y()))
        if not settings.get_output_filename():
            settings.set_output_filename(settings.get_title().replace(" ", "_").replace("/", "_Per_"))

        output_filename = settings.get_output_filename()
        if not os.path.splitext(output_filename)[1]:
            output_filename += settings.get_image_properties().get_file_format()
        output_filepath = os.path.join(output_path, output_filename)

        if create_plot(relation.get_header_x(), relation.get_header_y(), [source], settings):
            save_current_plot(output_filepath, settings.get_image_properties())
        clear_current_plot()

def time_series_plotter(plotter: SETimeSeriesPlotter):
    if not plotter.has_plot_source():
        raise Exception("No plot source provided")
    source = plotter.get_plot_source()

    output_path = plotter.get_plot_settings().get_output_path_override()
    if not os.path.exists(output_path):
        os.makedirs(output_path)

    time_col = source.get_data_frame().columns[0]

    for series in plotter.get_series():
        settings = deepcopy(plotter.get_plot_settings())
        settings.set_output_filename(series.get_output_filename() if series.has_output_filename() else "")
        settings.set_title(series.get_title() if series.has_title() else None)
        settings.set_y1_label(series.get_label() if series.has_label() else None)
        if not settings.has_title():
            settings.set_title(generate_title(time_col, series.get_header()))
        if not settings.get_output_filename():
            settings.set_output_filename(settings.get_title().replace(" ", "_").replace("/", "_Per_"))

        output_filename = settings.get_output_filename()
        if not os.path.splitext(output_filename)[1]:
            output_filename += settings.get_image_properties().get_file_format()
        output_filepath = os.path.join(output_path, output_filename)

        create_plot(time_col, series.get_header(), [source], settings)
        save_current_plot(output_filepath, settings.get_image_properties())
        clear_current_plot()

    if not plotter.has_series(): # If no headers provided, plot all headers individually
        for header in source.get_data_frame().columns[1:]:
            settings = deepcopy(plotter.get_plot_settings())
            settings.set_title(generate_title(time_col, header))
            settings.set_output_filename(settings.get_title().replace(" ", "_").replace("/", "_Per_"))
            output_filename = settings.get_output_filename() + settings.get_image_properties().get_file_format()
            output_filepath = os.path.join(output_path, output_filename)

            print(output_filepath)

            if create_plot(time_col, header, [source], settings):
                save_current_plot(output_filepath, settings.get_image_properties())
            clear_current_plot()

def generate_title(column_x, column_y):
    return column_y + " vs "+ column_x

def create_plot(column: str, plot_sources: [SEPlotSource], plot_settings : SEPlotSettings=None):
    # Time as x axis (time is always first column)
    return create_plot(plot_sources[0].get_data_frame().columns[0], column, plot_sources, plot_settings)

def create_plot(column_x: str, column_y: str, plot_sources: [SEPlotSource], plot_settings : SEPlotSettings=None):
    if plot_settings is None:
        plot_settings = PlotSettings()

    plot_source0 = plot_sources[0]
    plot_source0_df = plot_source0.get_data_frame()

    # Determine y axis range
    min_val_y = sys.float_info.max
    max_val_y = -sys.float_info.max
    for ps in plot_sources:
        ps_df = ps.get_data_frame()
        if ps_df.shape != plot_source0_df.shape or ps_df.size != plot_source0_df.size:
            print("Data frames are not equal for plotting")
            return False
        d_x = ps_df[column_x]
        d_y = ps_df[column_y]
        min_val_y = min(min_val_y, d_y[np.isfinite(d_y)].min())
        max_val_y = max(max_val_y, d_y[np.isfinite(d_y)].max())

    plt.figure()
    plt.xlabel(plot_settings.get_x1_label() if plot_settings.has_x1_label() else column_x)
    plt.ylabel(plot_settings.get_y1_label() if plot_settings.has_y1_label() else column_y)
    plt.title(plot_settings.get_title() if plot_settings.has_title() else generate_title(column_x, column_y))
    plt.ticklabel_format(axis="y", style=plot_settings.get_tick_style().name, scilimits=plot_settings.get_sci_limits())
    plt.tight_layout()

    # Set y axis range and data
    plt.ylim([min_val_y-(abs(min_val_y)*.15), max_val_y+(abs(max_val_y)*.05)])
    for ps in plot_sources:
        plt.plot(column_x, column_y, ps.get_line_format(), data=ps.get_data_frame(), label=ps.get_label())
        if ps.get_fill_area():
            plt.fill_between(column_x, column_y, data=ps.get_data_frame(), facecolor=ps.get_line_format()[-1])

    # Create legend
    if not plot_settings.get_remove_legends():
        plt.legend()
    plt.grid(plot_settings.get_gridlines())

    return True

def save_current_plot(filename: str, image_props: SEImageProperties):
    figure = plt.gcf()
    figure.set_size_inches(image_props.get_width_inch(), image_props.get_height_inch())
    figure.savefig(filename, bbox_inches='tight', dpi=image_props.get_dpi())

def clear_current_plot():
    plt.close()
    plt.clf()

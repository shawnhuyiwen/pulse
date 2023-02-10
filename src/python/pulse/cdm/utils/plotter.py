# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import os
import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import colors as mcolors
from cycler import cycler
from timeit import default_timer as timer
from datetime import timedelta

from pulse.cdm.plots import *
from pulse.cdm.io.plots import serialize_plotter_list_from_file
from pulse.cdm.utils.file_utils import get_config_dir

def create_plots(plots_file: str, benchmark: bool = False):
    plotters = []
    serialize_plotter_list_from_file(plots_file, plotters)
    for p in plotters:
        if isinstance(p, SEMultiHeaderSeriesPlotter):
            multi_header_series_plotter(p, benchmark)
        else:
            raise Exception("Unknown plotter type")

def multi_header_series_plotter(plotter: SEMultiHeaderSeriesPlotter, benchmark: bool = False):
    if benchmark:
        start = timer()

    if not plotter.has_plot_sources():
        raise Exception("No plot source provided")
    sources = plotter.get_plot_sources()
    validation_source = plotter.get_validation_source()

    # Create output directory if it does not exist
    output_path = plotter.get_plot_config().get_output_path_override()
    if not os.path.exists(output_path):
        os.makedirs(output_path)

    for series in plotter.get_series():
        if benchmark:
            start_series = timer()

        config = series.get_plot_config()

        # Default x header is time (first column)
        x_header = series.get_x_header() if series.has_x_header() else sources[0].get_data_frame().columns[0]
        x2_header = None
        if series.has_x2_header():
            x2_header = series.get_x2_header()
        elif series.has_y2_headers():
            x2_header = x_header

        config.set_output_filename(series.get_output_filename())
        config.set_x_bounds(series.get_x_bounds())
        config.set_y_bounds(series.get_y_bounds())
        config.set_y2_bounds(series.get_y2_bounds())
        if series.has_title():
            config.set_title(series.get_title())
        else:
            config.invalidate_title()
        if series.has_x_label():
            config.set_x_label(series.get_x_label())
        else:
            config.invalidate_x_label()
        if series.has_y_label():
            config.set_y_label(series.get_y_label())
        else:
            config.invalidate_y_label()
        if series.has_y2_label():
            config.set_y2_label(series.get_y2_label())
        else:
            config.invalidate_y2_label()

        if not config.has_title():
            config.set_title(generate_title(x_header, series.get_y_headers()[0]))
        if not config.get_output_filename():
            config.set_output_filename(config.get_title().replace(" ", "_").replace("/", "_Per_"))

        output_filename = config.get_output_filename()
        if not os.path.splitext(output_filename)[1]:
            output_filename += config.get_image_properties().get_file_format()
        output_filepath = os.path.join(output_path, output_filename)

        if create_plot(sources,
                       config,
                       x_header,
                       series.get_y_headers(),
                       x2_header,
                       series.get_y2_headers(),
                       validation_source):
            save_current_plot(output_filepath, config.get_image_properties())
        clear_current_plot()

        if benchmark:
            end_series = timer()
            print(f'Series Execution Time: {timedelta(seconds=end_series - start_series)}')

    if benchmark:
        end = timer()
        print(f'Plotter Execution Time: {timedelta(seconds=end - start)}')

def generate_title(x_header, y_header):
    return y_header + " vs "+ x_header

def percentage_of_baseline(baseline_mode, df, x_header, y_headers, x2_header, y2_headers):
    xcols = []
    ycols = []
    if baseline_mode == ePercentageOfBaselineMode.All:
        xcols = [x_header]
        if x2_header:
            xcols.append(x2_header)
        ycols = list(y_headers)
        if y2_headers:
            ycols.extend(y2_headers)
    elif baseline_mode == ePercentageOfBaselineMode.OnlyX:
        xcols = [x_header]
        if x2_header:
            xcols.append(x2_header)
    elif baseline_mode == ePercentageOfBaselineMode.OnlyY:
        ycols = list(y_headers)
        if y2_headers:
            ycols.extend(y2_headers)
    elif baseline_mode != ePercentageOfBaselineMode.Off:
        print(f"Unknown percentage of baseline mode: {baseline_mode}")
        return False
    df[xcols] = df[xcols].apply(lambda x: (1-(x / x[0])) * 100.0)
    df[ycols] = df[ycols].apply(lambda x: x * 100.0 / x[0])
    return True

def create_plot(plot_sources: [SEPlotSource],
                plot_config : SEPlotConfig,
                x_header: str,
                y_headers: [str],
                x2_header: str = None,
                y2_headers: [str] = [],
                validation_source: SEPlotSource = None):

    # To line formats across axes
    setup_fmt_cycler = cycler(linestyle=['-', '--', '-.', ':']) * cycler(color=mcolors.TABLEAU_COLORS)
    fmt_cycler = setup_fmt_cycler()

    # To support legend across axes
    lns = []

    # Labels and title
    fig, ax1 = plt.subplots()
    ax1.set_xlabel(plot_config.get_x_label() if plot_config.has_x_label() else x_header, fontsize=plot_config.get_font_size())
    ax1.set_ylabel(plot_config.get_y_label() if plot_config.has_y_label() else y_headers[0], fontsize=plot_config.get_font_size())
    ax1.set_title(plot_config.get_title() if plot_config.has_title() else generate_title(x_header, y_headers[0]), fontsize=plot_config.get_font_size())
    ax1.ticklabel_format(axis="y", style=plot_config.get_tick_style().name, scilimits=plot_config.get_sci_limits())

    # Main axes
    if plot_config.get_log_axis():
        ax1.set_yscale("log")
    x_bounds = plot_config.get_x_bounds()
    if x_bounds.has_lower_bound():
        ax1.set_xlim(left=x_bounds.get_lower_bound())
    if x_bounds.has_upper_bound():
        ax1.set_xlim(right=x_bounds.get_upper_bound())
    y_bounds = plot_config.get_y_bounds()
    if y_bounds.has_lower_bound():
        ax1.set_ylim(bottom=y_bounds.get_lower_bound())
    if y_bounds.has_upper_bound():
        ax1.set_ylim(top=y_bounds.get_upper_bound())

    # Secondary y axis
    if y2_headers:
        ax2 = ax1.twinx()
        ax2.set_ylabel(plot_config.get_y2_label() if plot_config.has_y2_label() else y2_headers[0], fontsize=plot_config.get_font_size())
        ax2.ticklabel_format(axis="y", style=plot_config.get_tick_style().name, scilimits=plot_config.get_sci_limits())
        if plot_config.get_log_axis():
            ax2.set_yscale("log")
        y2_bounds = plot_config.get_y2_bounds()
        if y2_bounds.has_lower_bound():
            ax2.set_ylim(bottom=y2_bounds.get_lower_bound())
        if y2_bounds.has_upper_bound():
            ax2.set_ylim(top=y2_bounds.get_upper_bound())

    # Plot data
    for ps in plot_sources:
        baseline_mode = plot_config.get_percent_of_baseline_mode()
        df = ps.get_data_frame()
        if not percentage_of_baseline(baseline_mode,
                                      df,
                                      x_header,
                                      y_headers,
                                      x2_header if not validation_source else None,
                                      y2_headers if not validation_source else None):
            return False

        for y_header in y_headers:
            if ps.has_line_format():
                lns.extend(ax1.plot(x_header, y_header, ps.get_line_format(), data=df, label=ps.get_label() if ps.has_label() else plot_config.get_y_label()))
                color = ps.get_line_format()[-1]
            else:
                c = next(fmt_cycler)
                color = c['color']
                lns.extend(ax1.plot(x_header, y_header, **c, data=df, label=ps.get_label() if ps.has_label() else plot_config.get_y_label()))

            if plot_config.get_fill_area():
                ax1.fill_between(x_header, y_header, data=df, facecolor=color)

        # Secondary axis, not from validation data
        if not validation_source:
            for y2_header in y2_headers:
                if ps.has_line_format():
                    lns.extend(ax2.plot(x2_header, y2_header, ps.get_line_format(), data=df, label=ps.get_label() if ps.has_label() else plot_config.get_y2_label()))
                    color = ps.get_line_format()[-1]
                else:
                    c = next(fmt_cycler)
                    color = c['color']
                    lns.extend(ax2.plot(x2_header, y2_header, **c, data=df, label=ps.get_label() if ps.has_label() else plot_config.get_y2_label()))
                if plot_config.get_fill_area():
                    ax2.fill_between(x2_header, y2_header, data=df, facecolor=color)

    # Plot validation data if needed
    if validation_source:
        df = validation_source.get_data_frame()
        for y2_header in y2_headers:
            if validation_source.has_line_format():
                lns.extend(ax2.plot(x2_header, y2_header, validation_source.get_line_format(), data=df, label=validation_source.get_label() if validation_source.has_label() else plot_config.get_y2_label()))
                color = validation_source.get_line_format()[-1]
            else:
                c = next(fmt_cycler)
                color = c['color']
                lns.extend(ax2.plot(x2_header, y2_header, **c, data=df, label=validation_source.get_label() if validation_source.has_label() else plot_config.get_y2_label()))
            if plot_config.get_fill_area():
                ax2.fill_between(x2_header, y2_header, data=df, facecolor=color)

    # Legend and gridline settings
    if not plot_config.get_remove_legends():
        lbls = [l.get_label() for l in lns]
        plt.legend(lns, lbls, fontsize=plot_config.get_legend_font_size())
    plt.grid(plot_config.get_gridlines())

    fig.tight_layout()

    return True

def save_current_plot(filename: str, image_props: SEImageProperties):
    print("Creating plot "+filename)
    figure = plt.gcf()
    figure.set_size_inches(image_props.get_width_inch(), image_props.get_height_inch())
    figure.savefig(filename, bbox_inches='tight', dpi=image_props.get_dpi())

def clear_current_plot():
    plt.close()
    plt.clf()

if __name__ == "__main__":
    benchmark = False
    plot_config = None

    if len(sys.argv) > 1:
        if os.path.isfile(sys.argv[1]):
          plot_config = sys.argv[1]
        elif os.path.isfile(get_config_dir()+sys.argv[1]):
          plot_config = get_config_dir()+sys.argv[1]
    if len(sys.argv) > 2 and (sys.argv[2] == '--benchmark' or sys.argv[2] == '-b'):
        benchmark = True

    if plot_config is None:
        print("Please provide a valid json configuration")
    else:
        if benchmark:
            start = timer()

        create_plots(plot_config, benchmark)

        if benchmark:
            end = timer()
            print(f'Total Execution Time: {timedelta(seconds=end - start)}')

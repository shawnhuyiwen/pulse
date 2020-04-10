import os
import argparse
import matplotlib.pyplot as plt
import pandas as pd

def read_csv_into_df(csv1, csv2):
    file_one_data = pd.read_csv(csv1)
    file_two_data = pd.read_csv(csv2)
    return file_one_data, file_two_data

def create_plots_from_df(file_one_name: str, file_one_data: pd.DataFrame, file_two_name: str, file_two_data: pd.DataFrame):
    plot_list = {}
    time_col = file_one_data.columns[0]
    for column in file_one_data.columns[1:]:
        print(column)
        # First Labels
        figure = plt.figure()
        plot = figure.add_subplot(1, 1, 1)
        plot.set_xlabel("Time(s)")
        plot.set_ylabel(column)
        plot.set_title("%svsTime" % column)
        plot.ticklabel_format(axis="y", style="sci", scilimits=(0, 0))
        # Math to set Y axis "buffer" zone
        min_val = file_two_data[column].min()
        max_val = file_two_data[column].max()
        # Set Y axis range and data
        plot.set_ylim([ min_val - (min_val * .15) , max_val + (abs(max_val) * .05)])
        plot.plot(time_col, column, data=file_two_data, color="red", label=file_two_name)
        plot.plot(time_col, column, data=file_one_data, linestyle="dashed", color="black", label=file_one_name)
        # create legend
        plot.legend()
        plot.grid(True)
        # Set ticks on both sides of the Y axis
        plot.tick_params(labelright=True)

        # Set output image size to 1600 x 800
        # Coupled with 100DPI below, should create the right image size
        figure.set_size_inches(16, 8)
        plot_list[column] = figure
    return plot_list

# Unused for now, as we determine how to save lists of plots
# plot_name parameter to show only desired plot, not implemented yet.
# Use with caution.
def show_plots(plot_list, plot_name = ''):
    for plot_name in plot_list:
        plot_list[plot_name].show(block=True)
    # Needed for direct runs to give plots time to load and be shown.  TODO: Make it not this :-(
    # import pdb; pdb.set_trace()

def save_plots(outdir, plot_list):
    generated_files = []
    for plot_name in plot_list:
       # Split of plot_name removes units from plot file name#
       outfile_name = os.path.join(outdir, plot_name.split("(")[0])
       plot_list[plot_name].savefig(outfile_name, dpi=100)
       generated_files.append(outfile_name)
    return generated_files


def create_compare_plot(csv1, csv2):
    # reads CSV into PANDAS dataFrame
    file_one_dataframe, file_two_dataframe = read_csv_into_df(csv1,csv2)
    line_one_name = os.path.split(csv1)[1]
    line_two_name = os.path.split(csv2)[1]
    plot_dictionary = create_plots_from_df(line_one_name, file_one_dataframe,
                         line_two_name, file_two_dataframe)
    return plot_dictionary


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", '--outdir',  dest="output_path",required=True, action="store")
    parser.add_argument("-f1", '--file_one', dest="file_one", required=True, action="store")
    parser.add_argument("-f2", '--file_two',  dest="file_two",required=True, action="store")
    options = parser.parse_args()
    plot_dictionary = create_compare_plot(options.file_one, options.file_two)
    line_one_name = os.path.split(options.file_one)[1]
    line_two_name = os.path.split(options.file_two)[1]
    # checks for output directory and makes it if necessary
    proposed_output = os.path.join(options.output_path, line_one_name)
    if not os.path.exists(proposed_output):
        os.makedirs(proposed_output)
    save_plots(proposed_output, plot_dictionary)
    # show_plots(plot_dictionary)
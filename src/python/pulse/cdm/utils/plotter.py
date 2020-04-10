import os
import argparse
import matplotlib.pyplot as plt
import pandas as pd

def read_csv_into_df(csv1, csv2):
    file_one_data = pd.read_csv(csv1)
    file_two_data = pd.read_csv(csv2)
    return file_one_data, file_two_data

def create_plots_from_df(file_one_name: str, file_one_data: pd.DataFrame, file_two_name: str, file_two_data: pd.DataFrame, out_dir = os.getcwd()):
    plot_list = {}
    time_col = file_one_data.columns[0]
    for column in file_one_data.columns[1:]:
        print(column)
        # First Lables
        plt.xlabel("Time(s)")
        plt.ylabel(column)
        plt.title("%svsTime" % column)
        plt.ticklabel_format(axis="y", style="sci", scilimits=(0, 0))
        # Math to set Y axis "buffer" zone
        min_val = file_two_data[column].min()
        max_val = file_two_data[column].max()
        # Set Y axis range and data
        plt.ylim([ min_val - (min_val * .15) , max_val + (abs(max_val) * .05)])
        plt.plot(time_col, column, data=file_two_data, color="red", label=file_two_name)
        plt.plot(time_col, column, data=file_one_data, linestyle="dashed", color="black", label=file_one_name)
        # create legend
        plt.legend()
        plt.grid(True)
        # Set ticks on both sides of the Y axis
        plt.tick_params(labelright=True)
        #plot_list[column] = fig

        # Set output image size to 1600 x 800
        # Coupled with 100DPI below, should create the right image size
        figure = plt.gcf() # get current figure
        figure.set_size_inches(16, 8)

        outfile_name = os.path.join(out_dir, column.split("(")[0])
        plt.savefig(outfile_name, dpi = 100)
        plt.clf()
    return plot_list

# Unused for now, as we determine how to save lists of plots
def show_plots(plot_list):
    for plot_name in plot_list:
        plot_list[plot_name].show()

# Unused for now, while we determine how to save list of plots
def save_plots(outdir, plot_list):
    generated_files = []
    for plot_name in plot_list:
       # Split of plot_name removes units from plot file name#
       outfile_name = os.path.join(outdir, plot_name.split("(")[0])
       plot_list[plot_name].savefig(outfile_name)
       generated_files.append(outfile_name)
    return generated_files


def create_compare_plot(out_dir, csv1, csv2):
    # First split gives me the whole file name
    line_one_name = os.path.split(csv1)[1]
    line_two_name = os.path.split(csv2)[1]
    # checks for output directory and makes it if necessary
    proposed_output = os.path.join(options.output_path, line_one_name)
    if not os.path.exists(proposed_output):
        os.makedirs(proposed_output)
    # reads CSV into PANDAS dataFrame
    file_one_dataframe, file_two_dataframe = read_csv_into_df(csv1,csv2)

    create_plots_from_df(line_one_name, file_one_dataframe,
                         line_two_name, file_two_dataframe,
                         proposed_output)

    #
    # show_plots(plot_list)
    # generated_files = save_plots(proposed_output, plot_list)
    # Column.split  drops the units from the object
    return []# generated_files


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", '--outdir',  dest="output_path",required=True, action="store")
    parser.add_argument("-f1", '--file_one', dest="file_one", required=True, action="store")
    parser.add_argument("-f2", '--file_two',  dest="file_two",required=True, action="store")
    options = parser.parse_args()
    create_compare_plot(options.output_path, options.file_one, options.file_two)
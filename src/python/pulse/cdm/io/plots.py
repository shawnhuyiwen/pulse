from pulse.cdm.plots import *
from pulse.cdm.bind.Plots_pb2 import *
from pulse.cdm.engine import eSerializationFormat
from google.protobuf import json_format

def serialize_plotter_list_from_file(filename: str, dst: [SEPlotter]):
    with open(filename) as f:
        string = f.read()
    serialize_plotter_list_from_string(string, dst, eSerializationFormat.JSON)
def serialize_plotter_list_from_string(string: str, dst: [SEPlotter], fmt: eSerializationFormat):
    src = PlotterListData()
    json_format.Parse(string, src)
    serialize_plotter_list_from_bind(src, dst)
def serialize_plotter_list_from_bind(src: PlotterListData, dst: [SEPlotter]):
    dst.clear()
    for any_plotter_data in src.Plotter:
        if any_plotter_data.HasField("ActionEventPlotter"):
            raise Exception("Action Event Plotter not implemented")
        elif any_plotter_data.HasField("MultiHeaderSeriesPlotter"):
            plot = SEMultiHeaderSeriesPlotter()
            serialize_multi_header_series_plotter_from_bind(any_plotter_data.MultiHeaderSeriesPlotter, plot)
            dst.append(plot)
        else:
            raise Exception("No valid plotter in serialize_plot_list_from_bind")

def serialize_image_properties_to_bind(src: SEImageProperties, dst: ImagePropertyData):
    dst.FileFormat = src.get_file_format()
    dst.Height_inch = src.get_height_inch()
    dst.Width_inch = src.get_width_inch()
    dst.DPI = src.get_dpi()
def serialize_image_properties_from_bind(src: ImagePropertyData, dst: SEImageProperties):
    if src.HasField("FileFormat"):
        dst.set_file_format(src.FileFormat)
    if src.HasField("Height_inch"):
        dst.set_height_inch(src.Height_inch)
    if src.HasField("Width_inch"):
        dst.set_width_inch(src.Width_inch)
    if src.HasField("DPI"):
        dst.set_dpi(src.DPI)

def serialize_bounds_to_bind(src: SEBounds, dst: BoundsData):
    if src.has_lower_bound():
        dst.LowerBound = src.get_lower_bound()
    if src.has_upper_bound():
        dst.UpperBound = src.get_upper_bound()
def serialize_bounds_from_bind(src: ImagePropertyData, dst: SEBounds):
    if src.HasField("LowerBound"):
        dst.set_lower_bound(src.LowerBound)
    if src.HasField("UpperBound"):
        dst.set_upper_bound(src.UpperBound)

def serialize_plot_settings_to_bind(src: SEPlotSettings, dst: PlotSettingsData):
    dst.FillArea = src.get_fill_area()
    dst.FontSize = src.get_font_size()
    dst.Gridlines = src.get_gridlines()
    dst.ImageProperties = src.get_image_properties()
    dst.LegendFontSize = src.get_legend_font_size()
    dst.LogAxis = src.get_log_axis()
    if src.has_output_path_override():
        dst.OutputPathOverride = src.get_output_path_override()
    dst.PercentOfBaselineMode = src.get_percent_of_baseline_mode().value
    dst.RemoveLegends = src.get_remove_legends()
    if src.has_sci_limits():
        limits = src.get_sci_limits()
        dst.SciLimits.M = limits[0]
        dst.SciLimits.N = limits[1]
    dst.TickStyle = src.get_tick_style().value

    # System variables
    dst.OutputFilename = src.get_output_filename()
    if src.has_title():
        dst.Title = src.get_title()
    if src.has_x_label():
        dst.X1Label = src.get_x_label()
    serialize_bounds_to_bind(src.get_x_bounds(), dst.XBounds)
    if src.has_y_label():
        dst.YLabel = src.get_y_label()
    serialize_bounds_to_bind(src.get_y1_bounds(), dst.Y1Bounds)
    if src.has_y2_label():
        dst.Y2Label = src.get_y2_label()
    serialize_bounds_to_bind(src.get_y2_bounds(), dst.Y2Bounds)
def serialize_plot_settings_from_bind(src: PlotSettingsData, dst: SEPlotSettings):
    dst.set_fill_area(src.FillArea)
    if src.HasField("FontSize"):
        dst.set_font_size(src.FontSize)
    dst.set_gridlines(src.Gridlines)
    if src.HasField("ImageProperties"):
        serialize_image_properties_from_bind(src.ImageProperties, dst.get_image_properties())
    if src.HasField("LegendFontSize"):
        dst.set_legend_font_size(src.LegendFontSize)
    dst.set_log_axis(src.LogAxis)
    if src.HasField("OutputPathOverride"):
        dst.set_output_path_override(src.OutputPathOverride)
    dst.set_percent_of_baseline_mode(ePercentageOfBaselineMode(src.PercentOfBaselineMode))
    dst.set_remove_legends(src.RemoveLegends)
    if src.HasField("SciLimits"):
        dst.set_sci_limits((src.SciLimits.M, src.SciLimits.N))
    dst.set_tick_style(eTickStyle(src.TickStyle))

def serialize_plot_source_to_bind(src: SEPlotSource, dst: PlotSourceData):
    if src.has_csv_data():
        dst.CSVData = src.get_csv_data()
    dst.LineFormat = src.get_line_format()
    if src.has_label():
        dst.Label = src.get_label()
    if src.has_begin_row():
        dst.BeginRow = src.get_begin_row()
    if src.has_end_row():
        dst.EndRow = src.get_end_row()
def serialize_plot_source_from_bind(src: PlotSourceData, dst: SEPlotSource):
    dst.set_csv_data(src.CSVData)
    if src.HasField("LineFormat"):
        dst.set_line_format(src.LineFormat)
    if src.HasField("Label"):
        dst.set_label(src.Label)
    if src.HasField("BeginRow"):
        dst.set_begin_row(src.BeginRow)
    if src.HasField("EndRow"):
        dst.set_end_row(src.EndRow)

def serialize_series_to_bind(src: SESeries, dst: MultiHeaderSeriesPlotterData.SeriesData):
    if src.has_plot_settings():
        serialize_plot_settings_to_bind(src.get_plot_settings(), dst.PlotSettings)
    if src.has_output_filename():
        dst.OutputFilename = src.get_output_filename()
    if src.has_title():
        dst.Title = src.get_title()
    if src.has_x_header():
        dst.XHeader = src.get_x_header()
    if src.has_x_label():
        dst.XLabel = src.get_x_label()
    if src.has_x_bounds():
        serialize_bounds_to_bind(src.get_x_bounds(), dst.XBounds)
    if src.has_y_header():
        dst.YHeader = src.get_y_header()
    if src.has_y_label():
        dst.YLabel = src.get_y_label()
    if src.has_y_bounds():
        serialize_bounds_to_bind(src.get_y_bounds(), dst.YBounds)
    if src.has_y2_header():
        dst.Y2Header = src.get_y2_header()
    if src.has_y2_label():
        dst.Y2Label = src.get_y2_label()
    if src.has_y2_bounds():
        serialize_bounds_to_bind(src.get_y2_bounds(), dst.Y2Bounds)
def serialize_series_from_bind(src: MultiHeaderSeriesPlotterData.SeriesData, dst: SESeries):
    if src.HasField("PlotSettings"):
        ps = SEPlotSettings()
        serialize_plot_settings_from_bind(src.PlotSettings, ps)
        dst.set_plot_settings(ps)
    if src.HasField("OutputFilename"):
        dst.set_output_filename(src.OutputFilename)
    if src.HasField("Title"):
        dst.set_title(src.Title)
    if src.HasField("XHeader"):
        dst.set_x_header(src.XHeader)
    if src.HasField("XLabel"):
        dst.set_x_label(src.XLabel)
    if src.HasField("XBounds"):
        bounds = SEBounds()
        serialize_bounds_from_bind(src.XBounds, bounds)
        dst.set_x_bounds(bounds)
    dst.set_y_header(src.YHeader)
    if src.HasField("YLabel"):
        dst.set_y_label(src.YLabel)
    if src.HasField("YBounds"):
        bounds = SEBounds()
        serialize_bounds_from_bind(src.YBounds, bounds)
        dst.set_y_bounds(bounds)
    if src.HasField("Y2Header"):
        dst.set_y2_header(src.Y2Header)
    if src.HasField("Y2Label"):
        dst.set_y2_label(src.Y2Label)
    if src.HasField("Y2Bounds"):
        bounds = SEBounds()
        serialize_bounds_from_bind(src.Y2Bounds, bounds)
        dst.set_y2_bounds(bounds)

def serialize_multi_header_series_plotter_to_bind(src: SEMultiHeaderSeriesPlotter, dst: MultiHeaderSeriesPlotterData):
    if src.has_plot_settings():
        serialize_plot_settings_to_bind(src.get_plot_settings(), dst.PlotSettings)
    for s in src.get_plot_sources():
        sourceData = dst.PlotSource.add()
        serialize_plot_source_to_bind(s, sourceData)
    for s in src.get_series():
        seriesData = dst.Series.add()
        serialize_series_to_bind(s, seriesData)
def serialize_multi_header_series_plotter_from_bind(src: MultiHeaderSeriesPlotterData, dst: SEMultiHeaderSeriesPlotter):
    serialize_plot_settings_from_bind(src.PlotSettings, dst.get_plot_settings())
    for sourceData in src.PlotSource:
        s = SEPlotSource()
        serialize_plot_source_from_bind(sourceData, s)
        dst.add_plot_source(s)
    for seriesData in src.Series:
        s = SESeries()
        serialize_series_from_bind(seriesData, s)
        dst.add_series(s)

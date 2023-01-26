from pulse.cdm.plots import *
from pulse.cdm.bind.Plots_pb2 import *
from pulse.cdm.engine import eSerializationFormat
from google.protobuf import json_format

def serialize_plot_list_from_file(filename: str, dst: [SEPlotter]):
    with open(filename) as f:
        string = f.read()
    serialize_plot_list_from_string(string, dst, eSerializationFormat.JSON)
def serialize_plot_list_from_string(string: str, dst: [SEPlotter], fmt: eSerializationFormat):
    src = PlotListData()
    json_format.Parse(string, src)
    serialize_plot_list_from_bind(src, dst)
def serialize_plot_list_from_bind(src: PlotListData, dst: [SEPlotter]):
    dst.clear()
    for any_plot_data in src.Plot:
        if any_plot_data.HasField("ActionEventPlot"):
            print(any_plot_data.ActionEventPlot)
            raise Exception("Action Event Plot not implemented")
        elif any_plot_data.HasField("ConvexHullPlot"):
            raise Exception("Convex Hull Plot not implemented")
        elif any_plot_data.HasField("MultiPlot"):
            raise Exception("Multi Plot not implemented")
        elif any_plot_data.HasField("MultiSourcePlot"):
            raise Exception("Multi Source Plot not implemented")
        elif any_plot_data.HasField("RelationalPlot"):
            plot = SERelationalPlotter()
            serialize_reational_plotter_from_bind(any_plot_data.RelationalPlot, plot)
            dst.append(plot)
        elif any_plot_data.HasField("TimeSeriesPlot"):
            plot = SETimeSeriesPlotter()
            serialize_time_series_plotter_from_bind(any_plot_data.TimeSeriesPlot, plot)
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
    dst.FontSize = src.get_font_size()
    dst.Gridlines = src.get_gridlines()
    dst.ImageProperties = src.get_image_properties()
    dst.LegendFontSize = src.get_legend_font_size()
    dst.LogAxis = src.get_log_axis()
    dst.OutputFilename = src.get_output_filename()
    if src.has_output_path_override():
        dst.OutputPathOverride = src.get_output_path_override()
    dst.PercentOfBaselineMode = src.get_percent_of_baseline_mode().value
    dst.RemoveLegends = src.get_remove_legends()
    if src.has_sci_limits():
        limits = src.get_sci_limits()
        dst.SciLimits.M = limits[0]
        dst.SciLimits.N = limits[1]
    dst.TickStyle = src.get_tick_style().value
    if src.has_title():
        dst.Title = src.get_title()
    if src.has_x1_label():
        dst.X1Label = src.get_x1_label()
    serialize_bounds_to_bind(src.get_x1_bounds(), dst.X1Bounds)
    if src.has_y1_label():
        dst.Y1Label = src.get_Y1_label()
    serialize_bounds_to_bind(src.get_y1_bounds(), dst.Y1Bounds)
def serialize_plot_settings_from_bind(src: PlotSettingsData, dst: SEPlotSettings):
    if src.HasField("FontSize"):
        dst.set_font_size(src.FontSize)
    dst.set_gridlines(src.Gridlines)
    if src.HasField("ImageProperties"):
        serialize_image_properties_from_bind(src.ImageProperties, dst.get_image_properties())
    if src.HasField("LegendFontSize"):
        dst.set_legend_font_size(src.LegendFontSize)
    dst.set_log_axis(src.LogAxis)
    dst.set_output_filename(src.OutputFilename)
    if src.HasField("OutputPathOverride"):
        dst.set_output_path_override(src.OutputPathOverride)
    dst.set_percent_of_baseline_mode(ePercentageOfBaselineMode(src.PercentOfBaselineMode))
    dst.set_remove_legends(src.RemoveLegends)
    if src.HasField("SciLimits"):
        dst.set_sci_limits((src.SciLimits.M, src.SciLimits.N))
    dst.set_tick_style(eTickStyle(src.TickStyle))
    if src.HasField("Title"):
        dst.set_title(src.Title)
    if src.HasField("X1Label"):
        dst.set_x1_label(src.X1Label)
    serialize_bounds_from_bind(src.X1Bounds, dst.get_x1_bounds())
    if src.HasField("Y1Label"):
        dst.set_y1_label(src.Y1Label)
    serialize_bounds_from_bind(src.Y1Bounds, dst.get_y1_bounds())

def serialize_plot_source_to_bind(src: SEPlotSource, dst: PlotSourceData):
    if src.has_csv_data():
        dst.CSVData = src.get_csv_data()
    dst.FillArea = src.get_fill_area()
    dst.LineFormat = src.get_line_format()
    if src.has_label():
        dst.Label = src.get_label()
    if src.has_begin_row():
        dst.BeginRow = src.get_begin_row()
    if src.has_end_row():
        dst.EndRow = src.get_end_row()
def serialize_plot_source_from_bind(src: PlotSourceData, dst: SEPlotSource):
    dst.set_csv_data(src.CSVData)
    dst.set_fill_area(src.FillArea)
    if src.HasField("LineFormat"):
        dst.set_line_format(src.LineFormat)
    if src.HasField("Label"):
        dst.set_label(src.Label)
    if src.HasField("BeginRow"):
        dst.set_begin_row(src.BeginRow)
    if src.HasField("EndRow"):
        dst.set_end_row(src.EndRow)

def serialize_time_series_plotter_to_bind(src: SETimeSeriesPlotter, dst: TimeSeriesPlotterData):
    if src.has_plot_settings():
        serialize_plot_settings_to_bind(src.get_plot_settings(), dst.PlotSettings)
    if src.has_plot_source():
        serialize_plot_source_to_bind(src.get_plot_source(), dst.PlotSource)
    for s in src.get_series():
        seriesData = dst.Series.add()
        if s.has_header():
            seriesData.Header = s.get_header()
        if s.has_output_filename():
            seriesData.OutputFilename = s.get_output_filename()
        if s.has_title():
            seriesData.Title = s.get_title()
        if s.has_label():
            seriesData.Label = s.get_label()
        if s.has_bounds():
            serialize_bounds_from_bind(src.Bounds, dst.get_bounds())
def serialize_time_series_plotter_from_bind(src: TimeSeriesPlotterData, dst: SETimeSeriesPlotter):
    serialize_plot_settings_from_bind(src.PlotSettings, dst.get_plot_settings())
    serialize_plot_source_from_bind(src.PlotSource, dst.get_plot_source())
    for seriesData in src.Series:
        s = SESeries()
        s.set_header(seriesData.Header)
        if seriesData.HasField("OutputFilename"):
            s.set_output_filename(seriesData.OutputFilename)
        if seriesData.HasField("Title"):
            s.set_title(seriesData.Title)
        if seriesData.HasField("Label"):
            s.set_label(seriesData.Label)
        if seriesData.HasField("Bounds"):
            s.set_bounds(seriesData.Bounds)
        dst.add_series(s)

def serialize_relational_plotter_to_bind(src: SERelationalPlotter, dst: RelationalPlotterData):
    if src.has_plot_settings():
        serialize_plot_settings_to_bind(src.get_plot_settings(), dst.PlotSettings)
    if src.has_plot_source():
        serialize_plot_source_to_bind(src.get_plot_source(), dst.PlotSource)
    for r in src.get_relationships():
        relationData = dst.Relationships.add()
        if r.has_x_header():
            relationData.XHeader = r.get_x_header()
        if r.has_y_header():
            relationData.YHeader = r.get_y_header()
        if r.has_output_filename():
            relationData.OutputFilename = r.get_output_filename()
        if r.has_title():
            relationData.Title = r.get_title()
        if r.has_x_label():
            relationData.XLabel = r.get_x_label()
        if r.has_y_label():
            relationData.YLabel = r.get_y_label()
        if r.has_x_bounds():
            serialize_bounds_from_bind(src.XBounds, dst.get_x_bounds())
        if r.has_y_bounds():
            serialize_bounds_from_bind(src.YBounds, dst.get_y_bounds())
def serialize_reational_plotter_from_bind(src: RelationalPlotterData, dst: SERelationalPlotter):
    serialize_plot_settings_from_bind(src.PlotSettings, dst.get_plot_settings())
    serialize_plot_source_from_bind(src.PlotSource, dst.get_plot_source())
    for relationData in src.Relationships:
        r = SERelation()
        r.set_x_header(relationData.XHeader)
        r.set_y_header(relationData.YHeader)
        if relationData.HasField("OutputFilename"):
            r.set_output_filename(relationData.OutputFilename)
        if relationData.HasField("Title"):
            r.set_title(relationData.Title)
        if relationData.HasField("XLabel"):
            r.set_x_label(relationData.XLabel)
        if relationData.HasField("YLabel"):
            r.set_y_label(relationData.LabelY)
        if relationData.HasField("XBounds"):
            r.set_x_bounds(relationData.XBounds)
        if relationData.HasField("YBounds"):
            r.set_y_bounds(relationData.YBounds)
        dst.add_relation(r)

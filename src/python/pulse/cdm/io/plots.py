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
    base_config = SEPlotConfig()
    if src.HasField("PlotConfig"):
        serialize_plot_config_from_bind(src.PlotConfig, base_config)
    else:
        base_config.set_defaults()

    for any_plotter_data in src.Plotter:
        if any_plotter_data.HasField("MultiHeaderSeriesPlotter"):
            plot = SEMultiHeaderSeriesPlotter()
            serialize_multi_header_series_plotter_from_bind(any_plotter_data.MultiHeaderSeriesPlotter,
                                                            plot,
                                                            base_config)
            dst.append(plot)
        else:
            raise Exception("No valid plotter in serialize_plot_list_from_bind")

def serialize_image_properties_to_bind(src: SEImageProperties, dst: ImagePropertyData):
    dst.DimensionMode = src.get_dimension_mode().value
    dst.FileFormat = src.get_file_format()
    dst.Height_inch = src.get_height_inch()
    dst.Width_inch = src.get_width_inch()
    dst.DPI = src.get_dpi()
def serialize_image_properties_from_bind(src: ImagePropertyData, dst: SEImageProperties):
    if src.HasField("DimensionMode"):
        dst.set_dimension_mode(eDimensionMode(src.DimensionMode))
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

def serialize_plot_config_to_bind(src: SEPlotConfig, dst: PlotConfigData):
    if src.has_fill_area_setting():
        dst.FillArea = src.get_fill_area()
    if src.has_font_size():
        dst.FontSize = src.get_font_size()
    if src.has_gridlines_setting():
        dst.Gridlines = src.get_gridlines()
    if src.has_image_properties():
        dst.ImageProperties = src.get_image_properties()
    if src.has_legend_font_size():
        dst.LegendFontSize = src.get_legend_font_size()
    if src.has_legend_mode():
        dst.LegendMode = src.get_legend_mode().value
    if src.has_log_axis_setting():
        dst.LogAxis = src.get_log_axis()
    if src.has_omit_actions_with():
        for o in src.get_omit_actions_with()():
            omitData = dst.OmitActionsWith.add()
            omitData = o
    if src.has_omit_events_with():
        for o in src.get_omit_events_with()():
            omitData = dst.OmitEventsWith.add()
            omitData = o
    if src.has_output_path_override():
        dst.OutputPathOverride = src.get_output_path_override()
    if src.has_percent_of_baseline_mode():
        dst.PercentOfBaselineMode = src.get_percent_of_baseline_mode().value
    if src.has_plot_actions_setting():
        dst.PlotActions = src.get_plot_actions()
    if src.has_plot_events_setting():
        dst.PlotEvents = src.get_plot_events()
    if src.has_sci_limits():
        limits = src.get_sci_limits()
        dst.SciLimits.M = limits[0]
        dst.SciLimits.N = limits[1]
    if src.has_tick_style():
        dst.TickStyle = src.get_tick_style().value
    if src.has_zero_axis_setting():
        dst.ZeroAxis = src.get_zero_axis()
def serialize_plot_config_from_bind(src: PlotConfigData,
                                    dst: SEPlotConfig,
                                    base_config: SEPlotConfig = None):
    if base_config:
        dst.merge_configs(base_config)
    else:
        dst.set_defaults()

    if src.HasField("FillArea"):
        dst.set_fill_area(src.FillArea)
    if src.HasField("FontSize"):
        dst.set_font_size(src.FontSize)
    if src.HasField("Gridlines"):
        dst.set_gridlines(src.Gridlines)
    if src.HasField("ImageProperties"):
        serialize_image_properties_from_bind(src.ImageProperties, dst.get_image_properties())
    if src.HasField("LegendFontSize"):
        dst.set_legend_font_size(src.LegendFontSize)
    if src.HasField("LegendMode"):
        dst.set_legend_mode(eLegendMode(src.LegendMode))
    if src.HasField("LogAxis"):
        dst.set_log_axis(src.LogAxis)
    for omitData in src.OmitActionsWith:
        dst.add_omit_actions_with(omitData)
    for omitData in src.OmitEventsWith:
        dst.add_omit_events_with(omitData)
    if src.HasField("OutputPathOverride"):
        dst.set_output_path_override(src.OutputPathOverride)
    if src.HasField("PercentOfBaselineMode"):
        dst.set_percent_of_baseline_mode(ePercentageOfBaselineMode(src.PercentOfBaselineMode))
    if src.HasField("PlotActions"):
        dst.set_plot_actions(src.PlotActions)
    if src.HasField("PlotEvents"):
        dst.set_plot_events(src.PlotEvents)
    if src.HasField("SciLimits"):
        dst.set_sci_limits((src.SciLimits.M, src.SciLimits.N))
    if src.HasField("TickStyle"):
        dst.set_tick_style(eTickStyle(src.TickStyle))
    if src.HasField("ZeroAxis"):
        dst.set_zero_axis(src.ZeroAxis)

def serialize_plot_source_to_bind(src: SEPlotSource, dst: PlotSourceData):
    if src.has_csv_data():
        dst.CSVData = src.get_csv_data()
    if src.has_line_format():
        dst.LineFormat = src.get_line_format()
    if src.has_label():
        dst.Label = src.get_label()
    if src.has_start_row():
        dst.StartRow = src.get_start_row()
    if src.has_end_row():
        dst.EndRow = src.get_end_row()
    if src.has_log_file():
        dst.LogFile = src.get_log_file()
def serialize_plot_source_from_bind(src: PlotSourceData, dst: SEPlotSource):
    dst.set_csv_data(src.CSVData)
    if src.HasField("LineFormat"):
        dst.set_line_format(src.LineFormat)
    if src.HasField("Label"):
        dst.set_label(src.Label)
    if src.HasField("StartRow"):
        dst.set_start_row(src.StartRow)
    if src.HasField("EndRow"):
        dst.set_end_row(src.EndRow)
    if src.HasField("LogFile"):
        dst.set_log_file(src.LogFile)

def serialize_series_to_bind(src: SESeries, dst: MultiHeaderSeriesPlotterData.SeriesData):
    if src.has_plot_config():
        serialize_plot_config_to_bind(src.get_plot_config(), dst.PlotConfig)
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
    if src.has_x2_header():
        dst.X2Header = src.get_x2_header()
    for y in src.get_y_headers():
        headerData = dst.YHeader.add()
        headerData = y
    if src.has_y_label():
        dst.YLabel = src.get_y_label()
    if src.has_y_bounds():
        serialize_bounds_to_bind(src.get_y_bounds(), dst.YBounds)
    for y2 in src.get_y2_headers():
        headerData = dst.Y2Header.add()
        headerData = y2
    if src.has_y2_label():
        dst.Y2Label = src.get_y2_label()
    if src.has_y2_bounds():
        serialize_bounds_to_bind(src.get_y2_bounds(), dst.Y2Bounds)
def serialize_series_from_bind(src: MultiHeaderSeriesPlotterData.SeriesData,
                               dst: SESeries,
                               base_config: SEPlotConfig):
    if src.HasField("PlotConfig"):
        serialize_plot_config_from_bind(src.PlotConfig, dst.get_plot_config(), base_config)
    else:
        dst.set_plot_config(base_config)
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
    if src.HasField("X2Header"):
        dst.set_x2_header(src.X2Header)
    for headerData in src.YHeader:
        dst.add_y_header(headerData)
    if src.HasField("YLabel"):
        dst.set_y_label(src.YLabel)
    if src.HasField("YBounds"):
        bounds = SEBounds()
        serialize_bounds_from_bind(src.YBounds, bounds)
        dst.set_y_bounds(bounds)
    for headerData in src.Y2Header:
        dst.add_y2_header(headerData)
    if src.HasField("Y2Label"):
        dst.set_y2_label(src.Y2Label)
    if src.HasField("Y2Bounds"):
        bounds = SEBounds()
        serialize_bounds_from_bind(src.Y2Bounds, bounds)
        dst.set_y2_bounds(bounds)

def serialize_multi_header_series_plotter_to_bind(src: SEMultiHeaderSeriesPlotter, dst: MultiHeaderSeriesPlotterData):
    if src.has_plot_config():
        serialize_plot_config_to_bind(src.get_plot_config(), dst.PlotConfig)
    for s in src.get_plot_sources():
        sourceData = dst.PlotSource.add()
        serialize_plot_source_to_bind(s, sourceData)
    for s in src.get_series():
        seriesData = dst.Series.add()
        serialize_series_to_bind(s, seriesData)
    if src.has_valiation_source():
        serialize_plot_source_to_bind(src.get_validation_source(), dst.ValidationSource)
def serialize_multi_header_series_plotter_from_bind(src: MultiHeaderSeriesPlotterData,
                                                    dst: SEMultiHeaderSeriesPlotter,
                                                    base_config: SEPlotConfig):
    if src.HasField("PlotConfig"):
        serialize_plot_config_from_bind(src.PlotConfig, dst.get_plot_config(), base_config)
    else:
        dst.set_plot_config(base_config)

    for sourceData in src.PlotSource:
        s = SEPlotSource()
        serialize_plot_source_from_bind(sourceData, s)
        dst.add_plot_source(s)
    for seriesData in src.Series:
        s = SESeries()
        serialize_series_from_bind(seriesData, s, dst.get_plot_config())
        dst.add_series(s)
    if src.HasField("ValidationSource"):
        val_source = SEPlotSource()
        serialize_plot_source_from_bind(src.ValidationSource, val_source)
        dst.set_validation_source(val_source)

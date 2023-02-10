# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from enum import Enum
from typing import Tuple
from copy import deepcopy

from pulse.cdm.scalars import SEScalar, SEScalarLength
from pulse.cdm.utils.file_utils import get_dir_from_run_config
from pulse.cdm.utils.csv_utils import read_csv_into_df

class SEImageProperties():
    __slots__ = ["_file_format", "_height_inch", "_width_inch", "_dpi"]

    def __init__(self):
        self._file_format = ".jpg"
        self._height_inch = 8
        self._width_inch = 16
        self._dpi = 100

    def get_file_format(self):
        return self._file_format
    def set_file_format(self, fmt: str):
        self._file_format = fmt

    def get_height_inch(self):
        return self._height_inch
    def set_height_inch(self, h: float):
        self._height_inch = h

    def get_width_inch(self):
        return self._width_inch
    def set_width_inch(self, w: float):
        self._width_inch = w

    def get_dpi(self):
        return self._dpi
    def set_dpi(self, dpi: float):
        self._dpi = dpi


class SEBounds():
    __slots__ = ["_lower_bound", "_upper_bound"]

    def __init__(self):
        self._lower_bound = None
        self._upper_bound = None

    def get_lower_bound(self):
        return self._lower_bound
    def set_lower_bound(self, bound: float):
        self._lower_bound = bound
    def has_lower_bound(self):
        return self._lower_bound is not None
    def invalidate_lower_bound(self):
        self._lower_bound = None

    def get_upper_bound(self):
        return self._upper_bound
    def set_upper_bound(self, bound: float):
        self._upper_bound = bound
    def has_upper_bound(self):
        return self._upper_bound is not None
    def invalidate_upper_bound(self):
        self._upper_bound = None


class ePercentageOfBaselineMode(Enum):
    Off = 0
    All = 1
    OnlyX = 2
    OnlyY = 3
class eTickStyle(Enum):
    Scientific = 0
    Plain = 1
class SEPlotConfig():
    __slots__ = [ "_fill_area", "_font_size", "_gridlines",
                  "_image_properties", "_legend_font_size", "_log_axis",
                  "_output_filename", "_output_path_override",
                  "_percent_of_baseline_mode", "_remove_legends", "_sci_limits",
                  "_tick_style", "_title", "_x_label", "_x_bounds", "_y_label",
                  "_y_bounds", "_y2_label", "_y2_bounds"]

    def __init__(self):
        self.clear()

    def clear(self):
        self._fill_area = None
        self._font_size = None
        self._gridlines = None
        self._image_properties = None
        self._legend_font_size = None
        self._log_axis = None
        self._output_path_override = None
        self._percent_of_baseline_mode = None
        self._remove_legends = None
        self._sci_limits = None
        self._tick_style = None

        # Internal
        self._output_filename = None
        self._title = None
        self._x_label = None
        self._x_bounds = None
        self._y_label = None
        self._y_bounds = None
        self._y2_label = None
        self._y2_bounds = None

    def set_defaults(self):
        if self._fill_area is None:
            self._fill_area = False
        if self._font_size is None:
            self._font_size = 14
        if self._gridlines is None:
            self._gridlines = False
        if self._image_properties is None:
            self._image_properties = SEImageProperties()
        if self._legend_font_size is None:
            self._legend_font_size = 12
        if self._log_axis is None:
            self._log_axis = False
        if self._output_path_override is None:
            self._output_path_override = "./verification/Plots/"
        if self._percent_of_baseline_mode is None:
            self._percent_of_baseline_mode = ePercentageOfBaselineMode.Off
        if self._remove_legends is None:
            self._remove_legends = False
        if self._tick_style is None:
            self._tick_style = eTickStyle.Scientific

        # Internal
        if self._x_bounds is None:
            self._x_bounds = SEBounds()
        if self._y_bounds is None:
            self._y_bounds = SEBounds()
        if self._y2_bounds is None:
            self._y2_bounds = SEBounds()

    def merge_configs(self, src):
        src = deepcopy(src)
        if src._fill_area is not None:
            self._fill_area = src._fill_area
        if src._font_size is not None:
            self._font_size = src._font_size
        if src._gridlines is not None:
            self._gridlines = src._gridlines
        if src._image_properties is not None:
            self._image_properties = src._image_properties
        if src._legend_font_size is not None:
            self._legend_font_size = src._legend_font_size
        if src._log_axis is not None:
            self._log_axis = src._log_axis
        if src._output_path_override is not None:
            self._output_path_override = src._output_path_override
        if src._percent_of_baseline_mode is not None:
            self._percent_of_baseline_mode = src._percent_of_baseline_mode
        if src._remove_legends is not None:
            self._remove_legends = src._remove_legends
        if src._sci_limits is not None:
            self._sci_limits = src._sci_limits
        if src._tick_style is not None:
            self._tick_style = src._tick_style

        # Internal
        if src._output_filename is not None:
            self._output_filename = src._output_filename
        if src._title is not None:
            self._title = src._title
        if src._x_label is not None:
            self._x_label = src._x_label
        if src._x_bounds is not None:
            self._x_bounds = src._x_bounds
        if src._y_label is not None:
            self._y_label = src._y_label
        if src._y_bounds is not None:
            self._y_bounds = src._y_bounds
        if src._y2_label is not None:
            self._y2_label = src._y2_label
        if src._y2_bounds is not None:
            self._y2_bounds = src._y2_bounds

    def get_fill_area(self):
        return self._fill_area
    def set_fill_area(self, fill: bool):
        self._fill_area = fill
    def has_fill_area_setting(self):
        return self._fill_area is not None
    def invalidate_fill_area_setting(self):
        self._fill_area = None

    def get_font_size(self):
        return self._font_size
    def set_font_size(self, font_size: float):
        self._font_size = font_size
    def has_font_size(self):
        return self._font_size is not None
    def invalidate_font_size(self):
        self._font_size = None

    def get_gridlines(self):
        return self._gridlines
    def set_gridlines(self, gridlines: bool):
        self._gridlines = gridlines
    def has_gridlines_setting(self):
        return self._gridlines is not None
    def invalidate_gridlines_setting(self):
        self._gridlines = None

    def get_image_properties(self):
        return self._image_properties
    def set_image_properties(self, image_props: SEImageProperties):
        self._image_properties = deepcopy(image_props)
    def has_image_properties(self):
        return self._image_properties is not None
    def invalidate_image_properties(self):
        self._image_properties = None

    def get_legend_font_size(self):
        return self._legend_font_size
    def set_legend_font_size(self, font_size: float):
        self._legend_font_size = font_size
    def has_legend_font_size(self):
        return self._legend_font_size is not None
    def invalidate_legend_font_size(self):
        self._legend_font_size = None

    def get_log_axis(self):
        return self._log_axis
    def set_log_axis(self, log_axis: bool):
        self._log_axis = log_axis
    def has_log_axis_setting(self):
        return self._log_axis is not None
    def invalidate_log_axis_setting(self):
        self._log_axis = None

    def get_output_filename(self):
        return self._output_filename
    def set_output_filename(self, output_filename: str):
        self._output_filename = output_filename
    def has_output_filename(self):
        return self._output_filename is not None and len(self._output_filename) > 0
    def invalidate_output_filename(self):
        self._output_filename = None

    def get_output_path_override(self):
        return self._output_path_override
    def set_output_path_override(self, output_path: str):
        self._output_path_override = output_path
    def has_output_path_override(self):
        return self._output_path_override is not None
    def invalidate_output_path_override(self):
        self._output_path_override = None

    def get_percent_of_baseline_mode(self):
        return self._percent_of_baseline_mode
    def set_percent_of_baseline_mode(self, mode: ePercentageOfBaselineMode):
        self._percent_of_baseline_mode = mode
    def has_percent_of_baseline_mode(self):
        return self._percent_of_baseline_mode is not None
    def invalidate_percent_of_baseline_mode(self):
        self._percent_of_baseline_mode = None

    def get_remove_legends(self):
        return self._remove_legends
    def set_remove_legends(self, remove_legends: bool):
        self._remove_legends = remove_legends
    def has_remove_legends_setting(self):
        return self._remove_legends is not None
    def invalidate_remove_legends_setting(self):
        self._remove_legends = None

    def get_sci_limits(self):
        return self._sci_limits
    def set_sci_limits(self, limits: Tuple[int, int]):
        self._sci_limits = limits
    def has_sci_limits(self):
        return self._sci_limits is not None
    def invalidate_sci_limits(self):
        self._sci_limis = None

    def get_tick_style(self):
        return self._tick_style
    def set_tick_style(self, style: eTickStyle):
        self._tick_style = style
    def has_tick_style(self):
        return self._tick_style is not None
    def invalidate_tick_style(self):
        self._tick_style = None

    def get_title(self):
        return self._title
    def set_title(self, title: str):
        self._title = title
    def has_title(self):
        return self._title is not None
    def invalidate_title(self):
        self._title = None

    def get_x_label(self):
        return self._x_label
    def set_x_label(self, label: str):
        self._x_label = label
    def has_x_label(self):
        return self._x_label is not None
    def invalidate_x_label(self):
        self._x_label = None

    def get_x_bounds(self):
        return self._x_bounds
    def set_x_bounds(self, bounds: SEBounds):
        self._x_bounds = deepcopy(bounds)
    def has_x_bounds(self):
        return self._x_bounds is not None
    def invalidate_x_bounds(self):
        self._x_bounds = None

    def get_y_label(self):
        return self._y_label
    def set_y_label(self, label: str):
        self._y_label = label
    def has_y_label(self):
        return self._y_label is not None
    def invalidate_y_label(self):
        self._y_label = None

    def get_y_bounds(self):
        return self._y_bounds
    def set_y_bounds(self, bounds: SEBounds):
        self._y_bounds = deepcopy(bounds)
    def has_y_bounds(self):
        return self._y_bounds is not None
    def invalidate_y_bounds(self):
        self._y_bounds = None

    def get_y2_label(self):
        return self._y2_label
    def set_y2_label(self, label: str):
        self._y2_label = label
    def has_y2_label(self):
        return self._y2_label is not None
    def invalidate_y2_label(self):
        self._y2_label = None

    def get_y2_bounds(self):
        return self._y2_bounds
    def set_y2_bounds(self, bounds: SEBounds):
        self._y2_bounds = deepcopy(bounds)
    def has_y2_bounds(self):
        return self._y2_bounds is not None
    def invalidate_y2_bounds(self):
        self._y2_bounds = None


class SEPlotSource():
    __slots__ = ["_csv_data", "_df", "_label", "_line_format",
                 "_start_row", "_end_row"]

    def __init__(self):
        self._csv_data = None
        self._df = None
        self._label = None
        self._line_format = ""
        self._start_row = None
        self._end_row = None

    def get_csv_data(self):
        return self._csv_data
    def set_csv_data(self, csv_data: str):
        self._csv_data = csv_data
    def has_csv_data(self):
        return self._csv_data is not None
    def invalidate_csv_data(self):
        self._csv_data = None
        self._df = None

    def _read_csv_into_df(self):
        if not self.has_csv_data():
            raise Exception("No CSV data provided")

        # Perform replacement if needed
        keys = ['$ROOT_DIR', '$DATA_DIR', '$SCENARIO_DIR','$VALIDATION_DIR', '$VERIFICATION_DIR']
        for key in keys:
            if key in self._csv_data:
                self._csv_data = self._csv_data.replace(key, get_dir_from_run_config(key[1:]))

        self._df = read_csv_into_df(self._csv_data, replace_slashes=False)
    def get_data_frame(self):
        if self._df is None:
            self._read_csv_into_df()
        return self._df.loc[self._start_row:self._end_row]
    def invalidate_data_frame(self):
        self._df = None

    def get_label(self):
        return self._label
    def set_label(self, label: str):
        self._label = label
    def has_label(self):
        return self._label is not None
    def invalidate_label(self):
        self._label = None

    def get_line_format(self):
        return self._line_format
    def set_line_format(self, fmt: str):
        self._line_format = fmt
    def has_line_format(self):
        return len(self._line_format) > 0
    def invalidate_line_format(self):
        self._line_format = ""

    def get_start_row(self):
        return self._start_row
    def set_start_row(self, row: int):
        self._start_row = row
    def has_start_row(self):
        return self._start_row is not None

    def get_end_row(self):
        return self._end_row
    def set_end_row(self, row: int):
        self._end_row = row
    def has_end_row(self):
        return self._end_row is not None

class SESeries():
    __slots__ = ["_plot_config", "_output_filename", "_title",
                 "_x_header", "_x_label", "_x_bounds", "_x2_header",
                 "_y_headers", "_y_label", "_y_bounds", "_y2_headers",
                 "_y2_label", "_y2_bounds"]

    def __init__(self):
        self._plot_config = None
        self._output_filename = ""
        self._title = None
        self._x_header = None
        self._x_label = None
        self._x_bounds = SEBounds()
        self._x2_header = None
        self._y_headers = []
        self._y_label = None
        self._y_bounds = SEBounds()
        self._y2_headers = []
        self._y2_label = None
        self._y2_bounds = SEBounds()

    def get_plot_config(self):
        if self._plot_config is None:
            self._plot_config = SEPlotConfig()
        return self._plot_config
    def set_plot_config(self, config: SEPlotConfig):
        self._plot_config = deepcopy(config)
    def has_plot_config(self):
        return self._plot_config is not None

    def get_output_filename(self):
        return self._output_filename
    def set_output_filename(self, output_filename: str):
        self._output_filename = output_filename
    def has_output_filename(self):
        return self._output_filename is not None and len(self._output_filename) > 0
    def invalidate_output_filename(self):
        self._output_filename = ""

    def get_title(self):
        return self._title
    def set_title(self, title: str):
        self._title = title
    def has_title(self):
        return self._title is not None
    def invalidate_title(self):
        self._title = None

    def get_x_header(self):
        return self._x_header
    def set_x_header(self, header: str):
        self._x_header = header
    def has_x_header(self):
        return self._x_header is not None
    def invalidate_x_header(self):
        self._x_header = None

    def get_x_label(self):
        return self._x_label
    def set_x_label(self, label: str):
        self._x_label = label
    def has_x_label(self):
        return self._x_label is not None
    def invalidate_x_label(self):
        self._x_label = None

    def get_x_bounds(self):
        return self._x_bounds
    def set_x_bounds(self, bounds: SEBounds):
        self._x_bounds = deepcopy(bounds)

    def get_x2_header(self):
        return self._x2_header
    def set_x2_header(self, header: str):
        self._x2_header = header
    def has_x2_header(self):
        return self._x2_header is not None
    def invalidate_x2_header(self):
        self._x2_header = None

    def get_y_headers(self):
        return self._y_headers
    def add_y_header(self, header: str):
        self._y_headers.append(header)
    def has_y_headers(self):
        return len(self._y_headers) > 0
    def invalidate_y_headers(self):
        self._y_headers = []

    def get_y_label(self):
        return self._y_label
    def set_y_label(self, label: str):
        self._y_label = label
    def has_y_label(self):
        return self._y_label is not None
    def invalidate_y_label(self):
        self._y_label = None

    def get_y_bounds(self):
        return self._y_bounds
    def set_y_bounds(self, bounds: SEBounds):
        self._y_bounds = deepcopy(bounds)

    def get_y2_headers(self):
        return self._y2_headers
    def add_y2_header(self, header: str):
        self._y2_headers.append(header)
    def has_y2_headers(self):
        return len(self._y2_headers) > 0
    def invalidate_y2_headers(self):
        self._y2_headers = []

    def get_y2_label(self):
        return self._y2_label
    def set_y2_label(self, label: str):
        self._y2_label = label
    def has_y2_label(self):
        return self._y2_label is not None
    def invalidate_y2_label(self):
        self._y2_label = None

    def get_y2_bounds(self):
        return self._y2_bounds
    def set_y2_bounds(self, bounds: SEBounds):
        self._y2_bounds = deepcopy(bounds)

class SEPlotter():
    __slots__ = ["_plot_config"]

    def __init__(self):
        self._plot_config = None

    def get_plot_config(self):
        if self._plot_config is None:
            self._plot_config = SEPlotConfig()
        return self._plot_config
    def set_plot_config(self, config: SEPlotConfig):
        self._plot_config = deepcopy(config)
    def has_plot_config(self):
        return self._plot_config is not None

class SEMultiHeaderSeriesPlotter(SEPlotter):
    __slots__ = ["_plot_sources", "_series", "_validation_source"]

    def __init__(self):
        super().__init__()

        self._plot_sources = []
        self._series = []
        self._validation_source = None

    def get_plot_sources(self):
        return self._plot_sources
    def add_plot_source(self, plot_source: SEPlotSource):
        self._plot_sources.append(deepcopy(plot_source))
    def has_plot_sources(self):
        return len(self._plot_sources) > 0
    def invalidate_plot_sources(self):
        self._plot_sources = []

    def get_series(self):
        return self._series
    def add_series(self, series: SESeries):
        self._series.append(series)
    def has_series(self):
        return len(self._series) > 0
    def invalidate_series(self):
        self._series = []

    def get_validation_source(self):
        return self._validation_source
    def set_validation_source(self, validation_source: SEPlotSource):
        self._validation_source = deepcopy(validation_source)
    def has_valiation_source(self):
        return self._validation_source is not None
    def invalidate_validation_source(self):
        self._validation_source = None

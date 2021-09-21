# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.curve import SESegmentConstant, SESegmentLinear, SESegmentParabolic, SESegmentSigmoidal, SECurve
from pulse.cdm.bind.Properties_pb2 import AnySegmentData, CurveData, \
                                          SegmentConstantData, SegmentLinearData, \
                                          SegmentParabolicData, SegmentSigmoidalData
from pulse.cdm.io.scalars import serialize_scalar_to_bind, \
                                 serialize_scalar_from_bind, \
                                 serialize_scalar_pressure_to_bind, \
                                 serialize_scalar_pressure_from_bind, \
                                 serialize_scalar_pressure_per_volume_to_bind, \
                                 serialize_scalar_pressure_per_volume_from_bind, \
                                 serialize_scalar_volume_to_bind, \
                                 serialize_scalar_volume_from_bind, \
                                 serialize_scalar_volume_per_pressure_to_bind, \
                                 serialize_scalar_volume_per_pressure_from_bind

def serialize_segment_constant_to_bind(src: SESegmentConstant, dst: SegmentConstantData):
    serialize_scalar_volume_to_bind(src.get_begin_volume(), dst.BeginVolume)
    serialize_scalar_volume_to_bind(src.get_end_volume(), dst.EndVolume)
    serialize_scalar_volume_per_pressure_to_bind(src.get_compliance(), dst.Compliance)

def serialize_segment_constant_from_bind(src: SegmentConstantData, dst: SESegmentConstant):
    serialize_scalar_volume_from_bind(src.BeginVolume, dst.get_begin_volume())
    serialize_scalar_volume_from_bind(src.EndVolume, dst.get_end_volume())
    serialize_scalar_volume_per_pressure_from_bind(src.Compliance, dst.get_compliance())

def serialize_segment_linear_to_bind(src: SESegmentLinear, dst: SegmentLinearData):
    serialize_scalar_volume_to_bind(src.get_begin_volume(), dst.BeginVolume)
    serialize_scalar_volume_to_bind(src.get_end_volume(), dst.EndVolume)
    serialize_scalar_pressure_per_volume_to_bind(src.get_slope(), dst.Slope)
    serialize_scalar_pressure_to_bind(src.get_y_intercept(), dst.YIntercept)

def serialize_segment_linear_from_bind(src: SegmentLinearData, dst: SESegmentLinear):
    serialize_scalar_volume_from_bind(src.BeginVolume, dst.get_begin_volume())
    serialize_scalar_volume_from_bind(src.EndVolume, dst.get_end_volume())
    serialize_scalar_pressure_per_volume_from_bind(src.Slope, dst.get_slope())
    serialize_scalar_pressure_from_bind(src.YIntercept, dst.get_y_intercept())

def serialize_segment_parabolic_to_bind(src: SESegmentParabolic, dst: SegmentParabolicData):
    serialize_scalar_volume_to_bind(src.get_begin_volume(), dst.BeginVolume)
    serialize_scalar_volume_to_bind(src.get_end_volume(), dst.EndVolume)
    serialize_scalar_to_bind(src.get_coefficient1(), dst.Coefficient1)
    serialize_scalar_to_bind(src.get_coefficient2(), dst.Coefficient2)
    serialize_scalar_to_bind(src.get_coefficient3(), dst.Coefficient3)
    serialize_scalar_to_bind(src.get_coefficient4(), dst.Coefficient4)

def serialize_segment_parabolic_from_bind(src: SegmentParabolicData, dst: SESegmentParabolic):
    serialize_scalar_volume_from_bind(src.BeginVolume, dst.get_begin_volume())
    serialize_scalar_volume_from_bind(src.EndVolume, dst.get_end_volume())
    serialize_scalar_from_bind(src.Coefficient1, dst.get_coefficient1())
    serialize_scalar_from_bind(src.Coefficient2, dst.get_coefficient2())
    serialize_scalar_from_bind(src.Coefficient3, dst.get_coefficient3())
    serialize_scalar_from_bind(src.Coefficient4, dst.get_coefficient4())

def serialize_segment_sigmoidal_to_bind(src: SESegmentSigmoidal, dst: SegmentSigmoidalData):
    serialize_scalar_volume_to_bind(src.get_begin_volume(), dst.BeginVolume)
    serialize_scalar_volume_to_bind(src.get_end_volume(), dst.EndVolume)
    serialize_scalar_pressure_to_bind(src.get_lower_corner(), dst.LowerCorner)
    serialize_scalar_pressure_to_bind(src.get_upper_corner(), dst.UpperCorner)
    serialize_scalar_volume_per_pressure_to_bind(src.get_baseline_compliance(), dst.BaselineCompliance)

def serialize_segment_sigmoidal_from_bind(src: SegmentSigmoidalData, dst: SESegmentSigmoidal):
    serialize_scalar_volume_from_bind(src.BeginVolume, dst.get_begin_volume())
    serialize_scalar_volume_from_bind(src.EndVolume, dst.get_end_volume())
    serialize_scalar_pressure_from_bind(src.LowerCorner, dst.get_lower_corner())
    serialize_scalar_pressure_from_bind(src.UpperCorner, dst.get_upper_corner())
    serialize_scalar_volume_per_pressure_from_bind(src.BaselineCompliance, dst.get_baseline_compliance())

def serialize_curve_to_bind(src: SECurve, dst: CurveData):
    for segment in src.get_segments():
        any_segment_data = AnySegmentData()
        if isinstance(segment, SESegmentConstant):
            serialize_segment_constant_to_bind(segment, any_segment_data.ConstantSegment)
        elif isinstance(segment, SESegmentLinear):
            serialize_segment_linear_to_bind(segment, any_segment_data.LinearSegment)
        elif isinstance(segment, SESegmentParabolic):
            serialize_segment_parabolic_to_bind(segment, any_segment_data.ParabolicSegment)
        elif isinstance(segment, SESegmentSigmoidal):
            serialize_segment_sigmoidal_to_bind(segment, any_segment_data.SigmoidalSegment)
        else:
            raise Exception("Unknown segment type in serialize_curve_to_bind")
        dst.Segment.append(any_segment_data)

def serialize_curve_from_bind(src: CurveData, dst: SECurve):
    dst.invalidate()
    for any_segment_data in src.Segment:
        if any_segment_data.ConstantSegment is not None:
            segment = dst.add_constant_segment()
            serialize_segment_constant_from_bind(any_segment_data.ConstantSegment, segment)
        elif any_segment_data.LinearSegment is not None:
            segment = dst.add_linear_segment()
            serialize_segment_linear_from_bind(any_segment_data.LinearSegment, segment)
        elif any_segment_data.ParabolicSegment is not None:
            segment = dst.add_parabolic_segment()
            serialize_segment_parabolic_from_bind(any_segment_data.ParabolicSegment, segment)
        elif any_segment_data.SigmoidalSegment is not None:
            segment = dst.add_sigmoidal_segment()
            serialize_segment_sigmoidal_from_bind(any_segment_data.SigmoidalSegment, segment)
        else:
            raise Exception("No valid segment in serialize_curve_to_bind")

from pulse.cdm.physiology import SERespiratoryMechanics
from pulse.cdm.bind.Physiology_pb2 import RespiratoryMechanicsData

from pulse.cdm.io.curve import serialize_curve_to_bind
from pulse.cdm.io.scalars import serialize_scalar_pressure_time_per_volume_to_bind, \
                                 serialize_scalar_pressure_to_bind, \
                                 serialize_scalar_time_to_bind

def serialize_respiratory_mechanics_to_bind(src: SERespiratoryMechanics, dst: RespiratoryMechanicsData):
    dst.Active = src.get_active().value

    if src.has_left_compliance_curve():
        serialize_curve_to_bind(src.get_left_compliance_curve(), dst.LeftComplianceCurve)
    if src.has_left_compliance_curve():
        serialize_curve_to_bind(src.get_right_compliance_curve(), dst.RightComplianceCurve)

    if src.has_left_expiratory_resistance():
        serialize_scalar_pressure_time_per_volume_to_bind(src.get_left_expiratory_resistance(), dst.LeftExpiratoryResistance)
    if src.has_left_inspiratory_resistance():
        serialize_scalar_pressure_time_per_volume_to_bind(src.get_left_inspiratory_resistance(), dst.LeftInspiratoryResistance)
    if src.has_right_expiratory_resistance():
        serialize_scalar_pressure_time_per_volume_to_bind(src.get_right_expiratory_resistance(), dst.RightExpiratoryResistance)
    if src.has_right_inspiratory_resistance():
        serialize_scalar_pressure_time_per_volume_to_bind(src.get_right_inspiratory_resistance(), dst.RightInspiratoryResistance)
    if src.has_upper_expiratory_resistance():
        serialize_scalar_pressure_time_per_volume_to_bind(src.get_upper_expiratory_resistance(), dst.UpperExpiratoryResistance)
    if src.has_upper_inspiratory_resistance():
        serialize_scalar_pressure_time_per_volume_to_bind(src.get_upper_inspiratory_resistance(), dst.UpperInspiratoryResistance)

    if src.has_inspiratory_peak_pressure():
        serialize_scalar_pressure_to_bind(src.get_inspiratory_peak_pressure(), dst.InspiratoryPeakPressure)
    if src.has_expiratory_peak_pressure():
        serialize_scalar_pressure_to_bind(src.get_expiratory_peak_pressure(), dst.ExpiratoryPeakPressure)

    if src.has_inspiratory_rise_time():
        serialize_scalar_time_to_bind(src.get_inspiratory_rise_time(), dst.InspiratoryRiseTime)
    if src.has_expiratory_rise_time():
        serialize_scalar_time_to_bind(src.get_expiratory_rise_time(), dst.ExpiratoryRiseTime)
    if src.has_inspiratory_release_time():
        serialize_scalar_time_to_bind(src.get_inspiratory_release_time(), dst.InspiratoryReleaseTime)
    if src.has_inspiratory_to_expiratory_pause_time():
        serialize_scalar_time_to_bind(src.get_inspiratory_to_expiratory_pause_time(), dst.InspiratoryToExpiratoryPauseTime)
    if src.has_expiratory_rise_time():
        serialize_scalar_time_to_bind(src.get_expiratory_rise_time(), dst.ExpiratoryRiseTime)
    if src.has_expiratory_hold_time():
        serialize_scalar_time_to_bind(src.get_expiratory_hold_time(), dst.ExpiratoryHoldTime)
    if src.has_expiratory_release_time():
        serialize_scalar_time_to_bind(src.get_expiratory_release_time(), dst.ExpiratoryReleaseTime)
    if src.has_residue_time():
        serialize_scalar_time_to_bind(src.get_residue_time(), dst.ResidueTime)

def serialize_respiratory_mechanics_from_bind(src: RespiratoryMechanicsData, dst: SERespiratoryMechanics):
    raise Exception("serialize_respiratory_mechanics_from_bind not implemented")
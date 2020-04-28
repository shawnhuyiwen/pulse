# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.mechanical_ventilator import SEMechanicalVentilator
from pulse.cdm.bind.MechanicalVentilator_pb2 import MechanicalVentilatorData
from pulse.cdm.bind.Substance_pb2 import SubstanceFractionData, SubstanceConcentrationData

from pulse.cdm.io.scalars import *

def serialize_mechanical_ventilator_to_bind(src: SEMechanicalVentilator, dst: MechanicalVentilatorData):

    if (src.has_breath_period() or src.has_respiration_rate()) and src.has_inspiratory_experiatory_ratioy():
        if src.has_breath_period():
            serialize_scalar_time_to_bind(src.get_breath_period(), dst.BreathProfile.Period)
        elif src.has_respiration_rate():
            serialize_scalar_frequency_to_bind(src.get_respiration_rate(), dst.BreathProfile.Rate)
        serialize_scalar_to_bind(src.get_inspiratory_experiatory_ratio(), dst.BreathProfile.InspiratoryExpiratoryRatio)
    elif src.has_expiratory_period() and src.has_inspriatory_period():
        serialize_scalar_time_to_bind(src.get_expiratory_period(), dst.InspiratoryExpiratoryPeriod.ExpiratoryPeriod)
        serialize_scalar_time_to_bind(src.get_inspriatory_period(), dst.InspiratoryExpiratoryPeriod.InspiratoryPeriod)

    dst.Connection = src.get_connection().value
    dst.Control = src.get_control().value
    dst.DriverWaveform = src.get_driver_waveform().value
    if src.has_peak_inspriatory_pressure():
        serialize_scalar_pressure_to_bind(src.get_peak_inspriatory_pressure(), dst.PeakInspiratoryPressure)
    if src.has_positive_end_expired_pressure():
        serialize_scalar_pressure_to_bind(src.get_positive_end_expired_pressure(), dst.PositiveEndExpiredPressure)

    for aGas in src.get_fraction_inspired_gasses():
        sf = SubstanceFractionData()
        sf.Name = aGas.get_substance()
        serialize_scalar_0to1_to_bind(aGas.get_fraction_amount(), sf.Amount)
        dst.FractionInspiredGas.append(sf)

    for aAerosol in src.get_concentration_inspired_aerosols():
        sc = SubstanceConcentrationData()
        sc.Name = aAerosol.get_substance()
        serialize_scalar_mass_per_volume_to_bind(aAerosol.get_concentration(), sc.Concentration)
        dst.ConcentrationInspiredAerosol.append(sc)

def serialize_mechanical_ventilator_from_bind(src: MechanicalVentilatorData, dst: SEMechanicalVentilator):
    raise Exception("serialize_mechanical_ventilator_from_bind not implemented")
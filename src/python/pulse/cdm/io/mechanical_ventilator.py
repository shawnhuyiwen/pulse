# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.mechanical_ventilator import SEMechanicalVentilator
from pulse.cdm.bind.MechanicalVentilator_pb2 import MechanicalVentilatorData
from pulse.cdm.bind.Substance_pb2 import SubstanceFractionData, SubstanceConcentrationData

from pulse.cdm.io.scalars import *

def serialize_mechanical_ventilator_to_bind(src: SEMechanicalVentilator, dst: MechanicalVentilatorData):
    dst.Connection = src.get_connection().value
    if src.has_endotracheal_tube_resistance():
        serialize_scalar_pressure_time_per_volume_to_bind(src.get_endotracheal_tube_resistance(), dst.EndotrachealTubeResistance)

    if src.has_positive_end_expired_pressure():
        serialize_scalar_pressure_to_bind(src.get_positive_end_expired_pressure(), dst.PositiveEndExpiredPressure)
    if src.has_functional_residual_capacity():
        serialize_scalar_pressure_to_bind(src.get_functional_residual_capacity(), dst.FunctionalResidualCapacity)

    if src.has_expiration_cycle_flow():
        serialize_scalar_volume_per_time_to_bind(src.get_expiration_cycle_flow(), dst.ExpirationCycleFlow)
    elif src.has_expiration_cycle_pressure():
        serialize_scalar_pressure_to_bind(src.get_expiration_cycle_pressure(), dst.ExpirationCyclePressure)
    elif src.has_expiration_cycle_time():
        serialize_scalar_time_to_bind(src.get_expiration_cycle_time(), dst.ExpirationCycleTime)
    elif src.has_expiration_cycle_volume():
        serialize_scalar_volume_to_bind(src.get_expiration_cycle_volume(), dst.ExpirationCycleVolume)

    if src.has_expiration_tube_resistance():
        serialize_scalar_pressure_time_per_volume_to_bind(src.get_expiration_tube_resistance(), dst.ExpirationTubeResistance)
    if src.has_expiration_valve_resistance():
        serialize_scalar_pressure_time_per_volume_to_bind(src.get_expiration_valve_resistance(), dst.ExpirationValveResistance)
    dst.ExpirationWaveform = src.get_expiration_waveform().value

    if src.has_inspiration_limit_flow():
        serialize_scalar_volume_per_time_to_bind(src.get_inspiration_limit_flow(), dst.InspirationLimitFlow)
    elif src.has_inspiration_limit_pressure():
        serialize_scalar_pressure_to_bind(src.get_inspiration_limit_pressure(), dst.InspirationLimitPressure)
    elif src.has_inspiration_limit_volume():
        serialize_scalar_volume_to_bind(src.get_inspiration_limit_volume(), dst.InspirationLimitVolume)

    if src.has_inspiration_pause_time():
        serialize_scalar_time_to_bind(src.get_inspiration_pause_time(), dst.InspirationPauseTime)

    if src.has_peak_inspiratory_pressure():
        serialize_scalar_pressure_to_bind(src.get_peak_inspiratory_pressure(), dst.PeakInspiratoryPressure)
    if src.has_inspiration_target_flow():
        serialize_scalar_pressure_to_bind(src.get_inspiration_target_flow(), dst.InspirationTargetFlow)

    if src.has_inspiration_machine_trigger_time():
        serialize_scalar_time_to_bind(src.get_inspiration_machine_trigger_time(), dst.InspirationMachineTriggerTime)

    if src.has_inspiration_patient_trigger_flow():
        serialize_scalar_volume_per_time_to_bind(src.get_inspiration_patient_trigger_flow(), dst.InspirationPatientTriggerFlow)
    if src.has_inspiration_patient_trigger_pressure():
        serialize_scalar_pressure_to_bind(src.get_inspiration_patient_trigger_pressure(), dst.InspirationPatientTriggerPressure)

    if src.has_inspiration_tube_resistance():
        serialize_scalar_pressure_time_per_volume_to_bind(src.get_inspiration_tube_resistance(), dst.InspirationTubeResistance)
    if src.has_inspiration_valve_resistance():
        serialize_scalar_pressure_time_per_volume_to_bind(src.get_inspiration_valve_resistance(), dst.InspirationValveResistance)
    dst.InspirationWaveform = src.get_inspiration_waveform().value

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
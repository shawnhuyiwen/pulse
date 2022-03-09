# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from enum import Enum
from pulse.cdm.engine import eSwitch
from pulse.cdm.equipment import SEEquipment
from pulse.cdm.substance import SESubstanceFraction, \
                                SESubstanceConcentration
from pulse.cdm.scalars import SEScalarFrequency, SEScalarPressureTimePerVolume, SEScalarVolumePerTime, \
                              SEScalarPressure, SEScalarTime, SEScalarVolume, SEScalarVolumePerPressure, \
                              MassPerVolumeUnit

class eECMO_CannulationLocation(Enum):
    NullCannulationLocation = 0
    InternalJugular = 1
    RightFemoralVein = 2
    LeftFemoralVein = 3
    RightSubclavianVein = 4
    LeftSubclavianVein = 5

class SEECMOSettings(SEEquipment):
    __slots__ = ["_inflow_location",
                 "_outflow_location",
                 "_oxygenator_volume",
                 "_transfusion_flow",
                 "_substance_compound",
                 "_substance_concentrations"]

    def __init__(self):
        super().__init__()
        self._inflow_location = eECMO_CannulationLocation.NullCannulationLocation
        self._outflow_location = eECMO_CannulationLocation.NullCannulationLocation
        self._oxygenator_volume = None
        self._transfusion_flow = None
        self._substance_compound = None
        self._substance_concentrations = []

    def clear(self):
        self._inflow_location = eECMO_CannulationLocation.NullCannulationLocation
        self._outflow_location = eECMO_CannulationLocation.NullCannulationLocation
        if self._oxygenator_volume is not None: self._oxygenator_volume.invalidate()
        if self._transfusion_flow is not None: self._transfusion_flow.invalidate()
        self._substance_compound = None
        self._substance_concentrations = []

    def copy(self, src):
        if not isinstance(SEECMOSettings, src):
            raise Exception("Provided argument must be a SEECMOSettings")
        self.clear()
        self._inflow_location = src._inflow_location
        self._outflow_location = src._outflow_location
        if src.has_oxygenator_volume(): self.get_oxygenator_volume().set(src._oxygenator_volume)
        if src.has_transfusion_flow(): self.get_transfusion_flow().set(src._transfusion_flow)
        self._substance_compound = src._substance_compound
        if src.has_substance_concentrations:
            self._substance_concentrations.append(src._substance_concentrations[:])

    def get_inflow_location(self):
        return self._inflow_location
    def set_inflow_location(self, t: eECMO_CannulationLocation):
        self._inflow_location = t
    def has_connection(self):
        return self._connection != eSwitch.NullSwitch

    def has_connection_volume(self):
        return False if self._connection_volume is None else self._connection_volume.is_valid()
    def get_connection_volume(self):
        if self._connection_volume is None:
            self._connection_volume = SEScalarVolume()
        return self._connection_volume

    def has_compliance(self):
        return False if self._compliance is None else self._compliance.is_valid()
    def get_compliance(self):
        if self._compliance is None:
            self._compliance = SEScalarVolumePerPressure()
        return self._compliance

    def has_driver_damping_parameter(self):
        return False if self._driver_damping_parameter is None else self._driver_damping_parameter.is_valid()
    def get_driver_damping_parameter(self):
        if self._driver_damping_parameter is None:
            self._driver_damping_parameter = SEScalarFrequency()
        return self._driver_damping_parameter

    def has_positive_end_expired_pressure(self):
        return False if self._positive_end_expired_pressure is None else self._positive_end_expired_pressure.is_valid()
    def get_positive_end_expired_pressure(self):
        if self._positive_end_expired_pressure is None:
            self._positive_end_expired_pressure = SEScalarPressure()
        return self._positive_end_expired_pressure
    
    def has_functional_residual_capacity(self):
        return False if self._functional_residual_capacity is None else self._functional_residual_capacity.is_valid()
    def get_functional_residual_capacity(self):
        if self._functional_residual_capacity is None:
            self._functional_residual_capacity = SEScalarPressure()
        return self._functional_residual_capacity
    
    def has_expiration_cycle_flow(self):
        return False if self._expiration_cycle_flow is None else self._expiration_cycle_flow.is_valid()
    def get_expiration_cycle_flow(self):
        if self._expiration_cycle_flow is None:
            self._expiration_cycle_flow = SEScalarVolumePerTime()
        return self._expiration_cycle_flow
    
    def has_expiration_cycle_pressure(self):
        return False if self._expiration_cycle_pressure is None else self._expiration_cycle_pressure.is_valid()
    def get_expiration_cycle_pressure(self):
        if self._expiration_cycle_pressure is None:
            self._expiration_cycle_pressure = SEScalarPressure()
        return self._expiration_cycle_pressure
    
    def has_expiration_cycle_time(self):
        return False if self._expiration_cycle_time is None else self._expiration_cycle_time.is_valid()
    def get_expiration_cycle_time(self):
        if self._expiration_cycle_time is None:
            self._expiration_cycle_time = SEScalarTime()
        return self._expiration_cycle_time
    
    def has_expiration_cycle_volume(self):
        return False if self._expiration_cycle_volume is None else self._expiration_cycle_volume.is_valid()
    def get_expiration_cycle_volume(self):
        if self._expiration_cycle_volume is None:
            self._expiration_cycle_volume = SEScalarVolume()
        return self._expiration_cycle_volume

    def get_expiration_cycle_respiratory_model(self):
        return self._expiration_cycle_respiratory_model
    def set_expiration_cycle_respiratory_model(self, s: eSwitch):
        self._expiration_cycle_respiratory_model = s
    def has_expiration_cycle_respiratory_model(self):
        return self._expiration_cycle_respiratory_model != eSwitch.NullSwitch

    def has_expiration_limb_volume(self):
        return False if self._expiration_limb_volume is None else self._expiration_limb_volume.is_valid()
    def get_expiration_limb_volume(self):
        if self._expiration_limb_volume is None:
            self._expiration_limb_volume = SEScalarVolume()
        return self._expiration_limb_volume
    
    def has_expiration_tube_resistance(self):
        return False if self._expiration_tube_resistance is None else self._expiration_tube_resistance.is_valid()
    def get_expiration_tube_resistance(self):
        if self._expiration_tube_resistance is None:
            self._expiration_tube_resistance = SEScalarPressureTimePerVolume()
        return self._expiration_tube_resistance
    
    def has_expiration_valve_resistance(self):
        return False if self._expiration_valve_resistance is None else self._expiration_valve_resistance.is_valid()
    def get_expiration_valve_resistance(self):
        if self._expiration_valve_resistance is None:
            self._expiration_valve_resistance = SEScalarPressureTimePerVolume()
        return self._expiration_valve_resistance
    
    def has_expiration_valve_volume(self):
        return False if self._expiration_valve_volume is None else self._expiration_valve_volume.is_valid()
    def get_expiration_valve_volume(self):
        if self._expiration_valve_volume is None:
            self._expiration_valve_volume = SEScalarVolume()
        return self._expiration_valve_volume

    def has_expiration_waveform_period(self):
        return False if self._expiration_waveform_period is None else self._expiration_waveform_period.is_valid()
    def get_expiration_waveform_period(self):
        if self._expiration_waveform_period is None:
            self._expiration_waveform_period = SEScalarTime()
        return self._expiration_waveform_period
        
    def has_inspiration_limit_flow(self):
        return False if self._inspiration_limit_flow is None else self._inspiration_limit_flow.is_valid()
    def get_inspiration_limit_flow(self):
        if self._inspiration_limit_flow is None:
            self._inspiration_limit_flow = SEScalarVolumePerTime()
        return self._inspiration_limit_flow

    def has_inspiration_limit_pressure(self):
        return False if self._inspiration_limit_pressure is None else self._inspiration_limit_pressure.is_valid()
    def get_inspiration_limit_pressure(self):
        if self._inspiration_limit_pressure is None:
            self._inspiration_limit_pressure = SEScalarPressure()
        return self._inspiration_limit_pressure

    def has_inspiration_limit_volume(self):
        return False if self._inspiration_limit_volume is None else self._inspiration_limit_volume.is_valid()
    def get_inspiration_limit_volume(self):
        if self._inspiration_limit_volume is None:
            self._inspiration_limit_volume = SEScalarVolume()
        return self._inspiration_limit_volume
        
    def has_inspiration_pause_time(self):
        return False if self._inspiration_pause_time is None else self._inspiration_pause_time.is_valid()
    def get_inspiration_pause_time(self):
        if self._inspiration_pause_time is None:
            self._inspiration_pause_time = SEScalarTime()
        return self._inspiration_pause_time

    def has_peak_inspiratory_pressure(self):
        return False if self._peak_inspiratory_pressure is None else self._peak_inspiratory_pressure.is_valid()
    def get_peak_inspiratory_pressure(self):
        if self._peak_inspiratory_pressure is None:
            self._peak_inspiratory_pressure = SEScalarPressure()
        return self._peak_inspiratory_pressure

    def has_inspiration_target_flow(self):
        return False if self._inspiration_target_flow is None else self._inspiration_target_flow.is_valid()
    def get_inspiration_target_flow(self):
        if self._inspiration_target_flow is None:
            self._inspiration_target_flow = SEScalarPressure()
        return self._inspiration_target_flow

    def has_inspiration_machine_trigger_time(self):
        return False if self._inspiration_machine_trigger_time is None else self._inspiration_machine_trigger_time.is_valid()
    def get_inspiration_machine_trigger_time(self):
        if self._inspiration_machine_trigger_time is None:
            self._inspiration_machine_trigger_time = SEScalarTime()
        return self._inspiration_machine_trigger_time

    def has_inspiration_patient_trigger_flow(self):
        return False if self._inspiration_patient_trigger_flow is None else self._inspiration_patient_trigger_flow.is_valid()
    def get_inspiration_patient_trigger_flow(self):
        if self._inspiration_patient_trigger_flow is None:
            self._inspiration_patient_trigger_flow = SEScalarVolumePerTime()
        return self._inspiration_patient_trigger_flow

    def has_inspiration_patient_trigger_pressure(self):
        return False if self._inspiration_patient_trigger_pressure is None else self._inspiration_patient_trigger_pressure.is_valid()
    def get_inspiration_patient_trigger_pressure(self):
        if self._inspiration_patient_trigger_pressure is None:
            self._inspiration_patient_trigger_pressure = SEScalarPressure()
        return self._inspiration_patient_trigger_pressure

    def get_inspiration_patient_trigger_respiratory_model(self):
        return self._inspiration_patient_trigger_respiratory_model
    def set_inspiration_patient_trigger_respiratory_model(self, s: eSwitch):
        self._inspiration_patient_trigger_respiratory_model = s
    def has_inspiration_patient_trigger_respiratory_model(self):
        return self._inspiration_patient_trigger_respiratory_model != eSwitch.NullSwitch

    def has_inspiration_limb_volume(self):
        return False if self._inspiration_limb_volume is None else self._inspiration_limb_volume.is_valid()
    def get_inspiration_limb_volume(self):
        if self._inspiration_limb_volume is None:
            self._inspiration_limb_volume = SEScalarVolume()
        return self._inspiration_limb_volume

    def has_inspiration_tube_resistance(self):
        return False if self._inspiration_tube_resistance is None else self._inspiration_tube_resistance.is_valid()
    def get_inspiration_tube_resistance(self):
        if self._inspiration_tube_resistance is None:
            self._inspiration_tube_resistance = SEScalarPressureTimePerVolume()
        return self._inspiration_tube_resistance

    def has_inspiration_valve_resistance(self):
        return False if self._inspiration_valve_resistance is None else self._inspiration_valve_resistance.is_valid()
    def get_inspiration_valve_resistance(self):
        if self._inspiration_valve_resistance is None:
            self._inspiration_valve_resistance = SEScalarPressureTimePerVolume()
        return self._inspiration_valve_resistance

    def has_inspiration_valve_volume(self):
        return False if self._inspiration_valve_volume is None else self._inspiration_valve_volume.is_valid()
    def get_inspiration_valve_volume(self):
        if self._inspiration_valve_volume is None:
            self._inspiration_valve_volume = SEScalarVolume()
        return self._inspiration_valve_volume

    def has_relief_valve_threshold(self):
        return False if self._relief_valve_threshold is None else self._relief_valve_threshold.is_valid()
    def get_relief_valve_threshold(self):
        if self._relief_valve_threshold is None:
            self._relief_valve_threshold = SEScalarPressure()
        return self._relief_valve_threshold

    def has_y_piece_volume(self):
        return False if self._y_piece_volume is None else self._y_piece_volume.is_valid()
    def get_y_piece_volume(self):
        if self._y_piece_volume is None:
            self._y_piece_volume = SEScalarVolume()
        return self._y_piece_volume

    def has_inspiration_waveform_period(self):
        return False if self._inspiration_waveform_period is None else self._inspiration_waveform_period.is_valid()
    def get_inspiration_waveform_period(self):
        if self._inspiration_waveform_period is None:
            self._inspiration_waveform_period = SEScalarTime()
        return self._inspiration_waveform_period
        
    def has_fraction_inspired_gas(self, substance_name: str = None):
        if substance_name is None:
            return len(self._fraction_inspired_gasses) > 0
        else:
            for sf in self._fraction_inspired_gasses:
                if sf.get_substance() is substance_name:
                    return True
        return False
    def get_fraction_inspired_gas(self, substance_name: str):
        for sf in self._fraction_inspired_gasses:
            if sf.get_substance() is substance_name:
                return sf
        sf = SESubstanceFraction()
        sf.set_substance(substance_name)
        sf.get_fraction_amount().set_value(0)
        self._fraction_inspired_gasses.append(sf)
        return sf
    def get_fraction_inspired_gasses(self):
        return self._fraction_inspired_gasses
    def remove_fraction_inspired_gas(self, substance_name: str):
        for sf in self._fraction_inspired_gasses:
            if sf.get_substance() is substance_name:
                self._fraction_inspired_gasses.remove(sf)
    def remove_fraction_inspired_gasses(self):
        self._fraction_inspired_gasses = []
        return self._inflow_location != eECMO_CannulationLocation.NullCannulationLocation

    def get_outflow_location(self):
        return self._outflow_location
    def set_outflow_location(self, t: eECMO_CannulationLocation):
        self._outflow_location = t
    def has_outflow_location(self):
        return self._outflow_location != eECMO_CannulationLocation.NullCannulationLocation

    def has_oxygenator_volume(self):
        return False if self._oxygenator_volume is None else self._oxygenator_volume.is_valid()
    def get_oxygenator_volume(self):
        if self._oxygenator_volume is None:
            self._oxygenator_volume = SEScalarVolume()
        return self._oxygenator_volume

    def has_transfusion_flow(self):
        return False if self._transfusion_flow is None else self._transfusion_flow.is_valid()
    def get_transfusion_flow(self):
        if self._transfusion_flow is None:
            self._transfusion_flow = SEScalarVolumePerTime()
        return self._transfusion_flow

    def get_substance_compound(self):
        return self._substance_compound
    def set_substance_compound(self, sc: str):
        self._substance_compound = sc
    def has_substance_compound(self):
        return self._substance_compound != None

    def has_concentration_inspired_aerosol(self, substance_name: str=None):
        if substance_name is None:
            return len(self._concentration_inspired_aerosol) > 0
        else:
            for sf in self._concentration_inspired_aerosol:
                if sf.get_substance() is substance_name:
                    return True
        return False
    def get_substance_concentration(self, substance_name: str):
        for sf in self._substance_concentrations:
            if sf.get_substance() is substance_name:
                return sf
        sf = SESubstanceConcentration()
        sf.set_substance(substance_name)
        sf.get_concentration().set_value(0, MassPerVolumeUnit.kg_Per_m3)
        self._substance_concentrations.append(sf)
        return sf
    def get_substance_concentrations(self):
        return self._substance_concentrations
    def remove_substance_concentration(self, substance_name: str):
        for sf in self._substance_concentrations:
            if sf.get_substance() is substance_name:
                self._substance_concentrations.remove(sf)
    def remove_substance_concentrations(self):
        self._substance_concentrations = []
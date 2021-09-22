# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.equipment_actions import SEEquipmentAction
from pulse.cdm.mechanical_ventilator import SEMechanicalVentilatorSettings, eSwitch
from pulse.cdm.scalars import SEScalarPressure, SEScalar0To1, SEScalarTime, \
                              SEScalarFrequency, SEScalarVolumePerTime, SEScalarVolume
from pulse.cdm.bind.Actions_pb2 import eAppliedRespiratoryCycle
from enum import Enum


class SEMechanicalVentilatorAction(SEEquipmentAction):
    def __init__(self):
        super().__init__()


class SEMechanicalVentilatorConfiguration(SEMechanicalVentilatorAction):
    __slots__ = ["_settings_file",
                 "_settings"]

    def __init__(self):
        super().__init__()
        self._settings_file = None
        self._settings = None

    def clear(self):
        self._settings_file = None
        self._settings = None

    def copy(self, src):
        if not isinstance(SEMechanicalVentilatorConfiguration, src):
            raise Exception("Provided argument must be a SEMechanicalVentilatorConfiguration")
        self.clear()
        self._settings_file = src._settings_file
        self._settings.copy(src._settings)

    def is_valid(self):
        return self.has_settings() or self.has_settings_file()

    def is_active(self):
        return True

    def has_settings_file(self):
        return self._settings_file is not None
    def get_settings_file(self):
        return self._settings_file
    def set_settings_file(self, filename: str):
        self._settings_file = filename

    def has_settings(self):
        return self._settings is not None
    def get_settings(self):
        if self._settings is None:
            self._settings = SEMechanicalVentilatorSettings()
        return self._settings

class SEMechanicalVentilatorMode(SEMechanicalVentilatorAction):
    __slots__ = ["_connection"]

    def __init__(self):
        super().__init__()
        self._connection = eSwitch.NullSwitch

    def clear(self):
        self._connection = eSwitch.NullSwitch

    def copy(self, src):
        if not isinstance(SEMechanicalVentilatorMode, src):
            raise Exception("Provided argument must be a SEMechanicalVentilatorMode")
        self.clear()
        super().copy(src)
        self._connection = src._connection

    def is_valid(self):
        return self.has_connection()

    def is_active(self):
        return True

    def has_connection(self):
        return self._connection is not eSwitch.NullSwitch
    def get_connection(self):
        return self._connection
    def set_connection(self, src : eSwitch):
        self._connection = src

    def __repr__(self):
        return "Mechanical Ventilator Mode" + "\n\tConnection: " + str(self._connection)


class SEMechanicalVentilatorContinuousPositiveAirwayPressure(SEMechanicalVentilatorMode):
    __slots__ = ["_delta_pressure_support",
                 "_fraction_inspired_oxygen",
                 "_positive_end_expired_pressure",
                 "_slope"]

    def __init__(self):
        super().__init__()
        self._delta_pressure_support = None
        self._fraction_inspired_oxygen = None
        self._positive_end_expired_pressure = None
        self._slope = None

    def clear(self):
        super().clear()
        self._delta_pressure_support = None
        self._fraction_inspired_oxygen = None
        self._positive_end_expired_pressure = None
        self._slope = None

    def copy(self, src):
        if not isinstance(SEMechanicalVentilatorContinuousPositiveAirwayPressure, src):
            raise Exception("Provided argument must be a SEMechanicalVentilatorContinuousPositiveAirwayPressure")
        self.clear()
        super().copy(src)
        self._delta_pressure_support = src._delta_pressure_support
        self._fraction_inspired_oxygen = src._fraction_inspired_oxygen
        self._positive_end_expired_pressure = src._positive_end_expired_pressure
        self._slope = src._slope

    def is_valid(self):
        return super().is_valid() \
               and self.has_delta_pressure_support() \
               and self.has_fraction_inspired_oxygen() \
               and self.has_positive_end_expired_pressure() \
               and self.has_slope()

    def is_active(self):
        return True

    def has_delta_pressure_support(self):
        return self._delta_pressure_support is not None
    def get_delta_pressure_support(self):
        if self._delta_pressure_support is None:
            self._delta_pressure_support = SEScalarPressure()
        return self._delta_pressure_support

    def has_fraction_inspired_oxygen(self):
        return self._fraction_inspired_oxygen is not None
    def get_fraction_inspired_oxygen(self):
        if self._fraction_inspired_oxygen is None:
            self._fraction_inspired_oxygen = SEScalar0To1()
        return self._fraction_inspired_oxygen

    def has_positive_end_expired_pressure(self):
        return self._positive_end_expired_pressure is not None
    def get_positive_end_expired_pressure(self):
        if self._positive_end_expired_pressure is None:
            self._positive_end_expired_pressure = SEScalarPressure()
        return self._positive_end_expired_pressure

    def has_slope(self):
        return self._slope is not None
    def get_slope(self):
        if self._slope is None:
            self._slope = SEScalarTime()
        return self._slope

    def __repr__(self):
        return "Mechanical Ventilator Continuous Positive Airway Pressure" + \
               "\n\tDeltaPressureSupport: " + str(self._delta_pressure_support) if self.has_delta_pressure_support() else "Not Provided" + \
               "\n\tFractionInspiredOxygen: " + str(self._fraction_inspired_oxygen) if self.has_fraction_inspired_oxygen() else "Not Provided" + \
               "\n\tPositiveEndExpiredPressure: " + str(self._positive_end_expired_pressure) if self.has_positive_end_expired_pressure() else "Not Provided" + \
               "\n\tSlope: " + str(self._slope) if self.has_slope() else "Not Provided"


class eMechanicalVentilator_PressureControlMode(Enum):
    AssistedControl = 0
    ContinuousMandatoryVentilation = 1


class SEMechanicalVentilatorPressureControl(SEMechanicalVentilatorMode):
    __slots__ = ["_mode",
                 "_fraction_inspired_oxygen",
                 "_inspiratory_period",
                 "_inspiratory_pressure",
                 "_positive_end_expired_pressure",
                 "_respiration_rate",
                 "_slope"]

    def __init__(self):
        super().__init__()
        self._mode = eMechanicalVentilator_PressureControlMode.AssistedControl
        self._fraction_inspired_oxygen = None
        self._inspiratory_period = None
        self._inspiratory_pressure = None
        self._positive_end_expired_pressure = None
        self._respiration_rate = None
        self._slope = None

    def clear(self):
        super().clear()
        self._mode = eMechanicalVentilator_PressureControlMode.AssistedControl
        self._fraction_inspired_oxygen = None
        self._inspiratory_period = None
        self._inspiratory_pressure = None
        self._positive_end_expired_pressure = None
        self._respiration_rate = None
        self._slope = None

    def copy(self, src):
        if not isinstance(SEMechanicalVentilatorPressureControl, src):
            raise Exception("Provided argument must be a SEMechanicalVentilatorPressureControl")
        self.clear()
        super().copy(src)
        self._mode = src._mode
        self._fraction_inspired_oxygen = src._fraction_inspired_oxygen
        self._inspiratory_period = src._inspiratory_period
        self._inspiratory_pressure = src._inspiratory_pressure
        self._positive_end_expired_pressure = src._positive_end_expired_pressure
        self._respiration_rate = src._respiration_rate
        self._slope = src._slope

    def is_valid(self):
        return super().is_valid() \
               and self.has_fraction_inspired_oxygen() \
               and self.has_inspiratory_period() \
               and self.has_inspiratory_pressure() \
               and self.has_positive_end_expired_pressure() \
               and self.has_respiration_rate() \
               and self.has_slope()

    def is_active(self):
        return True

    def get_mode(self):
        return self._mode
    def set_mode(self, src : eMechanicalVentilator_PressureControlMode):
        self._mode = src

    def has_fraction_inspired_oxygen(self):
        return self._fraction_inspired_oxygen is not None
    def get_fraction_inspired_oxygen(self):
        if self._fraction_inspired_oxygen is None:
            self._fraction_inspired_oxygen = SEScalar0To1()
        return self._fraction_inspired_oxygen

    def has_inspiratory_period(self):
        return self._inspiratory_period is not None
    def get_inspiratory_period(self):
        if self._inspiratory_period is None:
            self._inspiratory_period = SEScalarTime()
        return self._inspiratory_period

    def has_inspiratory_pressure(self):
            return self._inspiratory_pressure is not None
    def get_inspiratory_pressure(self):
        if self._inspiratory_pressure is None:
            self._inspiratory_pressure = SEScalarPressure()
        return self._inspiratory_pressure

    def has_positive_end_expired_pressure(self):
        return self._positive_end_expired_pressure is not None
    def get_positive_end_expired_pressure(self):
        if self._positive_end_expired_pressure is None:
            self._positive_end_expired_pressure = SEScalarPressure()
        return self._positive_end_expired_pressure

    def has_respiration_rate(self):
        return self._respiration_rate is not None
    def get_respiration_rate(self):
        if self._respiration_rate is None:
            self._respiration_rate = SEScalarFrequency()
        return self._respiration_rate

    def has_slope(self):
        return self._slope is not None
    def get_slope(self):
        if self._slope is None:
            self._slope = SEScalarTime()
        return self._slope

    def __repr__(self):
        return "Mechanical Ventilator Pressure Control" + \
               "\n\tFractionInspiredOxygen: " + str(self._fraction_inspired_oxygen) if self.has_fraction_inspired_oxygen() else "Not Provided" + \
               "\n\tInspiratoryPeriod: " + str(self._inspiratory_period) if self.has_inspiratory_period() else "Not Provided" + \
               "\n\tInspiratoryPressure: " + str(self._inspiratory_pressure) if self.has_inspiratory_pressure() else "Not Provided" + \
               "\n\tPositiveEndExpiredPressure: " + str(self._positive_end_expired_pressure) if self.has_positive_end_expired_pressure() else "Not Provided" + \
               "\n\tRespirationRate: " + str(self._respiration_rate) if self.has_respiration_rate() else "Not Provided" + \
               "\n\tSlope: " + str(self._slope) if self.has_slope() else "Not Provided"


class eMechanicalVentilator_VolumeControlMode(Enum):
    AssistedControl = 0
    ContinuousMandatoryVentilation = 1


class SEMechanicalVentilatorVolumeControl(SEMechanicalVentilatorMode):
    __slots__ = ["_mode",
                 "_flow",
                 "_fraction_inspired_oxygen",
                 "_inspiratory_period",
                 "_positive_end_expired_pressure",
                 "_respiration_rate",
                 "_tidal_volume"]

    def __init__(self):
        super().__init__()
        self._mode = eMechanicalVentilator_PressureControlMode.AssistedControl
        self._flow = None
        self._fraction_inspired_oxygen = None
        self._inspiratory_period = None
        self._positive_end_expired_pressure = None
        self._respiration_rate = None
        self._tidal_volume = None

    def clear(self):
        super().clear()
        self._mode = eMechanicalVentilator_PressureControlMode.AssistedControl
        self._flow = None
        self._fraction_inspired_oxygen = None
        self._inspiratory_period = None
        self._positive_end_expired_pressure = None
        self._respiration_rate = None
        self._tidal_volume = None

    def copy(self, src):
        if not isinstance(SEMechanicalVentilatorVolumeControl, src):
            raise Exception("Provided argument must be a SEMechanicalVentilatorVolumeControl")
        self.clear()
        super().copy(src)
        self._mode = src._mode
        self._flow = src._flow
        self._fraction_inspired_oxygen = src._fraction_inspired_oxygen
        self._inspiratory_period = src._inspiratory_period
        self._positive_end_expired_pressure = src._positive_end_expired_pressure
        self._respiration_rate = src._respiration_rate
        self._tidal_volume = src._tidal_volume

    def is_valid(self):
        return super().is_valid() \
               and self.has_flow() \
               and self.has_fraction_inspired_oxygen() \
               and self.has_inspiratory_period() \
               and self.has_positive_end_expired_pressure() \
               and self.has_respiration_rate() \
               and self.has_tidal_volume()

    def is_active(self):
        return True

    def get_mode(self):
        return self._mode
    def set_mode(self, src : eMechanicalVentilator_VolumeControlMode):
        self._mode = src

    def has_flow(self):
        return self._flow is not None
    def get_flow(self):
        if self._flow is None:
            self._flow = SEScalarVolumePerTime()
        return self._flow

    def has_fraction_inspired_oxygen(self):
        return self._fraction_inspired_oxygen is not None
    def get_fraction_inspired_oxygen(self):
        if self._fraction_inspired_oxygen is None:
            self._fraction_inspired_oxygen = SEScalar0To1()
        return self._fraction_inspired_oxygen

    def has_inspiratory_period(self):
        return self._inspiratory_period is not None
    def get_inspiratory_period(self):
        if self._inspiratory_period is None:
            self._inspiratory_period = SEScalarTime()
        return self._inspiratory_period

    def has_positive_end_expired_pressure(self):
        return self._positive_end_expired_pressure is not None
    def get_positive_end_expired_pressure(self):
        if self._positive_end_expired_pressure is None:
            self._positive_end_expired_pressure = SEScalarPressure()
        return self._positive_end_expired_pressure

    def has_respiration_rate(self):
        return self._respiration_rate is not None
    def get_respiration_rate(self):
        if self._respiration_rate is None:
            self._respiration_rate = SEScalarFrequency()
        return self._respiration_rate

    def has_tidal_volume(self):
        return self._tidal_volume is not None
    def get_tidal_volume(self):
        if self._tidal_volume is None:
            self._tidal_volume = SEScalarVolume()
        return self._tidal_volume

    def __repr__(self):
        return "Mechanical Ventilator Volume Control" + \
               "\n\tFlow: " + str(self._flow) if self.has_flow() else "Not Provided" + \
               "\n\tFractionInspiredOxygen: " + str(self._fraction_inspired_oxygen) if self.has_fraction_inspired_oxygen() else "Not Provided" + \
               "\n\tInspiratoryPeriod: " + str(self._inspiratory_period) if self.has_inspiratory_period() else "Not Provided" + \
               "\n\tPositiveEndExpiredPressure: " + str(self._positive_end_expired_pressure) if self.has_positive_end_expired_pressure() else "Not Provided" + \
               "\n\tRespirationRate: " + str(self._respiration_rate) if self.has_respiration_rate() else "Not Provided" + \
               "\n\tTidalVolume: " + str(self._tidal_volume) if self.has_tidal_volume() else "Not Provided"


class SEMechanicalVentilatorHold(SEMechanicalVentilatorAction):
    __slots__ = ["_state",
                 "_applied_respiratory_cycle"]

    def __init__(self):
        super().__init__()
        self._state = eSwitch.Off
        self._applied_respiratory_cycle = eAppliedRespiratoryCycle.Expiration

    def clear(self):
        super().clear()
        self._state = eSwitch.Off
        self._applied_respiratory_cycle = eAppliedRespiratoryCycle.Expiration

    def copy(self, src):
        super().copy(src)
        self._state = src._state
        self._applied_respiratory_cycle = src._applied_respiratory_cycle

    def is_valid(self):
        return True

    def is_active(self):
        return True

    def get_state(self):
        return self._state
    def set_state(self, src : eSwitch):
        self._state = src

    def get_applied_respiratory_cycle(self):
        return self._applied_respiratory_cycle
    def set_applied_respiratory_cycle(self, src : eAppliedRespiratoryCycle):
        self._applied_respiratory_cycle = src

    def __repr__(self):
        return "Mechanical Ventilator Hold" + \
               "\n\tState: " + str(self._state) + \
               "\n\tAppliedRespiratoryCycle: " + str(self._applied_respiratory_cycle)


class SEMechanicalVentilatorLeak(SEMechanicalVentilatorAction):
    __slots__ = ["_severity"]

    def __init__(self):
        super().__init__()
        self._severity = None

    def clear(self):
        super().clear()
        self._severity = None

    def copy(self, src):
        super().copy(src)
        self._severity = src._severity

    def is_valid(self):
        return self.has_severity()

    def is_active(self):
        return True

    def has_severity(self):
        return self._severity is not None
    def get_severity(self):
        if self._severity is None:
            self._severity = SEScalar0To1()
        return self._severity

    def __repr__(self):
        return "Mechanical Ventilator Leak" + \
               "\n\tSeverity: " + str(self._severity) if self.has_severity() else "Not Provided"

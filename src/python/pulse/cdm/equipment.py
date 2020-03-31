# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from enum import Enum
from pulse.cdm.scalars import SEScalar, SEScalarFrequency, \
                              SEScalarPressure, SEScalarTime
from pulse.cdm.substance import SESubstanceFraction

class SEEquipment():
    __slots__ = []
    def __init__(self):
        pass
    def copy(self, equipment):
        self.clear()
    def clear(self):
        pass

class eConnection(Enum):
    NullConnection = 0
    Off = 1
    Mask = 2
    Tube = 3

class eControl(Enum):
    NullControl = 0
    P_CMV = 1

class eDriverWaveform(Enum):
    NullDriverWaveform = 0
    Square = 1

class SEMechanicalVentilator(SEEquipment):
    __slots__ = ["_breath_period", "_connection", "_control", "_driver_waveform",
                 "_expiratory_period", "_inspiratory_experiatory_ratio", "_inspriatory_period",
                 "_peak_inspriatory_pressure", "_positive_end_expired_pressure", "_respiration_rate",
                 "_fraction_inspired_gasses"]

    def __init__(self):
        super().__init__()
        self._breath_period = None
        self._connection = eConnection.NullConnection
        self._control = eControl.NullControl
        self._driver_waveform = eDriverWaveform.NullDriverWaveform
        self._expiratory_period = None
        self._inspiratory_experiatory_ratio = None
        self._inspriatory_period = None
        self._peak_inspriatory_pressure = None
        self._positive_end_expired_pressure = None
        self._respiration_rate = None
        self._fraction_inspired_gasses = []
        
    def clear(self):
        if self._breath_period is not None: self._breath_period.invalidate()
        self._connection = eConnection.NullConnection
        self._control = eControl.NullControl
        self._driver_waveform = eDriverWaveform.NullDriverWaveform
        if self._expiratory_period is not None: self._expiratory_period.invalidate()
        if self._inspiratory_experiatory_ratio is not None: self._inspiratory_experiatory_ratio.invalidate()
        if self._inspriatory_period is not None: self._inspriatory_period.invalidate()
        if self._peak_inspriatory_pressure is not None: self._peak_inspriatory_pressure.invalidate()
        if self._positive_end_expired_pressure is not None: self._positive_end_expired_pressure.invalidate()
        if self._respiration_rate is not None: self._respiration_rate.invalidate()
        self._fraction_inspired_gasses = []

    def copy(self, src):
        if not isinstance(SEMechanicalVentilator, src):
            raise Exception("Provided argument must be a SEMechanicalVentilator")
        self.clear()
        if src.has_breath_period(): self.get_breath_period().set(src._breath_period)
        self._connection = src._connection
        self._control = src._control
        self._driver_waveform = src._driver_waveform
        if src.has_expiratory_period(): self.get_expiratory_period().set(src._expiratory_period)
        if src.has_inspiratory_experiatory_ratio(): self.get_inspiratory_experiatory_ratio().set(src._inspiratory_experiatory_ratio)
        if src.has_inspriatory_period(): self.get_inspriatory_period().set(src._inspriatory_period)
        if src.has_peak_inspriatory_pressure(): self.get_peak_inspriatory_pressure().set(src._peak_inspriatory_pressure)
        if src.has_positive_end_expired_pressure(): self.get_positive_end_expired_pressure().set(src._positive_end_expired_pressure)
        if src.has_respiration_rate(): self.get_respiration_rate().set(src._respiration_rate)

        if src.has_fraction_inspired_gasses:
            self._fraction_inspired_gasses.append(src._fraction_inspired_gasses[:])

    def has_breath_period(self):
        return False if self._breath_period is None else self._breath_period.is_valid()
    def get_breath_period(self):
        if self._breath_period is None:
            self._breath_period = SEScalarTime()
        return self._breath_period
    
    def get_connection(self):
        return self._connection
    def set_connection(self, t: eConnection):
        self._connection = t
    
    def get_control(self):
        return self._control
    def set_control(self, t: eControl):
        self._control = t
        
    def get_driver_waveform(self):
        return self._driver_waveform
    def set_driver_waveform(self, t: eDriverWaveform):
        self._driver_waveform = t

    def has_expiratory_period(self):
        return False if self._expiratory_period is None else self._expiratory_period.is_valid()
    def get_expiratory_period(self):
        if self._expiratory_period is None:
            self._expiratory_period = SEScalarTime()
        return self._expiratory_period

    def has_inspiratory_experiatory_ratioy(self):
        return False if self._inspiratory_experiatory_ratio is None else self._inspiratory_experiatory_ratio.is_valid()
    def get_inspiratory_experiatory_ratio(self):
        if self._inspiratory_experiatory_ratio is None:
            self._inspiratory_experiatory_ratio = SEScalar()
        return self._inspiratory_experiatory_ratio

    def has_inspriatory_period(self):
        return False if self._inspriatory_period is None else self._inspriatory_period.is_valid()
    def get_inspriatory_period(self):
        if self._inspriatory_period is None:
            self._inspriatory_period = SEScalarTime()
        return self._inspriatory_period

    def has_peak_inspriatory_pressure(self):
        return False if self._peak_inspriatory_pressure is None else self._peak_inspriatory_pressure.is_valid()
    def get_peak_inspriatory_pressure(self):
        if self._peak_inspriatory_pressure is None:
            self._peak_inspriatory_pressure = SEScalarPressure()
        return self._peak_inspriatory_pressure

    def has_positive_end_expired_pressure(self):
        return False if self._positive_end_expired_pressure is None else self._positive_end_expired_pressure.is_valid()
    def get_positive_end_expired_pressure(self):
        if self._positive_end_expired_pressure is None:
            self._positive_end_expired_pressure = SEScalarPressure()
        return self._positive_end_expired_pressure

    def has_respiration_rate(self):
        return False if self._respiration_rate is None else self._respiration_rate.is_valid()
    def get_respiration_rate(self):
        if self._respiration_rate is None:
            self._respiration_rate = SEScalarFrequency()
        return self._respiration_rate

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

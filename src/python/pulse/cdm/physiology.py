# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.curve import SECurve
from pulse.cdm.engine import eSwitch
from pulse.cdm.scalars import SEScalarPressureTimePerVolume, SEScalarPressure, SEScalarTime

class SERespiratoryMechanics():
    __slots__ = ["_active",
                 "_left_compliance_curve",
                 "_right_compliance_curve",
                 "_left_expiratory_resistance",
                 "_left_inspiratory_resistance",
                 "_right_expiratory_resistance",
                 "_right_inspiratory_resistance",
                 "_upper_expiratory_resistance",
                 "_upper_inspiratory_resistance",
                 "_inspiratory_peak_pressure",
                 "_expiratory_peak_pressure",
                 "_inspiratory_rise_time",
                 "_inspiratory_hold_time",
                 "_inspiratory_release_time",
                 "_inspiratory_to_expiratory_pause_time",
                 "_expiratory_rise_time",
                 "_expiratory_hold_time",
                 "_expiratory_release_time",
                 "_residue_time"]

    def __init__(self):
        self._active = eSwitch.NullSwitch
        self._left_compliance_curve = None
        self._right_compliance_curve = None
        self._left_expiratory_resistance = None
        self._left_inspiratory_resistance = None
        self._right_expiratory_resistance = None
        self._right_inspiratory_resistance = None
        self._upper_expiratory_resistance = None
        self._upper_inspiratory_resistance = None
        self._inspiratory_peak_pressure = None
        self._expiratory_peak_pressure = None
        self._inspiratory_rise_time = None
        self._inspiratory_hold_time = None
        self._inspiratory_release_time = None
        self._inspiratory_to_expiratory_pause_time = None
        self._expiratory_rise_time = None
        self._expiratory_hold_time = None
        self._expiratory_release_time = None
        self._residue_time = None

    def clear(self):
        self._active = eSwitch.NullSwitch
        if self._left_compliance_curve is not None: self._left_compliance_curve.invalidate()
        if self._right_compliance_curve is not None: self._right_compliance_curve.invalidate()
        if self._left_expiratory_resistance is not None: self._left_expiratory_resistance.invalidate()
        if self._left_inspiratory_resistance is not None: self._left_inspiratory_resistance.invalidate()
        if self._right_expiratory_resistance is not None: self._right_expiratory_resistance.invalidate()
        if self._right_inspiratory_resistance is not None: self._right_inspiratory_resistance.invalidate()
        if self._upper_expiratory_resistance is not None: self._upper_expiratory_resistance.invalidate()
        if self._upper_inspiratory_resistance is not None: self._upper_inspiratory_resistance.invalidate()
        if self._inspiratory_peak_pressure is not None: self._inspiratory_peak_pressure.invalidate()
        if self._expiratory_peak_pressure is not None: self._expiratory_peak_pressure.invalidate()
        if self._inspiratory_rise_time is not None: self._inspiratory_rise_time.invalidate()
        if self._inspiratory_hold_time is not None: self._inspiratory_hold_time.invalidate()
        if self._inspiratory_release_time is not None: self._inspiratory_release_time.invalidate()
        if self._inspiratory_to_expiratory_pause_time is not None: self._inspiratory_to_expiratory_pause_time.invalidate()
        if self._expiratory_rise_time is not None: self._expiratory_rise_time.invalidate()
        if self._expiratory_hold_time is not None: self._expiratory_hold_time.invalidate()
        if self._expiratory_release_time is not None: self._expiratory_release_time.invalidate()
        if self._residue_time is not None: self._residue_time.invalidate()

    def copy(self, src):
        if not isinstance(SERespiratoryMechanics, src):
            raise Exception("Provided argument must be a SERespiratoryMechanics")
        self.clear()
        self._active = src._active
        if src.has_left_compliance_curve() is not None: self.get_left_compliance_curve().set(src._left_compliance_curve)
        if src.has_right_compliance_curve() is not None: self.get_right_compliance_curve().set(src._right_compliance_curve)
        if src.has_left_expiratory_resistance() is not None: self.get_left_expiratory_resistance.set(src._left_expiratory_resistance)
        if src.has_left_inspiratory_resistance() is not None: self.get_left_inspiratory_resistance.set(src._left_inspiratory_resistance)
        if src.has_right_expiratory_resistance() is not None: self.get_right_expiratory_resistance.set(src._right_expiratory_resistance)
        if src.has_right_inspiratory_resistance() is not None: self.get_right_inspiratory_resistance.set(src._right_inspiratory_resistance)
        if src.has_upper_expiratory_resistance() is not None: self.get_upper_expiratory_resistance.set(src._upper_expiratory_resistance)
        if src.has_upper_inspiratory_resistance() is not None: self.get_upper_inspiratory_resistance.set(src._upper_inspiratory_resistance)
        if src.has_inspiratory_peak_pressure() is not None: self.get_inspiratory_peak_pressure.set(src._inspiratory_peak_pressure)
        if src.has_expiratory_peak_pressure() is not None: self.get_expiratory_peak_pressure.set(src._expiratory_peak_pressure)
        if src.has_inspiratory_rise_time() is not None: self.get_inspiratory_rise_time.set(src._inspiratory_rise_time)
        if src.has_inspiratory_hold_time() is not None: self.get_inspiratory_hold_time.set(src._inspiratory_hold_time)
        if src.has_inspiratory_release_time() is not None: self.get_inspiratory_release_time.set(src._inspiratory_release_time)
        if src.has_inspiratory_to_expiratory_pause_time() is not None: self.get_inspiratory_to_expiratory_pause_time.set(src._inspiratory_to_expiratory_pause_time)
        if src.has_expiratory_rise_time() is not None: self.get_expiratory_rise_time.set(src._expiratory_rise_time)
        if src.has_expiratory_hold_time() is not None: self.get_expiratory_hold_time.set(src._expiratory_hold_time)
        if src.has_expiratory_release_time() is not None: self.get_expiratory_release_time.set(src._expiratory_release_time)
        if src.has_residue_time() is not None: self.get_residue_time.set(src._residue_time)

    def get_active(self):
        return self._active
    def set_active(self, src : eSwitch):
        self._active = src

    def has_left_compliance_curve(self):
        return False if self._left_compliance_curve is None else self._left_compliance_curve.is_valid()
    def get_left_compliance_curve(self):
        if self._left_compliance_curve is None:
            self._left_compliance_curve = SECurve()
        return self._left_compliance_curve

    def has_right_compliance_curve(self):
        return False if self._right_compliance_curve is None else self._right_compliance_curve.is_valid()
    def get_right_compliance_curve(self):
        if self._right_compliance_curve is None:
            self._right_compliance_curve = SECurve()
        return self._right_compliance_curve

    def has_left_expiratory_resistance(self):
        return False if self._left_expiratory_resistance is None else self._left_expiratory_resistance.is_valid()
    def get_left_expiratory_resistance(self):
        if self._left_expiratory_resistance is None:
            self._left_expiratory_resistance = SEScalarPressureTimePerVolume()
        return self._left_expiratory_resistance

    def has_left_inspiratory_resistance(self):
        return False if self._left_inspiratory_resistance is None else self._left_inspiratory_resistance.is_valid()
    def get_left_inspiratory_resistance(self):
        if self._left_inspiratory_resistance is None:
            self._left_inspiratory_resistance = SEScalarPressureTimePerVolume()
        return self._left_inspiratory_resistance

    def has_right_expiratory_resistance(self):
        return False if self._right_expiratory_resistance is None else self._right_expiratory_resistance.is_valid()
    def get_right_expiratory_resistance(self):
        if self._right_expiratory_resistance is None:
            self._right_expiratory_resistance = SEScalarPressureTimePerVolume()
        return self._right_expiratory_resistance

    def has_right_inspiratory_resistance(self):
        return False if self._right_inspiratory_resistance is None else self._right_inspiratory_resistance.is_valid()
    def get_right_inspiratory_resistance(self):
        if self._right_inspiratory_resistance is None:
            self._right_inspiratory_resistance = SEScalarPressureTimePerVolume()
        return self._right_inspiratory_resistance

    def has_upper_expiratory_resistance(self):
        return False if self._upper_expiratory_resistance is None else self._upper_expiratory_resistance.is_valid()
    def get_upper_expiratory_resistance(self):
        if self._upper_expiratory_resistance is None:
            self._upper_expiratory_resistance = SEScalarPressureTimePerVolume()
        return self._upper_expiratory_resistance

    def has_upper_inspiratory_resistance(self):
        return False if self._upper_inspiratory_resistance is None else self._upper_inspiratory_resistance.is_valid()
    def get_upper_inspiratory_resistance(self):
        if self._upper_inspiratory_resistance is None:
            self._upper_inspiratory_resistance = SEScalarPressureTimePerVolume()
        return self._upper_inspiratory_resistance

    def has_inspiratory_peak_pressure(self):
        return False if self._inspiratory_peak_pressure is None else self._inspiratory_peak_pressure.is_valid()
    def get_inspiratory_peak_pressure(self):
        if self._inspiratory_peak_pressure is None:
            self._inspiratory_peak_pressure = SEScalarPressure()
        return self._inspiratory_peak_pressure

    def has_expiratory_peak_pressure(self):
        return False if self._expiratory_peak_pressure is None else self._expiratory_peak_pressure.is_valid()
    def get_expiratory_peak_pressure(self):
        if self._expiratory_peak_pressure is None:
            self._expiratory_peak_pressure = SEScalarPressure()
        return self._expiratory_peak_pressure

    def has_inspiratory_rise_time(self):
        return False if self._inspiratory_rise_time is None else self._inspiratory_rise_time.is_valid()
    def get_inspiratory_rise_time(self):
        if self._inspiratory_rise_time is None:
            self._inspiratory_rise_time = SEScalarTime()
        return self._inspiratory_rise_time

    def has_inspiratory_hold_time(self):
        return False if self._inspiratory_hold_time is None else self._inspiratory_hold_time.is_valid()
    def get_inspiratory_hold_time(self):
        if self._inspiratory_hold_time is None:
            self._inspiratory_hold_time = SEScalarTime()
        return self._inspiratory_hold_time

    def has_inspiratory_release_time(self):
        return False if self._inspiratory_release_time is None else self._inspiratory_release_time.is_valid()
    def get_inspiratory_release_time(self):
        if self._inspiratory_release_time is None:
            self._inspiratory_release_time = SEScalarTime()
        return self._inspiratory_release_time

    def has_inspiratory_to_expiratory_pause_time(self):
        return False if self._inspiratory_to_expiratory_pause_time is None else self._inspiratory_to_expiratory_pause_time.is_valid()
    def get_inspiratory_to_expiratory_pause_time(self):
        if self._inspiratory_to_expiratory_pause_time is None:
            self._inspiratory_to_expiratory_pause_time = SEScalarTime()
        return self._inspiratory_to_expiratory_pause_time

    def has_expiratory_rise_time(self):
        return False if self._expiratory_rise_time is None else self._expiratory_rise_time.is_valid()
    def get_expiratory_rise_time(self):
        if self._expiratory_rise_time is None:
            self._expiratory_rise_time = SEScalarTime()
        return self._expiratory_rise_time

    def has_expiratory_hold_time(self):
        return False if self._expiratory_hold_time is None else self._expiratory_hold_time.is_valid()
    def get_expiratory_hold_time(self):
        if self._expiratory_hold_time is None:
            self._expiratory_hold_time = SEScalarTime()
        return self._expiratory_hold_time

    def has_expiratory_release_time(self):
        return False if self._expiratory_release_time is None else self._expiratory_release_time.is_valid()
    def get_expiratory_release_time(self):
        if self._expiratory_release_time is None:
            self._expiratory_release_time = SEScalarTime()
        return self._expiratory_release_time

    def has_residue_time(self):
        return False if self._residue_time is None else self._residue_time.is_valid()
    def get_residue_time(self):
        if self._residue_time is None:
            self._residue_time = SEScalarTime()
        return self._residue_time

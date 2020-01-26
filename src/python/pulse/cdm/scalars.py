# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

class SEScalar():
    __slots__ = ["_value"]

    def __init__(self, value=None):
        if value is None:
            self._value = None
        else:
            self._value = value

    def set(self, scalar):
        if isinstance(SEScalar, scalar):
            self._value = scalar._value
        else:
            raise Exception("Provided argument must be a SEScalar")

    def is_valid(self):
        return self._value is not None

    def get_value(self):
        return self._value
    def set_value(self, value: float):
        self._value = value
    def invalidate(self):
        self._value = None

    def __repr__(self):
        return " {} """.format(self._value)

class ScalarUnit():
    __slots__ = ["_unit"]

    def __init__(self, unit: str):
        self._unit = unit

    def unit(self):
        return self._unit

    def __repr__(self):
        return "({})".format(self._unit)


class VolumePerTimeUnit(ScalarUnit):
    pass
VolumePerTimeUnit.L_Per_s = VolumePerTimeUnit("L/s")
VolumePerTimeUnit.mL_Per_s = VolumePerTimeUnit("mL/s")
VolumePerTimeUnit.mL_Per_min = VolumePerTimeUnit("mL/min")
VolumePerTimeUnit.L_Per_min = VolumePerTimeUnit("L/min")
VolumePerTimeUnit.m3_Per_s = VolumePerTimeUnit("m^3/s")
VolumePerTimeUnit.mL_Per_min = VolumePerTimeUnit("mL/min")
VolumePerTimeUnit.mL_Per_day = VolumePerTimeUnit("mL/day")
class SEScalarVolumePerTime(SEScalar):
    __slots__ = ["_units"]

    def __init__(self, value=None, units=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarVolumePerTime, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalar")

    def is_valid(self):
        return self._value is not None and self._units is not None

    def get_value(self, units: VolumePerTimeUnit):
        if units is None:
            raise Exception("Must provide a unit")
        if self.is_valid() is False:
            return None
        if self._units.unit() is units.unit():
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: VolumePerTimeUnit):
        if isinstance(units, VolumePerTimeUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a VolumePerTimeUnit")

    def invalidate(self):
        self._value = None
        self._units = None

    def __repr__(self):
        return "{}{}".format(self._value, self._units)
# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

class SEScalar():
    __slots__ = ["_value"]

    def __init__(self, value: float=None):
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

class SEScalar0To1(SEScalar):
    def set_value(self, value: float):
        if value>1 or value<0:
            raise Exception("Scalar0To1 must be [0,1]")
        self._value = value

class SEScalarNegative1To1(SEScalar):
    def set_value(self, value: float):
        if value>1 or value<-1:
            raise Exception("ScalarNegative1To1 must be [-1,1]")
        self._value = value

class SEScalarQuantity(SEScalar):
    __slots__ = ["_units"]

    def is_valid(self):
        return self._value is not None and self._units is not None

    def get_unit(self):
        return self._units

    def invalidate(self):
        self._value = None
        self._units = None

    def __repr__(self):
        return "{}{}".format(self._value, self._units)

class SEScalarUnit():
    __slots__ = ["_string"]

    def __init__(self, string: str):
        self._string = string

    def get_string(self):
        return self._string

    def __repr__(self):
        return "({})".format(self._string)

class AreaUnit(SEScalarUnit):
    pass
AreaUnit.cm2 = AreaUnit("cm^2")
AreaUnit.m2 = AreaUnit("m^2")
class SEScalarArea(SEScalarQuantity):

    def __init__(self, value:float=None, units:AreaUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarArea, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalar")

    def get_value(self, units: AreaUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units.get_string() is units.get_string():
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: AreaUnit):
        if isinstance(units, AreaUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a AreaUnit")

class FrequencyUnit(SEScalarUnit):
    pass
FrequencyUnit.Per_min = FrequencyUnit("1/min")
FrequencyUnit.Per_s = FrequencyUnit("1/s")
FrequencyUnit.Hz = FrequencyUnit("Hz")
class SEScalarFrequency(SEScalarQuantity):

    def __init__(self, value:float=None, units:FrequencyUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarFrequency, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalar")

    def get_value(self, units: FrequencyUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units.get_string() is units.get_string():
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: FrequencyUnit):
        if isinstance(units, FrequencyUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a FrequencyUnit")

class InversePressureUnit(SEScalarUnit):
    pass
InversePressureUnit.Inverse_Pa = InversePressureUnit("1/Pa")
InversePressureUnit.Inverse_mmHg = InversePressureUnit("1/mmHg")
InversePressureUnit.Inverse_cmH2O = InversePressureUnit("1/cmH2O")
InversePressureUnit.Inverse_atm = InversePressureUnit("1/atm")
class SEScalarInversePressure(SEScalarQuantity):

    def __init__(self, value:float=None, units:InversePressureUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarInversePressure, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalar")

    def get_value(self, units: InversePressureUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units.get_string() is units.get_string():
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: InversePressureUnit):
        if isinstance(units, InversePressureUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a InversePressureUnit")

class LengthUnit(SEScalarUnit):
    pass
LengthUnit.m = LengthUnit("m")
LengthUnit.cm = LengthUnit("cm")
LengthUnit.mm = LengthUnit("mm")
LengthUnit.um = LengthUnit("um")
LengthUnit.inch = LengthUnit("inch")
LengthUnit.ft = LengthUnit("ft")
class SEScalarLength(SEScalarQuantity):

    def __init__(self, value:float=None, units:LengthUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarLength, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalar")

    def get_value(self, units: LengthUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units.get_string() is units.get_string():
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: LengthUnit):
        if isinstance(units, LengthUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a LengthUnit")

class MassUnit(SEScalarUnit):
    pass
MassUnit.g = MassUnit("g")
MassUnit.ug = MassUnit("ug")
MassUnit.mg = MassUnit("mg")
MassUnit.kg = MassUnit("kg")
MassUnit.lb = MassUnit("lb")
class SEScalarMass(SEScalarQuantity):

    def __init__(self, value:float=None, units:MassUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarMass, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalar")

    def get_value(self, units: MassUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units.get_string() is units.get_string():
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: MassUnit):
        if isinstance(units, MassUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a MassUnit")

class MassPerAmountUnit(SEScalarUnit):
    pass
MassPerAmountUnit.g_Per_ct = MassPerAmountUnit("g/ct")
MassPerAmountUnit.g_Per_mol = MassPerAmountUnit("g/mol")
MassPerAmountUnit.g_Per_mmol = MassPerAmountUnit("g/mmol")
MassPerAmountUnit.g_Per_umol = MassPerAmountUnit("g/umol")
MassPerAmountUnit.mg_Per_mol = MassPerAmountUnit("mg/mol")
MassPerAmountUnit.mg_Per_mmol = MassPerAmountUnit("mg/mmol")
MassPerAmountUnit.kg_Per_mol = MassPerAmountUnit("kg/mol")
MassPerAmountUnit.ug_Per_mmol = MassPerAmountUnit("ug/mmol")
MassPerAmountUnit.ug_Per_mol = MassPerAmountUnit("ug/mol")
MassPerAmountUnit.ug_Per_ct = MassPerAmountUnit("ug/ct")
MassPerAmountUnit.pg_Per_ct = MassPerAmountUnit("pg/ct")
class SEScalarMassPerAmount(SEScalarQuantity):

    def __init__(self, value:float=None, units:MassPerAmountUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarMassPerAmount, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalar")

    def get_value(self, units: MassPerAmountUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units.get_string() is units.get_string():
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: MassPerAmountUnit):
        if isinstance(units, MassPerAmountUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a MassPerAmountUnit")

class MassPerAreaTimeUnit(SEScalarUnit):
    pass
MassPerAreaTimeUnit.g_Per_cm2_s = MassPerAreaTimeUnit("g/cm^2 s")
class SEScalarMassPerAreaTime(SEScalarQuantity):

    def __init__(self, value:float=None, units:MassPerAreaTimeUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarMassPerAreaTime, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalar")

    def get_value(self, units: MassPerAreaTimeUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units.get_string() is units.get_string():
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: MassPerAreaTimeUnit):
        if isinstance(units, MassPerAreaTimeUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a MassPerAreaTimeUnit")

class MassPerTimeUnit(SEScalarUnit):
    pass
MassPerTimeUnit.g_Per_s = MassPerTimeUnit("g/s")
MassPerTimeUnit.g_Per_min = MassPerTimeUnit("g/min")
MassPerTimeUnit.g_Per_day = MassPerTimeUnit("g/day")
MassPerTimeUnit.mg_Per_s = MassPerTimeUnit("mg/s")
MassPerTimeUnit.mg_Per_min = MassPerTimeUnit("mg/min")
MassPerTimeUnit.ug_Per_s = MassPerTimeUnit("ug/s")
MassPerTimeUnit.kg_Per_s = MassPerTimeUnit("kg/s")
MassPerTimeUnit.ug_Per_min = MassPerTimeUnit("ug/min")
class SEScalarMassPerTime(SEScalarQuantity):

    def __init__(self, value:float=None, units:MassPerTimeUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarMassPerTime, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalar")

    def get_value(self, units: MassPerTimeUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units.get_string() is units.get_string():
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: MassPerTimeUnit):
        if isinstance(units, MassPerTimeUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a MassPerTimeUnit")

class MassPerVolumeUnit(SEScalarUnit):
    pass
MassPerVolumeUnit.g_Per_dL = MassPerVolumeUnit("g/dL")
MassPerVolumeUnit.g_Per_cm3 = MassPerVolumeUnit("g/cm^3")
MassPerVolumeUnit.g_Per_m3 = MassPerVolumeUnit("g/m^3")
MassPerVolumeUnit.ug_Per_mL = MassPerVolumeUnit("ug/mL")
MassPerVolumeUnit.mg_Per_m3 = MassPerVolumeUnit("mg/m^3")
MassPerVolumeUnit.kg_Per_m3 = MassPerVolumeUnit("kg/m^3")
MassPerVolumeUnit.ug_Per_L = MassPerVolumeUnit("ug/L")
MassPerVolumeUnit.g_Per_L = MassPerVolumeUnit("g/L")
MassPerVolumeUnit.g_Per_mL = MassPerVolumeUnit("g/mL")
MassPerVolumeUnit.mg_Per_mL = MassPerVolumeUnit("mg/mL")
MassPerVolumeUnit.mg_Per_L = MassPerVolumeUnit("mg/L")
MassPerVolumeUnit.mg_Per_dL = MassPerVolumeUnit("mg/dL")
MassPerVolumeUnit.kg_Per_mL = MassPerVolumeUnit("kg/mL")
MassPerVolumeUnit.kg_Per_L = MassPerVolumeUnit("kg/L")
class SEScalarMassPerVolume(SEScalarQuantity):

    def __init__(self, value:float=None, units:MassPerVolumeUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarMassPerVolume, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalar")

    def get_value(self, units: MassPerVolumeUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units.get_string() is units.get_string():
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: MassPerVolumeUnit):
        if isinstance(units, MassPerVolumeUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a MassPerVolumeUnit")

class PowerUnit(SEScalarUnit):
    pass
PowerUnit.W = PowerUnit("W")
PowerUnit.kcal_Per_s = PowerUnit("kcal_Per_s")
PowerUnit.kcal_Per_hr = PowerUnit("kcal_Per_hr")
PowerUnit.kcal_Per_day = PowerUnit("kcal_Per_day")
PowerUnit.J_Per_s = PowerUnit("J_Per_s")
PowerUnit.BTU_Per_hr = PowerUnit("BTU_Per_hr")
class SEScalarPower(SEScalarQuantity):

    def __init__(self, value:float=None, units:PowerUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarPower, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalar")

    def get_value(self, units: PowerUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units.get_string() is units.get_string():
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: PowerUnit):
        if isinstance(units, PowerUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a PowerUnit")

class PressureUnit(SEScalarUnit):
    pass
PressureUnit.Pa = PressureUnit("Pa")
PressureUnit.mmHg = PressureUnit("mmHg")
PressureUnit.cmH2O = PressureUnit("cmH2O")
PressureUnit.psi = PressureUnit("psi")
PressureUnit.atm = PressureUnit("atm")
class SEScalarPressure(SEScalarQuantity):

    def __init__(self, value:float=None, units:PressureUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarPressure, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalar")

    def get_value(self, units: PressureUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units.get_string() is units.get_string():
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: PressureUnit):
        if isinstance(units, PressureUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a PressureUnit")

class TimeUnit(SEScalarUnit):
    pass
TimeUnit.s = TimeUnit("s")
TimeUnit.min = TimeUnit("min")
TimeUnit.hr = TimeUnit("hr")
TimeUnit.day = TimeUnit("day")
TimeUnit.yr = TimeUnit("yr")
class SEScalarTime(SEScalarQuantity):

    def __init__(self, value:float=None, units:TimeUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarTime, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalar")

    def get_value(self, units: TimeUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units.get_string() is units.get_string():
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: TimeUnit):
        if isinstance(units, TimeUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a TimeUnit")

class VolumeUnit(SEScalarUnit):
    pass
VolumeUnit.L = VolumeUnit("L")
VolumeUnit.mL = VolumeUnit("mL")
VolumeUnit.dL = VolumeUnit("dL")
VolumeUnit.m3 = VolumeUnit("m^3")
class SEScalarVolume(SEScalarQuantity):

    def __init__(self, value:float=None, units:VolumeUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarVolume, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalar")

    def get_value(self, units: VolumeUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units.get_string() is units.get_string():
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: VolumeUnit):
        if isinstance(units, VolumeUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a VolumeUnit")

class VolumePerTimeUnit(SEScalarUnit):
    pass
VolumePerTimeUnit.L_Per_s = VolumePerTimeUnit("L/s")
VolumePerTimeUnit.mL_Per_s = VolumePerTimeUnit("mL/s")
VolumePerTimeUnit.mL_Per_min = VolumePerTimeUnit("mL/min")
VolumePerTimeUnit.L_Per_min = VolumePerTimeUnit("L/min")
VolumePerTimeUnit.m3_Per_s = VolumePerTimeUnit("m^3/s")
VolumePerTimeUnit.mL_Per_min = VolumePerTimeUnit("mL/min")
VolumePerTimeUnit.mL_Per_day = VolumePerTimeUnit("mL/day")
class SEScalarVolumePerTime(SEScalarQuantity):

    def __init__(self, value:float=None, units:VolumePerTimeUnit=None):
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

    def get_value(self, units: VolumePerTimeUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units.get_string() is units.get_string():
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: VolumePerTimeUnit):
        if isinstance(units, VolumePerTimeUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a VolumePerTimeUnit")

class VolumePerTimeMassUnit(SEScalarUnit):
    pass
VolumePerTimeMassUnit.L_Per_s_g = VolumePerTimeMassUnit("L/s g")
VolumePerTimeMassUnit.mL_Per_s_g = VolumePerTimeMassUnit("mL/s g")
VolumePerTimeMassUnit.mL_Per_min_kg = VolumePerTimeMassUnit("mL/min kg")
VolumePerTimeMassUnit.mL_Per_s_kg = VolumePerTimeMassUnit("mL/s kg")
VolumePerTimeMassUnit.uL_Per_min_kg = VolumePerTimeMassUnit("uL/min kg")
class SEScalarVolumePerTimeMass(SEScalarQuantity):

    def __init__(self, value:float=None, units:VolumePerTimeMassUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarVolumePerTimeMass, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalar")

    def get_value(self, units: VolumePerTimeMassUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units.get_string() is units.get_string():
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: VolumePerTimeMassUnit):
        if isinstance(units, VolumePerTimeMassUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a VolumePerTimeMassUnit")

class VolumePerTimePressureUnit(SEScalarUnit):
    pass
VolumePerTimePressureUnit.L_Per_s_mmHg = VolumePerTimePressureUnit("L/s mmHg")
VolumePerTimePressureUnit.mL_Per_s_mmHg = VolumePerTimePressureUnit("mL/s mmHg")
VolumePerTimePressureUnit.L_Per_min_mmHg = VolumePerTimePressureUnit("L/min mmHg")
VolumePerTimePressureUnit.mL_Per_min_mmHg = VolumePerTimePressureUnit("mL/min mmHg")
class SEScalarVolumePerTimePressure(SEScalarQuantity):

    def __init__(self, value:float=None, units:VolumePerTimePressureUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarVolumePerTimePressure, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalar")

    def get_value(self, units: VolumePerTimePressureUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units.get_string() is units.get_string():
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: VolumePerTimePressureUnit):
        if isinstance(units, VolumePerTimePressureUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a VolumePerTimePressureUnit")
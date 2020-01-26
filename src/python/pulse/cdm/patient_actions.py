# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from enum import Enum
from pulse.cdm.engine import SEAction
from pulse.cdm.scalars import SEScalarVolumePerTime

class SEPatientAction(SEAction):
    pass

class eHemorrhageType(Enum):
    External = 0
    Internal = 1
class SEHemorrhage(SEPatientAction):
    __slots__ = ["_compartment","_rate","_type"]

    def __init__(self):
        self._compartment = None
        self._rate = None
        self._type = eHemorrhageType.External

    def clear(self):
        super().clear()
        self._compartment = None
        if self._rate is not None:
            self._rate.invalidate()
        self._type = eHemorrhageType.External

    def is_valid(self):
        return self.has_rate() and self.has_compartment()

    def has_compartment(self):
        return self._compartment is not None
    def _get_compartment(self):
        return self._compartment
    def set_compartment(self, compartment: str):
        self._compartment = compartment
    def invalidate_compartment(self):
        self._compartment = None

    def has_rate(self):
        return False if self._rate is None else self._rate.is_valid()
    def get_rate(self):
        if self._rate is None:
            self._rate = SEScalarVolumePerTime()
        return self._rate

    def get_type(self):
        return self._type
    def set_type(self, type: eHemorrhageType):
        self._type = type

    def __repr__(self):
        return ("Hemorrhage\n"
                "  Type: {}\n"
                "  Compartment: {}\n"
                "  Rate: {}\n").format(self._type,self._compartment,self._rate)



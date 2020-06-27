# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from pulse.cdm.scalars import SEScalar0To1, SEScalarMassPerVolume

class SESubstanceFraction():
    __slots__ = ["_substance", "_fraction_amount"]

    def __init__(self):
        self._substance = None
        self._fraction_amount = None

    def clear(self):
        self._substance = None
        if self._fraction_amount is not None: self._fraction_amount.invalidate()

    def get_substance(self):
        return self._substance
    def set_substance(self, s: str):
        self._substance = s

    def has_fraction_amount(self):
        return False if self._fraction_amount is None else self._fraction_amount.is_valid()
    def get_fraction_amount(self):
        if self._fraction_amount is None:
            self._fraction_amount = SEScalar0To1()
        return self._fraction_amount

class SESubstanceConcentration():
    __slots__ = ["_substance", "_concentration"]

    def __init__(self):
        self._substance = None
        self._concentration = None

    def clear(self):
        self._substance = None
        if self._concentration is not None: self._concentration.invalidate()

    def get_substance(self):
        return self._substance
    def set_substance(self, s: str):
        self._substance = s

    def has_concentration(self):
        return False if self._concentration is None else self._concentration.is_valid()
    def get_concentration(self):
        if self._concentration is None:
            self._concentration = SEScalarMassPerVolume()
        return self._concentration
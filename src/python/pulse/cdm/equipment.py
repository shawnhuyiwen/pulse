# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from enum import Enum

class SEEquipment():
    __slots__ = []
    def __init__(self):
        pass
    def copy(self, equipment):
        self.clear()
    def clear(self):
        pass

class eBreathState(Enum):
    NoBreath=0
    PatientInhale=1
    PatientPause=2
    PatientExhale=3
    EquipmentInhale=4
    EquipmentPause=5
    EquipmentExhale=6
    ExpiratoryHold=7
    InspiratoryHold=8

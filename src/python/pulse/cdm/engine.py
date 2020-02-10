# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from abc import ABC, abstractmethod
from enum import Enum

class eSerializationFormat(Enum):
    BINARY = 0
    JSON = 1

class eSide(Enum):
    NullSide = 0
    Left = 1
    Right = 2

class eGate(Enum):
    NullGate = 0
    Open = 1
    Closed = 2

class eSwitch(Enum):
    NullSwitch = 0
    Off = 1
    On = 2

class eCharge(Enum):
    NullCharge = 0
    Negative = 1
    Neutral = 2
    Positive = 3

class SEAction(ABC):
    __slots__ = ["_comment"]

    def __init__(self):
        self._comment = None

    def copy(self, action):
        self.clear()
        self._comment = action._comment

    def clear(self):
        self.invalidate_comment()

    def has_comment(self):
        return self._comment is not None
    def get_comment(self):
        return self._comment
    def set_comment(self, comment: str):
        self._comment = comment
    def invalidate_comment(self):
        self._comment = None

    @abstractmethod
    def is_valid(self):
        pass

class SECondition(ABC):
    __slots__ = ["_comment"]

    def __init__(self):
        self._comment = None

    def copy(self, condition):
        self.clear()
        self._comment = condition._comment

    def clear(self):
        self.invalidate_comment()

    @abstractmethod
    def get_name(self):
        pass

    def has_comment(self):
        return self._comment is not None
    def get_comment(self):
        return self._comment
    def set_comment(self, comment: str):
        self._comment = comment
    def invalidate_comment(self):
        self._comment = None

    @abstractmethod
    def is_valid(self):
        pass

    @abstractmethod
    def is_active(self):
        pass

from pulse.cdm.environment_conditions import SEInitialEnvironmentalConditions

class SEConditionManager():
    __slots__ = ["_ards", "_anemia", "_copd", "_cvsd", "_impaired_alveolar_exchange",
                 "_pericardial_effusion", "_lobar_pneumonia",
                 "_pulmonary_fibrosis", "_renal_stenosis", "_sepsis",
                 "_initial_environmental_conditions"]

    def __init__(self):
        self.clear()

    def clear(self):
        self._ards = None
        self._anemia = None
        self._copd = None
        self._cvsd = None
        self._impaired_alveolar_exchange = None
        self._pericardial_effusion = None
        self._lobar_pneumonia = None
        self._pulmonary_fibrosis = None
        self._renal_stenosis = None
        self._sepsis = None
        self._initial_environmental_conditions = None

    def is_empty(self):
        if self.has_acute_respiratory_distress_syndrome():
            return False
        if self.has_chronic_anemia():
            return False
        if self.has_chronic_obstructive_pulmonary_disease():
            return False
        if self.has_chronic_ventricular_systolic_dysfunction():
            return False
        if self.has_chronic_pericardial_effusion():
            return False
        if self.has_chronic_renal_stenosis():
            return False
        if self.has_impaired_alveolar_exchange():
            return False
        if self.has_lobar_pneumonia():
            return False
        if self.has_pulmonary_fibrosis():
            return False
        if self.has_sepsis():
            return False

        if self.has_initial_environmental_conditions():
            return False;
        return True;
    
    def has_acute_respiratory_distress_syndrome(self):
        return False if self._ards is None else self._ards.is_valid()
    def get_acute_respiratory_distress_syndrome(self):
        if self._ards is None:
            self._ards = None
        return self._ards
    def remove_acute_respiratory_distress_syndrome(self):
        self._ards = None

    def has_chronic_anemia(self):
        return False if self._anemia is None else self._anemia.is_valid()
    def get_chronic_anemia(self):
        if self._anemia is None:
            self._anemia = None
        return self._anemia
    def remove_chronic_anemia(self):
        self._anemia = None

    def has_chronic_obstructive_pulmonary_disease(self):
        return False if self._copd is None else self._copd.is_valid()
    def get_chronic_obstructive_pulmonary_disease(self):
        if self._copd is None:
            self._copd = None
        return self._copd
    def remove_chronic_obstructive_pulmonary_disease(self):
        self._copd = None

    def has_chronic_ventricular_systolic_dysfunction(self):
        return False if self._cvsd is None else self._cvsd.is_valid()
    def get_chronic_ventricular_systolic_dysfunction(self):
        if self._cvsd is None:
            self._cvsd = None
        return self._cvsd
    def remove_chronic_ventricular_systolic_dysfunction(self):
        self._cvsd = None

    def has_chronic_pericardial_effusion(self):
        return False if self._pericardial_effusion is None else self._pericardial_effusion.is_valid()
    def get_chronic_pericardial_effusion(self):
        if self._pericardial_effusion is None:
            self._pericardial_effusion = None
        return self._pericardial_effusion
    def remove_chronic_pericardial_effusion(self):
        self._pericardial_effusion = None

    def has_chronic_renal_stenosis(self):
        return False if self._renal_stenosis is None else self._renal_stenosis.is_valid()
    def get_chronic_renal_stenosis(self):
        if self._renal_stenosis is None:
            self._renal_stenosis = None
        return self._renal_stenosis
    def remove_chronic_renal_stenosis(self):
        self._renal_stenosis = None

    def has_impaired_alveolar_exchange(self):
        return False if self._impaired_alveolar_exchange is None else self._impaired_alveolar_exchange.is_valid()
    def get_impaired_alveolar_exchange(self):
        if self._impaired_alveolar_exchange is None:
            self._impaired_alveolar_exchange = None
        return self._impaired_alveolar_exchange
    def remove_impaired_alveolar_exchange(self):
        self._impaired_alveolar_exchange = None

    def has_lobar_pneumonia(self):
        return False if self._lobar_pneumonia is None else self._lobar_pneumonia.is_valid()
    def get_lobar_pneumonia(self):
        if self._lobar_pneumonia is None:
            self._lobar_pneumonia = None
        return self._lobar_pneumonia
    def remove_lobar_pneumonia(self):
        self._lobar_pneumonia = None

    def has_pulmonary_fibrosis(self):
        return False if self._pulmonary_fibrosis is None else self._pulmonary_fibrosis.is_valid()
    def get_pulmonary_fibrosis(self):
        if self._pulmonary_fibrosis is None:
            self._pulmonary_fibrosis = None
        return self._pulmonary_fibrosis
    def remove_pulmonary_fibrosis(self):
        self._pulmonary_fibrosis = None

    def has_renal_stenosis(self):
        return False if self._renal_stenosis is None else self._renal_stenosis.is_valid()
    def get_renal_stenosis(self):
        if self._renal_stenosis is None:
            self._renal_stenosis = None
        return self._renal_stenosis
    def remove_renal_stenosis(self):
        self._renal_stenosis = None

    def has_sepsis(self):
        return False if self._sepsis is None else self._sepsis.is_valid()
    def get_sepsis(self):
        if self._sepsis is None:
            self._sepsis = None
        return self._sepsis
    def remove_sepsis(self):
        self._sepsis = None

    def has_initial_environmental_conditions(self):
        return False if self._initial_environmental_conditions is None else self._initial_environmental_conditions.is_valid()
    def get_initial_environmental_conditions(self):
        if self._initial_environmental_conditions is None:
            self._initial_environmental_conditions = SEInitialEnvironmentalConditions()
        return self._initial_environmental_conditions
    def remove_initial_environmental_conditions(self):
        self._initial_environmental_conditions = None


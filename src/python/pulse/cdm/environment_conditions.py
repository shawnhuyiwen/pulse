# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from pulse.cdm.engine import SECondition
from pulse.cdm.environment import SEEnvironmentalConditions

class SEEnvironmentCondition(SECondition):
    def __init__(self):
        super().__init__()

class SEInitialEnvironmentalConditions(SEEnvironmentCondition):
    __slots__ = ["_environmental_conditions_file", "_environmental_conditions"]

    def __init__(self):
        super().__init__()
        self._environmental_conditions = None
        self._environmental_conditions_file = None

    def get_name(self):
        return "InitialEnvironmentalConditions"
    
    def is_valid(self):
        if not self.has_environmental_conditions() and not self.has_environmental_conditions_file():
            return False;
        return True

    def is_active(self):
        return self.is_valid()

    def has_environmental_conditions_file(self):
        return self._environmental_conditions_file is not None
    def get_environmental_conditions_file(self):
        return self._environmental_conditions_file
    def set_environmental_conditions_file(self, file: str):
        self._environmental_conditions_file = file
    def invalidate_environmental_conditions_file(self):
        self._environmental_conditions_file = None

    def has_environmental_conditions(self):
        return self._environmental_conditions is not None
    def get_environmental_conditions(self):
        if self._environmental_conditions is None:
            self._environmental_conditions = SEEnvironmentalConditions()
        return self._environmental_conditions


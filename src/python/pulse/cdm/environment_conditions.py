# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from pulse.cdm.engine import SECondition
from pulse.cdm.environment import SEEnvironmentalConditions

class SEEnvironmentCondition(SECondition):
    pass

class SEInitialEnvironmentalConditions(SEEnvironmentCondition):
    __slots__ = ["_conditions_file", "_conditions"]

    def __init__(self):
        self._conditions = None
        self._conditions_file = None

    def get_name(self):
        return "InitialEnvironment"
    
    def is_valid(self):
        if not self.has_conditions() and not self.has_conditions_file():
            return False;
        return True

    def is_active(self):
        return self.is_valid()

    def has_conditions_file(self):
        return self._conditions_file is not None
    def get_conditions_file(self):
        return self._conditions_file
    def set_conditions_file(self, file: str):
        self._conditions_file = file
    def invalidate_conditions_file(self):
        self._conditions_file = None

    def has_conditions(self):
        return self._conditions is not None
    def get_conditions(self):
        if self._conditions is None:
            self._conditions = SEEnvironmentalConditions()
        return self._conditions


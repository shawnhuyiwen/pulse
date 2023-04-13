# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

class SEFunction():
    __slots__ = ["_dependent", "_independent", "_dependent_unit", "_independent_unit"]

    def __init__(self):
        self.clear()

    def clear(self):
        self._dependent = []
        self._independent = []
        self._dependent_unit = ""
        self._independent_unit = ""

    def get_dependent(self):
        return self._dependent

    def get_independent(self):
        return self._independent

    def get_dependent_unit(self):
        return self._dependent_unit
    def set_dependent_unit(self, unit: str):
        self._dependent_unit = unit

    def get_independent_unit(self):
        return self._independent_unit
    def set_independent_unit(self, unit: str):
        self._independent_unit = unit

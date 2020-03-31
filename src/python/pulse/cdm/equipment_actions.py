# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from pulse.cdm.engine import SEAction
from pulse.cdm.equipment import SEMechanicalVentilator

class SEEquipmentAction(SEAction):
    def __init__(self):
        super().__init__()

class SEMechanicalVentilatorConfiguration(SEEquipmentAction):
    __slots__ = ["_configuration_file",
                 "_configuration"]

    def __init__(self):
        super().__init__()
        self._configuration_file = None
        self._configuration = None

    def clear(self):
        self._configuration_file = None
        self._configuration = None

    def copy(self, src):
        if not isinstance(SEMechanicalVentilatorConfiguration, src):
            raise Exception("Provided argument must be a SEMechanicalVentilatorConfiguration")
        self.clear()
        self._configuration_file = src._configuration_file
        self._configuration.copy(src._configuration)

    def is_valid(self):
        return self.has_configuration() or self.has_configuration_file()

    def is_active(self):
        return True;

    def has_configuration_file(self):
        return self._configuration_file is not None
    def get_configuration_file(self):
        return self._configuration_file
    def set_configuration_file(self, filename: str):
        self._configuration_file = filename

    def has_configuration(self):
        return self._configuration is not None
    def get_configuration(self):
        if self._configuration is None:
            self._configuration = SEMechanicalVentilator()
        return self._configuration
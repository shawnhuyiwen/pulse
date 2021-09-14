# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.equipment_actions import SEEquipmentAction
from pulse.cdm.mechanical_ventilator import SEMechanicalVentilator, eSwitch

class SEMechanicalVentilatorAction(SEEquipmentAction):
    def __init__(self):
        super().__init__()

class SEMechanicalVentilatorConfiguration(SEMechanicalVentilatorAction):
    __slots__ = ["_settings_file",
                 "_settings"]

    def __init__(self):
        super().__init__()
        self._settings_file = None
        self._settings = None

    def clear(self):
        self._settings_file = None
        self._settings = None

    def copy(self, src):
        if not isinstance(SEMechanicalVentilatorConfiguration, src):
            raise Exception("Provided argument must be a SEMechanicalVentilatorConfiguration")
        self.clear()
        self._settings_file = src._settings_file
        self._settings.copy(src._settings)

    def is_valid(self):
        return self.has_settings() or self.has_settings_file()

    def is_active(self):
        return True

    def has_settings_file(self):
        return self._settings_file is not None
    def get_settings_file(self):
        return self._settings_file
    def set_settings_file(self, filename: str):
        self._settings_file = filename

    def has_settings(self):
        return self._settings is not None
    def get_settings(self):
        if self._settings is None:
            self._settings = SEMechanicalVentilator()
        return self._settings

class SEMechanicalVentilatorMode(SEMechanicalVentilatorAction):
    __slots__= ["_connection"]
    def __init__(self):
        super().__init__()
        self._connection = eSwitch.NullSwitch

    def clear(self):
        self._connection = eSwitch.NullSwitch

    def copy(self, src):
        if not isinstance(SEMechanicalVentilatorMode, src):
            raise Exception("Provided argument must be a SEMechanicalVentilatorMode")
        self.clear()
        self._connection = src._connection

    def is_valid(self):
        return self.has_connection()

    def is_active(self):
        return True

    def has_connection(self):
        return self._connection is not eSwitch.NullSwitch
    def get_connection(self):
        return self._connection
    def set_connection(self, src_connection: eSwitch):
        self._connection = src_connection

    def __str__(self):
        to_string = "Mechanical Ventilator Mode" + "\n\tConnection: " + str(self._connection)
        return to_string

class SEMechanicalVentilatorContinuousPositiveAirwayPressure(SEMechanicalVentilatorMode):
    pass

class SEMechanicalVentilatorPressureControl(SEMechanicalVentilatorMode):
    pass

class SEMechanicalVentilatorVolumeControl(SEMechanicalVentilatorMode):
    pass

class SEMechanicalVentilatorHold(SEMechanicalVentilatorMode):
    pass

class SEMechanicalVentilatorLeak(SEMechanicalVentilatorMode):
    pass
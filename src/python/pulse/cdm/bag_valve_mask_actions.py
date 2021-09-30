# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.equipment_actions import SEEquipmentAction
from pulse.cdm.bag_valve_mask import SEBagValveMask
from pulse.cdm.scalars import SEScalar, SEScalarFrequency, SEScalarPressure, \
                              SEScalarTime, SEScalarVolume, SEScalarVolumePerTime


class SEBagValveMaskAction(SEEquipmentAction):
    def __init__(self):
        super().__init__()

class SEBagValveMaskConfiguration(SEBagValveMaskAction):
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
        if not isinstance(SEBagValveMaskConfiguration, src):
            raise Exception("Provided argument must be a SEBagValveMaskConfiguration")
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
            self._configuration = SEBagValveMask()
        return self._configuration

class SEBagValveMaskAutomated(SEBagValveMaskAction):
    __slots__ = ["_breath_frequency", "_inspiratory_expiratory_ratio",
                 "_squeeze_pressure", "_squeeze_volume"]

    def __init__(self):
        super().__init__()
        self._breath_frequency = None
        self._inspiratory_expiratory_ratio   = None
        self._squeeze_pressure = None
        self._squeeze_volume = None

    def clear(self):
        if self._breath_frequency is not None:
            self._breath_frequency.invalidate()
        if self._inspiratory_expiratory_ratio is not None:
            self._inspiratory_expiratory_ratio.invalidate()
        if self._squeeze_pressure is not None:
            self._squeeze_pressure.invalidate()
        if self._squeeze_volume is not None:
            self._squeeze_volume.invalidate()

    def is_valid(self):
        return True

    def has_breath_frequency(self):
        return self._breath_frequency is not None
    def get_breath_frequency(self):
        if self._breath_frequency is None:
            self._breath_frequency = SEScalarFrequency()
        return self._breath_frequency

    def has_inspiratory_expiratory_ratio(self):
        return self._flow is not None
    def get_inspiratory_expiratory_ratio(self):
        if self._inspiratory_expiratory_ratio is None:
            self._inspiratory_expiratory_ratio = SEScalar()
        return self._inspiratory_expiratory_ratio

    def has_squeeze_pressure(self):
        return self._squeeze_pressure is not None
    def get_squeeze_pressure(self):
        if self._squeeze_pressure is None:
            self._squeeze_pressure = SEScalarPressure()
        return self._squeeze_pressure

    def has_volume(self):
        return self._squeeze_volume is not None
    def get_squeeze_volume(self):
        if self._squeeze_volume is None:
            self._squeeze_volume = SEScalarVolume()
        return self._squeeze_volume

    def __repr__(self):
        return ("Bag Valve Mask Automated\n"
                "  Breath Frequency: {}\n"
                "  Inspiratory Expiratory Ratio: {}\n"
                "  Squeeze Pressure: {}\n"
                "  Squeeze Volume {}").format(self._breath_frequency,
                                              self._inspiratory_expiratory_ratio,
                                              self._squeeze_pressure,
                                              self._squeeze_volume)

class SEBagValveMaskInstantaneous(SEBagValveMaskAction):
    __slots__ = ["_flow", "_pressure"]

    def __init__(self):
        super().__init__()
        self._flow   = None
        self._pressure = None

    def clear(self):
        if self._flow is not None:
            self._flow.invalidate()
        if self._pressure is not None:
            self._pressure.invalidate()

    def is_valid(self):
        return self.has_flow() or self.has_pressure()

    def has_flow(self):
        return self._flow is not None
    def get_flow(self):
        if self._flow is None:
            self._flow = SEScalarVolumePerTime()
        return self._flow

    def has_pressure(self):
        return self._pressure is not None
    def get_pressure(self):
        if self._pressure is None:
            self._pressure = SEScalarVolume()
        return self._pressure

    def __repr__(self):
        return ("Bag Valve Mask Instantaneous\n"
                "  Flow: {}\n"
                "  Pressure: {}").format(self._flow, self._pressure)

class SEBagValveMaskSqueeze(SEBagValveMaskAction):
    __slots__ = ["_squeeze_pressure", "_squeeze_volume",
                 "_expiratory_period", "_inspiratory_period"]

    def __init__(self):
        super().__init__()
        self._squeeze_pressure = None
        self._squeeze_volume   = None
        self._expiratory_period = None
        self._inspiratory_period = None

    def clear(self):
        if self._squeeze_pressure is not None:
            self._squeeze_pressure.invalidate()
        if self._squeeze_volume is not None:
            self._squeeze_volume.invalidate()
        if self._expiratory_period is not None:
            self._expiratory_period.invalidate()
        if self._inspiratory_period is not None:
            self._inspiratory_period.invalidate()

    def is_valid(self):
        return True

    def has_squeeze_pressure(self):
        return self._squeeze_pressure is not None
    def get_squeeze_pressure(self):
        if self._squeeze_pressure is None:
            self._squeeze_pressure = SEScalarPressure()
        return self._squeeze_pressure

    def has_squeeze_volume(self):
        return self._squeeze_volume is not None
    def get_squeeze_volume(self):
        if self._squeeze_volume is None:
            self._squeeze_volume = SEScalarVolume()
        return self._squeeze_volume

    def has_expiratory_period(self):
        return self._expiratory_period is not None
    def get_expiratory_period(self):
        if self._expiratory_period is None:
            self._expiratory_period = SEScalarTime()
        return self._expiratory_period

    def has_inspiratory_period(self):
        return self._inspiratory_period is not None
    def get_inspiratory_period(self):
        if self._inspiratory_period is None:
            self._inspiratory_period = SEScalarTime()
        return self._inspiratory_period

    def __repr__(self):
        return ("Bag Valve Mask Squeeze\n"
                "  Squeeze Pressure: {}\n"
                "  Squeeze Volume: {}\n"
                "  Expiratory Period: {}\n"
                "  Inspiratory Period: {}").format(self._squeeze_pressure,
                                                   self._squeeze_volume,
                                                   self._expiratory_period,
                                                   self._inspiratory_period)

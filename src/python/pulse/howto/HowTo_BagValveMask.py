# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat, eSwitch
from pulse.cdm.scalars import VolumeUnit, VolumePerTimeUnit, PressureUnit, TimeUnit
from pulse.cdm.patient_actions import SESupplementalOxygen, eDevice
from pulse.engine.PulseEngine import PulseEngine


# BVM
from pulse.cdm.bag_valve_mask import SEBagValveMask
from pulse.cdm.bag_valve_mask_actions import SEBagValveMaskAction, SEBagValveMaskAction, SEBagValveMaskConfiguration, SEBagValveMaskAutomated, SEBagValveMaskInstantaneous, SEBagValveMaskSqueeze

def HowTo_BagValveMask():
        pulse = PulseEngine()
        pulse.set_log_filename("./test_results/howto/HowTo_BagValveMask.py.log")
        pulse.log_to_console(True)

        # NOTE: No data requests are being provided, so Pulse will return the default vitals data
        if not pulse.serialize_from_file("./states/Soldier@0s.json", None):
                print("Unable to load initial state file")
                return

        # Get some data from the engine
        results = pulse.pull_data()
        pulse.print_results()

        # BVM
        cfg = SEBagValveMaskConfiguration()
        cfg.get_configuration().set_connection(eSwitch.On)
        pulse.process_action(cfg)

        squeeze = SEBagValveMaskSqueeze()
        squeeze.get_expiratory_period().set_value(2, TimeUnit.s)
        squeeze.get_inspiratory_period().set_value(3, TimeUnit.s)
        squeeze.get_squeeze_volume().set_value(500, VolumeUnit.mL)
        pulse.process_action(squeeze)

        # Advance some time and print out the vitals
        pulse.advance_time_s(30)
        results = pulse.pull_data()
        pulse.print_results()

HowTo_BagValveMask()
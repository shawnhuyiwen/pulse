# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from enum import Enum
from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient_actions import SEBrainInjury, eBrainInjuryType
from pulse.engine.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_BrainInjury():
    pulse = PulsePhysiologyEngine("pulse_BrainInjury.log")

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/Soldier@0s.json", None, eSerializationFormat.JSON, 0):
        print("Unable to load initial state file")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    brain_injury = SEBrainInjury()
    brain_injury.set_comment("Patient receives a brain injury")
    brain_injury.set_injury_type(eBrainInjuryType.LeftFocal)
    brain_injury.get_severity().set_value(.5)
    pulse.process_action(brain_injury)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_BrainInjury()


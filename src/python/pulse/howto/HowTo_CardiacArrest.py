# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient_actions import SECardiacArrest, eSwitch
from pulse.cpm.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_CardiacArrest():
    pulse = PulsePhysiologyEngine("pulse_cardiac_arrest.log", True)

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/Soldier@0s.json", None, eSerializationFormat.JSON, 0):
        print("Unable to load initial state file")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    arrest = SECardiacArrest()
    arrest.set_comment("Patient experiences cardiac arrest")
    arrest.set_state(eSwitch.On)
    pulse.process_action(arrest)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_CardiacArrest()


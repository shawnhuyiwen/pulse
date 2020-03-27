# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient_actions import SERespiratoryFatigue
from pulse.engine.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_RepiratoryFatigue():
    pulse = PulsePhysiologyEngine("pulse_RespiratoryFatigue.log")

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/Soldier@0s.json", None, eSerializationFormat.JSON, 0):
        print("Unable to load initial state file")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    # Perform an action
    fatigue = SERespiratoryFatigue()
    fatigue.set_comment("Patient undergoes respiratory fatigue")
    fatigue.get_severity().set_value(0.6)
    pulse.process_action(fatigue)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_RepiratoryFatigue()


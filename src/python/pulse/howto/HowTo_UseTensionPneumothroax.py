# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient_actions import SETensionPneumothorax, eSide, eGate
from pulse.engine.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_TensionPneumothorax():
    pulse = PulsePhysiologyEngine("pulse_tension_pneumothorax.log")

    if not pulse.serialize_from_file("./states/Soldier@0s.json", None, eSerializationFormat.JSON, 0):
        print("Unable to load initial state file")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    # Perform an action
    tension = SETensionPneumothorax()
    tension.set_comment("Patient ")
    tension.get_severity().set_value(.7)
    tension.set_side(eSide.Right)
    tension.set_type(eGate.Open)
    pulse.process_action(tension)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

    tension.set_type(eGate.Closed)
    pulse.process_action(tension)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_TensionPneumothorax()


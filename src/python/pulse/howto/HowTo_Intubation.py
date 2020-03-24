# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from enum import Enum
from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient import SEPatientConfiguration
from pulse.cdm.patient_actions import SEIntubation, eIntubationType
from pulse.engine.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_Intubation():
    pulse = PulsePhysiologyEngine("pulse_Intubation.log")
    if not pulse.serialize_from_file("./states/Soldier@0s.json", None, eSerializationFormat.JSON, 0):
        print("Unable to load initial state file")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    intubation = SEIntubation()
    intubation.set_comment("Patient undergoes intubation of the left lung")
    intubation.set_type(eIntubationType.LeftMainstem)
    pulse.process_action(intubation)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_Intubation()


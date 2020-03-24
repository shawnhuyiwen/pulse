# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from enum import Enum
from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient_actions import SEBronchoConstriction
from pulse.engine.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_BronchoConstriction():
    pulse = PulsePhysiologyEngine("pulse_BronchoConstriction.log")

    if not pulse.serialize_from_file("./states/Soldier@0s.json", None, eSerializationFormat.JSON, 0):
        print("Unable to load initial state file")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    # Perform an action to exacerbate the initial condition state
    broncho_constriction = SEBronchoConstriction()
    broncho_constriction.set_comment("Patient undergoes constriction of the bronchus")
    broncho_constriction.get_severity().set_value(0.8)
    pulse.process_action(broncho_constriction)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_BronchoConstriction()


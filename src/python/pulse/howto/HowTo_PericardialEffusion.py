# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.scalars import VolumePerTimeUnit
from pulse.cdm.patient_actions import SEPericardialEffusion
from pulse.engine.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_PericardalEffusion():
    pulse = PulsePhysiologyEngine("pulse_pericardial_effusion.log")

    if not pulse.serialize_from_file("./states/Soldier@0s.json", None, eSerializationFormat.JSON, 0):
        print("Unable to load initial state file")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    # Perform an action
    effusion = SEPericardialEffusion()
    effusion.set_comment("Patient undergoes a pericardial effusion")
    effusion.get_effusion_rate().set_value(50, VolumePerTimeUnit.mL_Per_min)
    pulse.process_action(effusion)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_PericardalEffusion()


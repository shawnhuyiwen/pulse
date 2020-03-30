# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient import SEPatientConfiguration
from pulse.cdm.patient_actions import SEExercise
from pulse.engine.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_UseChronicPericardialEffusion():
    pulse = PulsePhysiologyEngine("pulse_chronic_pericardial_effusion.log")

    pc = SEPatientConfiguration()
    pc.set_patient_file("./patients/StandardMale.json")
    effusion = pc.get_conditions().get_chronic_pericardial_effusion()
    effusion.get_accumulated_volume().set_value(.3)

    # Initialize the engine with our configuration
    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.initialize_engine(pc, None):
        print("Unable to load stabilize engine")
        return

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_UseChronicPericardialEffusion()


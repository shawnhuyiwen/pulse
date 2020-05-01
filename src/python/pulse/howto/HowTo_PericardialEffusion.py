# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.patient import SEPatientConfiguration
from pulse.cdm.scalars import VolumeUnit, VolumePerTimeUnit
from pulse.cdm.patient_actions import SEPericardialEffusion
from pulse.cpm.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_PericardalEffusion():
    pulse = PulsePhysiologyEngine("pulse_PericardialEffusion.log")

    pc = SEPatientConfiguration()
    pc.set_patient_file("./patients/StandardMale.json")
    effusion = pc.get_conditions().get_chronic_pericardial_effusion()
    effusion.get_accumulated_volume().set_value(.3, VolumeUnit.mL)

    # Initialize the engine with our configuration
    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.initialize_engine(pc, None):
        print("Unable to load stabilize engine")
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


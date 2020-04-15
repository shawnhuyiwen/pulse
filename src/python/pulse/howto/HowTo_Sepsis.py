# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.patient import SEPatientConfiguration
from pulse.cpm.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_UseSepsis():
    pulse = PulsePhysiologyEngine("pulse_sepsis.log")

    pc = SEPatientConfiguration()
    pc.set_patient_file("./patients/StandardMale.json")
    sepsis = pc.get_conditions().get_sepsis()
    sepsis.get_severity().set_value(.7)

    # Initialize the engine with our configuration
    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.initialize_engine(pc, None):
        print("Unable to load stabilize engine")
        return

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_UseSepsis()


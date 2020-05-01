# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.patient import SEPatientConfiguration
from pulse.cpm.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_UseAnemia():
    pulse = PulsePhysiologyEngine("pulse_ChronicVentricularSystolicDysfunction.log")

    pc = SEPatientConfiguration()
    pc.set_patient_file("./patients/StandardMale.json")
    cvsd = pc.get_conditions().get_chronic_ventricular_systolic_dysfunction()

    # Initialize the engine with our configuration
    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.initialize_engine(pc, None):
        print("Unable to load stabilize engine")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_UseAnemia()


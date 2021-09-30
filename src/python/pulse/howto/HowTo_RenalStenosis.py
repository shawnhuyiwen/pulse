# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.patient import SEPatientConfiguration
from pulse.engine.PulseEngine import PulseEngine

def HowTo_UseRenalStenosis():
    pulse = PulseEngine()
    pulse.set_log_filename("./test_results/howto/HowTo_RenalStenosis.py.log")
    pulse.log_to_console(True)

    pc = SEPatientConfiguration()
    pc.set_patient_file("./patients/StandardMale.json")
    renal_stenosis = pc.get_conditions().get_chronic_renal_stenosis()
    renal_stenosis.get_left_kidney_severity().set_value(.4)
    renal_stenosis.get_right_kidney_severity().set_value(0)

    # Initialize the engine with our configuration
    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.initialize_engine(pc, None):
        print("Unable to load stabilize engine")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_UseRenalStenosis()


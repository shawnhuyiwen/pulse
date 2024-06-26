# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.patient import SEPatientConfiguration
from pulse.engine.PulseEngine import PulseEngine

def HowTo_UsePulmonaryFibrosis():
    pulse = PulseEngine()
    pulse.set_log_filename("./test_results/howto/HowTo_pulmonary_fibrosis.py.log")
    pulse.log_to_console(True)

    pc = SEPatientConfiguration()
    pc.set_patient_file("./patients/StandardMale.json")
    pulmonary_fibrosis = pc.get_conditions().get_pulmonary_fibrosis()
    pulmonary_fibrosis.get_severity().set_value(.2)

    # Initialize the engine with our configuration
    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.initialize_engine(pc, None):
        print("Unable to load stabilize engine")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    pulse.print_results()

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    pulse.print_results()

HowTo_UsePulmonaryFibrosis()


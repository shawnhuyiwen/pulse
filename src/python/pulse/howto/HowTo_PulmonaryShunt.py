# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.patient import SEPatientConfiguration
from pulse.cdm.patient_actions import SEPulmonaryShuntExacerbation
from pulse.cpm.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_PulmonaryShunt():
    pulse = PulsePhysiologyEngine()
    pulse.set_log_filename("./test_results/pypulse_pulmonary_shunt.log")
    pulse.log_to_console(True)

    pc = SEPatientConfiguration()
    pc.set_patient_file("./patients/StandardMale.json")
    pulmonary_shunt = pc.get_conditions().get_pulmonary_shunt()
    pulmonary_shunt.get_severity().set_value(0.3)

    # Initialize the engine with our configuration
    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.initialize_engine(pc, None):
        print("Unable to load stabilize engine")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    # Perform an action to exacerbate the initial condition state
    exacerbation = SEPulmonaryShuntExacerbation()
    exacerbation.set_comment("Patient's Pulmonary Shunt worsens")
    exacerbation.get_severity().set_value(.7)

    pulse.process_action(exacerbation)

    # Advance some time and print out the vitals
    # look for the Oxygen saturation to drastically drop
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_PulmonaryShunt()


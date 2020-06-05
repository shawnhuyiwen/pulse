# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.patient import SEPatientConfiguration
from pulse.cdm.patient_actions import SEAcuteRespiratoryDistressSyndromeExacerbation
from pulse.cpm.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_ARDS():
    pulse = PulsePhysiologyEngine()
    pulse.set_log_filename("./test_results/pypulse_ARDS.log")
    pulse.log_to_console(True)

    pc = SEPatientConfiguration()
    pc.set_patient_file("./patients/StandardMale.json")
    ards = pc.get_conditions().get_acute_respiratory_distress_syndrome()
    ards.get_left_lung_affected().set_value(0.4)
    ards.get_right_lung_affected().set_value(0.1)
    ards.get_severity().set_value(0.3)

    # Initialize the engine with our configuration
    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.initialize_engine(pc, None):
        print("Unable to load stabilize engine")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    # Perform an action to exacerbate the initial condition state
    exacerbation = SEAcuteRespiratoryDistressSyndromeExacerbation()
    exacerbation.set_comment("Patient's Acute Respiratory Distress Syndrome is exacerbated")
    exacerbation.get_severity().set_value(0.4)
    exacerbation.get_left_lung_affected().set_value(0.4)
    exacerbation.get_right_lung_affected().set_value(0.1)
    pulse.process_action(exacerbation)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_ARDS()


# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.patient_actions import SEUrinate
from pulse.engine.PulseEngine import PulseEngine

def HowTo_Urinate():
    pulse = PulseEngine()
    pulse.set_log_filename("./test_results/howto/HowTo_Urinate.py.log")
    pulse.log_to_console(True)

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/Soldier@0s.json", None):
        print("Unable to load initial state file")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    pulse.print_results()

    urinate = SEUrinate()
    urinate.set_comment("Patient goes to the restroom")
    pulse.process_action(urinate)

    # Advance some time and print out the vitals
    pulse.advance_time_s(60)
    results = pulse.pull_data()
    pulse.print_results()

HowTo_Urinate()


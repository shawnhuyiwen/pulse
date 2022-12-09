# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient_actions import SEHemothorax, \
                                      SETubeThoracostomy, \
                                      eSide
from pulse.engine.PulseEngine import PulseEngine

def HowTo_Hemothorax():
    pulse = PulseEngine()
    pulse.set_log_filename("./test_results/howto/HowTo_Hemothorax.py.log")
    pulse.log_to_console(True)

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/StandardMale@0s.json", None):
        print("Unable to load initial state file")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    # Give the patient a hemothorax
    hemothorax = SEHemothorax()
    hemothorax.set_comment("Patient ")
    hemothorax.get_severity().set_value(0.75)
    hemothorax.set_side(eSide.Left)
    pulse.process_action(hemothorax)

    # Advance some time and print out the vitals
    pulse.advance_time_s(120)
    results = pulse.pull_data()
    print(results)

    # Apply intervention
    tube_thoracostomy = SETubeThoracostomy()
    tube_thoracostomy.set_comment("Patient undergoes tube thoracostomy on left site")
    tube_thoracostomy.set_side(eSide.Left)
    pulse.process_action(tube_thoracostomy)

    # Advance some time and print out the vitals
    pulse.advance_time_s(400)
    results = pulse.pull_data()
    print(results)

HowTo_Hemothorax()

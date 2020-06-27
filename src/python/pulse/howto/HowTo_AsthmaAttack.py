# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient_actions import SEAsthmaAttack
from pulse.cpm.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_AsthmaAttack():
    pulse = PulsePhysiologyEngine()
    pulse.set_log_filename("./test_results/pypulse_AsthmaAttack.log")
    pulse.log_to_console(True)

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/Soldier@0s.pbb", None):
        print("Unable to load initial state file")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    # Perform an action
    asthma_attack = SEAsthmaAttack()
    asthma_attack.set_comment("Patient undergoes Asthma attack")
    asthma_attack.get_severity().set_value(0.4)
    pulse.process_action(asthma_attack)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_AsthmaAttack()


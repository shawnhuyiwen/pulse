# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient_actions import SEHemorrhage, eHemorrhageType
from pulse.engine.PulseEngine import PulseEngine
from pulse.cdm.scalars import VolumePerTimeUnit

def HowTo_Hemorrhage():
    pulse = PulseEngine()
    pulse.set_log_filename("./test_results/howto/HowTo_Hemorrhage.py.log")
    pulse.log_to_console(True)

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/Soldier@0s.pbb", None):
        print("Unable to load initial state file")
        return

    # Get default data at time 0s from the engine
    results = pulse.pull_data()
    print(results)

    # Perform an action
    hemorrhage = SEHemorrhage()
    hemorrhage.set_comment("Laceration to the leg")
    hemorrhage.set_type(eHemorrhageType.External)
    hemorrhage.set_compartment("RightLeg")
    hemorrhage.get_severity().set_value(0.8)
    # Optionally, You can set the flow rate of the hemorrhage,
    # but this needs to be provided the proper flow rate associated with the anatomy
    # This is implemented as a flow source, this rate will be constant, and will not be affected by dropping blood pressures
    # It is intended to interact with sensors or with something continuously monitoring physiology and updating the flow
    #hemorrhage.get_flow_rate().set_value(75,VolumePerTimeUnit.mL_Per_min)
    pulse.process_action(hemorrhage)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

    # For this example, you will see a decrease in blood volume

HowTo_Hemorrhage()

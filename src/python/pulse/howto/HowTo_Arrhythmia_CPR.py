# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.patient_actions import SEArrhythmia, eHeartRhythm, \
                                      SEChestCompressionForceScale, SEChestCompressionForce
from pulse.cdm.scalars import ForceUnit, TimeUnit
from pulse.engine.PulseEngine import PulseEngine

def HowTo_Arrhythmia():
    pulse = PulseEngine()
    pulse.set_log_filename("./test_results/howto/HowTo_Arrhythmia.py.log")
    pulse.log_to_console(True)

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/Soldier@0s.pbb", None):
        print("Unable to load initial state file")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    arryhthmia = SEArrhythmia()
    arryhthmia.set_comment("Patient experiences cardiac arrest")
    arryhthmia.set_rhythm(eHeartRhythm.StableVentricularTachycardia)
    pulse.process_action(arryhthmia)

    # Advance some time and print out the vitals
    pulse.advance_time_s(10)
    results = pulse.pull_data()
    print(results)

    # If you are using a sensor, you can directly pass over a force
    # You should pass the sensor readings over at a decent rate continuously
    cpr_force = SEChestCompressionForce()
    cpr_force.set_comment("Press and hold the chest")
    cpr_force.get_force().set_value(400, ForceUnit.N)
    pulse.process_action(cpr_force)

    pulse.advance_time_s(1)
    results = pulse.pull_data()
    print(results)

    cpr_force.set_comment("Stop pressing")
    cpr_force.get_force().set_value(0, ForceUnit.N)
    pulse.process_action(cpr_force)

    # If you are in software, you can pass in a scaled force value for a time period
    cpr_scale= SEChestCompressionForceScale()
    cpr_scale.set_comment("Press and hold the chest")
    cpr_scale.get_force_scale().set_value(0.8)# Pretty hard!
    cpr_scale.get_force_period().set_value(1, TimeUnit.s)
    pulse.process_action(cpr_scale)

    pulse.advance_time_s(10)
    results = pulse.pull_data()
    print(results)

HowTo_Arrhythmia()


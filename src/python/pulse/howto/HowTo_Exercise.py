# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient_actions import SEExercise
from pulse.engine.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_Exercise():
    pulse = PulsePhysiologyEngine("pulse_exercise.log")

    if not pulse.serialize_from_file("./states/Soldier@0s.json", None, eSerializationFormat.JSON, 0):
        print("Unable to load initial state file")
        return

    # Get default data at time 0s from the engine
    results = pulse.pull_data()
    print(results)

    # Perform an action
    exercise = SEExercise()
    exercise.set_comment("Start some star jumps")
    exercise.get_intensity().set_value(0.06)
    pulse.process_action(exercise)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

    # For this example, you will see an increase in heart rate

HowTo_Exercise()
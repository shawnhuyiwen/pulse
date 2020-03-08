# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from enum import Enum
from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient import eSex, SEPatient, SEPatientConfiguration
from pulse.cdm.patient_actions import eHemorrhageType
from pulse.cdm.patient_actions import SEExercise, SEHemorrhage
from pulse.engine.PulsePhysiologyEngine import PulsePhysiologyEngine
from pulse.cdm.scalars import FrequencyUnit, LengthUnit, MassUnit, \
                              MassPerVolumeUnit, PressureUnit, TemperatureUnit, \
                              TimeUnit, VolumePerTimeUnit

from pulse.cdm.io.patient import serialize_patient_from_file
from pulse.cdm.io.environment import serialize_environmental_conditions_from_file

# TODO things that still need to be implemented:
#    Getting Events from Pulse
#    Getting log information from Pulse
#    Requesting specific data from Pulse
#    EngineThunk interface is incomplete

class eStartType(Enum):
    State = 0
    Stabilize_PatientFile = 1
    Stabilize_PatientObject = 2

def HowTo_UseEngine():
    pulse = PulsePhysiologyEngine("pulse.log")

    # There are several ways to initialize an engine to a patient
    start_type = eStartType.Stabilize_PatientObject

    if start_type is eStartType.State: # The engine is ready instantaneously
        if not pulse.serialize_from_file("./states/Soldier@0s.json", None, eSerializationFormat.JSON, 0):
            print("Unable to load initial state file")
            return
        # Stabilization will require the engine to run for several minutes
        # to get to a homeostatic state based on your patients properties
    elif start_type is eStartType.Stabilize_PatientFile or \
         start_type is eStartType.Stabilize_PatientObject:
        pc = SEPatientConfiguration()
        # Add a patient definition to our patient configuration
        if start_type is eStartType.Stabilize_PatientFile:
            # We can point to a patient file on disk
            pc.set_patient_file("./patients/Soldier.json")
        elif start_type is eStartType.Stabilize_PatientObject:
            # We can create our own patient
            p = pc.get_patient()
            # You only need to set sex and all other properties will be computed
            # per https://pulse.kitware.com/_patient_methodology.html
            # Let's  load up a file from disk (You don't have to start with a file)
            serialize_patient_from_file("./patients/Soldier.json", p, eSerializationFormat.JSON)
            # Now let's modify a few properties
            p.set_name("Wenye")
            p.get_age().set_value(22,TimeUnit.yr)
            p.get_height().set_value(72, LengthUnit.inch)
            p.get_weight().set_value(180, MassUnit.lb)
            p.get_heart_rate_baseline().set_value(72, FrequencyUnit.Per_min)
            p.get_systolic_arterial_pressure_baseline().set_value(117, PressureUnit.mmHg)
            p.get_diastolic_arterial_pressure_baseline().set_value(72, PressureUnit.mmHg)
            p.get_respiration_rate_baseline().set_value(12, FrequencyUnit.Per_min)
        # Let's add a condition to our stabilization
        # This is optional, you do not need any conditions
        # But if you do, you can add as many as you want like this
        env = pc.get_conditions().get_initial_environmental_conditions()
        # Let's  load up a file from disk (You don't have to start with a file)
        serialize_environmental_conditions_from_file("./environments/ExerciseEnvironment.json",
                                                        env.get_environmental_conditions(),
                                                        eSerializationFormat.JSON)
        # Now let's modify a few properties
        env.get_environmental_conditions().get_air_density().set_value(1.225, MassPerVolumeUnit.kg_Per_m3)
        env.get_environmental_conditions().get_ambient_temperature().set_value(33, TemperatureUnit.C)
        env.get_environmental_conditions().get_respiration_ambient_temperature().set_value(33, TemperatureUnit.C)

        # Initialize the engine with our configuration
        if not pulse.initialize_engine(pc, None):
            print("Unable to load stabilize engine")
            return

    # Get some data from the engine
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

HowTo_UseEngine()


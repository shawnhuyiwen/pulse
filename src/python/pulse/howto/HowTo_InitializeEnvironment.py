# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient import SEPatientConfiguration
from pulse.engine.PulsePhysiologyEngine import PulsePhysiologyEngine
from pulse.cdm.scalars import MassPerVolumeUnit, TemperatureUnit
from pulse.cdm.io.environment import serialize_environmental_conditions_from_file

def HowTo_InitializeEnvironment():
    pulse = PulsePhysiologyEngine("pulse.log")

    # Configure the engine with some files
    # You can always get objects and fill them out manually if you want
    # See the HowTo_ChangeEnvironment for an example of using the SEEnvionmentalConditions object
    pc = SEPatientConfiguration()
    pc.set_patient_file("./patients/Soldier.json")
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

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

    # No change is expected since we stabilized to this environment

HowTo_InitializeEnvironment()
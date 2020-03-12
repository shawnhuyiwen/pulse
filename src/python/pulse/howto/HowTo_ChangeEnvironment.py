# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.environment_actions import SEChangeEnvironmentalConditions
from pulse.engine.PulsePhysiologyEngine import PulsePhysiologyEngine
from pulse.cdm.scalars import LengthPerTimeUnit, TemperatureUnit, \
                              PressureUnit, HeatResistanceAreaUnit

def HowTo_ChangeEnvironment():
    pulse = PulsePhysiologyEngine("pulse_exercise.log")

    if not pulse.serialize_from_file("./states/Soldier@0s.json", None, eSerializationFormat.JSON, 0):
        print("Unable to load initial state file")
        return

    # Get default data at time 0s from the engine
    results = pulse.pull_data()
    print(results)

    # Perform an action
    change = SEChangeEnvironmentalConditions()
    change.set_comment("Modify Environmental Conditions")
    # You could just point to an environment file if you want
    # We provide a few in the bin/environments folder
    #change.set_environmental_conditions_file("/path/to/file")
    environmental_conditions = change.get_environmental_conditions()
    # Not all properties have to be set on the environmental conditions
    # Properties not included will retain their current value
    environmental_conditions.get_air_velocity().set_value(0.2,LengthPerTimeUnit.m_Per_s)
    environmental_conditions.get_ambient_temperature().set_value(22, TemperatureUnit.C)
    environmental_conditions.get_atmospheric_pressure().set_value(525, PressureUnit.mmHg)
    environmental_conditions.get_clothing_resistance().set_value(0.5, HeatResistanceAreaUnit.clo)
    environmental_conditions.get_relative_humidity().set_value(0.5)
    # When changing ambient gas mixture, fractions must sum to 1
    environmental_conditions.remove_ambient_gasses()
    environmental_conditions.get_ambient_gas("Nitrogen").get_fraction_amount().set_value(0.7896)
    environmental_conditions.get_ambient_gas("Oxygen").get_fraction_amount().set_value(0.21)
    environmental_conditions.get_ambient_gas("CarbonDioxide").get_fraction_amount().set_value(4.0E-4)
    # Note CarbonMonoxide can be added to the ambient gas mixture this way
    pulse.process_action(change)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_ChangeEnvironment()
# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.patient_actions import SEConsumeNutrients
from pulse.cdm.scalars import MassUnit, MassPerTimeUnit, VolumeUnit
from pulse.cpm.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_ConsumeNutrients():
    pulse = PulsePhysiologyEngine()
    pulse.set_log_filename("./test_results/pypulse_ConsumeNutrients.log")
    pulse.log_to_console(True)

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/Soldier@0s.pbb", None):
        print("Unable to load initial state file")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    consume = SEConsumeNutrients()
    consume.set_comment("Patient eats lunch")
    # Optionally you can set a nutrition file on disk
    consume.get_nutrition().get_carbohydrate().set_value(130,MassUnit.g)
    consume.get_nutrition().get_carbohydrate_digestion_rate().set_value(1.083,MassPerTimeUnit.g_Per_min)
    consume.get_nutrition().get_fat().set_value(27,MassUnit.g)
    consume.get_nutrition().get_fat_digestion_rate().set_value(0.055,MassPerTimeUnit.g_Per_min)
    consume.get_nutrition().get_protein().set_value(20,MassUnit.g)
    consume.get_nutrition().get_protein_digestion_rate().set_value(0.071,MassPerTimeUnit.g_Per_min)
    consume.get_nutrition().get_calcium().set_value(5000,MassUnit.mg)
    consume.get_nutrition().get_sodium().set_value(1,MassUnit.g)
    consume.get_nutrition().get_water().set_value(300,VolumeUnit.mL)
    pulse.process_action(consume)

    # Advance some time and print out the vitals
    pulse.advance_time_s(60)
    results = pulse.pull_data()
    print(results)

HowTo_ConsumeNutrients()


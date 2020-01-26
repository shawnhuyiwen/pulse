# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient_actions import SEHemorrhage
from pulse.cdm.scalars import VolumePerTimeUnit
from pulse.engine.PulsePhysiologyEngine import PulsePhysiologyEngine

pulse = PulsePhysiologyEngine("pulse.log")

if not pulse.serialize_from_file("./states/StandardMale@0s.json",None,eSerializationFormat.JSON,0):
    print("Unable to load file")

pulse.advance_time()

results = pulse.pull_data()
print(results)

hemorrhage = SEHemorrhage()
hemorrhage.set_compartment("RightLeg")
hemorrhage.get_rate().set_value(75, VolumePerTimeUnit.mL_Per_min)

print(hemorrhage)
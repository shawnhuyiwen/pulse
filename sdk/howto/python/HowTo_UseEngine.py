from engine.python.PulsePhysiologyEngine import *

pulse = PulsePhysiologyEngine("pulse.log")

if not pulse.serialize_from_file("./states/StandardMale@0s.json",None,SerializationFormat.JSON,0):
    print("Unable to load file")

pulse.advance_time()

results = pulse.pull_data()
print(results)





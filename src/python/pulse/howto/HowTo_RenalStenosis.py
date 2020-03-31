# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient import SEPatientConfiguration
from pulse.cdm.patient_actions import SEExercise
from pulse.engine.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_UseRenalStenosis():
    pulse = PulsePhysiologyEngine("pulse_RenalStenosis.log")

    pc = SEPatientConfiguration()
    pc.set_patient_file("./patients/StandardMale.json")
    renal_stenosis = pc.get_conditions().get_chronic_renal_stenosis()
    renal_stenosis.get_left_kidney_severity().set_value(.4)
    renal_stenosis.get_right_kidney_severity().set_value(0)

    # Initialize the engine with our configuration
    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.initialize_engine(pc, None):
        print("Unable to load stabilize engine")
        return

    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_UseRenalStenosis()


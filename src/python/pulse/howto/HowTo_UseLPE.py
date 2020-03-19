# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient import SEPatientConfiguration
from pulse.cdm.patient_actions import SELobarPneumoniaExacerbation
from pulse.cdm.patient_conditions import SELobarPneumonia
from pulse.engine.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_UseLPE():
    pulse = PulsePhysiologyEngine("pulse_LPE.log")

    pc = SEPatientConfiguration()
    pc.set_patient_file("./patients/StandardMale.json")
    pneumonia = pc.get_conditions().get_lobar_pneumonia()
    pneumonia.get_left_lung_affected().set_value(0.1)
    pneumonia.get_right_lung_affected().set_value(0.1)
    pneumonia.get_severity().set_value(0.3)

    # Initialize the engine with our configuration
    if not pulse.initialize_engine(pc, None):
        print("Unable to load stabilize engine")
        return

    # Perform an action
    exacerbation = SELobarPneumoniaExacerbation()
    exacerbation.set_comment("Patient's Lobar Pneumonia is exacerbated")
    exacerbation.get_severity().set_value(0.4)
    pulse.process_action(exacerbation)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_UseLPE()


# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient_actions import SENeedleDecompression, eSide, eSwitch
from pulse.engine.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTONeedleDecompression():
    pulse = PulsePhysiologyEngine("pulse_needle_decompression.log")

    pc = SEPatientConfiguration()
    pc.set_patient_file("./patients/StandardMale.json")
    peumo = pc.get_conditions().get_tension_pneumothorax()
    peumo.get_severity().set_value(0.3)

    # Initialize the engine with our configuration
    if not pulse.initialize_engine(pc, None):
        print("Unable to load stabilize engine")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    # Perform an action
    needle_decomp = SENeedleDecompression()
    needle_decomp.set_comment("Patient undergoes needle decompression on right site")
    needle_decomp.set_side(eSide.Right)
    needle_decomp.set_state(eSwitch.On)
    pulse.process_action(needle_decomp)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

    needle_decomp.set_comment("Switch turned off for Needle decompression")
    needle_decomp.set_state(eSwitch.Off)
    pulse.process_action(needle_decomp)

    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)
HowTONeedleDecompression()


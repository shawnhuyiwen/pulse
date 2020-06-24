# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.patient_actions import SESubstanceCompoundInfusion, eSubstance_Administration
from pulse.cdm.scalars import MassPerVolumeUnit, VolumePerTimeUnit, VolumeUnit, SEScalarVolume
from pulse.cpm.PulsePhysiologyEngine import PulsePhysiologyEngine

def HowTo_CompoundSubstanceInfusion():
    pulse = PulsePhysiologyEngine()
    pulse.set_log_filename("./test_results/pypulse_SubstanceCompoundInfusion.log")
    pulse.log_to_console(True)

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/Soldier@0s.pbb", None):
        print("Unable to load initial state file")
        return

    # Get some data from the engine
    results = pulse.pull_data()
    print(results)

    # Perform an action
    substance_compound = SESubstanceCompoundInfusion()
    substance_compound.set_comment("Patient receives infusion of blood")
    substance_compound.set_compound("Blood")
    substance_compound.get_rate().set_value(100, VolumePerTimeUnit.mL_Per_min)
    substance_compound.get_bag_volume().set_value(.5, VolumeUnit.L)
    pulse.process_action(substance_compound)

    # Advance some time and print out the vitals
    pulse.advance_time_s(30)
    results = pulse.pull_data()
    print(results)

HowTo_CompoundSubstanceInfusion()


# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.scalars import VolumePerTimeUnit, MassPerVolumeUnit
from pulse.cdm.patient_actions import SESubstanceInfusion
from pulse.engine.PulseEngine import PulseEngine
from pulse.cdm.engine import eSerializationFormat, SEDataRequestManager, SEDataRequest
from pulse.cdm.scalars import ElectricPotentialUnit, FrequencyUnit, LengthUnit, MassUnit, MassPerVolumeUnit, \
    PressureUnit, TemperatureUnit, TimeUnit, VolumeUnit, VolumePerTimeUnit


def propofol_infusion():
    pulse = PulseEngine()
    pulse.set_log_filename("./test_results/howto/HowTo_SubstanceInfusion.py.log")
    pulse.log_to_console(True)

    data_requests = [
        SEDataRequest.create_physiology_request("HeartRate", unit=FrequencyUnit.Per_min),
        SEDataRequest.create_physiology_request("ArterialPressure", unit=PressureUnit.mmHg),
        SEDataRequest.create_physiology_request("MeanArterialPressure", unit=PressureUnit.mmHg),
        SEDataRequest.create_physiology_request("SystolicArterialPressure", unit=PressureUnit.mmHg),
        SEDataRequest.create_physiology_request("DiastolicArterialPressure", unit=PressureUnit.mmHg),
        SEDataRequest.create_substance_request("Propofol", "PlasmaConcentration", unit=MassPerVolumeUnit.ug_Per_L),
    ]
    data_req_mgr = SEDataRequestManager(data_requests)
    data_req_mgr.set_results_filename("E:/PulseEngine/engine/src/python/pulse/data_generation/visualisation"
                                      "/standard_male_override.csv")

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("states/StandardMale@0s.json", data_req_mgr):
        print("Unable to load initial state file")
        return

    # Get some data from the engine
    # results = pulse.pull_data()
    # print(results)

    # pulse.advance_time_s(600)
    # stable_results = pulse.pull_data()
    # data_req_mgr.to_console(stable_results)

    # Perform an action
    infusion = SESubstanceInfusion()
    infusion.set_comment("Patient receives an infusion of Propofol 2")
    infusion.set_substance("PropofolChange")
    infusion.get_rate().set_value(100, VolumePerTimeUnit.mL_Per_min)
    infusion.get_concentration().set_value(2, MassPerVolumeUnit.from_string("ug/mL"))
    pulse.process_action(infusion)

    # Advance some time and print out the vitals
    pulse.advance_time_s(600)

    infusion.get_rate().set_value(100, VolumePerTimeUnit.mL_Per_min)
    infusion.get_concentration().set_value(2, MassPerVolumeUnit.from_string("ug/mL"))
    pulse.process_action(infusion)

    # Advance some time and print out the vitals
    pulse.advance_time_s(300)
    # results = pulse.pull_data()
    # data_req_mgr.to_console(results)

    infusion.set_comment("Patient receives an infusion of Propofol 5")
    infusion.get_rate().set_value(100, VolumePerTimeUnit.mL_Per_min)
    infusion.get_concentration().set_value(5, MassPerVolumeUnit.from_string("ug/mL"))
    pulse.process_action(infusion)

    pulse.advance_time_s(600)

    infusion.get_rate().set_value(100, VolumePerTimeUnit.mL_Per_min)
    infusion.get_concentration().set_value(5, MassPerVolumeUnit.from_string("ug/mL"))
    pulse.process_action(infusion)

    pulse.advance_time_s(300)
    # results = pulse.pull_data()
    # data_req_mgr.to_console(results)

    infusion.set_comment("Patient receives an infusion of Propofol 2")
    infusion.get_rate().set_value(100, VolumePerTimeUnit.mL_Per_min)
    infusion.get_concentration().set_value(2, MassPerVolumeUnit.from_string("ug/mL"))
    pulse.process_action(infusion)

    pulse.advance_time_s(600)

    infusion.get_rate().set_value(100, VolumePerTimeUnit.mL_Per_min)
    infusion.get_concentration().set_value(2, MassPerVolumeUnit.from_string("ug/mL"))
    pulse.process_action(infusion)

    pulse.advance_time_s(300)

    results = pulse.pull_data()
    data_req_mgr.to_console(results)


propofol_infusion()

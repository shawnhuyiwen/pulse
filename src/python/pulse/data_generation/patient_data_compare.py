# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.engine import eSerializationFormat
from pulse.cdm.scalars import VolumePerTimeUnit, MassPerVolumeUnit
from pulse.cdm.patient_actions import SESubstanceInfusion
from pulse.engine.PulseEngine import PulseEngine
from pulse.cdm.engine import eSerializationFormat, SEDataRequestManager, SEDataRequest
from pulse.cdm.scalars import ElectricPotentialUnit, FrequencyUnit, LengthUnit, MassUnit, MassPerVolumeUnit, \
    PressureUnit, TemperatureUnit, TimeUnit, VolumeUnit, VolumePerTimeUnit


def get_patient_params():
    pulse = PulseEngine()

    data_requests = [
        SEDataRequest.create_physiology_request("HeartRate"),
        SEDataRequest.create_tissue_request("Muscle", "ExtracellularFluidVolume"),
        SEDataRequest.create_tissue_request("Tissue", "RespiratoryExchangeRatio"),
    ]
    data_req_mgr = SEDataRequestManager(data_requests)

    # data_req_mgr.set_results_filename("E:/PulseEngine/engine/src/python/pulse/data_generation/visualisation"
    #                                   "/underweight_male_sim.csv")

    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.serialize_from_file("./states/StandardMale@0s.json", data_req_mgr):
        print("Unable to load initial state file")
        return

    print("Patient file successfully loaded!")
    # Get some data from the engine
    results = pulse.pull_data()
    print(results)


get_patient_params()

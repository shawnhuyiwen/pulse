# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import sys
import json
from pulse.cpm.PulsePhysiologyEnginePool import PulsePhysiologyEnginePool
from pulse.cdm.engine import SEDataRequestManager, SEDataRequest, \
                             SEAdvanceTime
from pulse.cdm.patient import eSex, SEPatientConfiguration
from pulse.cdm.patient_actions import SEHemorrhage, eHemorrhageType, \
                                      SESubstanceBolus, eSubstance_Administration
from pulse.cdm.scalars import MassPerVolumeUnit, TimeUnit, VolumeUnit

def HowTo_PatientPool():
    # You may also specify how many threads to use via this ctor
    pool = PulsePhysiologyEnginePool()

    # Create a data request object that we will use for all patients
    # i.e. We want the same data from each of our patients
    # You can create individual requests for each patient if you want...
    # Data Requests are used to get access to the hundreds of parameters available in Pulse
    # To learn more about Data Requests please look at the data request section here:
    # https://pulse.kitware.com/_scenario_file.html
    data_requests = [
        SEDataRequest.create_physiology_request("HeartRate", unit="1/min"),
        SEDataRequest.create_physiology_request("ArterialPressure", unit="mmHg"),
        SEDataRequest.create_physiology_request("MeanArterialPressure", unit="mmHg"),
        SEDataRequest.create_physiology_request("SystolicArterialPressure", unit="mmHg"),
        SEDataRequest.create_physiology_request("DiastolicArterialPressure", unit="mmHg"),
        SEDataRequest.create_physiology_request("OxygenSaturation"),
        SEDataRequest.create_physiology_request("EndTidalCarbonDioxidePressure", unit="mmHg"),
        SEDataRequest.create_physiology_request("RespirationRate", unit="1/min"),
        SEDataRequest.create_physiology_request("SkinTemperature", unit="degC"),
        SEDataRequest.create_physiology_request("CardiacOutput", unit="L/min"),
        SEDataRequest.create_physiology_request("BloodVolume", unit="mL"),
    ]
    data_req_mgr = SEDataRequestManager(data_requests)

    # Create all your patients up front
    # You can create a new patient (See HowTo_EngineUse.py)
    # The append method will return the SEEnginePoolEngine
    # associated with the provided Engine Configuration
    # The SEEnginePoolEngine will have active/actions/results/logs/events data
    pe1 = pool.create_engine(1)
    pe1.engine_initialization.data_request_mgr = data_req_mgr
    #pe1.engine_initialization.state_filename = "./states/StandardMale@0s.json"
    pe1.engine_initialization.patient_configuration = SEPatientConfiguration()
    pe1.engine_initialization.patient_configuration.get_patient().set_name("patient_" + str(pe1.get_id()))
    pe1.engine_initialization.patient_configuration.get_patient().set_sex(eSex.Male)
    pe1.engine_initialization.log_filename = "./test_results/HowTo_PatientPool/engine_" + str(pe1.get_id()) + ".log"
    # Or you can use a preexisting state for a patient
    pe2 = pool.create_engine(2)
    pe2.engine_initialization.data_request_mgr = data_req_mgr
    pe2.engine_initialization.state_filename = "./states/StandardMale@0s.json"
    pe2.engine_initialization.log_filename = "./test_results/HowTo_PatientPool/engine_" + str(pe2.get_id()) + ".log"

    # Initialize all engines
    # initialize_engines will update engine data/logs/events
    if not pool.initialize_engines():
        print("Unable to load/stabilize any engine")
        return
    # True will be returned if at least 1 engine was initialized
    # Check the engines to handle any engines that had issues
    for e in pool.get_engines().values():
        if e.is_active:
            print("Engine "+str(e.get_id())+" values:")
            print(json.dumps(e.data_requested.values, indent=2))
        else:
            print("Engine "+str(e.get_id())+" failed to initialize")

    # Advance different amounts of time for each patient
    t1 = SEAdvanceTime()
    t1.get_time().set_value(4, TimeUnit.s)
    pe1.actions.append(t1)
    # Let's also give the 2nd patient a hemorrhage
    h2 = SEHemorrhage()
    h2.set_type(eHemorrhageType.External)
    h2.set_compartment("VenaCava")
    h2.get_severity().set_value(0.4)
    pe2.actions.append(h2)
    t2 = SEAdvanceTime()
    t2.get_time().set_value(15, TimeUnit.s)
    pe2.actions.append(t2)
    pool.process_actions()
    # process_actions will update engine data/logs/events if any engine had an SEAdvanceTime action

    # Examine each patient and apply any actions to a patient
    for e in pool.get_engines().values():
        if not e.is_active:
            print("Engine " + str(e.get_id()) + " is inactive")
            continue
        print("Engine " + str(e.get_id()) + " values:")
        print(json.dumps(e.data_requested.values, indent=2))
        if e.data_requested.get_value(3) < 80:
            bolus = SESubstanceBolus()
            bolus.set_comment("Patient receives injection of Epinephrine")
            bolus.set_admin_route(eSubstance_Administration.Intramuscular)
            bolus.set_substance("Epinephrine")
            bolus.get_dose().set_value(1.0, VolumeUnit.mL)
            bolus.get_concentration().set_value(1.1, MassPerVolumeUnit.ug_Per_L)
            e.actions.append(bolus)

    # Advance the same amount of time for each patient
    pool.advance_time_s(10)
    # advance_time_s will update engine data/logs/events
    for e in pool.get_engines().values():
        if e.is_active:
            print("Engine " + str(e.get_id()) + " values:")
            print(json.dumps(e.data_requested.values, indent=2))
            print("\n")
        else:
            print("Engine " + str(e.get_id()) + " is inactive")

HowTo_PatientPool()

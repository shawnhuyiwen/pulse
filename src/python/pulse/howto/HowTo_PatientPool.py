# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

import sys
from pulse.cpm.PulsePhysiologyEnginePool import PulsePhysiologyEnginePool
from pulse.cdm.engine import SEDataRequestManager, SEDataRequest, \
                             SEAdvanceTime, SEEngineInitialization
from pulse.cdm.patient import eSex, SEPatientConfiguration
from pulse.cdm.patient_actions import SESubstanceBolus, eSubstance_Administration
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
    e1 = SEEngineInitialization()
    e1.id = 1
    e1.patient_configuration = SEPatientConfiguration()
    e1.patient_configuration.get_patient().set_name("patient_" + str(e1.id))
    e1.patient_configuration.get_patient().set_sex(eSex.Male)
    e1.log_filename = "./pool/engine_" + str(e1.id) + ".log"
    pe1 = pool.append_engine(e1)
    # Or you can use a preexisting state for a patient
    e2 = SEEngineInitialization()
    e2.id = 2
    e2.state_filename = "./states/StandardMale@0s.json"
    e2.log_filename = "./pool/engine_" + str(e2.id) + ".log"
    pe2 = pool.append_engine(e2)

    # Initialize all engines, 1 per pool patient
    if not pool.initialize_engines():
        print("Unable to load/stabilize one or more engines")
        return
    # initialize_engines will update engine data/logs/events
    for e in pool.get_engines().values():
        if e.is_active:
            print("Engine "+str(e.get_id())+" values\n")
            data_req_mgr.to_console(e.results)
            print("\n")
        else:
            print("Engine "+str(e.get_id())+" failed to initialize\n")

    # Advance different amounts of time for each patient
    t1 = SEAdvanceTime()
    t1.get_time().set_value(4, TimeUnit.s)
    pe1.actions.append(t1)
    t2 = SEAdvanceTime()
    t2.get_time().set_value(14, TimeUnit.s)
    pe2.actions.append(t2)
    pool.process_actions()
    # process_actions will update engine data/logs/events if any engine had an SEAdvanceTime action

    # Examine each patient and apply any actions to a patient
    for p in pool.get_engines().values():
        if not e.is_active:
            print("Engine " + str(e.get_id()) + " is inactive\n")
            continue

        print("Engine " + str(e.get_id()) + " values\n")
        data_req_mgr.to_console(e.results)
        print("\n")
        if p.results[3] < 80:
            bolus = SESubstanceBolus()
            bolus.set_comment("Patient receives injection of Epinephrine")
            bolus.set_admin_route(eSubstance_Administration.Intramuscular)
            # bolus.get_admin_duration().set_value(2, TimeUnit.s) (optional)
            bolus.set_substance("Epinephrine")
            bolus.get_dose().set_value(1.0, VolumeUnit.mL)
            bolus.get_concentration().set_value(1.1, MassPerVolumeUnit.ug_Per_L)
            p.actions.append(bolus)

    # Advance the same amount of time for each patient
    pool.advance_time_s(10)
    # advance_time_s will update engine data/logs/events
    for e in pool.get_engines().values():
        if e.is_active:
            print("Engine " + str(e.get_id()) + " values\n")
            data_req_mgr.to_console(e.results)
            print("\n")
        else:
            print("Engine " + str(e.get_id()) + " is inactive\n")

HowTo_PatientPool()


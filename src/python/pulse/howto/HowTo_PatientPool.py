# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cpm.PulsePhysiologyEnginePool import PulsePhysiologyEnginePool
from pulse.cdm.engine import SEDataRequestManager, SEDataRequest, SEAdvanceTime
from pulse.cdm.patient import eSex, SEPatientConfiguration
from pulse.cdm.patient_actions import SESubstanceBolus, eSubstance_Administration
from pulse.cdm.scalars import MassPerVolumeUnit, TimeUnit, VolumeUnit

def HowTo_PatientPool():
    pool = PulsePhysiologyEnginePool()

    # Tell all engines to log to console (or not)
    pool.log_to_console(False)

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
    p1 = pool.create_patient(1)
    p1.patient_configuration = SEPatientConfiguration()
    p1.patient_configuration.get_patient().set_sex(eSex.Male)
    p1.log_filename = "./pool/p" + str(p1.get_id()) + ".log"
    # Or you can use a preexisting state for a patient
    p2 = pool.create_patient(2)
    p2.state_filename = "./states/StandardMale@0s.json"
    p1.log_filename = "./pool/p" + str(p2.get_id()) + ".log"

    # Initialize all engines, 1 per pool patient
    if not pool.initialize_engines():
        print("Unable to load/stabilize one or more engines")
        return
    # If you want to see which engine(s) failed, iterate the engines and any engine that is not active, failed

    # Pull data from all the engines
    pool.pull_data()
    # Results are updated on each pool patient object after the pool.pull_data call
    for p in pool.get_patients().values():
        print("Patient "+str(p.get_id())+" values\n")
        data_req_mgr.to_console(p.results)
        print("\n")

    # Advance the same amount of time for each patient
    pool.advance_time_s(5)

    for p in pool.get_patients().values():
        print("Patient "+str(p.get_id())+" values\n")
        data_req_mgr.to_console(p.results)
        print("\n")

    # Advance different amounts of time for each patient
    t1 = SEAdvanceTime()
    t1.get_time().set_value(4, TimeUnit.s)
    p1.actions.append(t1)
    t2 = SEAdvanceTime()
    t2.get_time().set_value(14, TimeUnit.s)
    p2.actions.append(t2)
    pool.process_actions()

    # Pull data from all the engines
    pool.pull_data()
    # Examine each patient and apply any actions to a patient
    for p in pool.get_patients().values():
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
    pool.advance_time_s(60)

HowTo_PatientPool()


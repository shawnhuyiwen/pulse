# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.engine.PulseEngine import PulseEngine
from pulse.cdm.ecmo_actions import SEECMOConfiguration
from pulse.cdm.scalars import FrequencyUnit, MassPerTimeUnit, MassPerVolumeUnit, \
                              PressureUnit, TimeUnit, VolumeUnit, VolumePerTimeUnit
from pulse.cdm.engine import SEDataRequest, SEDataRequestManager
from pulse.cdm.patient import eSex, SEPatient, SEPatientConfiguration

def HowTo_ECMO():
    pulse = PulseEngine()
    pulse.set_log_filename("./test_results/howto/HowTo_ECMO.py.log")
    pulse.log_to_console(True)

    data_requests = [
        SEDataRequest.create_physiology_request("BloodPH"),
        SEDataRequest.create_physiology_request("BloodVolume", unit=VolumeUnit.mL),
        SEDataRequest.create_physiology_request("CarbonDioxideSaturation", unit=VolumeUnit.mL),
        SEDataRequest.create_physiology_request("CardiacOutput", unit=VolumePerTimeUnit.L_Per_min),
        SEDataRequest.create_physiology_request("DiastolicArterialPressure", unit=PressureUnit.mmHg),
        SEDataRequest.create_physiology_request("EndTidalCarbonDioxidePressure", unit=PressureUnit.mmHg),
        SEDataRequest.create_physiology_request("HeartRate", unit=FrequencyUnit.Per_min),
        SEDataRequest.create_physiology_request("Hematocrit"),
        SEDataRequest.create_physiology_request("OxygenSaturation"),
        SEDataRequest.create_physiology_request("RespirationRate", unit=FrequencyUnit.Per_min),
        SEDataRequest.create_physiology_request("SystolicArterialPressure", unit=PressureUnit.mmHg),
        SEDataRequest.create_physiology_request("DiastolicArterialPressure", unit=PressureUnit.mmHg),
        SEDataRequest.create_physiology_request("TidalVolume", unit=VolumeUnit.mL),
        SEDataRequest.create_physiology_request("TotalPulmonaryVentilation", unit=VolumePerTimeUnit.L_Per_min),
        SEDataRequest.create_substance_request("Bicarbonate", "BloodConcentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_substance_request("CarbonDioxide", "AlveolarTransfer", unit=VolumePerTimeUnit.mL_Per_s),
        SEDataRequest.create_substance_request("Oxygen", "AlveolarTransfer", unit=VolumePerTimeUnit.mL_Per_s),
        SEDataRequest.create_substance_request("Sodium", "BloodConcentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("Aorta", "CarbonDioxide", "PartialPressure", unit=PressureUnit.mmHg),
        SEDataRequest.create_liquid_compartment_substance_request("Aorta", "Oxygen", "PartialPressure", unit=PressureUnit.mmHg),
        # ECMO Oxygenator Data
        SEDataRequest.create_liquid_compartment_request("ECMOOxygenator", "PH"),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOOxygenator", "Albumin", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOOxygenator", "Bicarbonate", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOOxygenator", "Calcium", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOOxygenator", "CarbonDioxide", "PartialPressure", unit=PressureUnit.mmHg),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOOxygenator", "Chloride", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOOxygenator", "Creatinine", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOOxygenator", "Glucose", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOOxygenator", "Lactate", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOOxygenator", "Oxygen", "PartialPressure", unit=PressureUnit.mmHg),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOOxygenator", "Potassium", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOOxygenator", "Sodium", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOOxygenator", "Urea", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        # Various Hb concentrations
        SEDataRequest.create_liquid_compartment_substance_request("ECMOOxygenator", "Hemoglobin", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOOxygenator", "Carbaminohemoglobin", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOOxygenator", "Carboxyhemoglobin", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOOxygenator", "OxyCarbaminohemoglobin", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOOxygenator", "Oxyhemoglobin", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
    ]

    data_mgr = SEDataRequestManager(data_requests)
    data_mgr.set_results_filename("./test_results/howto/HowTo_ECMO.py.csv")

    pc = SEPatientConfiguration()
    patient = pc.get_patient()
    patient.set_name("ECMO")
    patient.set_sex(eSex.Female)
    # NOTE: No data requests are being provided, so Pulse will return the default vitals data
    if not pulse.initialize_engine(pc, data_mgr):
        print("Unable to create/load patient, check the error")
        return

    pulse.advance_time_s(10)
    # Get the values of the data you requested at this time
    results = pulse.pull_data()
    # And write it out to the console
    data_mgr.to_console(results)


HowTo_ECMO()

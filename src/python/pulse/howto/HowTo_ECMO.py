# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.engine.PulseEngine import PulseEngine
from pulse.cdm.ecmo import eECMO_CannulationLocation
from pulse.cdm.ecmo_actions import SEECMOConfiguration
from pulse.cdm.scalars import FrequencyUnit, MassPerVolumeUnit, \
                              PressureUnit, VolumeUnit, VolumePerTimeUnit
from pulse.cdm.engine import SEDataRequest, SEDataRequestManager

def HowTo_ECMO():
    pulse = PulseEngine()
    pulse.set_log_filename("./test_results/howto/HowTo_ECMO.py.log")
    pulse.log_to_console(True)

    data_requests = [
        SEDataRequest.create_physiology_request("BloodPH"),
        SEDataRequest.create_physiology_request("BloodVolume", unit=VolumeUnit.mL),
        SEDataRequest.create_physiology_request("CarbonDioxideSaturation", unit=VolumeUnit.mL),
        SEDataRequest.create_physiology_request("CardiacOutput", unit=VolumePerTimeUnit.L_Per_min),
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
        SEDataRequest.create_liquid_compartment_request("ECMOBloodSamplingPort", "PH"),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOBloodSamplingPort", "Albumin", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOBloodSamplingPort", "Bicarbonate", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOBloodSamplingPort", "Calcium", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOBloodSamplingPort", "CarbonDioxide", "PartialPressure", unit=PressureUnit.mmHg),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOBloodSamplingPort", "Chloride", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOBloodSamplingPort", "Creatinine", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOBloodSamplingPort", "Glucose", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOBloodSamplingPort", "Lactate", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOBloodSamplingPort", "Oxygen", "PartialPressure", unit=PressureUnit.mmHg),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOBloodSamplingPort", "Potassium", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOBloodSamplingPort", "Sodium", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOBloodSamplingPort", "Urea", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        # Various Hb concentrations
        SEDataRequest.create_liquid_compartment_substance_request("ECMOBloodSamplingPort", "Hemoglobin", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOBloodSamplingPort", "Carbaminohemoglobin", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOBloodSamplingPort", "Carboxyhemoglobin", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOBloodSamplingPort", "OxyCarbaminohemoglobin", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
        SEDataRequest.create_liquid_compartment_substance_request("ECMOBloodSamplingPort", "Oxyhemoglobin", "Concentration", unit=MassPerVolumeUnit.g_Per_L),
    ]

    data_mgr = SEDataRequestManager(data_requests)
    data_mgr.set_results_filename("./test_results/howto/HowTo_ECMO.py.csv")

    if not pulse.serialize_from_file("./states/StandardMale@0s.json", data_mgr):
        print("Unable to load initial state file")
        return

    cfg = SEECMOConfiguration()
    settings = cfg.get_settings()
    settings.set_inflow_location(eECMO_CannulationLocation.InternalJugular)
    settings.set_outflow_location(eECMO_CannulationLocation.InternalJugular)
    settings.get_oxygenator_volume().set_value(500, VolumeUnit.mL)
    settings.get_transfusion_flow().set_value(5, VolumePerTimeUnit.mL_Per_s)
    settings.set_substance_compound("Saline")
    pulse.process_action(cfg)

    pulse.advance_time_s(30)
    # Get the values of the data you requested at this time
    results = pulse.pull_data()
    # And write it out to the console
    data_mgr.to_console(results)

    settings.clear()
    oxyhemoglobin = results.get(data_requests[36].to_string())[-1];
    bicarb = results.get("Bicarbonate - BloodConcentration (g/L)")[-1];
    settings.get_substance_concentration("Oxyhemoglobin").get_concentration().set_value(oxyhemoglobin+2, MassPerVolumeUnit.g_Per_L)
    settings.get_substance_concentration("Bicarbonate").get_concentration().set_value(bicarb+5, MassPerVolumeUnit.g_Per_L)
    pulse.process_action(cfg)

    pulse.advance_time_s(30)
    # Get the values of the data you requested at this time
    results = pulse.pull_data()
    # And write it out to the console
    data_mgr.to_console(results)


HowTo_ECMO()

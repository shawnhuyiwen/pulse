/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#define _USE_MATH_DEFINES

#include "EngineHowTo.h"
#include "PulseEngine.h"

// Include the various types you will be using in your code
#include "cdm/engine/SEConditionManager.h"
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/engine/SEEventManager.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/patient/SEPatient.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceFraction.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/system/equipment/ecmo/SEECMO.h"
#include "cdm/system/equipment/ecmo/actions/SEECMOConfiguration.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressurePerVolume.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarPressureTimePerVolumeArea.h"
#include "cdm/properties/SEScalarTemperature.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerPressure.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarVolumePerTimeArea.h"
#include "cdm/properties/SEScalarLengthPerTime.h"
#include "cdm/properties/SEScalar0To1.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for the ECMO
///
/// \details
/// Refer to the SEECMO class
//--------------------------------------------------------------------------------------------------
void HowToECMO()
{
  //Note: Setting circuit values (resistance/compliances/etc.) needs to be done in the engine code - they currently are not directly exposed
  
  std::stringstream ss;
  // Create a Pulse Engine and load the standard patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine();
  pe->GetLogger()->SetLogFile("./test_results/HowTo/HowTo_ECMO.cpp.log");
  
  pe->GetLogger()->Info("HowTo_ECMO");

  if (!pe->SerializeFromFile("./states/StandardMale@0s.json"))
  {
    pe->GetLogger()->Error("Unable to load initial state file");
    return;
  }

  // Patient Data
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("BloodPH");
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("BloodVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CarbonDioxideSaturation", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CardiacOutput", VolumePerTimeUnit::L_Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("EndTidalCarbonDioxidePressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("Hematocrit");
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalPulmonaryVentilation", VolumePerTimeUnit::L_Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest("Bicarbonate", "BloodConcentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest("CarbonDioxide", "AlveolarTransfer", VolumePerTimeUnit::mL_Per_s);
  pe->GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest("Oxygen", "AlveolarTransfer", VolumePerTimeUnit::mL_Per_s);
  pe->GetEngineTracker()->GetDataRequestManager().CreateSubstanceDataRequest("Sodium", "BloodConcentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, "CarbonDioxide", "PartialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::VascularCompartment::Aorta, "Oxygen", "PartialPressure", PressureUnit::mmHg);
  // ECMO Oxygenator Data
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "PH");
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "Albumin", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "Bicarbonate", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "Calcium", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "CarbonDioxide", "PartialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "Chloride", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "Creatinine", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "Glucose", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "Lactate", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "Oxygen", "PartialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "Potassium", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "Sodium", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "Urea", "Concentration", MassPerVolumeUnit::g_Per_L);
  // Various Hb concentrations
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "Hemoglobin", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "Carbaminohemoglobin", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "Carboxyhemoglobin", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "OxyCarbaminohemoglobin", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "Oxyhemoglobin", "Concentration", MassPerVolumeUnit::g_Per_L);
  
  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("./test_results/HowTo/HowTo_ECMO.cpp.csv");

  SEECMOConfiguration cfg(pe->GetLogger());
  SEECMOSettings& settings = cfg.GetSettings();
  settings.SetInflowLocation(eECMO_CannulationLocation::InternalJugular);
  settings.SetOutflowLocation(eECMO_CannulationLocation::InternalJugular);
  settings.GetOxygenatorVolume().SetValue(500, VolumeUnit::mL);
  settings.GetTransfusionFlow().SetValue(5, VolumePerTimeUnit::mL_Per_s);
  const SESubstanceCompound* saline = pe->GetSubstanceManager().GetCompound("Saline");
  settings.SetSubstanceCompound(*saline);
  pe->ProcessAction(cfg);

  AdvanceAndTrackTime_s(30.0, *pe);
  pe->GetEngineTracker()->LogRequestedValues(true);

  double oxyhemoglobin_val = pe->GetEngineTracker()->GetValue(*(pe->GetEngineTracker()->GetDataRequestManager().GetDataRequests()[36]));
  double bicarb_val = pe->GetEngineTracker()->GetValue(*(pe->GetEngineTracker()->GetDataRequestManager().GetDataRequests()[21]));
  const SESubstance* oxyhemoglobin = pe->GetSubstanceManager().GetSubstance("Oxyhemoglobin");
  const SESubstance* bicarb = pe->GetSubstanceManager().GetSubstance("Bicarbonate");
  settings.GetSubstanceConcentration(*oxyhemoglobin).GetConcentration().SetValue(oxyhemoglobin_val+2, MassPerVolumeUnit::g_Per_L);
  settings.GetSubstanceConcentration(*bicarb).GetConcentration().SetValue(bicarb_val+5, MassPerVolumeUnit::g_Per_L);
  pe->ProcessAction(cfg);

  AdvanceAndTrackTime_s(30.0, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);

  pe->GetLogger()->Info("Finished");
}

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

  // ECMO Compartments
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "InFlow", VolumePerTimeUnit::mL_Per_s);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::BloodSamplingPort, "OutFlow", VolumePerTimeUnit::mL_Per_s);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "InFlow", VolumePerTimeUnit::mL_Per_s);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "OutFlow", VolumePerTimeUnit::mL_Per_s);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "Sodium", "Concentration", MassPerVolumeUnit::g_Per_dL);
  
  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("./test_results/HowTo/HowTo_ECMO.cpp.csv");

  AdvanceAndTrackTime_s(5.0, *pe);

  // Attach ECMO from jugular to left leg
  SEECMOConfiguration cfg(pe->GetLogger());
  SEECMOSettings& settings = cfg.GetSettings();
  settings.SetInflowLocation(eECMO_CannulationLocation::InternalJugular);
  settings.SetOutflowLocation(eECMO_CannulationLocation::LeftFemoralVein);
  settings.GetOxygenatorVolume().SetValue(500, VolumeUnit::mL);
  settings.GetTransfusionFlow().SetValue(0.8, VolumePerTimeUnit::mL_Per_s);
  const SESubstanceCompound* saline = pe->GetSubstanceManager().GetCompound("Saline");
  settings.SetSubstanceCompound(*saline);
  const SESubstance* sodium = pe->GetSubstanceManager().GetSubstance("Sodium");
  settings.GetSubstanceConcentration(*sodium).GetConcentration().SetValue(0.7, MassPerVolumeUnit::g_Per_dL);
  pe->ProcessAction(cfg);

  AdvanceAndTrackTime_s(30.0, *pe);
  pe->GetEngineTracker()->LogRequestedValues(true);

  // Increase flow
  settings.GetTransfusionFlow().SetValue(4, VolumePerTimeUnit::mL_Per_s);
  pe->ProcessAction(cfg);

  AdvanceAndTrackTime_s(30.0, *pe);
  pe->GetEngineTracker()->LogRequestedValues(true);

  // Further increase flow
  settings.GetTransfusionFlow().SetValue(5, VolumePerTimeUnit::mL_Per_s);
  pe->ProcessAction(cfg);

  AdvanceAndTrackTime_s(30.0, *pe);
  pe->GetEngineTracker()->LogRequestedValues(true);

  // Detach ECMO
  settings.GetTransfusionFlow().SetValue(0, VolumePerTimeUnit::mL_Per_s);
  pe->ProcessAction(cfg);

  AdvanceAndTrackTime_s(30.0, *pe);
  pe->GetEngineTracker()->LogRequestedValues(true);

  pe->GetLogger()->Info("Finished");
}

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
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceFraction.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/system/equipment/ecmo/SEECMO.h"
#include "cdm/system/equipment/ecmo/actions/SEECMOConfiguration.h"
#include "cdm/patient/actions/SEDyspnea.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarElectricPotential.h"
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
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // Vitals Monitor Data
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("ExpiratoryPulmonaryResistance", PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("InspiratoryPulmonaryResistance", PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("PulmonaryCompliance", VolumePerPressureUnit::L_Per_cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalPulmonaryVentilation", VolumePerTimeUnit::L_Per_min);
  // Ventilator Monitor Data
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("AirwayPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("EndTidalCarbonDioxideFraction");
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("EndTidalCarbonDioxidePressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("EndTidalOxygenFraction");
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("EndTidalOxygenPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("ExpiratoryFlow", VolumePerTimeUnit::L_Per_s);
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("ExpiratoryTidalVolume", VolumeUnit::L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("InspiratoryExpiratoryRatio");
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("InspiratoryFlow", VolumePerTimeUnit::L_Per_s);
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("InspiratoryTidalVolume", VolumeUnit::L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("IntrinsicPositiveEndExpiredPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("LeakFraction");
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("MeanAirwayPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("PeakInspiratoryPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("PlateauPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("PositiveEndExpiratoryPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("RespirationRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("TidalVolume", VolumeUnit::L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("TotalLungVolume", VolumeUnit::L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateECMODataRequest("TotalPulmonaryVentilation", VolumePerTimeUnit::L_Per_s);
  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("./test_results/HowTo/HowTo_ECMO.cpp.csv");

  //Dyspnea
  SEDyspnea Dyspnea;
  Dyspnea.GetSeverity().SetValue(1.0);
  pe->ProcessAction(Dyspnea);
  AdvanceAndTrackTime_s(10.0, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);

  pe->GetLogger()->Info("Finished");
}

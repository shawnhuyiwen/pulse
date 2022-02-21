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

  SEPatientConfiguration pc;
  SEPatient& patient = pc.GetPatient();
  patient.SetName("ECMO");
  patient.SetSex(ePatient_Sex::Female);
  if (!pe->InitializeEngine(pc))
  //if (!pe->SerializeFromFile("./states/StandardMale@0s.json"))
  {
    pe->GetLogger()->Error("Could not create/load patient, check the error");
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
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "PH");
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "Albumin", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "Bicarbonate", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "Calcium", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "CarbonDioxide", "PartialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "Chloride", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "Creatinine", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "Glucose", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "Lactate", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "Oxygen", "PartialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "Potassium", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "Sodium", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "Urea", "Concentration", MassPerVolumeUnit::g_Per_L);
  // Various Hb concentrations
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "Hemoglobin", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "Carbaminohemoglobin", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "Carboxyhemoglobin", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "OxyCarbaminohemoglobin", "Concentration", MassPerVolumeUnit::g_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateLiquidCompartmentDataRequest(pulse::ECMOCompartment::Oxygenator, "Oxyhemoglobin", "Concentration", MassPerVolumeUnit::g_Per_L);
  
  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("./test_results/HowTo/HowTo_ECMO.cpp.csv");

  AdvanceAndTrackTime_s(10.0, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);

  pe->GetLogger()->Info("Finished");
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"
#include "PulseEngine.h"

// Include the various types you will be using in your code
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/compartment/SECompartmentManager.h"
#include "cdm/compartment/fluid/SELiquidCompartment.h"
#include "cdm/system/physiology/SECardiovascularSystem.h"
#include "cdm/system/physiology/SERespiratorySystem.h"
#include "cdm/system/physiology/SEBloodChemistrySystem.h"
#include "cdm/patient/actions/SEArrhythmia.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarLength.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarPower.h"
#include "cdm/properties/SEScalarAmountPerVolume.h"
#include "cdm/properties/SEScalar0To1.h"

void HowToArrythmia()
{
  std::stringstream ss;
  // Create a Pulse Engine and load the standard patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine();
  pe->GetLogger()->SetLogFile("./test_results/howto/HowToArrythmia.log");
  
  pe->GetLogger()->Info("HowToArrythmia");
  if (!pe->SerializeFromFile("./states/StandardMale@0s.json"))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects 
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");
  pe->GetEngineTracker()->GetDataRequestManager().CreateECGDataRequest("Lead3ElectricPotential", ElectricPotentialUnit::mV);

  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("./test_results/howto/HowToArrythmia.csv");

  AdvanceAndTrackTime_s(.5, *pe);
  pe->GetLogger()->Info("The patient is nice and healthy");
  pe->GetEngineTracker()->LogRequestedValues(false);
  
  // Create an SEArrythmia object
  SEArrhythmia arrhythmia;
  
  //arrhythmia.SetRhythm(eHeartRhythm::StableVentricularTachycardia);
  //pe->ProcessAction(arrhythmia);
  //pe->GetLogger()->Info("Giving the patient stable ventricular tachycardia.");
  //// Advance time to see how the injury affects the patient
  //AdvanceAndTrackTime_s(10, *pe);
  //pe->GetLogger()->Info(std::stringstream() << "The patient has had stable ventricular tachycardia for 10s, not doing well...");
  //pe->GetEngineTracker()->LogRequestedValues(false);
  //
  //// You can go back to normal sinus.
  //arrhythmia.SetRhythm(eHeartRhythm::NormalSinus);
  //pe->ProcessAction(arrhythmia);
  //pe->GetLogger()->Info("Removing the arrythmia.");
  //AdvanceAndTrackTime_s(30, *pe);
  //pe->GetLogger()->Info(std::stringstream() << "The patient's arrythmia has been removed for 30s; patient is much better");
  //pe->GetEngineTracker()->LogRequestedValues(false);

  arrhythmia.SetRhythm(eHeartRhythm::FineVentricularFibrillation);
  pe->ProcessAction(arrhythmia);
  pe->GetLogger()->Info("Giving the patient course ventricular fibrillation.");

  AdvanceAndTrackTime_s(90, *pe);
  pe->GetLogger()->Info(std::stringstream() << "The patient has had course ventricular fibrillation for 90 s");
  pe->GetEngineTracker()->LogRequestedValues(false);


  pe->GetLogger()->Info("Finished");
}

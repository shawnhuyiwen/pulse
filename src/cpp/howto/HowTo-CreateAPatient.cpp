/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"

// Include the various types you will be using in your code
#include "patient/SEPatient.h"
#include "patient/assessments/SEPulmonaryFunctionTest.h"
#include "compartment/SECompartmentManager.h"
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SEEnergySystem.h"
#include "system/physiology/SERespiratorySystem.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstance.h"
#include "engine/SEEngineTracker.h"
#include "engine/SEPatientConfiguration.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEFunctionVolumeVsTime.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarLength.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Creating a patient
///
/// \details
/// Creating a customized patient in Pulse
//--------------------------------------------------------------------------------------------------
void HowToCreateAPatient()
{
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine();
  pe->GetLogger()->SetLogFile("./test_results/HowTo_CreateAPatient.log");
  pe->GetLogger()->Info("HowTo_CreateAPatient");

  SEPatientConfiguration pc;
  SEPatient& patient = pc.GetPatient();
  patient.SetName("HowToCreateAPatient");
  //Patient sex is the only thing that is absolutely required to be set.
  //All value not explicitly set based or standard values or calculations.
  //If you do something out of bounds or set something you're not allowed to, it will alert you with a warning/error.
  patient.SetSex(ePatient_Sex::Male);
  patient.GetAge().SetValue(44, TimeUnit::yr);
  patient.GetWeight().SetValue(170, MassUnit::lb);
  patient.GetHeight().SetValue(71, LengthUnit::in);
  patient.GetBodyFatFraction().SetValue(0.21);
  patient.GetDiastolicArterialPressureBaseline().SetValue(74, PressureUnit::mmHg);
  patient.GetHeartRateBaseline().SetValue(72, FrequencyUnit::Per_min);
  patient.GetRespirationRateBaseline().SetValue(16, FrequencyUnit::Per_min);
  patient.GetSystolicArterialPressureBaseline().SetValue(114, PressureUnit::mmHg);

  // You can save off the patient if you want to use it later
  patient.SerializeToFile("./patients/HowToCreateAPatient.json");


  if (!pe->InitializeEngine(pc))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // You can save off the initial patient state if you want to use it later
  pe->SerializeToFile("./states/HowToCreateAPatient@0s.json");
}
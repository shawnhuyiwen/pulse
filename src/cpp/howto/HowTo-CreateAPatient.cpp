/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"
#include "PulseEngine.h"

// Include the various types you will be using in your code
#include "cdm/patient/SEPatient.h"
#include "cdm/patient/conditions/SEAcuteRespiratoryDistressSyndrome.h"
#include "cdm/compartment/SECompartmentManager.h"
#include "cdm/system/physiology/SEBloodChemistrySystem.h"
#include "cdm/system/physiology/SECardiovascularSystem.h"
#include "cdm/system/physiology/SEEnergySystem.h"
#include "cdm/system/physiology/SERespiratorySystem.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/engine/SEConditionManager.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTemperature.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEFunctionVolumeVsTime.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarLength.h"

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
  patient.SetSex(ePatient_Sex::Female);
  // Jane
  //patient.GetAge().SetValue(18, TimeUnit::yr);
  //patient.GetWeight().SetValue(81, MassUnit::lb);
  //patient.GetHeight().SetValue(60, LengthUnit::in);
  //patient.GetBodyFatFraction().SetValue(0.179);
  //patient.GetDiastolicArterialPressureBaseline().SetValue(75, PressureUnit::mmHg);
  //patient.GetHeartRateBaseline().SetValue(58, FrequencyUnit::Per_min);
  //patient.GetRespirationRateBaseline().SetValue(19.5, FrequencyUnit::Per_min);
  //patient.GetSystolicArterialPressureBaseline().SetValue(118, PressureUnit::mmHg);

  // Carol
  //patient.GetAge().SetValue(49, TimeUnit::yr);
  //patient.GetWeight().SetValue(120, MassUnit::lb);
  //patient.GetHeight().SetValue(63, LengthUnit::in);
  //patient.GetBodyFatFraction().SetValue(0.219);
  //patient.GetDiastolicArterialPressureBaseline().SetValue(75, PressureUnit::mmHg);
  //patient.GetHeartRateBaseline().SetValue(58, FrequencyUnit::Per_min);
  //patient.GetRespirationRateBaseline().SetValue(19.5, FrequencyUnit::Per_min);
  //patient.GetSystolicArterialPressureBaseline().SetValue(118, PressureUnit::mmHg);

  // You can save off the patient if you want to use it later
  patient.SerializeToFile("./patients/HowToCreateAPatient.json");

  SEAcuteRespiratoryDistressSyndrome& ards = pc.GetConditions().GetAcuteRespiratoryDistressSyndrome();
  ards.GetSeverity().SetValue(0.9);
  ards.GetLeftLungAffected().SetValue(1.0);
  ards.GetRightLungAffected().SetValue(1.0);


  if (!pe->InitializeEngine(pc))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // You can save off the initial patient state if you want to use it later
  pe->SerializeToFile("./states/HowToCreateAPatient@0s.json");
}
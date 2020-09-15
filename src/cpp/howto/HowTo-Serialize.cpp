/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "EngineHowTo.h"

#include "properties/SEScalarTime.h"
#include "engine/SEActionManager.h"

#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SEEnergySystem.h"
#include "system/physiology/SERespiratorySystem.h"
#include "patient/actions/SEAirwayObstruction.h"
#include "patient/actions/SEHemorrhage.h"
#include "patient/actions/SETensionPneumothorax.h"

#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Serializing a patient state to use later in your Pulse based application
///
/// \details
/// This will show you how to setup a patient and save it out for use in your application
//--------------------------------------------------------------------------------------------------
void HowToSerialize()
{
  std::stringstream ss;
  // Create a Pulse Engine and load the standard patient
  // This is a healty patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine();
  pe->GetLogger()->SetLogFile("./test_results/HowTo_Serialize.log");
  if (!pe->SerializeFromFile("./states/Soldier@0s.json"))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  pe->GetLogger()->Info("Healthy patient vitals");
  pe->GetLogger()->Info(std::stringstream() << "Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Arterial Pressure : " << pe->GetCardiovascularSystem()->GetArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());
  pe->GetLogger()->Info(std::stringstream() << "End Tidal Carbon Dioxide Fraction : " << pe->GetRespiratorySystem()->GetEndTidalCarbonDioxideFraction());
  pe->GetLogger()->Info(std::stringstream() << "End Tidal Carbon Dioxide Pressure : " << pe->GetRespiratorySystem()->GetEndTidalCarbonDioxidePressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Skin Temperature : " << pe->GetEnergySystem()->GetSkinTemperature(TemperatureUnit::C) << "C");
  pe->GetLogger()->Info(std::stringstream() << "Blood Volume : " << pe->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);

  // Next we will want to make the patient unhealthy in some way.
  // This will allow you to start your application with unhealty state

  // Let's mimic a battle field injury

  // Add a Hemorrhage
  SEHemorrhage hemorrhageLeg;
  hemorrhageLeg.SetType(eHemorrhage_Type::External);
  hemorrhageLeg.SetCompartment(pulse::VascularCompartment::RightLeg);//the location of the hemorrhage
  hemorrhageLeg.GetSeverity().SetValue(0.5);//the severity of hemorrhage
  pe->ProcessAction(hemorrhageLeg);

  // Add a Tension Pnuemothorax
  SETensionPneumothorax pneumo;
  pneumo.SetSide(eSide::Right);
  pneumo.SetType(eGate::Closed);
  pneumo.GetSeverity().SetValue(0.5);
  pe->ProcessAction(pneumo);

  // Add an Airway Obstruction
  SEAirwayObstruction obstruction;
  obstruction.GetSeverity().SetValue(0.25);
  pe->ProcessAction(obstruction);

  // Now let's advance 5 minutes with these actions to get the patient into a injured state
  pe->AdvanceModelTime(5, TimeUnit::min);

  pe->GetLogger()->Info("Wounded patient vitals");
  pe->GetLogger()->Info(std::stringstream() << "Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Arterial Pressure : " << pe->GetCardiovascularSystem()->GetArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());
  pe->GetLogger()->Info(std::stringstream() << "End Tidal Carbon Dioxide Fraction : " << pe->GetRespiratorySystem()->GetEndTidalCarbonDioxideFraction());
  pe->GetLogger()->Info(std::stringstream() << "End Tidal Carbon Dioxide Pressure : " << pe->GetRespiratorySystem()->GetEndTidalCarbonDioxidePressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Skin Temperature : " << pe->GetEnergySystem()->GetSkinTemperature(TemperatureUnit::C) << "C");
  pe->GetLogger()->Info(std::stringstream() << "Blood Volume : " << pe->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);

  // Save this state out.
  // You an then load this state in your application
  pe->SerializeToFile("./WoundedSoldier.json");
}
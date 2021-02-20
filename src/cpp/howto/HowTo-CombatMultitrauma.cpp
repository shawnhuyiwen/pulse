/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"

// Include the various types you will be using in your code
#include "engine/SEDataRequestManager.h"
#include "patient/actions/SEHemorrhage.h"
#include "patient/actions/SETensionPneumothorax.h"
#include "patient/actions/SENeedleDecompression.h"
#include "patient/actions/SESubstanceBolus.h"
#include "patient/actions/SESubstanceCompoundInfusion.h"
#include "substance/SESubstanceManager.h"
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SERespiratorySystem.h"
#include "system/physiology/SETissueSystem.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEFunctionVolumeVsTime.h"
#include "properties/SEScalar0To1.h"
#include "engine/SEEngineTracker.h"
#include "compartment/SECompartmentManager.h"

void CreateState()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine();
  pe->GetLogger()->SetLogFile("./test_results/HowTo_CombatMultitrauma_CreateState.log");
  pe->GetLogger()->Info("HowTo_CombatMultitrauma_CreateState");

  // Load the Soldier patient
  // You can alternatively define your own patient (see HowTo-CreateAPatient) and apply conditions (see HowTo-LobarPneumonia) 
  // This would require runnning stabilization rather than loading an existing state
  if (!pe->SerializeFromFile("./states/Soldier@0s.json"))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  HowToTracker tracker(*pe);

  // Create data requests for each value that should be written to the output log as the engine is executing
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("BloodVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartStrokeVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("ExtravascularFluidVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("ArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CardiacOutput", VolumePerTimeUnit::L_Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HemoglobinContent", MassUnit::g);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CentralVenousPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::mL);

  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("HowToCombatMultitrauma.csv");

  pe->GetLogger()->Info("Patient healthy state");
  pe->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());
  pe->GetLogger()->Info(std::stringstream() << "Blood Volume : " << pe->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Heart Stroke Volume : " << pe->GetCardiovascularSystem()->GetHeartStrokeVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "Extravascular Fluid Volume : " << pe->GetTissueSystem()->GetExtravascularFluidVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Cardiac Output : " << pe->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() << "Hemoglobin Content : " << pe->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
  pe->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);

  // Give insults
  SETensionPneumothorax pneumo;
  pneumo.SetType(eGate::Closed);
  pneumo.GetSeverity().SetValue(0.75);
  pneumo.SetSide(eSide::Left);
  pe->ProcessAction(pneumo);

  SEHemorrhage hemorrhageLeg;
  hemorrhageLeg.SetType(eHemorrhage_Type::External);
  hemorrhageLeg.SetCompartment(pulse::VascularCompartment::RightLeg);//the location of the hemorrhage
  hemorrhageLeg.GetSeverity().SetValue(0.8);//the severity of hemorrhage
  pe->ProcessAction(hemorrhageLeg);

  SEHemorrhage hemorrhageVenaCava;
  hemorrhageVenaCava.SetType(eHemorrhage_Type::External);
  hemorrhageVenaCava.SetCompartment(pulse::VascularCompartment::VenaCava);//the location of the hemorrhage
  hemorrhageVenaCava.GetSeverity().SetValue(0.2);//the severity of hemorrhage
  pe->ProcessAction(hemorrhageVenaCava);

  //Advance the engine to the point we would like to load later
  tracker.AdvanceModelTime(60);

  pe->GetLogger()->Info("Patient injured state at serialization");
  pe->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());
  pe->GetLogger()->Info(std::stringstream() << "Blood Volume : " << pe->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Heart Stroke Volume : " << pe->GetCardiovascularSystem()->GetHeartStrokeVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "Extravascular Fluid Volume : " << pe->GetTissueSystem()->GetExtravascularFluidVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Cardiac Output : " << pe->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() << "Hemoglobin Content : " << pe->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
  pe->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);

  // Save this state out.
  // You an then load this state in your application
  pe->SerializeToFile("./states/CombatMultirauma_Initial_Injuries.json");

  pe->GetLogger()->Info("State saved");
}

void LoadState()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine();
  pe->GetLogger()->SetLogFile("./test_results/HowTo_CombatMultitrauma_LoadState.log");
  pe->GetLogger()->Info("HowTo_CombatMultitrauma_LoadState");

  //Load the injured state we created
  if (!pe->SerializeFromFile("./states/CombatMultirauma_Initial_Injuries.json"))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  pe->GetLogger()->Info("State loaded");

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  HowToTracker tracker(*pe);

  pe->GetLogger()->Info("Patient loaded injured state");
  pe->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());
  pe->GetLogger()->Info(std::stringstream() << "Blood Volume : " << pe->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Heart Stroke Volume : " << pe->GetCardiovascularSystem()->GetHeartStrokeVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "Extravascular Fluid Volume : " << pe->GetTissueSystem()->GetExtravascularFluidVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Cardiac Output : " << pe->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() << "Hemoglobin Content : " << pe->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
  pe->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);

  // Reduce the bleeding by applying manual pressure
  // Just set the hemorrhage rates lower
  SEHemorrhage hemorrhageLeg;
  hemorrhageLeg.SetType(eHemorrhage_Type::External);
  hemorrhageLeg.SetCompartment(pulse::VascularCompartment::RightLeg);//the location of the hemorrhage
  hemorrhageLeg.GetSeverity().SetValue(0.15);//the severity of hemorrhage
  pe->ProcessAction(hemorrhageLeg);

  SEHemorrhage hemorrhageVenaCava;
  hemorrhageVenaCava.SetType(eHemorrhage_Type::External);
  hemorrhageVenaCava.SetCompartment(pulse::VascularCompartment::VenaCava);//the location of the hemorrhage
  hemorrhageVenaCava.GetSeverity().SetValue(0);//the severity of hemorrhage
  pe->ProcessAction(hemorrhageVenaCava);

  tracker.AdvanceModelTime(60);

  // Needle Decompression to help with pneumothorax
  SENeedleDecompression needleDecomp;
  needleDecomp.SetState(eSwitch::On);
  needleDecomp.SetSide(eSide::Left);
  pe->ProcessAction(needleDecomp);

  // Advance the engine while you prepare to treat the patient
  tracker.AdvanceModelTime(60.0 * 4.0); //4 min

  // Apply a tournaquet and stop the bleeding completely
  hemorrhageLeg.SetType(eHemorrhage_Type::External);
  hemorrhageLeg.SetCompartment(pulse::VascularCompartment::RightLeg);//the location of the hemorrhage
  hemorrhageLeg.GetSeverity().SetValue(0);//the severity of hemorrhage
  pe->ProcessAction(hemorrhageLeg);
   
  tracker.AdvanceModelTime(30);

  // Give an IV
  const SESubstanceCompound* saline = pe->GetSubstanceManager().GetCompound("Saline");
  SESubstanceCompoundInfusion iVSaline(*saline);
  iVSaline.GetBagVolume().SetValue(500, VolumeUnit::mL);//the total volume in the bag of Saline
  iVSaline.GetRate().SetValue(100, VolumePerTimeUnit::mL_Per_min);//The rate to admnister the compound in the bag in this case saline
  pe->ProcessAction(iVSaline);

  tracker.AdvanceModelTime(60.0 * 2.0); //2 min

  // Provide morphine
  const SESubstance* morphine = pe->GetSubstanceManager().GetSubstance("Morphine");
  SESubstanceBolus bolus(*morphine);
  bolus.GetConcentration().SetValue(1000, MassPerVolumeUnit::ug_Per_mL);
  bolus.GetDose().SetValue(5, VolumeUnit::mL);
  bolus.SetAdminRoute(eSubstanceAdministration_Route::Intravenous);
  pe->ProcessAction(bolus);

  tracker.AdvanceModelTime(60.0 * 3.0); //3 min
   
  pe->GetLogger()->Info("Patient state after interventions");
  pe->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());
  pe->GetLogger()->Info(std::stringstream() << "Blood Volume : " << pe->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Heart Stroke Volume : " << pe->GetCardiovascularSystem()->GetHeartStrokeVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "Extravascular Fluid Volume : " << pe->GetTissueSystem()->GetExtravascularFluidVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Cardiac Output : " << pe->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() << "Hemoglobin Content : " << pe->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
  pe->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);

  // Save this state out.
  // You an then load this state in your application
  pe->SerializeToFile("./states/CombatMultirauma_Treated_Injuries.json");

  pe->GetLogger()->Info("Finished");
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for serializing and loading a showcase scenario
///
/// \details
/// This follows the Combat Multitrauma showcase scenario. Validation is included in the system
/// methodology document.
///
//--------------------------------------------------------------------------------------------------
void HowToCombatMultitrauma()
{
  // Store the injured patient state for loading later
  CreateState();

  // Load the stored injured state and perform interventions
  LoadState();
}

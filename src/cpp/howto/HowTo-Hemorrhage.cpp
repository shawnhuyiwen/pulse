/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"

// Include the various types you will be using in your code
#include "engine/SEDataRequestManager.h"
#include "engine/SEEngineTracker.h"
#include "engine/SEActionManager.h"
#include "engine/SEPatientActionCollection.h"
#include "compartment/SECompartmentManager.h"
#include "patient/actions/SEHemorrhage.h"
#include "patient/actions/SESubstanceCompoundInfusion.h"
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SEEnergySystem.h"
#include "system/physiology/SERespiratorySystem.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstanceCompound.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for applying a Hemorrhage insult to the patient
///
/// \details
/// Refer to the SEHemorrhage class
/// Refer to the SESubstanceManager class
/// Refer to the SESubstanceIVFluids class for applying an IV to the patient
//--------------------------------------------------------------------------------------------------
void HowToHemorrhage() 
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine();
  pe->GetLogger()->LogToConsole(true);
  pe->GetLogger()->SetLogFile("./test_results/HowTo_Hemorrhage.log");
  pe->GetLogger()->Info("HowTo_Hemorrhage");
  if (!pe->SerializeFromFile("./states/Gus@1620.36s.pbb"))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  HowToTracker tracker(*pe);

  // Create data requests for each value that should be written to the output log as the engine is executing
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("BloodVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CardiacOutput", VolumePerTimeUnit::mL_Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HemoglobinContent",MassUnit::g);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalHemorrhageRate", VolumePerTimeUnit::mL_Per_s);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalHemorrhagedVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreateActionCompartmentDataRequest("Hemorrhage", pulse::VascularCompartment::RightLeg, "FlowRate", VolumePerTimeUnit::mL_Per_s);
  pe->GetEngineTracker()->GetDataRequestManager().CreateActionCompartmentDataRequest("Hemorrhage", pulse::VascularCompartment::RightLeg, "TotalBloodLost", VolumeUnit::mL);
  
  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("./test_results/HowToHemorrhage.csv");
  
  pe->GetLogger()->Info("The patient is nice and healthy");
  pe->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << pe->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() <<"Hemoglobin Content : " << pe->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
  pe->GetLogger()->Info(std::stringstream() <<"Blood Volume : " << pe->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() <<"Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Total Hemorrhage Rate : " << pe->GetCardiovascularSystem()->GetTotalHemorrhageRate(VolumePerTimeUnit::mL_Per_s) << "mL/s");
  pe->GetLogger()->Info(std::stringstream() << "Total Hemorrhaged Volume : " << pe->GetCardiovascularSystem()->GetTotalHemorrhagedVolume(VolumeUnit::mL) << "mL");

  // Hemorrhage Starts - instantiate a hemorrhage action and have the engine process it
  SEHemorrhage hemorrhageLeg;
  hemorrhageLeg.SetType(eHemorrhage_Type::External);
  hemorrhageLeg.SetCompartment(pulse::VascularCompartment::RightLeg);//the location of the hemorrhage
  hemorrhageLeg.GetSeverity().SetValue(0.8);
  // Optionally, You can set the flow rate of the hemorrhage,
  // This needs to be provided the proper flow rate associated with the anatomy
  // This is implemented as a flow source, this rate will be constant, and will not be affected by dropping blood pressures
  // It is intended to interact with sensors or with something continuously monitoring physiology and updating the flow
  //hemorrhageLeg.GetFlowRate().SetValue(250,VolumePerTimeUnit::mL_Per_min);//the rate of hemorrhage
  pe->ProcessAction(hemorrhageLeg);

  // Advance some time to let the body bleed out a bit
  if(!tracker.AdvanceModelTime(300)) // Check the return of advance time, if your hemorrhage is too extreme, the engine will enter an unsolvable/irreversable state
  {
    pe->GetLogger()->Fatal("Unable to advance engine time");
    return;
  }

  // You can get the current flow rate based on the severity, as well as the total blood lost from each hemorrhage via the action in the action manager
  // Note this is a copy of hemorrhageLeg internal to the engine
  auto hemorrhages = pe->GetActionManager().GetPatientActions().GetHemorrhages();
  for (auto h : hemorrhages)
    std::cout << *h << "\n";


  pe->GetLogger()->Info("The patient has been hemorrhaging for 300s");
  pe->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << pe->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() <<"Hemoglobin Content : " << pe->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
  pe->GetLogger()->Info(std::stringstream() <<"Blood Volume : " << pe->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() <<"Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Total Hemorrhage Rate : " << pe->GetCardiovascularSystem()->GetTotalHemorrhageRate(VolumePerTimeUnit::mL_Per_s) << "mL/s");
  pe->GetLogger()->Info(std::stringstream() << "Total Hemorrhaged Volume : " << pe->GetCardiovascularSystem()->GetTotalHemorrhagedVolume(VolumeUnit::mL) << "mL");

  // Hemorrhage is sealed
  hemorrhageLeg.SetType(eHemorrhage_Type::External);
  hemorrhageLeg.SetCompartment(pulse::VascularCompartment::RightLeg);//location of hemorrhage
  //hemorrhageLeg.GetSeverity().SetValue(0.);// Stop the hemorrhage
  // You can switch between severity and flow if you want,
  // But you will need to invalidate the severity for the flow to be used
  // If provided both severity and flow, Pulse will use the severity
  hemorrhageLeg.GetSeverity().Invalidate();
  hemorrhageLeg.GetFlowRate().SetValue(0,VolumePerTimeUnit::mL_Per_min);//rate is set to 0 to close the bleed
  pe->ProcessAction(hemorrhageLeg);
  
  
  // Advance some time while the medic gets the drugs ready
  if(!tracker.AdvanceModelTime(100))
  {
    pe->GetLogger()->Fatal("Unable to advance engine time");
    return;
  }

  pe->GetLogger()->Info("The patient has NOT been hemorrhaging for 100s");
  pe->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << pe->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() <<"Hemoglobin Content : " << pe->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
  pe->GetLogger()->Info(std::stringstream() <<"Blood Volume : " << pe->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() <<"Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Total Hemorrhage Rate : " << pe->GetCardiovascularSystem()->GetTotalHemorrhageRate(VolumePerTimeUnit::mL_Per_s) << "mL/s");
  pe->GetLogger()->Info(std::stringstream() << "Total Hemorrhaged Volume : " << pe->GetCardiovascularSystem()->GetTotalHemorrhagedVolume(VolumeUnit::mL) << "mL");

  // Patient is stabilizing, but not great

  // Let's administer a saline drip, we need to get saline from the substance maganer
  const SESubstanceCompound* saline = pe->GetSubstanceManager().GetCompound("Saline");
  SESubstanceCompoundInfusion iVSaline(*saline);
  iVSaline.GetBagVolume().SetValue(500,VolumeUnit::mL);//the total volume in the bag of Saline
  iVSaline.GetRate().SetValue(100,VolumePerTimeUnit::mL_Per_min);//The rate to admnister the compound in the bag in this case saline
  pe->ProcessAction(iVSaline);

  if (!tracker.AdvanceModelTime(400))
  {
    pe->GetLogger()->Fatal("Unable to advance engine time");
    return;
  }

  pe->GetLogger()->Info("The patient has been getting fluids for the past 400s");
  pe->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << pe->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  pe->GetLogger()->Info(std::stringstream() <<"Hemoglobin Content : " << pe->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
  pe->GetLogger()->Info(std::stringstream() <<"Blood Volume : " << pe->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() <<"Mean Arterial Pressure : " << pe->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Total Hemorrhage Rate : " << pe->GetCardiovascularSystem()->GetTotalHemorrhageRate(VolumePerTimeUnit::mL_Per_s) << "mL/s");
  pe->GetLogger()->Info(std::stringstream() << "Total Hemorrhaged Volume : " << pe->GetCardiovascularSystem()->GetTotalHemorrhagedVolume(VolumeUnit::mL) << "mL");
  pe->GetLogger()->Info("Finished");
}

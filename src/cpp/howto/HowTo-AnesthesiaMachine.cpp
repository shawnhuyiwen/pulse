/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"

// Include the various types you will be using in your code
#include "engine/SEDataRequestManager.h"
#include "engine/SEEngineTracker.h"
#include "patient/actions/SESubstanceBolus.h"
#include "system/equipment/anesthesia_machine/SEAnesthesiaMachine.h"
#include "system/equipment/anesthesia_machine/SEAnesthesiaMachineOxygenBottle.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineConfiguration.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineMaskLeak.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineOxygenWallPortPressureLoss.h"
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SERespiratorySystem.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalar0To1.h"

//---------------------------------------------------------------------------------------------------------------------
/// \brief
/// Usage for connecting the anesthesia machine to a patient
///
/// \details
/// Use an SESubstanceBolus action to stop the patient from breathing on their own
/// Use an SEAnesthesiaMachineConfiguration action to configure and turn on the anesthesia machine
/// Use an SEAnesthesiaMachineVentilatorMask action to place the mash and allow the machine to breath for the patient
/// End of example shows how to turn things off
//---------------------------------------------------------------------------------------------------------------------
void HowToAnesthesiaMachine()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine();
  pe->GetLogger()->SetLogFile("./test_results/HowTo_AnesthesiaMachine.log");
  pe->GetLogger()->Info("HowTo_AnesthesiaMachine");
  if (!pe->SerializeFromFile("./states/StandardMale@0s.json", JSON))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

    // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  HowToTracker tracker(*pe);

  // Create data requests for each value that should be written to the output log as the engine is executing
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SedationLevel");

  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("HowToAnesthesiaMachine.csv");

  pe->GetLogger()->Info("The patient is nice and healthy");
  pe->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());;

  tracker.AdvanceModelTime(50);

  // Turn the anesthesia machine on and get it configured for spontaneous breathing
  // Create an Anesthesia Machine and configure it as needed
  SEAnesthesiaMachineConfiguration AMConfig(pe->GetSubstanceManager());

  // You can set configuration by modifing the configuration class directly
  // Or you can point to an json with configuration data.
  // Modifying the class will keep any old settings that are not provided in the config
  // Using a json will set the anesthesia machine to only the property states specified in the file
  SEAnesthesiaMachine& config = AMConfig.GetConfiguration();
  config.SetConnection(eAnesthesiaMachine_Connection::Mask);
  config.GetInletFlow().SetValue(2.0, VolumePerTimeUnit::L_Per_min);
  config.GetInspiratoryExpiratoryRatio().SetValue(.5);
  config.GetOxygenFraction().SetValue(.5);
  config.SetOxygenSource(eAnesthesiaMachine_OxygenSource::Wall);
  config.GetPositiveEndExpiredPressure().SetValue(0.0, PressureUnit::cmH2O);
  config.SetPrimaryGas(eAnesthesiaMachine_PrimaryGas::Nitrogen);
  config.GetReliefValvePressure().SetValue(20.0, PressureUnit::cmH2O);
  config.GetRespiratoryRate().SetValue(12, FrequencyUnit::Per_min);
  config.GetPeakInspiratoryPressure().SetValue(0.0, PressureUnit::cmH2O);
  config.GetOxygenBottleOne().GetVolume().SetValue(660.0, VolumeUnit::L);
  config.GetOxygenBottleTwo().GetVolume().SetValue(660.0, VolumeUnit::L);

  // Process the action to propagate state into the engine
  pe->ProcessAction(AMConfig);
  pe->GetLogger()->Info(std::stringstream() <<"Turning on the Anesthesia Machine and placing mask on patient for spontaneous breathing with machine connection.");;

  tracker.AdvanceModelTime(60);

  pe->GetLogger()->Info("The patient is attempting to breath normally with Anesthesia Machine connected");
  pe->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());;

    // Get the Succinylcholine substance from the substance manager
  const SESubstance* succs = pe->GetSubstanceManager().GetSubstance("Succinylcholine");

    // Create a substance bolus action to administer the substance
  SESubstanceBolus bolus(*succs);
  bolus.GetConcentration().SetValue(4820, MassPerVolumeUnit::ug_Per_mL);
  bolus.GetDose().SetValue(20, VolumeUnit::mL);
  bolus.SetAdminRoute(eSubstanceAdministration_Route::Intravenous);
  pe->ProcessAction(bolus);
  
  pe->GetLogger()->Info("Giving the patient Succinylcholine to test machine-driven ventilation.");

  tracker.AdvanceModelTime(60);

  pe->GetLogger()->Info("It has been 60s since the Succinylcholine administration.");
  pe->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());;

  config.GetInletFlow().SetValue(5.0, VolumePerTimeUnit::L_Per_min);
  config.GetPositiveEndExpiredPressure().SetValue(3.0, PressureUnit::cmH2O);
  config.GetPeakInspiratoryPressure().SetValue(22.0, PressureUnit::cmH2O);
  pe->ProcessAction(AMConfig);
  pe->GetLogger()->Info("Setting the ventilator pressure to drive the machine. Also increasing the inlet flow and positive end expired pressure to test machine controls.");

  tracker.AdvanceModelTime(60);
   
  pe->GetLogger()->Info("Patient breathing is being controlled by the machine.");
  pe->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());;

  config.GetInspiratoryExpiratoryRatio().SetValue(1.0);
  config.GetPositiveEndExpiredPressure().SetValue(1.0, PressureUnit::cmH2O);
  config.GetRespiratoryRate().SetValue(18.0, FrequencyUnit::Per_min);
  config.GetPeakInspiratoryPressure().SetValue(10.0, PressureUnit::cmH2O);
  pe->ProcessAction(AMConfig);
  pe->GetLogger()->Info("More Anesthesia Machine control manipulation. Increasing respiratory rate, reducing driving pressure and increasing the inspiratory-expiratory ratio.");

  tracker.AdvanceModelTime(60);

  pe->GetLogger()->Info("Patient breathing is being controlled by the machine.");
  pe->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());;

  SEAnesthesiaMachineMaskLeak AMleak;
  AMleak.GetSeverity().SetValue(0.5);
  pe->ProcessAction(AMleak);
  pe->GetLogger()->Info("Testing an anesthesia machine failure mode. The mask is leaking with a severity of 0.5.");

  tracker.AdvanceModelTime(60);

  pe->GetLogger()->Info("Patient breathing is being controlled by the machine. The mask has been leaking for 60 seconds.");
  pe->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());;

  AMleak.GetSeverity().SetValue(0.0);
  pe->ProcessAction(AMleak);
  pe->GetLogger()->Info("Removing the mask leak.");

  tracker.AdvanceModelTime(60);

  SEAnesthesiaMachineOxygenWallPortPressureLoss AMpressureloss;
  AMpressureloss.SetActive(true);
  pe->ProcessAction(AMpressureloss);
  pe->GetLogger()->Info("Testing the oxygen pressure loss failure mode. The oxygen pressure from the wall source is dropping.");

  tracker.AdvanceModelTime(60);

  pe->GetLogger()->Info("Patient breathing is being controlled by the machine. The wall oxygen pressure loss occurred 60 seconds ago.");
  pe->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());

  AMpressureloss.SetActive(false);
  pe->ProcessAction(AMpressureloss);
  pe->GetLogger()->Info("Removing the wall oxygen pressure loss action.");

  tracker.AdvanceModelTime(60);

  pe->GetLogger()->Info("The anesthesia machine is operating normally");
  pe->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation());
  pe->GetLogger()->Info("Finished");
}
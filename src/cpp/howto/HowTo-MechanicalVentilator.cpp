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
#include "cdm/system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorHold.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorLeak.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorContinuousPositiveAirwayPressure.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorPressureControl.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorVolumeControl.h"
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
/// Usage for the Mechanical Ventilator
/// Drive respiration with equipment
///
/// \details
/// Refer to the SEMechanicalVentilator class
//--------------------------------------------------------------------------------------------------
void HowToMechanicalVentilator()
{
  //Note: Setting circuit values (resistance/compliances/etc.) needs to be done in the engine code - they currently are not directly exposed
  
  std::stringstream ss;
  // Create a Pulse Engine and load the standard patient
  std::unique_ptr<PhysiologyEngine> pe = pulse::engine::CreatePulseEngine();
  pe->GetLogger()->SetLogFile("./test_results/HowTo/HowTo_MechanicalVentilator.log");
  
  pe->GetLogger()->Info("HowTo_MechanicalVentilator");
  
 
  if (!pe->SerializeFromFile("./states/StandardMale@0s.json"))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // Vitals Monitor Data
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("ArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("EndTidalCarbonDioxidePressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SkinTemperature", TemperatureUnit::C);
  pe->GetEngineTracker()->GetDataRequestManager().CreateGasCompartmentDataRequest("Carina", "CarbonDioxide", "PartialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("BloodVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreateECGDataRequest("Lead3ElectricPotential", ElectricPotentialUnit::mV);
  // Ventilator Monitor Data
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("AirwayPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("EndTidalCarbonDioxideFraction");
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("EndTidalCarbonDioxidePressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("EndTidalOxygenFraction");
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("EndTidalOxygenPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("ExpiratoryFlow", VolumePerTimeUnit::L_Per_s);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("ExpiratoryTidalVolume", VolumeUnit::L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("InspiratoryExpiratoryRatio");
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("InspiratoryFlow", VolumePerTimeUnit::L_Per_s);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("InspiratoryTidalVolume", VolumeUnit::L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("IntrinsicPositiveEndExpiredPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("LeakFraction");
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("MeanAirwayPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("PeakInspiratoryPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("PlateauPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("PositiveEndExpiratoryPressure", PressureUnit::cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("PulmonaryCompliance", VolumePerPressureUnit::L_Per_cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("PulmonaryElastance", PressurePerVolumeUnit::cmH2O_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("RelativeTotalLungVolume", VolumeUnit::L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("RespirationRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("TidalVolume", VolumeUnit::L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("TotalLungVolume", VolumeUnit::L);
  pe->GetEngineTracker()->GetDataRequestManager().CreateMechanicalVentilatorDataRequest("TotalPulmonaryVentilation", VolumePerTimeUnit::L_Per_s);
  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("./test_results/HowTo/HowTo_MechanicalVentilator.cpp.csv");

  //Dyspnea
  SEDyspnea Dyspnea;
  Dyspnea.GetSeverity().SetValue(1.0);
  pe->ProcessAction(Dyspnea);
  AdvanceAndTrackTime_s(10.0, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);

  // We have action support for several commonly used ventilator modes
  // Pulse is not limited to these modes, These modes are designe for simple understanding
  // Our implementation supports any ventilator mode, you will just need to translate the user facing inputs
  // to a timing/control profile using our configuration settings
  // These modes are internally converted into a configuration setting,
  // You can retrieve the resulting settings action and use it as a basis for more configurations if you want.
  // For example, if you wanted to lengthen the InspirationPatientTriggerFlow of the mode

  SEMechanicalVentilatorContinuousPositiveAirwayPressure cpap;
  cpap.SetConnection(eSwitch::On);
  cpap.GetDeltaPressureSupport().SetValue(8.0, PressureUnit::cmH2O);
  cpap.GetFractionInspiredOxygen().SetValue(0.21);
  cpap.GetPositiveEndExpiredPressure().SetValue(5.0, PressureUnit::cmH2O);
  cpap.GetSlope().SetValue(0.2, TimeUnit::s);
  pe->ProcessAction(cpap);
  AdvanceAndTrackTime_s(10.0, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);

  SEMechanicalVentilatorPressureControl pc_ac;
  pc_ac.SetConnection(eSwitch::On);
  // There are several different modes to choose from
  pc_ac.GetFractionInspiredOxygen().SetValue(0.21);
  pc_ac.GetInspiratoryPeriod().SetValue(1.0, TimeUnit::s);
  pc_ac.GetInspiratoryPressure().SetValue(13.0, PressureUnit::cmH2O);
  pc_ac.GetPositiveEndExpiredPressure().SetValue(5.0, PressureUnit::cmH2O);
  pc_ac.GetRespirationRate().SetValue(12.0, FrequencyUnit::Per_min);
  pe->ProcessAction(pc_ac);
  AdvanceAndTrackTime_s(10.0, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);

  SEMechanicalVentilatorVolumeControl vc_ac;
  vc_ac.SetConnection(eSwitch::On);
  // There are several different modes to choose from
  vc_ac.SetMode(eMechanicalVentilator_VolumeControlMode::AssistedControl);
  vc_ac.GetFlow().SetValue(50.0, VolumePerTimeUnit::L_Per_min);
  vc_ac.GetFractionInspiredOxygen().SetValue(0.21);
  vc_ac.GetInspiratoryPeriod().SetValue(1.0, TimeUnit::s);
  vc_ac.GetPositiveEndExpiredPressure().SetValue(5.0, PressureUnit::cmH2O);
  vc_ac.GetRespirationRate().SetValue(12.0, FrequencyUnit::Per_min);
  vc_ac.GetTidalVolume().SetValue(600.0, VolumeUnit::mL);
  pe->ProcessAction(vc_ac);
  AdvanceAndTrackTime_s(10.0, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);

  // Here is an example of programming a custom ventilator mode
  SEMechanicalVentilatorConfiguration mv_config;
  SEMechanicalVentilatorSettings& mv = mv_config.GetSettings();
  mv.SetConnection(eSwitch::On);
  mv.SetInspirationWaveform(eMechanicalVentilator_DriverWaveform::Square);
  mv.SetExpirationWaveform(eMechanicalVentilator_DriverWaveform::Square);
  mv.GetPeakInspiratoryPressure().SetValue(21.0, PressureUnit::cmH2O);
  mv.GetPositiveEndExpiredPressure().SetValue(10.0, PressureUnit::cmH2O);
  SESubstanceFraction& fractionFiO2 = mv.GetFractionInspiredGas(*pe->GetSubstanceManager().GetSubstance("Oxygen"));
  fractionFiO2.GetFractionAmount().SetValue(0.5);
  double respirationRate_per_min = 20.0;
  double IERatio = 0.5;
  // Translate ventilator settings
  double totalPeriod_s = 60.0 / respirationRate_per_min;
  double inspiratoryPeriod_s = IERatio * totalPeriod_s / (1 + IERatio);
  double expiratoryPeriod_s = totalPeriod_s - inspiratoryPeriod_s;
  mv.GetInspirationMachineTriggerTime().SetValue(expiratoryPeriod_s, TimeUnit::s);
  mv.GetExpirationCycleTime().SetValue(inspiratoryPeriod_s, TimeUnit::s);
  pe->ProcessAction(mv_config);
  AdvanceAndTrackTime_s(10.0, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);

  // You can also perform a hold
  SEMechanicalVentilatorHold hold;
  hold.SetState(eSwitch::On);
  pe->ProcessAction(hold);
  AdvanceAndTrackTime_s(5, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);
  hold.SetState(eSwitch::Off);
  pe->ProcessAction(hold);
  AdvanceAndTrackTime_s(5, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);

  // A leak can be specified
  SEMechanicalVentilatorLeak leak;
  leak.GetSeverity().SetValue(0.5);
  pe->ProcessAction(leak);
  AdvanceAndTrackTime_s(5, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);
  leak.GetSeverity().SetValue(0.0);// Turn off the leak
  pe->ProcessAction(leak);
  AdvanceAndTrackTime_s(5, *pe);
  pe->GetEngineTracker()->LogRequestedValues(false);

  pe->GetLogger()->Info("Finished");
}

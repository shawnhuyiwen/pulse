/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineHowTo.h"

// Include the various types you will be using in your code
#include "engine/SEDataRequestManager.h"
#include "engine/SEEngineTracker.h"
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SERespiratorySystem.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstanceFraction.h"
#include "patient/SEPatient.h"
#include "patient/actions/SEAcuteRespiratoryDistressSyndromeExacerbation.h"
#include "patient/actions/SEDyspnea.h"
#include "patient/actions/SEIntubation.h"
#include "equipment/mechanical_ventilator/SEMechanicalVentilator.h"
#include "equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.h"
#include "engine/SEOverrides.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerPressure.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalar0To1.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// COVID-19 treatment example leveraging ARDS, parameter override, and mechanical ventilator.
///
/// \details
/// Refer to the SEAirwayObstruction class
//--------------------------------------------------------------------------------------------------
void HowToCOVID19Ventilated()
{
  std::stringstream ss;
  // Create a Pulse Engine and load the standard patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine();
  pe->GetLogger()->SetLogFile("HowTo_COVID19Ventilated.log");
  
  pe->GetLogger()->Info("HowTo_COVID19Ventilated");
  if (!pe->SerializeFromFile("./states/StandardMale@0s.json"))
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  HowToTracker tracker(*pe);

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");

  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("HowToCOVID19Ventilated.csv");

  // Set moderate ARDS because COVID-19 pneumonia patients that received invasive mechanical ventilation have shown the PaO2/FiO2 ratios 
  // were consistent with the Berlin criteria of moderate-to-severe ARDS. @cite bhatraju2020covid @cite yang2020clinical
  SEAcuteRespiratoryDistressSyndromeExacerbation ARDS;
  ARDS.GetSeverity().SetValue(0.6);
  ARDS.GetLeftLungAffected().SetValue(1.0);
  ARDS.GetRightLungAffected().SetValue(1.0);
  pe->ProcessAction(ARDS);

  // Update the total respiratory compliance, since COVID-19 patients present an atypical form of ARDS with a dissociation between 
  // well-preserved lung mechanics (i.e., compliance), when compared to the magnitude of pulmonary shunt fraction and severity of 
  // hypoxemia. @cite gattinoni2020covid @cite gattinoni2020covid2
  pe->GetLogger()->Info("Updating the patient's total respiratory static compliance to a specific constant/linear value.");
  SEOverrides overrides;
  overrides.AddScalarProperty("RespiratoryCompliance", 100.0, VolumePerPressureUnit::mL_Per_cmH2O);
  pe->ProcessAction(overrides);

  // Advance time enough to achieve a new pathophysiogical homeostatic state
  tracker.AdvanceModelTime(10.0 * 60.0); // 10 min

  pe->GetLogger()->Info(std::stringstream() << "The patient has moderate COVID-19");
  pe->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation() * 100 << "%");
  pe->GetLogger()->Info(std::stringstream() << "Pulmonary Compliance : " << pe->GetRespiratorySystem()->GetPulmonaryCompliance(VolumePerPressureUnit::mL_Per_cmH2O) << VolumePerPressureUnit::mL_Per_cmH2O);
  pe->GetLogger()->Info(std::stringstream() << "End Tidal Carbon Dioxide Pressure : " << pe->GetRespiratorySystem()->GetEndTidalCarbonDioxidePressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "P/F Ratio : " << pe->GetRespiratorySystem()->GetCarricoIndex(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Pulmonary Shunt Fraction : " << pe->GetBloodChemistrySystem()->GetShuntFraction() * 100 << "%");
  pe->GetLogger()->Info(std::stringstream() << "Mean Airway Pressure : " << pe->GetRespiratorySystem()->GetMeanAirwayPressure(PressureUnit::cmH2O) << PressureUnit::cmH2O);
  pe->GetLogger()->Info(std::stringstream() << "Oxygen Saturation Index : " << pe->GetRespiratorySystem()->GetOxygenSaturationIndex(PressureUnit::mmHg) << PressureUnit::mmHg);


  // Mimic a neuromuscular blockade by setting apnea (dyspnea = 1)
  // Alternatively, there are drugs in the library that can be administered that stop spontaneous breathing (e.g., succinylcholine)
  SEDyspnea dyspnea;
  dyspnea.GetSeverity().SetValue(1.0);
  pe->ProcessAction(dyspnea);

  // Mechanically ventilate the patient in PC-CMV mode with setting to acheive good lung recruitment, tidal volume ~6mL/kg, and SpO2 ~95%
  SEIntubation intubation;
  intubation.SetType(eIntubation_Type::Tracheal);
  pe->ProcessAction(intubation);

  // Setup the PC-CMV ventilator
  SEMechanicalVentilatorConfiguration MVConfig(pe->GetLogger());
  SEMechanicalVentilator& mv = MVConfig.GetConfiguration();
  mv.SetConnection(eMechanicalVentilator_Connection::Tube);
  mv.SetInspirationWaveform(eMechanicalVentilator_DriverWaveform::Square);
  mv.SetExpirationWaveform(eMechanicalVentilator_DriverWaveform::Square);
  mv.GetPeakInspiratoryPressure().SetValue(21.0, PressureUnit::cmH2O);
  mv.GetPositiveEndExpiredPressure().SetValue(10.0, PressureUnit::cmH2O);
  const SESubstance* oxygen = pe->GetSubstanceManager().GetSubstance("Oxygen");
  SESubstanceFraction* fractionFiO2 = &mv.GetFractionInspiredGas(*oxygen);
  fractionFiO2->GetFractionAmount().SetValue(0.5);
  double respirationRate_per_min = 20.0;
  double IERatio = 0.5;

  // Translate ventilator settings
  double totalPeriod_s = 60.0 / respirationRate_per_min;
  double inspiratoryPeriod_s = IERatio * totalPeriod_s / (1 + IERatio);
  double expiratoryPeriod_s = totalPeriod_s - inspiratoryPeriod_s;
  mv.GetInspirationMachineTriggerTime().SetValue(expiratoryPeriod_s, TimeUnit::s);
  mv.GetExpirationCycleTime().SetValue(inspiratoryPeriod_s, TimeUnit::s);
  pe->ProcessAction(MVConfig);

  // Update the compliance for postive pressure ventilation, which differs from negative pressure spontaneous breathing compliance @cite arnal2018parameters
  overrides.Clear();
  overrides.AddScalarProperty("RespiratoryCompliance", 50.0, VolumePerPressureUnit::mL_Per_cmH2O);
  pe->ProcessAction(overrides);

  // Advance time enough to achieve a new homeostatic state
  tracker.AdvanceModelTime(10.0 * 60.0); // 10 min

  pe->GetLogger()->Info("The patient has been successfully ventilated and stabilized");
  pe->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  pe->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << pe->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << pe->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Heart Rate : " << pe->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  pe->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << pe->GetBloodChemistrySystem()->GetOxygenSaturation() * 100 << "%");
  pe->GetLogger()->Info(std::stringstream() << "Pulmonary Compliance : " << pe->GetRespiratorySystem()->GetPulmonaryCompliance(VolumePerPressureUnit::mL_Per_cmH2O) << VolumePerPressureUnit::mL_Per_cmH2O);
  pe->GetLogger()->Info(std::stringstream() << "End Tidal Carbon Dioxide Pressure : " << pe->GetRespiratorySystem()->GetEndTidalCarbonDioxidePressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "P/F Ratio : " << pe->GetRespiratorySystem()->GetCarricoIndex(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info(std::stringstream() << "Pulmonary Shunt Fraction : " << pe->GetBloodChemistrySystem()->GetShuntFraction() * 100 << "%");
  pe->GetLogger()->Info(std::stringstream() << "Mean Airway Pressure : " << pe->GetRespiratorySystem()->GetMeanAirwayPressure(PressureUnit::cmH2O) << PressureUnit::cmH2O);
  pe->GetLogger()->Info(std::stringstream() << "Oxygen Saturation Index : " << pe->GetRespiratorySystem()->GetOxygenSaturationIndex(PressureUnit::mmHg) << PressureUnit::mmHg);
  pe->GetLogger()->Info("Finished");
}
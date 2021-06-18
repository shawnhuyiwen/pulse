/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#define _USE_MATH_DEFINES

#include "EngineHowTo.h"

// Include the various types you will be using in your code
#include "engine/SEDataRequestManager.h"
#include "engine/SEEngineTracker.h"
#include "patient/actions/SERespiratoryMechanicsConfiguration.h"
#include "system/physiology/SERespiratoryMechanics.h"
#include "system/physiology/SERespiratorySystem.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressurePerVolume.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerPressure.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SECurve.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Custom driver settings for our respiratory system
/// Drive respiration with your own driver
///
/// \details
/// Refer to the SERespiratoryMechanics class
//--------------------------------------------------------------------------------------------------
void HowToRespiratoryMechanics()
{
  // Create a Pulse Engine and load the standard patient
  std::unique_ptr<PhysiologyEngine> pe = CreatePulseEngine();
  pe->GetLogger()->SetLogFile("./test_results/howto/HowTo_RespiratoryMechanics.log");
  pe->GetLogger()->Info("HowTo_RespiratoryMechanics");

  // With this engine, you do not initialize it, its already ready to go at construction time

  // You can load a previously saved state, but this is optional!
  if (!pe->SerializeFromFile("./states/StandardMale@0s.json"))// Select patient
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // Note we can only request data from the respiratory system or mechanical ventilator
  // Those are the only systems this engine uses
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::mL);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("ExpiratoryPulmonaryResistance", PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("InspiratoryPulmonaryResistance", PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("PulmonaryCompliance", VolumePerPressureUnit::L_Per_cmH2O);
  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalPulmonaryVentilation", VolumePerTimeUnit::L_Per_min);
  pe->GetEngineTracker()->GetDataRequestManager().SetResultsFilename("./test_results/howto/HowTo_MechanicalVentilation.csv");

  for (size_t i = 0; i < 6; i++)
  {
    pe->AdvanceModelTime(10, TimeUnit::s);
    pe->GetLogger()->Info(std::stringstream() << "\nSimTime : " << pe->GetSimulationTime(TimeUnit::s) << "s");
    pe->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
    pe->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
    pe->GetLogger()->Info(std::stringstream() << "Total Lung Volume : " << pe->GetRespiratorySystem()->GetTotalLungVolume(VolumeUnit::mL) << "mL");
    pe->GetLogger()->Info(std::stringstream() << "Expiratory Pulmonary Resistance : " << pe->GetRespiratorySystem()->GetExpiratoryPulmonaryResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_mL) << "cmH2O s/mL");
    pe->GetLogger()->Info(std::stringstream() << "Inspiratory Pulmonary Resistance : " << pe->GetRespiratorySystem()->GetInspiratoryPulmonaryResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_mL) << "cmH2O s/mL");
    pe->GetLogger()->Info(std::stringstream() << "Pulmonary Compliance : " << pe->GetRespiratorySystem()->GetPulmonaryCompliance(VolumePerPressureUnit::mL_Per_cmH2O) << "mL/cmH2O");
    pe->GetLogger()->Info(std::stringstream() << "Total Pulmonary Ventilation : " << pe->GetRespiratorySystem()->GetTotalPulmonaryVentilation(VolumePerTimeUnit::mL_Per_s) << "mL/s\n");
  }

  SERespiratoryMechanicsConfiguration config;
  SERespiratoryMechanics& mechanics = config.GetConfiguration();
  // Create a mechanics profile
  mechanics.GetLeftExpiratoryResistance().SetValue(5, PressureTimePerVolumeUnit::cmH2O_s_Per_mL);
  SECurve& lcc = mechanics.GetLeftComplianceCurve();
  SESegmentConstant& c = lcc.AddConstantSegment();
  c.GetBeginVolume().SetValue(-std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  c.GetEndVolume().SetValue(0, VolumeUnit::mL);
  c.GetCompliance().SetValue(5, VolumePerPressureUnit::mL_Per_cmH2O);
  SESegmentLinear& l = lcc.AddLinearSegment();
  l.GetBeginVolume().SetValue(0, VolumeUnit::mL);
  l.GetEndVolume().SetValue(10, VolumeUnit::mL);
  l.GetSlope().SetValue(7, PressurePerVolumeUnit::cmH2O_Per_mL);
  l.GetYIntercept().SetValue(8, PressureUnit::cmH2O);
  SESegmentParabolic& p = lcc.AddParabolicSegment();
  p.GetBeginVolume().SetValue(10, VolumeUnit::mL);
  p.GetEndVolume().SetValue(1000, VolumeUnit::mL);
  p.GetCoefficient1().SetValue(11);
  p.GetCoefficient2().SetValue(13);
  p.GetCoefficient3().SetValue(15);
  p.GetCoefficient4().SetValue(17);
  SESegmentSigmoidal& s = lcc.AddSigmoidalSegment();
  s.GetBeginVolume().SetValue(1000, VolumeUnit::mL);
  s.GetEndVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  s.GetLowerCorner().SetValue(21, PressureUnit::cmH2O);
  s.GetUpperCorner().SetValue(23, PressureUnit::cmH2O);
  s.GetBaselineCompliance().SetValue(22, VolumePerPressureUnit::mL_Per_cmH2O);

  pe->ProcessAction(config);

  for (size_t i = 0; i < 12; i++)
  {
    pe->AdvanceModelTime(10, TimeUnit::s);
    pe->GetLogger()->Info(std::stringstream() << "\nSimTime : " << pe->GetSimulationTime(TimeUnit::s) << "s");
    pe->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << pe->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
    pe->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << pe->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
    pe->GetLogger()->Info(std::stringstream() << "Total Lung Volume : " << pe->GetRespiratorySystem()->GetTotalLungVolume(VolumeUnit::mL) << "mL");
    pe->GetLogger()->Info(std::stringstream() << "Expiratory Pulmonary Resistance : " << pe->GetRespiratorySystem()->GetExpiratoryPulmonaryResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_mL) << "cmH2O s/mL");
    pe->GetLogger()->Info(std::stringstream() << "Inspiratory Pulmonary Resistance : " << pe->GetRespiratorySystem()->GetInspiratoryPulmonaryResistance(PressureTimePerVolumeUnit::cmH2O_s_Per_mL) << "cmH2O s/mL");
    pe->GetLogger()->Info(std::stringstream() << "Pulmonary Compliance : " << pe->GetRespiratorySystem()->GetPulmonaryCompliance(VolumePerPressureUnit::mL_Per_cmH2O) << "mL/cmH2O");
    pe->GetLogger()->Info(std::stringstream() << "Total Pulmonary Ventilation : " << pe->GetRespiratorySystem()->GetTotalPulmonaryVentilation(VolumePerTimeUnit::mL_Per_s) << "mL/s\n");
  }

}
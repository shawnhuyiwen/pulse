/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#define _USE_MATH_DEFINES

#include "EngineHowTo.h"

// Include the various types you will be using in your code
#include "system/physiology/SERespiratoryMechanics.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarVolume.h"
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
  pe->GetLogger()->SetLogFile("./test_results/HowTo_RespiratoryMechanics.log");

  pe->GetLogger()->Info("HowTo_RespiratoryMechanics");

  if (!pe->SerializeFromFile("./states/StandardMale@0s.json"))// Select patient
  {
    pe->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  SERespiratoryMechanics mechanics(pe->GetLogger());
  // Create a mechanics profile
  mechanics.GetLeftExpiratoryResistance().SetValue(5, PressureTimePerVolumeUnit::cmH2O_s_Per_mL);
  SECurve& lcc = mechanics.GetLeftComplianceCurve();
  SESegmentConstant& c = lcc.AddConstantSegment();
  c.GetBeginVolume().SetValue(-std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  c.GetEndVolume().SetValue(0, VolumeUnit::mL);
  c.GetCompliance().SetValue(5, PressureTimePerVolumeUnit::cmH2O_s_Per_mL);
  SESegmentLinear& l = lcc.AddLinearSegment();
  l.GetBeginVolume().SetValue(0, VolumeUnit::mL);
  l.GetEndVolume().SetValue(10, VolumeUnit::mL);
  l.GetSlope().SetValue(7, PressureTimePerVolumeUnit::cmH2O_s_Per_mL);
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
  s.GetBaselineCompliance().SetValue(22, PressureTimePerVolumeUnit::cmH2O_s_Per_mL);

  std::cout << mechanics << std::endl;


}
/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

   // Testing creating and converting units
#include "CommonDataModelTest.h"

#include "cdm/system/equipment/electrocardiogram/SEElectroCardioGramWaveformInterpolator.h"
#include "cdm/utils/FileUtils.h"
#include "cdm/utils/TimingProfile.h"
#include "cdm/utils/testing/SETestReport.h"
#include "cdm/utils/testing/SETestSuite.h"
#include "cdm/utils/testing/SETestCase.h"

#include "cdm/properties/SEFunctionElectricPotentialVsTime.h"
#include "cdm/properties/SEScalarElectricPotential.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarTime.h"

void CommonDataModelTest::WaveformInterpolatorTest(const std::string& sOutputDirectory)
{
  TimingProfile pTimer;
  std::string testName = "WaveformInterpolatorTest";
  Logger logger(sOutputDirectory + "/" + testName + "/"+ testName + ".log");

  SEElectroCardioGramWaveformInterpolator ecg(&logger);
  ecg.SerializeFromFile("./ecg/StandardECG.json");

  SEElectroCardioGramWaveform& sinus =
    ecg.GetWaveform(eElectroCardioGram_WaveformLead::Lead3, eElectroCardioGram_WaveformType::Sinus);
  sinus.GetData().WriteCSVFile(sOutputDirectory + "/" + testName + "/sinus.orig.csv");

  SEElectroCardioGramWaveform& vtach =
    ecg.GetWaveform(eElectroCardioGram_WaveformLead::Lead3, eElectroCardioGram_WaveformType::VentricularTachycardia);
  vtach.GetData().WriteCSVFile(sOutputDirectory + "/" + testName + "/vtach.orig.csv");

  SETestReport testReport(&logger);
  SETestSuite& testSuite = testReport.CreateTestSuite();
  testSuite.SetName(testName);

  SEScalarFrequency hr;

  // Create Sinus waveform for HR = 185
  pTimer.Start("Case");
  hr.SetValue(185, FrequencyUnit::Per_min);
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("SINUS2HR185");
  double cycleTime_s = 1 / hr.GetValue(FrequencyUnit::Per_s);
  size_t numCyclePoints = (size_t)std::ceil(cycleTime_s / 0.02);

  // The new interpolated waveform
  SEFunctionElectricPotentialVsTime sinus185;
  sinus185.SetTimeUnit(TimeUnit::s);
  sinus185.SetElectricPotentialUnit(ElectricPotentialUnit::mV);
  for (size_t i = 0; i < numCyclePoints; i++)
  {
    sinus185.GetTime().push_back(0.02 * i);
    sinus185.GetElectricPotential().push_back(0);
  }
  // How do we get the mV of the 16 pts in the orig to expand across the 50 pts of this cardiac cycle?
  // There are some basic utils in GeneralMath.h
  // Eigen spline fitting could be a good option : https://eigen.tuxfamily.org/dox/unsupported/structEigen_1_1SplineFitting.html
  sinus185.WriteCSVFile(sOutputDirectory + "/" + testName + "/sinus.185.csv");
  // testCase.AddFailure("Unable to load patient " + *it);
  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Case"), TimeUnit::s);

  // Create VTACH waveform for HR = 60
  pTimer.Start("Case");
  hr.SetValue(60, FrequencyUnit::Per_min);
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("VTACH2HR60");
  double cycleTime_s = 1 / hr.GetValue(FrequencyUnit::Per_s);
  size_t numCyclePoints = (size_t)std::ceil(cycleTime_s / 0.02);

  // The new interpolated waveform
  SEFunctionElectricPotentialVsTime vtach60;
  vtach60.SetTimeUnit(TimeUnit::s);
  vtach60.SetElectricPotentialUnit(ElectricPotentialUnit::mV);
  for (size_t i = 0; i < numCyclePoints; i++)
  {
    vtach60.GetTime().push_back(0.02 * i);
    vtach60.GetElectricPotential().push_back(0);
  }
  // How do we get the mV of the 16 pts in the orig to expand across the 50 pts of this cardiac cycle?
  // There are some basic utils in GeneralMath.h
  // Eigen spline fitting could be a good option : https://eigen.tuxfamily.org/dox/unsupported/structEigen_1_1SplineFitting.html
  vtach60.WriteCSVFile(sOutputDirectory + "/" + testName + "/vtach.60.csv");
  // testCase.AddFailure("Unable to load patient " + *it);
  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Case"), TimeUnit::s);


  testReport.SerializeToFile(sOutputDirectory + "/" + testName + "Report.json");
}

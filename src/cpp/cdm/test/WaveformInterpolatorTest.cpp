/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

   // Testing creating and converting units
#include "CommonDataModelTest.h"

#include "cdm/system/equipment/electrocardiogram/SEElectroCardioGram.h"
#include "cdm/utils/FileUtils.h"
#include "cdm/utils/GeneralMath.h"
#include "cdm/utils/TimingProfile.h"
#include "cdm/utils/testing/SETestReport.h"
#include "cdm/utils/testing/SETestSuite.h"
#include "cdm/utils/testing/SETestCase.h"

#include "cdm/properties/SEArrayElectricPotential.h"
#include "cdm/properties/SEScalarElectricPotential.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarTime.h"

void CommonDataModelTest::WaveformInterpolatorTest(const std::string& sOutputDirectory)
{
  TimingProfile pTimer;
  std::string testName = "WaveformInterpolatorTest";
  Logger logger(sOutputDirectory + "/" + testName + "/"+ testName + ".log");

  SEElectroCardioGram ecg(&logger);
  ecg.SerializeFromFile("./ecg/StandardECG.json");

  SETestReport testReport(&logger);
  SETestSuite& testSuite = testReport.CreateTestSuite();
  testSuite.SetName(testName);

  SEScalarFrequency hr;
  SEArrayElectricPotential cycleWaveform;

  ///////////////////////////////////////
  // Generate sinus waveform for hr = 185
  ///////////////////////////////////////
  pTimer.Start("Case");
  hr.SetValue(185, FrequencyUnit::Per_min);
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("SINUS2HR185");

  ecg.StartNewCycle(eElectroCardioGram_WaveformType::Sinus, hr, 1.0);
  cycleWaveform.Copy(ecg.GetWaveform(eElectroCardioGram_WaveformLead::Lead3,eElectroCardioGram_WaveformType::Sinus).GetActiveCycle());

  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Case"), TimeUnit::s);

  //////////////////////////////////////
  // Generate sinus waveform for hr = 60
  //////////////////////////////////////
  pTimer.Start("Case");
  hr.SetValue(60, FrequencyUnit::Per_min);
  SETestCase& testCase1 = testSuite.CreateTestCase();
  testCase1.SetName("SINUS2HR60");

  ecg.StartNewCycle(eElectroCardioGram_WaveformType::Sinus, hr, 1.0);
  cycleWaveform.Copy(ecg.GetWaveform(eElectroCardioGram_WaveformLead::Lead3, eElectroCardioGram_WaveformType::Sinus).GetActiveCycle());

  testCase1.GetDuration().SetValue(pTimer.GetElapsedTime_s("Case"), TimeUnit::s);

  ///////////////////////////////////////
  // Generate vtach waveform for hr = 185
  ///////////////////////////////////////
  pTimer.Start("Case");
  hr.SetValue(185, FrequencyUnit::Per_min);
  SETestCase& testCase2 = testSuite.CreateTestCase();
  testCase2.SetName("VTACH2HR185");

  ecg.StartNewCycle(eElectroCardioGram_WaveformType::VentricularTachycardia, hr, 1.0);
  cycleWaveform.Copy(ecg.GetWaveform(eElectroCardioGram_WaveformLead::Lead3, eElectroCardioGram_WaveformType::Sinus).GetActiveCycle());

  //vtach185.WriteCSVFile(sOutputDirectory + "/" + testName + "/vtach.185.csv");
  testCase2.GetDuration().SetValue(pTimer.GetElapsedTime_s("Case"), TimeUnit::s);

  ///////////////////////////////////////
  // Generate vtach waveform for hr = 165
  ///////////////////////////////////////
  pTimer.Start("Case");
  hr.SetValue(165, FrequencyUnit::Per_min);
  SETestCase& testCase3 = testSuite.CreateTestCase();
  testCase3.SetName("VTACH2HR165");

  ecg.StartNewCycle(eElectroCardioGram_WaveformType::VentricularTachycardia, hr, 1.0);
  cycleWaveform.Copy(ecg.GetWaveform(eElectroCardioGram_WaveformLead::Lead3, eElectroCardioGram_WaveformType::Sinus).GetActiveCycle());

  //vtach165.WriteCSVFile(sOutputDirectory + "/" + testName + "/vtach.165.csv");
  testCase3.GetDuration().SetValue(pTimer.GetElapsedTime_s("Case"), TimeUnit::s);

  testReport.SerializeToFile(sOutputDirectory + "/" + testName + "Report.json");
}

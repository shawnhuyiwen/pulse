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
#include "cdm/properties/SEFunctionElectricPotentialVsTime.h"
#include "cdm/properties/SEScalarElectricPotential.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarTime.h"

void CommonDataModelTest::WaveformInterpolatorTest(const std::string& sOutputDirectory)
{
  TimingProfile pTimer;
  std::string testName = "WaveformInterpolatorTest";
  Logger logger(sOutputDirectory + "/" + testName + "/"+ testName + ".log");
  logger.LogToConsole(true);

  SEElectroCardioGram ecg(&logger);
  ecg.SerializeFromFile("./ecg/StandardECG.json");

  SETestReport testReport(&logger);
  SETestSuite& testSuite = testReport.CreateTestSuite();
  testSuite.SetName(testName);

  SEScalarFrequency hr;
  SEScalarTime dt;
  dt.SetValue(0.02, TimeUnit::s);
  std::vector<double> peaks;

  ///////////////////////////
  // Generate sinus waveforms
  ///////////////////////////
  pTimer.Start("Case");
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("Sinus");

  peaks.clear();
  double peakMin = 100;
  double peakMax = -100;
  for (size_t i = 60; i < 220; i++)
  {
    logger.Info("Generating Sinus rhythm for HR: " + std::to_string(i));
    hr.SetValue(i, FrequencyUnit::Per_min);
    ecg.StartNewCycle(eElectroCardioGram_WaveformType::Sinus, hr, dt, 1.0);
    SEArrayElectricPotential& signal = ecg.GetWaveform(eElectroCardioGram_WaveformLead::Lead3, eElectroCardioGram_WaveformType::Sinus).GetActiveCycle();

    SEFunctionElectricPotentialVsTime cardiacCycle;
    cardiacCycle.SetElectricPotentialUnit(*signal.GetUnit());
    cardiacCycle.SetTimeUnit(TimeUnit::s);

    size_t size = signal.GetData().size();
    for (size_t s = 0; s < size; s++)
    {
      cardiacCycle.GetElectricPotential().push_back(signal.GetData()[s]);
      cardiacCycle.GetTime().push_back(0.02 * s);
    }
    logger.Info("  - Generated a waveform of : " + std::to_string(size)+" points. Ending time is "+std::to_string(cardiacCycle.GetTime()[size -1]));
    double peak = *std::max_element(std::begin(signal.GetData()), std::end(signal.GetData()));
    peaks.push_back(peak);
    if (peak > peakMax)
      peakMax = peak;
    if (peak < peakMin)
      peakMin = peak;
    //cardiacCycle.WriteCSVFile(sOutputDirectory + "/" + testName + "/sinus."+std::to_string(i)+".csv");
  }
  // Check that the maxes are very close...
  if (peakMax - peakMin > 0.001)
    testCase.AddFailure("Peaks are too far apart");
  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Case"), TimeUnit::s);

  // Take a look at a vtach cycle
  hr.SetValue(0, FrequencyUnit::Per_min);
  ecg.StartNewCycle(eElectroCardioGram_WaveformType::Sinus, hr, dt, 1.0);
  SEArrayElectricPotential& vtach = ecg.GetWaveform(eElectroCardioGram_WaveformLead::Lead3, eElectroCardioGram_WaveformType::VentricularTachycardia).GetActiveCycle();

  SEFunctionElectricPotentialVsTime vtachCycle;
  vtachCycle.SetElectricPotentialUnit(*vtach.GetUnit());
  vtachCycle.SetTimeUnit(TimeUnit::s);

  size_t size = vtach.GetData().size();
  for (size_t s = 0; s < size; s++)
  {
    vtachCycle.GetElectricPotential().push_back(vtach.GetData()[s]);
    vtachCycle.GetTime().push_back(0.02 * s);
  }
  vtachCycle.WriteCSVFile(sOutputDirectory + "/" + testName + "/vtach.csv");

  testReport.SerializeToFile(sOutputDirectory + "/" + testName + "Report.json");
}

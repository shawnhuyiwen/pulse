/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "CommonDataModelTest.h"
#include "utils/testing/SETestReport.h"
#include "utils/testing/SETestCase.h"
#include "utils/testing/SETestSuite.h"
#include "properties/SEScalarTime.h"
#include "utils/FileUtils.h"
#include "utils/TimingProfile.h"
#include "engine/PulseScenarioExec.h"

void CommonDataModelTest::ConvertScenarioLogs(const std::string& rptDirectory)
{
  TimingProfile pTimer;
  std::string testName = "ConvertScenarioLogs";
  m_Logger->SetLogFile(rptDirectory + "/" + testName + ".log");

  std::string inputDir = GetCurrentWorkingDirectory();
  inputDir.append("/verification/scenarios");
  std::string outputDir = GetCurrentWorkingDirectory();
  outputDir.append("/test_results/converted_scenarios");

  SETestReport testReport(m_Logger);
  SETestSuite& testSuite = testReport.CreateTestSuite();
  testSuite.SetName(testName);

  SETestCase& testCase1 = testSuite.CreateTestCase();
  pTimer.Start("Log Conversion");
  PulseScenarioExec opts1(m_Logger);
  opts1.LogToConsole(eSwitch::Off);
  opts1.OrganizeOutputDirectory(eSwitch::On);
  opts1.SetOutputRootDirectory(outputDir);
  opts1.SetScenarioLogDirectory(inputDir);
  if (!opts1.Execute())
    testCase1.AddFailure("Log conversion failure");
  testCase1.GetDuration().SetValue(pTimer.GetElapsedTime_s("Log Conversion"), TimeUnit::s);
  testCase1.SetName("Log Conversion");
  // TODO: would be nice to listen to errors on the logger and add them to the testCase failures...

  // Eventually we may want to run converted scenarios and compare results
  //SETestCase& testCase2 = testSuite.CreateTestCase();
  //pTimer.Start("Run converted scenarios");
  //PulseScenarioExec opts2(m_Logger);
  //opts2.LogToConsole(eSwitch::Off);
  //opts2.SetModelType(eModelType::HumanAdultWholeBody);
  //opts2.SetScenarioDirectory(outputDir);
  //if (!opts2.Execute())
  //  testCase1.AddFailure("Converted scenario failed to run");
  //testCase2.GetDuration().SetValue(pTimer.GetElapsedTime_s("Run converted scenarios"), TimeUnit::s);
  //testCase2.SetName("Run Converted Scenarios");

  testReport.SerializeToFile(rptDirectory +"/"+testName+"Report.json");
}

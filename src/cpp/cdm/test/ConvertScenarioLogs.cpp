/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "CommonDataModelTest.h"
#include "cdm/engine/SEAction.h"
#include "cdm/engine/SEAdvanceTime.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/scenario/SEScenario.h"
#include "cdm/scenario/SEScenarioLog.h"
#include "utils/testing/SETestReport.h"
#include "utils/testing/SETestCase.h"
#include "utils/testing/SETestSuite.h"
#include "properties/SEScalarTime.h"
#include "utils/FileUtils.h"
#include "utils/TimingProfile.h"

#include <typeinfo>

void CommonDataModelTest::ConvertScenarioLogs(const std::string& rptDirectory)
{
  TimingProfile pTimer;
  std::string testName = "ConvertScenarioLogs";
  m_Logger->SetLogFile(rptDirectory + "/" + testName + ".log");

  SETestReport testReport(m_Logger);
  SETestSuite& testSuite = testReport.CreateTestSuite();
  testSuite.SetName(testName);

  // Locate verification directory for scenario logs
  std::string line;
  std::string inputDir;
  std::ifstream runConfig;
  runConfig.open(GetCurrentWorkingDirectory() + "/run.config");
  while (!runConfig.eof()) // To get you all the lines.
  {
    std::getline(runConfig, line); // Get the line.
    std::string keyDir = "verification_dir=";
    size_t pos = line.find(keyDir);
    if (pos != std::string::npos)
    {
      inputDir = line.substr(keyDir.length());
      break;
    }
  }
  runConfig.close();

  size_t num = 0;
  size_t err = 0;
  if (inputDir.empty())
  {
    SETestCase& testCase = testSuite.CreateTestCase();
    testCase.AddFailure("Unable to find run.config to get verification directory");
  }
  else
  {
    inputDir.append("/scenarios");
    std::string outputDir = GetCurrentWorkingDirectory();
    outputDir.append("/test_results/converted_scenarios");

    std::vector<std::string> logs;
    ListFiles(inputDir, logs, true, ".log", ".cnv.log");
    for (std::vector<std::string>::iterator it = logs.begin(); it != logs.end(); ++it)
    {
      Info("Converting " + *it);
      ++num;

      pTimer.Start(*it);
      SETestCase& testCase = testSuite.CreateTestCase();

      std::string path, baseFilename, ext;
      SplitPathFilenameExt(*it, path, baseFilename, ext);
      std::string relativePath = RelativePathFrom(inputDir, *it);
      std::string outputPath = outputDir + "/" + relativePath + "/";
      std::string outLogFilename = outputPath + baseFilename + ".cnv.log";
      std::string outScenarioFilename = outputPath + baseFilename + ".json";

      Logger log;
      log.SetLogFile(outLogFilename);
      log.LogToConsole(false);

      SEScenario sce(&log);
      SEScenarioLog sceL(&log);

      if (!sceL.Convert(*it, sce))
      {
        testCase.AddFailure("Failed to convert " + *it);
        ++err;
      }
      else if (!sce.IsValid())
      {
        testCase.AddFailure("Invalid scenario: " + *it);
        ++err;
      }

      testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s(*it), TimeUnit::s);
      testCase.SetName(*it);
    }
  }

  testReport.SerializeToFile(rptDirectory +"/"+testName+"Report.json");
  Info("Converted " + std::to_string(num - err) + " of " + std::to_string(num) + " scenario logs");
}

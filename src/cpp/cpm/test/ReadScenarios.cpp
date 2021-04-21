/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "EngineTest.h"
#include "PulseScenario.h"
#include "PulseConfiguration.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalarTime.h"
#include "utils/testing/SETestReport.h"
#include "utils/testing/SETestCase.h"
#include "utils/testing/SETestSuite.h"
#include "utils/FileUtils.h"
#include "utils/TimingProfile.h"

void PulseEngineTest::ReadScenarios(const std::string& rptDirectory)
{
  TimingProfile pTimer;
  std::string testName = "ReadScenarios";
  m_Logger->SetLogFile(rptDirectory + "/" + testName + ".log");
  PulseScenario scenario(m_Logger);

  std::string line;
  std::string sce_dir;
  std::ifstream run_config;
  run_config.open(GetCurrentWorkingDirectory()+"/run.config");
  while (!run_config.eof()) // To get you all the lines.
  {
    std::getline(run_config, line); // Get the line.
    size_t pos = line.find("scenario_dir=");
    if (pos != std::string::npos)
    {
      sce_dir = line.substr(13);
      break;
    }
  }
  run_config.close();

  size_t num = 0;
  size_t err = 0;
  SETestReport testReport(m_Logger);
  SETestSuite&  testSuite = testReport.CreateTestSuite();
  testSuite.SetName(testName);

  if (sce_dir.empty())
  {
    SETestCase& testCase = testSuite.CreateTestCase();
    testCase.AddFailure("Unable to find run.config to get scenario directory");
  }
  else
  {
    std::vector<std::string> files;
    ListFiles(sce_dir, files, true, ".json");
    for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
    {
      if (it->find("json") != std::string::npos)
      {
        if (it->find("PFT@") != std::string::npos ||
          it->find("CBC@") != std::string::npos ||
          it->find("MP@") != std::string::npos ||
          it->find("Urinalysis@") != std::string::npos)// Ignore PFT, CBC, UPanel  and MP files
          continue;// TODO should actually peek the file and ensure it starts with a <scenario> tag

        pTimer.Start("Case");
        SETestCase& testCase = testSuite.CreateTestCase();
        Info(it->c_str());
        try
        {
          num++;
          if (scenario.SerializeFromFile(*it))
          {
            if (!scenario.IsValid())
            {
              err++;
              testCase.AddFailure(*it + " is not a valid scenario!");
            }
            // todo check to see that all compartment, substances, property names are valid
          }
          else
          {
            err++;
            testCase.AddFailure(*it + " has failed to load!");
          }
        }
        catch (...)
        {
          err++;
          testCase.AddFailure(*it + " has failed to load! unknown exception.");
        }
        testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Case"), TimeUnit::s);
        testCase.SetName(*it);
      }
    }
  }
  testReport.SerializeToFile(rptDirectory + "/" + testName + "Report.json");
  m_Logger->Info("Opened "+std::to_string(num-err)+" of "+std::to_string(num)+" scenarios");
}

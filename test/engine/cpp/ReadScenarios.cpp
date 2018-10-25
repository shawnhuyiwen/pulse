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
  m_Logger->ResetLogFile(rptDirectory + "/" + testName + ".log");
  SESubstanceManager subMgr(m_Logger);
  subMgr.LoadSubstanceDirectory();
  PulseScenario scenario(subMgr);

  std::string dir = GetCurrentWorkingDirectory();
  dir.append("/verification/scenarios");

  SETestReport testReport(m_Logger);
  SETestSuite&  testSuite = testReport.CreateTestSuite();
  testSuite.SetName(testName);

  std::vector<std::string> files;
  ListFiles(dir, files, ".pba");
  for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
  {
    if (it->find("pba") != std::string::npos)
    {
      if (it->find("PFT@") != std::string::npos ||
          it->find("CBC@") != std::string::npos ||
          it->find("MP@") != std::string::npos  ||
          it->find("Urinalysis@") != std::string::npos)// Ignore PFT, CBC, UPanel  and MP files
        continue;// TODO should actually peek the file and ensure it starts with a <scenario> tag

      pTimer.Start("Case");
      SETestCase& testCase = testSuite.CreateTestCase();
      Info(it->c_str());
      try
      {
        if (scenario.SerializeFromFile(*it,ASCII))
        {
          if (!scenario.IsValid())
            testCase.AddFailure(*it + " is not a valid scenario!");
          // todo check to see that all compartment, substances, property names are valid
        }
        else
        {
          testCase.AddFailure(*it + " has failed to load!");
        }
      }
      catch (...)
      {
        testCase.AddFailure(*it + " has failed to load! unknown exception.");
      }
      testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Case"), TimeUnit::s);
      testCase.SetName(*it);
    }
  }
  testReport.SerializeToFile(rptDirectory + "/" + testName + "Report.pba",ASCII);  
}

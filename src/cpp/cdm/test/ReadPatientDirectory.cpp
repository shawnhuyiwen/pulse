/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "CommonDataModelTest.h"
#include "patient/SEPatient.h"
#include "utils/TimingProfile.h"
#include "utils/FileUtils.h"
#include "utils/testing/SETestReport.h"
#include "utils/testing/SETestSuite.h"
#include "utils/testing/SETestCase.h"
#include "properties/SEScalarTime.h"

void CommonDataModelTest::ReadPatientDirectory(const std::string& rptDirectory)
{
  TimingProfile pTimer;
  std::string testName = "ReadPatientDirectory";
  Logger logger(rptDirectory + "/" + testName+".log");
  SEPatient obj (&logger);

  std::string dir = GetCurrentWorkingDirectory();  
  dir.append("/patients");

  SETestReport testReport(&logger);
  SETestSuite&  testSuite = testReport.CreateTestSuite();
  testSuite.SetName(testName);

  std::vector<std::string> files;
  ListFiles(dir, files, true, ".json");
  for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it) 
  {
    if(it->find("json")!=std::string::npos)
    {
      pTimer.Start(*it);
      SETestCase& testCase = testSuite.CreateTestCase();
      logger.Info(it->c_str());
      if(!obj.SerializeFromFile(*it))
        testCase.AddFailure("Unable to load patient "+*it);
      testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Case"), TimeUnit::s);
      testCase.SetName(obj.GetName());
    }
  }
  testReport.SerializeToFile(rptDirectory +"/"+testName+"Report.json");
}

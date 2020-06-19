/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "CommonDataModelTest.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "utils/testing/SETestReport.h"
#include "utils/testing/SETestCase.h"
#include "utils/testing/SETestSuite.h"
#include "properties/SEScalarTime.h"
#include "utils/FileUtils.h"
#include "utils/TimingProfile.h"

void CommonDataModelTest::ReadSubstanceDirectory(const std::string& rptDirectory)
{
  TimingProfile pTimer;
  std::string testName = "ReadSubstanceDirectory";
  m_Logger->SetLogFile(rptDirectory + "/" + testName + ".log");

  std::string dir = GetCurrentWorkingDirectory();
  dir.append("/substances");

  SETestReport testReport(m_Logger);
  SETestSuite&  testSuite = testReport.CreateTestSuite();
  testSuite.SetName(testName);

  SETestCase& testCase1 = testSuite.CreateTestCase();
  pTimer.Start("Case");
  SESubstanceManager subMgr(m_Logger);
  if(!subMgr.LoadSubstanceDirectory())
    testCase1.AddFailure("Unable to load substances");
  testCase1.GetDuration().SetValue(pTimer.GetElapsedTime_s("Case"), TimeUnit::s);
  testCase1.SetName("Initial Load");
  // TODO would be nice to listen to errors on the logger and add them to the testCase failures...

  // Let's call load again and make sure our addresses did not change
  SETestCase& testCase2 = testSuite.CreateTestCase();
  pTimer.Start("Case");
  if (!subMgr.LoadSubstanceDirectory())
    testCase2.AddFailure("Unable to load substances");
  testCase2.GetDuration().SetValue(pTimer.GetElapsedTime_s("Case"), TimeUnit::s);
  testCase2.SetName("Second Load");

  testReport.SerializeToFile(rptDirectory +"/"+testName+"Report.json");
}

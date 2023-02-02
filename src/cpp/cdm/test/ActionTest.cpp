/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "CommonDataModelTest.h"
#include "cdm/substance/SESubstanceManager.h"

#include "cdm/patient/actions/SEHemorrhage.h"

#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarVolumePerTime.h"

#include "utils/testing/SETestReport.h"
#include "utils/testing/SETestCase.h"
#include "utils/testing/SETestSuite.h"
#include "utils/FileUtils.h"
#include "utils/TimingProfile.h"

template<typename ActionType>
void TestAction(SETestCase& testCase, SESubstanceManager& subMgr, SEAction& action, const std::string& desc="")
{
  testCase.SetName(action.GetName()+desc);
  testCase.Info("Testing " + testCase.GetName());
  std::string s1;
  action.SerializeToString(s1, eSerializationFormat::VERBOSE_JSON);

  ActionType a2;
  a2.SerializeFromString(s1, eSerializationFormat::VERBOSE_JSON, subMgr);
  std::string s2;
  action.SerializeToString(s2, eSerializationFormat::VERBOSE_JSON);

  if (s1 != s2)
    testCase.AddFailure("Serialization string do not match");

  // Not really sure how to test this printed everything...
  // Maybe the user should pass in the number of properties they set
  // And we make sure we have that many lines + 1 (for the action name line)
  testCase.Info(action.PrettyPrint(s1));
}

void CommonDataModelTest::ActionTest(const std::string& rptDirectory)
{
  std::string testName = "ActionTest";
  m_Logger->SetLogFile(rptDirectory + "/" + testName + ".log");
  //m_Logger->LogToConsole(true); // Uncomment to see pretty prints

  SESubstanceManager subMgr(m_Logger);
  if (!subMgr.LoadSubstanceDirectory())
  {
    m_Logger->Fatal("Unable to load substance directory");
    return;
  }

  std::string dir = GetCurrentWorkingDirectory();
  dir.append("/substances");

  SETestReport testReport(m_Logger);
  SETestSuite& testSuite = testReport.CreateTestSuite();
  testSuite.SetName(testName);

  SEHemorrhage h;
  h.GetSeverity().SetValue(0.8);
  h.SetType(eHemorrhage_Type::External);
  h.SetCompartment(eHemorrhage_Compartment::RightLeg);
  TestAction<SEHemorrhage>(testSuite.CreateTestCase(), subMgr, h, "-Severity");
  h.GetSeverity().Invalidate();
  h.SetType(eHemorrhage_Type::Internal);
  h.GetFlowRate().SetValue(1, VolumePerTimeUnit::mL_Per_s);
  TestAction<SEHemorrhage>(testSuite.CreateTestCase(), subMgr, h, "-FlowRate");

  testReport.SerializeToFile(rptDirectory + "/" + testName + "Report.json");
}

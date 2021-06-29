/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/respiratory/test/RespiratoryEngineTest.h"
#include "engine/respiratory/RespiratoryEngine.h"

#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/patient/SEPatient.h"
#include "cdm/utils/testing/SETestReport.h"
#include "cdm/utils/testing/SETestCase.h"
#include "cdm/utils/testing/SETestSuite.h"
#include "cdm/properties/SEScalarVolume.h"


RespiratoryEngineTest::RespiratoryEngineTest(Logger* logger) : SETestManager(logger)
{
  FillFunctionMap();
}
RespiratoryEngineTest::~RespiratoryEngineTest()
{

}

bool RespiratoryEngineTest::RunTest(const std::string& testName, const std::string& sOutputDirectory)
{
  try
  {
    //If you find the test name, run it
    if (testMap.find(testName) != testMap.end())
    {
      testFunction func = testMap.at(testName);
      (this->*func)(sOutputDirectory);
      return true;
    }
    else
    {
      //Try to find the function with "Test" appended; run it if found
      if (testMap.find(testName + "Test") != testMap.end())
      {
        testFunction func = testMap.at(testName + "Test");
        (this->*func)(sOutputDirectory);
        return true;
      }

      m_ss << "Could not find " << testName << " or " << testName << "Test in PulseEngineTest." << std::endl;
      Error(m_ss);
      return false;
    }
  }
  catch (CommonDataModelException& ex)
  {
    Error(ex.what());
  }
  catch (std::exception& ex)
  {
    Error(ex.what());
  }
  catch (...)
  {
    Error("Exception!!");
  }
  return false;
}

void RespiratoryEngineTest::FillFunctionMap()
{
  testMap.insert(std::make_pair("Smoketest", &RespiratoryEngineTest::SmokeTest));
}

void RespiratoryEngineTest::SmokeTest(const std::string& sTestDirectory)
{
  std::string testName = "SmokeTest";
  m_Logger->SetLogFile(sTestDirectory+"/"+testName+".log");

  std::unique_ptr<PhysiologyEngine> e;

  SETestReport testReport(m_Logger);
  SETestSuite& testSuite = testReport.CreateTestSuite();
  SETestCase& ctor = testSuite.CreateTestCase("ctor");
  try
  {
    e = CreateRespiratoryEngine(m_Logger);
  }
  catch (...)
  {
    ctor.AddFailure("Unable to instantiate a Respiratory Engine");
    return; // No sense in running the rest...
  }

  std::string output;
  SETestCase& unsupported = testSuite.CreateTestCase("serialization");
  if (e->SerializeFromFile("Not Suported"))
    unsupported.AddFailure("Respiratory Engine does not support SerializeFromFile, but says it does?");
  if (e->SerializeToFile("Not Suported"))
    unsupported.AddFailure("Respiratory Engine does not support SerializeToFile, but says it does?");
  if (e->SerializeFromString("Not Suported", SerializationFormat::JSON))
    unsupported.AddFailure("Respiratory Engine does not support SerializeFromString, but says it does?");
  if (e->SerializeToString(output, SerializationFormat::JSON))
    unsupported.AddFailure("Respiratory Engine does not support SerializeToString, but says it does?");

  // Setup a few data requests  pe->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  e->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TidalVolume", VolumeUnit::mL);
  e->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TotalLungVolume", VolumeUnit::mL);
  e->GetEngineTracker()->GetDataRequestManager().SetResultsFilename(sTestDirectory+"/"+testName+".csv");

  // Test Overrides?
  // Test Configuration?
  SEPatientConfiguration pCfg(m_Logger);
  pCfg.GetPatient().SetSex(ePatient_Sex::Male);
  SETestCase& simpleRun = testSuite.CreateTestCase("initialization");
  if (!e->InitializeEngine(pCfg))
  {
    simpleRun.AddFailure("Could not initialize Respiratory Engine.");
    return; // No sense in running the rest...
  }

  // Advance time
  for (int i = 0; i < 30*50; i++)
  {
    if (!e->AdvanceModelTime())  // Compute 1 time step
    {
      simpleRun.AddFailure("Could not advance time in Respiratory Engine.");
      break;
    }
    // Pull Track will pull data from the engine and append it to the file
    e->GetEngineTracker()->TrackData(e->GetSimulationTime(TimeUnit::s));
  }
  // Get data from respiratory, and mechanical ventilator

  // Process a RespiratoryMechanics action



  testReport.SerializeToFile(sTestDirectory+"/"+testName+"Report.json");
}

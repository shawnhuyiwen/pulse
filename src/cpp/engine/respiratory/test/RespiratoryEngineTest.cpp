/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/respiratory/test/RespiratoryEngineTest.h"
#include "engine/respiratory/RespiratoryEngine.h"

#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/patient/SEPatient.h"
#include "cdm/patient/actions/SERespiratoryMechanicsConfiguration.h"
#include "cdm/patient/actions/SEIntubation.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceFraction.h"
#include "substance/SESubstanceManager.h"
#include "cdm/system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.h"
#include "cdm/system/physiology/SERespiratoryMechanics.h"
#include "cdm/system/physiology/SERespiratorySystem.h"
#include "cdm/system/physiology/SERespiratoryMechanics.h"
#include "cdm/utils/testing/SETestReport.h"
#include "cdm/utils/testing/SETestCase.h"
#include "cdm/utils/testing/SETestSuite.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarVolumePerPressure.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SECurve.h"


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
  e->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  e->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("InspiratoryExpiratoryRatio");
  e->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanAirwayPressure", PressureUnit::cmH2O);
  e->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("TransrespiratoryPressure", PressureUnit::cmH2O);
  e->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("IntrapulmonaryPressure", PressureUnit::cmH2O);
  e->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("InspiratoryFlow", VolumePerTimeUnit::L_Per_min);
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

  SERespiratoryMechanicsConfiguration config;
  SERespiratoryMechanics& mechanics = config.GetConfiguration();
  // Create a mechanics profile
  double resistance_cmH2O_s_Per_L = 13.0;
  double compliance_mL_Per_cmH2O = 50.0;
  double musclePressure_cmH2O = 13.0;
  double respirationRate_bpm = 15.0;
  double inspiratoryTime_s = 0.9;

  // These need to add to 1.0 (100%)
  const double UpperResistanceFraction = 0.5;
  const double BronchiResistanceFraction = 0.3;
  const double AlveoliDuctResistanceFraction = 0.2;

  double upperResistance = resistance_cmH2O_s_Per_L - (BronchiResistanceFraction * resistance_cmH2O_s_Per_L + AlveoliDuctResistanceFraction * resistance_cmH2O_s_Per_L) / 2;
  double bronchiResistance = 2 * (resistance_cmH2O_s_Per_L - upperResistance) - AlveoliDuctResistanceFraction * resistance_cmH2O_s_Per_L;
  double alveoliDuctResistance = 2 * (resistance_cmH2O_s_Per_L - upperResistance) - bronchiResistance;
  double sideResistance = bronchiResistance + alveoliDuctResistance;

  double RespiratorySideCompliance_L_Per_cmH2O = compliance_mL_Per_cmH2O / 2.0;
  double LungCompliance_L_Per_cmH2O = 2.0 * RespiratorySideCompliance_L_Per_cmH2O;
  double ChestWallCompliance_L_Per_cmH2O = LungCompliance_L_Per_cmH2O;

  double totalBreathTime_s = 1.0 / (respirationRate_bpm / 60.0);
  double inspiratoryFraction = inspiratoryTime_s / totalBreathTime_s;
  double expiratoryFraction = 1.0 - inspiratoryFraction;

  double InspiratoryRiseFraction = inspiratoryFraction;
  double InspiratoryHoldFraction = 0.0;
  double InspiratoryReleaseFraction = MIN(inspiratoryFraction, expiratoryFraction * 0.5);
  double InspiratoryToExpiratoryPauseFraction = 1.0 - InspiratoryRiseFraction - InspiratoryReleaseFraction;
  double ExpiratoryRiseFraction = 0.0;
  double ExpiratoryHoldFraction = 0.0;
  double ExpiratoryReleaseFraction = 0.0;

  mechanics.GetUpperInspiratoryResistance().SetValue(upperResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  mechanics.GetUpperExpiratoryResistance().SetValue(upperResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  mechanics.GetLeftInspiratoryResistance().SetValue(sideResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  mechanics.GetLeftExpiratoryResistance().SetValue(sideResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  mechanics.GetRightInspiratoryResistance().SetValue(sideResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);
  mechanics.GetRightExpiratoryResistance().SetValue(sideResistance, PressureTimePerVolumeUnit::cmH2O_s_Per_L);

  SECurve& lcc = mechanics.GetLeftComplianceCurve();
  SESegmentConstant& lc = lcc.AddConstantSegment();
  lc.GetBeginVolume().SetValue(-std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  lc.GetEndVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  lc.GetCompliance().SetValue(RespiratorySideCompliance_L_Per_cmH2O, VolumePerPressureUnit::mL_Per_cmH2O);
  SECurve& rcc = mechanics.GetRightComplianceCurve();
  SESegmentConstant& rc = rcc.AddConstantSegment();
  rc.GetBeginVolume().SetValue(-std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  rc.GetEndVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  rc.GetCompliance().SetValue(RespiratorySideCompliance_L_Per_cmH2O, VolumePerPressureUnit::mL_Per_cmH2O);

  mechanics.GetInspiratoryRiseTime().SetValue(InspiratoryRiseFraction * totalBreathTime_s, TimeUnit::s);
  mechanics.GetInspiratoryHoldTime().SetValue(InspiratoryHoldFraction * totalBreathTime_s, TimeUnit::s);
  mechanics.GetInspiratoryReleaseTime().SetValue(InspiratoryReleaseFraction * totalBreathTime_s, TimeUnit::s);
  mechanics.GetInspiratoryToExpiratoryPauseTime().SetValue(InspiratoryToExpiratoryPauseFraction * totalBreathTime_s, TimeUnit::s);
  mechanics.GetExpiratoryRiseTime().SetValue(ExpiratoryRiseFraction * totalBreathTime_s, TimeUnit::s);
  mechanics.GetExpiratoryHoldTime().SetValue(ExpiratoryHoldFraction * totalBreathTime_s, TimeUnit::s);
  mechanics.GetExpiratoryReleaseTime().SetValue(ExpiratoryReleaseFraction * totalBreathTime_s, TimeUnit::s);

  mechanics.GetInspiratoryPeakPressure().SetValue(-13.0, PressureUnit::cmH2O);
  mechanics.GetExpiratoryPeakPressure().SetValue(0.0, PressureUnit::cmH2O);

  e->ProcessAction(config);

  // Intubate
  SEIntubation intubation;
  intubation.SetType(eIntubation_Type::Tracheal);
  e->ProcessAction(intubation);
  // TODO jbw/aaron - Figure out how to intubate and connect via a tube on the same timestep
  e->AdvanceModelTime();

  // Setup the ventilator
  double respirationRate_per_min = 12.0;
  double inspiratoryPeriod_s = 1.0;
  double tidalVolume_mL = 900.0;
  double positiveEndExpiredPressure_cmH2O = 5.0;
  double flow_L_Per_min = 60.0;
  double fractionInspiredOxygen = 0.21;

  // Translate ventilator settings
  double totalPeriod_s = 60.0 / respirationRate_per_min;
  double expiratoryPeriod_s = totalPeriod_s - inspiratoryPeriod_s;

  SEMechanicalVentilatorConfiguration MVConfig(e->GetLogger());
  SEMechanicalVentilator& mv = MVConfig.GetConfiguration();
  mv.SetConnection(eMechanicalVentilator_Connection::Tube);
  mv.SetInspirationWaveform(eMechanicalVentilator_DriverWaveform::Square);
  mv.SetExpirationWaveform(eMechanicalVentilator_DriverWaveform::Square);
  mv.GetInspirationTargetFlow().SetValue(flow_L_Per_min, VolumePerTimeUnit::L_Per_min);
  mv.GetPositiveEndExpiredPressure().SetValue(positiveEndExpiredPressure_cmH2O, PressureUnit::cmH2O);
  mv.GetInspirationMachineTriggerTime().SetValue(expiratoryPeriod_s, TimeUnit::s);
  mv.GetInspirationPatientTriggerPressure().SetValue(-0.000001, PressureUnit::cmH2O);
  mv.GetExpirationCycleVolume().SetValue(tidalVolume_mL, VolumeUnit::mL);
  const SESubstance* oxygen = e->GetSubstanceManager().GetSubstance("Oxygen");
  mv.GetFractionInspiredGas(*oxygen).GetFractionAmount().SetValue(fractionInspiredOxygen);
  e->ProcessAction(MVConfig);

  // Advance time
  double simiulationTime_s = 60.0;
  for (int i = 0; i < simiulationTime_s * 50.0; i++)
  {
    if (!e->AdvanceModelTime())  // Compute 1 time step
    {
      simpleRun.AddFailure("Could not advance time in Respiratory Engine.");
      break;
    }
    // Pull Track will pull data from the engine and append it to the file
    e->GetEngineTracker()->TrackData(e->GetSimulationTime(TimeUnit::s));
  }

  testReport.SerializeToFile(sTestDirectory+"/"+testName+"Report.json");
}

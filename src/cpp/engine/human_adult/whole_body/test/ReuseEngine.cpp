/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/human_adult/whole_body/test/EngineTest.h"

#include "cdm/engine/SEConditionManager.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/patient/SEPatient.h"
#include "cdm/patient/conditions/SELobarPneumonia.h"
#include "cdm/patient/actions/SEHemorrhage.h"
#include "cdm/system/physiology/SECardiovascularSystem.h"
#include "cdm/system/physiology/SERespiratorySystem.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarLength.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/utils/GeneralMath.h"
#include "cdm/utils/testing/SETestReport.h"
#include "cdm/utils/testing/SETestCase.h"
#include "cdm/utils/testing/SETestSuite.h"

namespace pulse { namespace human_adult_whole_body
{
  void EngineTest::ReuseEngine(const std::string& rptDirectory)
  {
    std::string testName = "ReuseEngine";
    m_Logger->SetLogFile(rptDirectory + "/" + testName + ".log");

    SETestReport testReport(m_Logger);
    SETestSuite& testSuite = testReport.CreateTestSuite();
    testSuite.SetName(testName);
    SETestCase& testCase = testSuite.CreateTestCase();

    Engine pe;
    SEPatientConfiguration pc;
    SEPatient& patient = pc.GetPatient();
    patient.SetName("ReuseEngineTest");
    patient.SetSex(ePatient_Sex::Male);
    patient.GetAge().SetValue(44, TimeUnit::yr);
    patient.GetWeight().SetValue(170, MassUnit::lb);
    patient.GetHeight().SetValue(71, LengthUnit::in);
    patient.GetBodyFatFraction().SetValue(0.21);
    patient.GetDiastolicArterialPressureBaseline().SetValue(74, PressureUnit::mmHg);
    patient.GetHeartRateBaseline().SetValue(72, FrequencyUnit::Per_min);
    patient.GetRespirationRateBaseline().SetValue(16, FrequencyUnit::Per_min);
    patient.GetSystolicArterialPressureBaseline().SetValue(114, PressureUnit::mmHg);
    // Add a condition
    pc.GetConditions().GetLobarPneumonia().GetSeverity().SetValue(0.2);
    pc.GetConditions().GetLobarPneumonia().GetLeftLungAffected().SetValue(0.2);
    pc.GetConditions().GetLobarPneumonia().GetRightLungAffected().SetValue(0.2);

    m_Logger->Info("First Stabilization...");
    if (!pe.InitializeEngine(pc))
    {
      testCase.AddFailure("Could not stabilize first initalization, check the error");
      return;
    }
    double dbp1 = pe.GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg);
    double sbp1 = pe.GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg);
    double hrp1 = pe.GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min);
    double tvp1 = pe.GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL);
    double rrp1 = pe.GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min);

    // Put the engine in a 'bad' way by running a hemorrhage for a while...
    SEHemorrhage h;
    h.SetCompartment(pulse::VascularCompartment::RightLeg);
    h.GetSeverity().SetValue(1.0);
    pe.ProcessAction(h);
    h.SetCompartment(pulse::VascularCompartment::LeftLeg);
    h.GetSeverity().SetValue(1.0);
    pe.ProcessAction(h);
    h.SetCompartment(pulse::VascularCompartment::VenaCava);
    h.GetSeverity().SetValue(0.3);
    pe.ProcessAction(h);
    pe.AdvanceModelTime(120, TimeUnit::s);

    // Now reinitialize the engine to the same initial patient
    m_Logger->Info("\nSecond Stabilization...");
    if (!pe.InitializeEngine(pc))
    {
      testCase.AddFailure("Could not stabilize second initalization, check the error");
      return;
    }
    double dbp2 = pe.GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg);
    double sbp2 = pe.GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg);
    double hrp2 = pe.GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min);
    double tvp2 = pe.GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL);
    double rrp2 = pe.GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min);

    m_Logger->Info("These valuse should be the exact same!");
    m_Logger->Info("dbp1=" + std::to_string(dbp1) + " dbp2=" + std::to_string(dbp2));
    m_Logger->Info("sbp1=" + std::to_string(sbp1) + " sbp2=" + std::to_string(sbp2));
    m_Logger->Info("hrp1=" + std::to_string(hrp1) + " hrp2=" + std::to_string(hrp2));
    m_Logger->Info("tvp1=" + std::to_string(tvp1) + " tvp2=" + std::to_string(tvp2));
    m_Logger->Info("rrp1=" + std::to_string(rrp1) + " rrp2=" + std::to_string(rrp2));

    if (dbp1 != dbp2 || sbp1 != sbp2 || hrp1 != hrp2 || tvp1 != tvp2 || rrp1 != rrp2)
    {
      testCase.AddFailure("Second stabilization not equal to first stabilization");
    }
    testReport.SerializeToFile(rptDirectory + "/" + testName + "Report.json");
  }
END_NAMESPACE_EX

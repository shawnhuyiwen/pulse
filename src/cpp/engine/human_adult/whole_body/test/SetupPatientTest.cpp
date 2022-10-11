/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/human_adult/whole_body/test/EngineTest.h"
#include "engine/human_adult/whole_body/Engine.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

#include "cdm/patient/SEPatient.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarArea.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarHeatCapacitance.h"
#include "cdm/properties/SEScalarHeatCapacitancePerMass.h"
#include "cdm/properties/SEScalarLength.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarMassPerMass.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPower.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressurePerVolume.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/utils/GeneralMath.h"
#include "cdm/utils/TimingProfile.h"
#include "cdm/utils/testing/SETestReport.h"
#include "cdm/utils/testing/SETestCase.h"
#include "cdm/utils/testing/SETestSuite.h"

namespace pulse { namespace human_adult_whole_body
{
  void EngineTest::SetupPatientTest(const std::string& sTestDirectory)
  {
    m_Logger->SetLogFile(sTestDirectory + "/SetupPatientTest.log");

    SETestReport testReport = SETestReport(m_Logger);
    SETestSuite& testSuite = testReport.CreateTestSuite();
    testSuite.SetName("SetupPatient");

    //////////////
    // Defaults //
    //////////////
    bool expectedPass = true;
    SEPatient patient(nullptr);
    SEPatient expectedPatient(nullptr);
    expectedPatient.SetSex(ePatient_Sex::Male);
    expectedPatient.GetAge().SetValue(44, TimeUnit::yr);
    expectedPatient.GetHeight().SetValue(177, LengthUnit::cm);
    expectedPatient.GetIdealBodyWeight().SetValue(72.2755, MassUnit::kg);
    expectedPatient.GetWeight().SetValue(68.1406, MassUnit::kg);
    expectedPatient.GetBodyMassIndex().SetValue(21.75);
    expectedPatient.GetBodyFatFraction().SetValue(0.21);
    expectedPatient.GetLeanBodyMass().SetValue(53.8311, MassUnit::kg);
    expectedPatient.GetBodyDensity().SetValue(1.0505, MassPerVolumeUnit::g_Per_cm3);
    expectedPatient.GetHeartRateBaseline().SetValue(72, FrequencyUnit::Per_min);
    expectedPatient.GetHeartRateMaximum().SetValue(177.2, FrequencyUnit::Per_min);
    expectedPatient.GetHeartRateMinimum().SetValue(0.001, FrequencyUnit::Per_min);
    expectedPatient.GetSystolicArterialPressureBaseline().SetValue(114, PressureUnit::mmHg);
    expectedPatient.GetDiastolicArterialPressureBaseline().SetValue(73.5, PressureUnit::mmHg);
    expectedPatient.GetMeanArterialPressureBaseline().SetValue(87, PressureUnit::mmHg);
    expectedPatient.GetPulsePressureBaseline().SetValue(40.5, PressureUnit::mmHg);
    expectedPatient.GetBloodVolumeBaseline().SetValue(4863.8247, VolumeUnit::mL);
    expectedPatient.GetRespirationRateBaseline().SetValue(12, FrequencyUnit::Per_min);
    expectedPatient.GetRightLungRatio().SetValue(0.525);
    expectedPatient.GetTotalLungCapacity().SetValue(5.782, VolumeUnit::L);
    expectedPatient.GetFunctionalResidualCapacity().SetValue(2.1683, VolumeUnit::L);
    expectedPatient.GetResidualVolume().SetValue(1.1564, VolumeUnit::L);
    expectedPatient.GetTidalVolumeBaseline().SetValue(0.5059, VolumeUnit::L);
    expectedPatient.GetVitalCapacity().SetValue(4.6256, VolumeUnit::L);
    expectedPatient.GetExpiratoryReserveVolume().SetValue(1.0119, VolumeUnit::L);
    expectedPatient.GetInspiratoryReserveVolume().SetValue(3.1079, VolumeUnit::L);
    expectedPatient.GetInspiratoryCapacity().SetValue(3.6138, VolumeUnit::L);
    expectedPatient.GetAlveoliSurfaceArea().SetValue(65.5986, AreaUnit::m2);
    expectedPatient.GetSkinSurfaceArea().SetValue(1.8422, AreaUnit::m2);
    expectedPatient.GetBasalMetabolicRate().SetValue(1601.1466, PowerUnit::kcal_Per_day);
    std::stringstream ss;
    ss << "Defaults (male)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.SetSex(ePatient_Sex::Female);
    expectedPatient.Clear();
    expectedPatient.SetSex(ePatient_Sex::Female);
    expectedPatient.GetAge().SetValue(44, TimeUnit::yr);
    expectedPatient.GetHeight().SetValue(163, LengthUnit::cm);
    expectedPatient.GetIdealBodyWeight().SetValue(55.0984, MassUnit::kg);
    expectedPatient.GetWeight().SetValue(57.7876, MassUnit::kg);
    expectedPatient.GetBodyMassIndex().SetValue(21.75);
    expectedPatient.GetBodyFatFraction().SetValue(0.28);
    expectedPatient.GetLeanBodyMass().SetValue(41.6071, MassUnit::kg);
    expectedPatient.GetBodyDensity().SetValue(1.0345, MassPerVolumeUnit::g_Per_cm3);
    expectedPatient.GetHeartRateBaseline().SetValue(72, FrequencyUnit::Per_min);
    expectedPatient.GetHeartRateMaximum().SetValue(177.2, FrequencyUnit::Per_min);
    expectedPatient.GetHeartRateMinimum().SetValue(0.001, FrequencyUnit::Per_min);
    expectedPatient.GetSystolicArterialPressureBaseline().SetValue(114, PressureUnit::mmHg);
    expectedPatient.GetDiastolicArterialPressureBaseline().SetValue(73.5, PressureUnit::mmHg);
    expectedPatient.GetMeanArterialPressureBaseline().SetValue(87, PressureUnit::mmHg);
    expectedPatient.GetPulsePressureBaseline().SetValue(40.5, PressureUnit::mmHg);
    expectedPatient.GetBloodVolumeBaseline().SetValue(4111.2605, VolumeUnit::mL);
    expectedPatient.GetRespirationRateBaseline().SetValue(12, FrequencyUnit::Per_min);
    expectedPatient.GetRightLungRatio().SetValue(0.525);
    expectedPatient.GetTotalLungCapacity().SetValue(4.4079, VolumeUnit::L);
    expectedPatient.GetFunctionalResidualCapacity().SetValue(1.6530, VolumeUnit::L);
    expectedPatient.GetResidualVolume().SetValue(0.8816, VolumeUnit::L);
    expectedPatient.GetTidalVolumeBaseline().SetValue(0.3857, VolumeUnit::L);
    expectedPatient.GetVitalCapacity().SetValue(3.5263, VolumeUnit::L);
    expectedPatient.GetExpiratoryReserveVolume().SetValue(0.7714, VolumeUnit::L);
    expectedPatient.GetInspiratoryReserveVolume().SetValue(2.3692, VolumeUnit::L);
    expectedPatient.GetInspiratoryCapacity().SetValue(2.7549, VolumeUnit::L);
    expectedPatient.GetAlveoliSurfaceArea().SetValue(50.0083, AreaUnit::m2);
    expectedPatient.GetSkinSurfaceArea().SetValue(1.6180, AreaUnit::m2);
    expectedPatient.GetBasalMetabolicRate().SetValue(1296.4087, PowerUnit::kcal_Per_day);
    ss.str("");
    ss.clear();
    ss << "Defaults (female)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    // Test calling setup multiple times on the same patient
    expectedPass = true;
    ss.str("");
    ss.clear();
    ss << "Set up twice";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    ///////////////////////
    // Standard Patients //
    ///////////////////////
    expectedPass = true;
    patient.Clear();
    patient.GetAge().SetValue(44, TimeUnit::yr);
    patient.GetHeight().SetValue(180.34, LengthUnit::cm);
    patient.GetBodyMassIndex().SetValue(23.71);
    patient.GetBodyFatFraction().SetValue(0.21);
    patient.GetHeartRateBaseline().SetValue(72, FrequencyUnit::Per_min);
    patient.GetMeanArterialPressureBaseline().SetValue(87, PressureUnit::mmHg);
    patient.GetPulsePressureBaseline().SetValue(40.5, PressureUnit::mmHg);
    patient.GetRespirationRateBaseline().SetValue(12, FrequencyUnit::Per_min);
    expectedPatient.Copy(patient);
    expectedPatient.GetWeight().SetValue(170, MassUnit::lb);
    expectedPatient.GetDiastolicArterialPressureBaseline().SetValue(73.5, PressureUnit::mmHg);
    expectedPatient.GetSystolicArterialPressureBaseline().SetValue(114, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Standard Male";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.SetSex(ePatient_Sex::Female);
    patient.GetAge().SetValue(44, TimeUnit::yr);
    patient.GetHeight().SetValue(162.56, LengthUnit::cm);
    patient.GetBodyMassIndex().SetValue(22.31);
    patient.GetBodyFatFraction().SetValue(0.28);
    patient.GetHeartRateBaseline().SetValue(72, FrequencyUnit::Per_min);
    patient.GetMeanArterialPressureBaseline().SetValue(87, PressureUnit::mmHg);
    patient.GetPulsePressureBaseline().SetValue(40.5, PressureUnit::mmHg);
    patient.GetRespirationRateBaseline().SetValue(12, FrequencyUnit::Per_min);
    expectedPatient.Copy(patient);
    expectedPatient.GetWeight().SetValue(129.975, MassUnit::lb);
    expectedPatient.GetDiastolicArterialPressureBaseline().SetValue(73.5, PressureUnit::mmHg);
    expectedPatient.GetSystolicArterialPressureBaseline().SetValue(114, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Standard Female";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    //////////
    // Age //
    /////////
    expectedPass = true;
    patient.Clear();
    patient.GetAge().SetValue(32, TimeUnit::yr);
    expectedPatient.Copy(patient);
    ss.str("");
    ss.clear();
    ss << "Specify Age";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetAge().SetValue(17, TimeUnit::yr);
    ss.str("");
    ss.clear();
    ss << "Underage";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetAge().SetValue(66, TimeUnit::yr);
    ss.str("");
    ss.clear();
    ss << "Over-Age";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    ///////////////////////////////////////////////////////////////
    // Height, Weight, BMI, Ideal Body Weight, Body Fat Fraction //
    ///////////////////////////////////////////////////////////////
    expectedPass = false;
    patient.Clear();
    patient.GetHeight().SetValue(130, LengthUnit::cm);
    ss.str("");
    ss.clear();
    ss << "Too Short";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetHeight().SetValue(220, LengthUnit::cm);
    ss.str("");
    ss.clear();
    ss << "Too Tall";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetHeight().SetValue(174.4557, LengthUnit::cm);
    patient.GetWeight().SetValue(70, MassUnit::kg);
    expectedPatient.Copy(patient);
    expectedPatient.GetBodyMassIndex().SetValue(23);
    expectedPatient.GetIdealBodyWeight().SetValue(69.9717, MassUnit::kg);
    ss.str("");
    ss.clear();
    ss << "Specify Height and Weight (male)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.SetSex(ePatient_Sex::Female);
    patient.GetHeight().SetValue(161.5146, LengthUnit::cm);
    patient.GetWeight().SetValue(60, MassUnit::kg);
    expectedPatient.Copy(patient);
    expectedPatient.GetBodyMassIndex().SetValue(23);
    expectedPatient.GetIdealBodyWeight().SetValue(53.7534, MassUnit::kg);
    ss.str("");
    ss.clear();
    ss << "Specify Height and Weight (female)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetBodyMassIndex().SetValue(15);
    ss.str("");
    ss.clear();
    ss << "Severly Underweight BMI (male)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.SetSex(ePatient_Sex::Female);
    patient.GetBodyMassIndex().SetValue(15);
    ss.str("");
    ss.clear();
    ss << "Severly Underweight BMI (female)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetBodyMassIndex().SetValue(31);
    ss.str("");
    ss.clear();
    ss << "Obese BMI (male)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.SetSex(ePatient_Sex::Female);
    patient.GetBodyMassIndex().SetValue(31);
    ss.str("");
    ss.clear();
    ss << "Obese BMI (female)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetBodyMassIndex().SetValue(17);
    expectedPatient.Copy(patient);
    ss.str("");
    ss.clear();
    ss << "Underweight BMI (male)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.SetSex(ePatient_Sex::Female);
    patient.GetBodyMassIndex().SetValue(17);
    expectedPatient.Copy(patient);
    ss.str("");
    ss.clear();
    ss << "Underweight BMI (female)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetBodyMassIndex().SetValue(27);
    expectedPatient.Copy(patient);
    ss.str("");
    ss.clear();
    ss << "Overweight BMI (male)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.SetSex(ePatient_Sex::Female);
    patient.GetBodyMassIndex().SetValue(27);
    expectedPatient.Copy(patient);
    ss.str("");
    ss.clear();
    ss << "Overweight BMI (female)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetBodyMassIndex().SetValue(23);
    patient.GetHeight().SetValue(174.4557, LengthUnit::cm);
    expectedPatient.Copy(patient);
    expectedPatient.GetWeight().SetValue(70, MassUnit::kg);
    ss.str("");
    ss.clear();
    ss << "Specify BMI and Height";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetBodyMassIndex().SetValue(23);
    patient.GetWeight().SetValue(70, MassUnit::kg);
    expectedPatient.Copy(patient);
    expectedPatient.GetHeight().SetValue(174.4557, LengthUnit::cm);
    ss.str("");
    ss.clear();
    ss << "Specify BMI and Weight";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetBodyMassIndex().SetValue(26);
    patient.GetHeight().SetValue(174.4557, LengthUnit::cm);
    patient.GetWeight().SetValue(70, MassUnit::kg);
    expectedPatient.Copy(patient);
    expectedPatient.GetBodyMassIndex().SetValue(23);
    ss.str("");
    ss.clear();
    ss << "Specify BMI (incorrect), Weight, and Height";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetBodyFatFraction().SetValue(0.22);
    patient.GetWeight().SetValue(70, MassUnit::kg);
    expectedPatient.Copy(patient);
    expectedPatient.GetLeanBodyMass().SetValue(54.6, MassUnit::kg);
    expectedPatient.GetBodyDensity().SetValue(1.0482, MassPerVolumeUnit::g_Per_cm3);
    ss.str("");
    ss.clear();
    ss << "Specify Body Fat Fraction";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetBodyFatFraction().SetValue(0.01);
    ss.str("");
    ss.clear();
    ss << "Less Than Essential Fat Body Fat Fraction (male)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.SetSex(ePatient_Sex::Female);
    patient.GetBodyFatFraction().SetValue(0.05);
    ss.str("");
    ss.clear();
    ss << "Less Than Essential Fat Body Fat Fraction (female)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetBodyFatFraction().SetValue(0.28);
    ss.str("");
    ss.clear();
    ss << "Obese Body Fat Fraction (male)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.SetSex(ePatient_Sex::Female);
    patient.GetBodyFatFraction().SetValue(0.33);
    ss.str("");
    ss.clear();
    ss << "Obese Body Fat Fraction (female)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    ////////////////
    // Heart Rate //
    ////////////////
    expectedPass = false;
    patient.Clear();
    patient.GetHeartRateBaseline().SetValue(40, FrequencyUnit::Per_min);
    ss.str("");
    ss.clear();
    ss << "Extremely Low HR Baseline";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetHeartRateBaseline().SetValue(120, FrequencyUnit::Per_min);
    ss.str("");
    ss.clear();
    ss << "Extremely High HR Baseline";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetHeartRateBaseline().SetValue(55, FrequencyUnit::Per_min);
    expectedPatient.Copy(patient);
    ss.str("");
    ss.clear();
    ss << "Bradycardic HR Baseline";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetHeartRateBaseline().SetValue(105, FrequencyUnit::Per_min);
    expectedPatient.Copy(patient);
    ss.str("");
    ss.clear();
    ss << "Tachycardic HR Baseline";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetHeartRateBaseline().SetValue(72, FrequencyUnit::Per_min);
    patient.GetAge().SetValue(44, TimeUnit::yr);
    expectedPatient.Copy(patient);
    expectedPatient.GetHeartRateMaximum().SetValue(177.2, FrequencyUnit::Per_min);
    ss.str("");
    ss.clear();
    ss << "Computed HR Maximum";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetHeartRateBaseline().SetValue(72, FrequencyUnit::Per_min);
    patient.GetHeartRateMaximum().SetValue(100, FrequencyUnit::Per_min);
    patient.GetHeartRateMinimum().SetValue(10, FrequencyUnit::Per_min);
    expectedPatient.Copy(patient);
    ss.str("");
    ss.clear();
    ss << "Specify HR Min/Max";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetHeartRateBaseline().SetValue(72, FrequencyUnit::Per_min);
    patient.GetHeartRateMaximum().SetValue(60, FrequencyUnit::Per_min);
    ss.str("");
    ss.clear();
    ss << "HR Max Lower Than HR Baseline";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetHeartRateBaseline().SetValue(72, FrequencyUnit::Per_min);
    patient.GetHeartRateMinimum().SetValue(80, FrequencyUnit::Per_min);
    ss.str("");
    ss.clear();
    ss << "HR Min Greater Than HR Baseline";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    ////////////////////////
    // Arterial Pressures //
    ////////////////////////
    expectedPass = true;
    patient.Clear();
    patient.GetSystolicArterialPressureBaseline().SetValue(105, PressureUnit::mmHg);
    expectedPatient.Copy(patient);
    expectedPatient.GetDiastolicArterialPressureBaseline().SetValue(73.5, PressureUnit::mmHg);
    expectedPatient.GetMeanArterialPressureBaseline().SetValue(84, PressureUnit::mmHg);
    expectedPatient.GetPulsePressureBaseline().SetValue(31.5, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Specify Systolic Arterial Pressure Baseline";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetDiastolicArterialPressureBaseline().SetValue(76, PressureUnit::mmHg);
    expectedPatient.Copy(patient);
    expectedPatient.GetSystolicArterialPressureBaseline().SetValue(114, PressureUnit::mmHg);
    expectedPatient.GetMeanArterialPressureBaseline().SetValue(88.6667, PressureUnit::mmHg);
    expectedPatient.GetPulsePressureBaseline().SetValue(38, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Specify Diastolic Arterial Pressure Baseline";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetSystolicArterialPressureBaseline().SetValue(112, PressureUnit::mmHg);
    patient.GetDiastolicArterialPressureBaseline().SetValue(75, PressureUnit::mmHg);
    expectedPatient.Copy(patient);
    expectedPatient.GetMeanArterialPressureBaseline().SetValue(87.3333, PressureUnit::mmHg);
    expectedPatient.GetPulsePressureBaseline().SetValue(37, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Specify Systolic/Diastolic Arterial Pressure Baselines";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetSystolicArterialPressureBaseline().SetValue(85, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Hypotension (Systolic)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetSystolicArterialPressureBaseline().SetValue(125, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Hypertension (Systolic)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetDiastolicArterialPressureBaseline().SetValue(55, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Hypotension (Diastolic)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetDiastolicArterialPressureBaseline().SetValue(85, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Hypertension (Diastolic)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetSystolicArterialPressureBaseline().SetValue(100, PressureUnit::mmHg);
    patient.GetDiastolicArterialPressureBaseline().SetValue(76, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Narrow Pulse Pressure";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetMeanArterialPressureBaseline().SetValue(82, PressureUnit::mmHg);
    expectedPatient.Copy(patient);
    expectedPatient.GetSystolicArterialPressureBaseline().SetValue(109, PressureUnit::mmHg);
    expectedPatient.GetDiastolicArterialPressureBaseline().SetValue(68.5, PressureUnit::mmHg);
    expectedPatient.GetPulsePressureBaseline().SetValue(40.5, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Specify Mean Arterial Pressure Baseline";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetPulsePressureBaseline().SetValue(39, PressureUnit::mmHg);
    expectedPatient.Copy(patient);
    expectedPatient.GetSystolicArterialPressureBaseline().SetValue(113, PressureUnit::mmHg);
    expectedPatient.GetDiastolicArterialPressureBaseline().SetValue(74, PressureUnit::mmHg);
    expectedPatient.GetMeanArterialPressureBaseline().SetValue(87, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Specify Pulse Pressure Baseline";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetPulsePressureBaseline().SetValue(37, PressureUnit::mmHg);
    patient.GetMeanArterialPressureBaseline().SetValue(85, PressureUnit::mmHg);
    expectedPatient.Copy(patient);
    expectedPatient.GetSystolicArterialPressureBaseline().SetValue(109.6667, PressureUnit::mmHg);
    expectedPatient.GetDiastolicArterialPressureBaseline().SetValue(72.6667, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Specify Mean Arterial Pressure and Pulse Pressure Baselines";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetMeanArterialPressureBaseline().SetValue(85, PressureUnit::mmHg);
    patient.GetSystolicArterialPressureBaseline().SetValue(106, PressureUnit::mmHg);
    expectedPatient.Copy(patient);
    expectedPatient.GetDiastolicArterialPressureBaseline().SetValue(74.5, PressureUnit::mmHg);
    expectedPatient.GetPulsePressureBaseline().SetValue(31.5, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Specify Systolic Pressure and Mean Arterial Pressure Baselines";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetPulsePressureBaseline().SetValue(36, PressureUnit::mmHg);
    patient.GetSystolicArterialPressureBaseline().SetValue(108, PressureUnit::mmHg);
    expectedPatient.Copy(patient);
    expectedPatient.GetDiastolicArterialPressureBaseline().SetValue(72, PressureUnit::mmHg);
    expectedPatient.GetMeanArterialPressureBaseline().SetValue(84, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Specify Systolic Pressure and Pulse Pressure Baselines";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetMeanArterialPressureBaseline().SetValue(82, PressureUnit::mmHg);
    patient.GetDiastolicArterialPressureBaseline().SetValue(70, PressureUnit::mmHg);
    expectedPatient.Copy(patient);
    expectedPatient.GetSystolicArterialPressureBaseline().SetValue(106, PressureUnit::mmHg);
    expectedPatient.GetPulsePressureBaseline().SetValue(36, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Specify Diastolic Pressure and Mean Arterial Pressure Baselines";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetPulsePressureBaseline().SetValue(34, PressureUnit::mmHg);
    patient.GetDiastolicArterialPressureBaseline().SetValue(76, PressureUnit::mmHg);
    expectedPatient.Copy(patient);
    expectedPatient.GetSystolicArterialPressureBaseline().SetValue(110, PressureUnit::mmHg);
    expectedPatient.GetMeanArterialPressureBaseline().SetValue(87.3333, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Specify Diastolic Pressure and Pulse Pressure Baselines";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetSystolicArterialPressureBaseline().SetValue(105, PressureUnit::mmHg);
    patient.GetDiastolicArterialPressureBaseline().SetValue(72, PressureUnit::mmHg);
    patient.GetMeanArterialPressureBaseline().SetValue(88, PressureUnit::mmHg);
    expectedPatient.Copy(patient);
    expectedPatient.GetPulsePressureBaseline().SetValue(33, PressureUnit::mmHg);
    expectedPatient.GetMeanArterialPressureBaseline().SetValue(83, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Specify Systolic, Diastolic, and Mean Arterial Pressure Baselines";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetSystolicArterialPressureBaseline().SetValue(110, PressureUnit::mmHg);
    patient.GetDiastolicArterialPressureBaseline().SetValue(76, PressureUnit::mmHg);
    patient.GetPulsePressureBaseline().SetValue(37, PressureUnit::mmHg);
    expectedPatient.Copy(patient);
    expectedPatient.GetPulsePressureBaseline().SetValue(34, PressureUnit::mmHg);
    expectedPatient.GetMeanArterialPressureBaseline().SetValue(87.3333, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Specify Systolic, Diastolic, and Pulse Pressure Baselines";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetSystolicArterialPressureBaseline().SetValue(104, PressureUnit::mmHg);
    patient.GetPulsePressureBaseline().SetValue(35, PressureUnit::mmHg);
    patient.GetMeanArterialPressureBaseline().SetValue(84, PressureUnit::mmHg);
    expectedPatient.Copy(patient);
    expectedPatient.GetDiastolicArterialPressureBaseline().SetValue(69, PressureUnit::mmHg);
    expectedPatient.GetMeanArterialPressureBaseline().SetValue(80.6667, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Specify Systolic, Mean Arterial Pressure, and Pulse Pressure Baselines";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetDiastolicArterialPressureBaseline().SetValue(65, PressureUnit::mmHg);
    patient.GetPulsePressureBaseline().SetValue(36, PressureUnit::mmHg);
    patient.GetMeanArterialPressureBaseline().SetValue(82, PressureUnit::mmHg);
    expectedPatient.Copy(patient);
    expectedPatient.GetSystolicArterialPressureBaseline().SetValue(101, PressureUnit::mmHg);
    expectedPatient.GetMeanArterialPressureBaseline().SetValue(77, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Specify Diastolic, Mean Arterial Pressure, and Pulse Pressure Baselines";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.GetSystolicArterialPressureBaseline().SetValue(102, PressureUnit::mmHg);
    patient.GetDiastolicArterialPressureBaseline().SetValue(68, PressureUnit::mmHg);
    patient.GetPulsePressureBaseline().SetValue(37, PressureUnit::mmHg);
    patient.GetMeanArterialPressureBaseline().SetValue(89, PressureUnit::mmHg);
    expectedPatient.Copy(patient);
    expectedPatient.GetPulsePressureBaseline().SetValue(34, PressureUnit::mmHg);
    expectedPatient.GetMeanArterialPressureBaseline().SetValue(79.3333, PressureUnit::mmHg);
    ss.str("");
    ss.clear();
    ss << "Specify Systolic, Diastolic, Mean Arterial Pressure, and Pulse Pressure Baselines";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    //////////////////
    // Blood volume //
    //////////////////
    expectedPass = true;
    patient.Clear();
    patient.GetBloodVolumeBaseline().SetValue(4700, VolumeUnit::mL);
    patient.GetWeight().SetValue(70, MassUnit::kg);
    expectedPatient.Copy(patient);
    ss.str("");
    ss.clear();
    ss << "Blood Volume";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetBloodVolumeBaseline().SetValue(3750, VolumeUnit::mL);
    patient.GetWeight().SetValue(70, MassUnit::kg);
    ss.str("");
    ss.clear();
    ss << "Hypovolemia";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetBloodVolumeBaseline().SetValue(6250, VolumeUnit::mL);
    patient.GetWeight().SetValue(70, MassUnit::kg);
    ss.str("");
    ss.clear();
    ss << "Excessive Blood Volume";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    //////////////////////
    // Respiration Rate //
    //////////////////////
    expectedPass = true;
    patient.Clear();
    patient.GetRespirationRateBaseline().SetValue(10, FrequencyUnit::Per_min);
    expectedPatient.Copy(patient);
    ss.str("");
    ss.clear();
    ss << "Specify Respiration Rate";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetRespirationRateBaseline().SetValue(25, FrequencyUnit::Per_min);
    ss.str("");
    ss.clear();
    ss << "Respiration Rate Too High";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetRespirationRateBaseline().SetValue(5, FrequencyUnit::Per_min);
    ss.str("");
    ss.clear();
    ss << "Respiration Rate Too Low";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    //////////////////////
    // Right Lung Ratio //
    //////////////////////
    expectedPass = true;
    patient.Clear();
    patient.GetRightLungRatio().SetValue(0.575);
    expectedPatient.Copy(patient);
    ss.str("");
    ss.clear();
    ss << "Specify Right Lung Ratio";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetRightLungRatio().SetValue(0.7);
    ss.str("");
    ss.clear();
    ss << "Right Lung Ratio Too High";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    expectedPass = false;
    patient.Clear();
    patient.GetRightLungRatio().SetValue(0.4);
    ss.str("");
    ss.clear();
    ss << "Right Lung Ratio Too Low";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, nullptr, expectedPass);

    /////////////////////////
    // Respiratory Volumes //
    /////////////////////////
    // ideal weight = 72.2755
    expectedPass = true;
    patient.Clear();
    patient.GetHeight().SetValue(69.685, LengthUnit::in);
    patient.GetTotalLungCapacity().SetValue(5.9, VolumeUnit::L);
    patient.GetFunctionalResidualCapacity().SetValue(2.2, VolumeUnit::L);
    patient.GetResidualVolume().SetValue(1.2, VolumeUnit::L);
    expectedPatient.Copy(patient);
    expectedPatient.GetTidalVolumeBaseline().SetValue(0.4742, VolumeUnit::L);
    expectedPatient.GetVitalCapacity().SetValue(4.7, VolumeUnit::L);
    expectedPatient.GetExpiratoryReserveVolume().SetValue(1.0, VolumeUnit::L);
    expectedPatient.GetInspiratoryReserveVolume().SetValue(3.2258, VolumeUnit::L);
    expectedPatient.GetInspiratoryCapacity().SetValue(3.7, VolumeUnit::L);
    ss.str("");
    ss.clear();
    ss << "Specify Total Lung Capacity, Functional Residual Capacity, and Residual Volume";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    //////////////////////////
    // Alveoli Surface Area //
    //////////////////////////
    expectedPass = true;
    patient.Clear();
    patient.GetHeight().SetValue(69.685, LengthUnit::in);
    patient.GetTotalLungCapacity().SetValue(5.9, VolumeUnit::L);
    patient.GetAlveoliSurfaceArea().SetValue(65, AreaUnit::m2);
    expectedPatient.Copy(patient);
    ss.str("");
    ss.clear();
    ss << "Specify Alveoli Surface Area";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    ///////////////////////
    // Skin Surface Area //
    ///////////////////////
    expectedPass = true;
    patient.Clear();
    patient.GetHeight().SetValue(177, LengthUnit::cm);
    patient.GetWeight().SetValue(70, MassUnit::kg);
    patient.GetSkinSurfaceArea().SetValue(1.9, AreaUnit::m2);
    expectedPatient.Copy(patient);
    ss.str("");
    ss.clear();
    ss << "Specify Skin Surface Area";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    //////////////////////////
    // Basal Metabolic Rate //
    //////////////////////////
    expectedPass = true;
    patient.Clear();
    patient.GetHeight().SetValue(177, LengthUnit::cm);
    patient.GetWeight().SetValue(70, MassUnit::kg);
    patient.GetAge().SetValue(30, TimeUnit::yr);
    patient.GetBasalMetabolicRate().SetValue(1600, PowerUnit::kcal_Per_day);
    expectedPatient.Copy(patient);
    ss.str("");
    ss.clear();
    ss << "Specify Basal Metabolic Rate (male)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    expectedPass = true;
    patient.Clear();
    patient.SetSex(ePatient_Sex::Female);
    patient.GetHeight().SetValue(160, LengthUnit::cm);
    patient.GetWeight().SetValue(60, MassUnit::kg);
    patient.GetAge().SetValue(30, TimeUnit::yr);
    patient.GetBasalMetabolicRate().SetValue(1400, PowerUnit::kcal_Per_day);
    expectedPatient.Copy(patient);
    ss.str("");
    ss.clear();
    ss << "Specify Basal Metabolic Rate (female)";
    SetupPatientTest(testSuite, sTestDirectory, ss.str(), &patient, &expectedPatient, expectedPass);

    testReport.SerializeToFile(sTestDirectory + "/SetupPatientTestReport.json");
  }

  void EngineTest::SetupPatientTest(SETestSuite& testSuite, const std::string& /*sTestDirectory*/, const std::string& sTestName, SEPatient* patient, SEPatient* expectedPatient, bool expectedPass)
  {
    TimingProfile timer;
    timer.Start("TestCase");
    testSuite.GetLogger()->Info("Running " + sTestName);

    SETestCase& testCase = testSuite.CreateTestCase();
    testCase.SetName(sTestName);

    bool pass = pulse::human_adult_whole_body::SetupPatient(*patient);
    std::vector<std::string> errs;
    if (pass != expectedPass)
      testCase.AddFailure("Unexpected pass/fail): " + sTestName);
    else if (pass && expectedPatient != nullptr && !CheckSetupPatient(*patient, *expectedPatient, errs))
    {
      for (auto& e : errs)
        testCase.AddFailure(e);
    }
      

    testCase.GetDuration().SetValue(timer.GetElapsedTime_s("TestCase"), TimeUnit::s);
  }

  bool EngineTest::CheckSetupPatient(SEPatient& setupPatient, SEPatient& expectedPatient, std::vector<std::string>& errs)
  {
    double tol = 0.01;
    std::stringstream ss;
    if (expectedPatient.GetSex() != setupPatient.GetSex())
    {
      ss.str("");
      ss.clear();
      ss << "Sex mismatch : " << "expected " << ((expectedPatient.GetSex() == ePatient_Sex::Male)? "male" : "female");
      errs.push_back(ss.str()); 
    }
    if (expectedPatient.HasAge() && GeneralMath::PercentDifference(expectedPatient.GetAge(TimeUnit::yr), setupPatient.GetAge(TimeUnit::yr)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Age mismatch : " << setupPatient.GetAge(TimeUnit::yr) << "yr expected " << expectedPatient.GetAge(TimeUnit::yr) << "yr";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasWeight() && GeneralMath::PercentDifference(expectedPatient.GetWeight(MassUnit::kg), setupPatient.GetWeight(MassUnit::kg)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Weight mismatch : " << setupPatient.GetWeight(MassUnit::kg) << "kg expected " << expectedPatient.GetWeight(MassUnit::kg) << "kg";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasHeight() && GeneralMath::PercentDifference(expectedPatient.GetHeight(LengthUnit::cm), setupPatient.GetHeight(LengthUnit::cm)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Height mismatch : " << setupPatient.GetHeight(LengthUnit::cm) << "cm expected " << expectedPatient.GetHeight(LengthUnit::cm) << "cm";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasBodyDensity() && GeneralMath::PercentDifference(expectedPatient.GetBodyDensity(MassPerVolumeUnit::g_Per_cm3), setupPatient.GetBodyDensity(MassPerVolumeUnit::g_Per_cm3)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Body density mismatch : " << setupPatient.GetBodyDensity(MassPerVolumeUnit::g_Per_cm3) << "g/cm3 expected " << expectedPatient.GetBodyDensity(MassPerVolumeUnit::g_Per_cm3) << "g/cm3";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasBodyFatFraction() && GeneralMath::PercentDifference(expectedPatient.GetBodyFatFraction().GetValue(), setupPatient.GetBodyFatFraction().GetValue()) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Body fat fraction mismatch : " << setupPatient.GetBodyFatFraction().GetValue() << " expected " << expectedPatient.GetBodyFatFraction().GetValue();
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasBodyMassIndex() && GeneralMath::PercentDifference(expectedPatient.GetBodyMassIndex().GetValue(), setupPatient.GetBodyMassIndex().GetValue()) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Body mass index mismatch : " << setupPatient.GetBodyMassIndex().GetValue() << " expected " << expectedPatient.GetBodyMassIndex().GetValue();
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasLeanBodyMass() && GeneralMath::PercentDifference(expectedPatient.GetLeanBodyMass(MassUnit::kg), setupPatient.GetLeanBodyMass(MassUnit::kg)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Lean body mass mismatch : " << setupPatient.GetLeanBodyMass(MassUnit::kg) << "kg expected " << expectedPatient.GetLeanBodyMass(MassUnit::kg) << "kg";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasIdealBodyWeight() && GeneralMath::PercentDifference(expectedPatient.GetIdealBodyWeight(MassUnit::kg), setupPatient.GetIdealBodyWeight(MassUnit::kg)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Ideal body weight mismatch : " << setupPatient.GetIdealBodyWeight(MassUnit::kg) << "kg expected " << expectedPatient.GetIdealBodyWeight(MassUnit::kg) << "kg";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasAlveoliSurfaceArea() && GeneralMath::PercentDifference(expectedPatient.GetAlveoliSurfaceArea(AreaUnit::m2), setupPatient.GetAlveoliSurfaceArea(AreaUnit::m2)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Alveoli surface area mismatch : " << setupPatient.GetAlveoliSurfaceArea(AreaUnit::m2) << "m2 expected " << expectedPatient.GetAlveoliSurfaceArea(AreaUnit::m2) << "m2";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasRightLungRatio() && GeneralMath::PercentDifference(expectedPatient.GetRightLungRatio().GetValue(), setupPatient.GetRightLungRatio().GetValue()) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Right lung ratio mismatch : " << setupPatient.GetRightLungRatio().GetValue() << " expected " << expectedPatient.GetRightLungRatio().GetValue();
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasSkinSurfaceArea() && GeneralMath::PercentDifference(expectedPatient.GetSkinSurfaceArea(AreaUnit::m2), setupPatient.GetSkinSurfaceArea(AreaUnit::m2)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Skin surface area mismatch : " << setupPatient.GetSkinSurfaceArea(AreaUnit::m2) << "m2 expected " << expectedPatient.GetSkinSurfaceArea(AreaUnit::m2) << "m2";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasBasalMetabolicRate() && GeneralMath::PercentDifference(expectedPatient.GetBasalMetabolicRate(PowerUnit::kcal_Per_day), setupPatient.GetBasalMetabolicRate(PowerUnit::kcal_Per_day)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Basal metabolic rate mismatch : " << setupPatient.GetBasalMetabolicRate(PowerUnit::kcal_Per_day) << "kcal/day expected " << expectedPatient.GetBasalMetabolicRate(PowerUnit::kcal_Per_day) << "kcal/day";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasBloodVolumeBaseline() && GeneralMath::PercentDifference(expectedPatient.GetBloodVolumeBaseline(VolumeUnit::mL), setupPatient.GetBloodVolumeBaseline(VolumeUnit::mL)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Blood volume baseline mismatch : " << setupPatient.GetBloodVolumeBaseline(VolumeUnit::mL) << "mL expected " << expectedPatient.GetBloodVolumeBaseline(VolumeUnit::mL) << "mL";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasDiastolicArterialPressureBaseline() && GeneralMath::PercentDifference(expectedPatient.GetDiastolicArterialPressureBaseline(PressureUnit::mmHg), setupPatient.GetDiastolicArterialPressureBaseline(PressureUnit::mmHg)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Diastolic arterial pressure baseline mismatch : " << setupPatient.GetDiastolicArterialPressureBaseline(PressureUnit::mmHg) << "mmHg expected " << expectedPatient.GetDiastolicArterialPressureBaseline(PressureUnit::mmHg) << "mmHg";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasHeartRateBaseline() && GeneralMath::PercentDifference(expectedPatient.GetHeartRateBaseline(FrequencyUnit::Per_min), setupPatient.GetHeartRateBaseline(FrequencyUnit::Per_min)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Heart rate baseline mismatch : " << setupPatient.GetHeartRateBaseline(FrequencyUnit::Per_min) << "bpm expected " << expectedPatient.GetHeartRateBaseline(FrequencyUnit::Per_min) << "bpm";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasMeanArterialPressureBaseline() && GeneralMath::PercentDifference(expectedPatient.GetMeanArterialPressureBaseline(PressureUnit::mmHg), setupPatient.GetMeanArterialPressureBaseline(PressureUnit::mmHg)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Mean arterial pressure baseline mismatch : " << setupPatient.GetMeanArterialPressureBaseline(PressureUnit::mmHg) << "mmHg expected " << expectedPatient.GetMeanArterialPressureBaseline(PressureUnit::mmHg) << "mmHg";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasPulsePressureBaseline() && GeneralMath::PercentDifference(expectedPatient.GetPulsePressureBaseline(PressureUnit::mmHg), setupPatient.GetPulsePressureBaseline(PressureUnit::mmHg)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Pulse pressure baseline mismatch : " << setupPatient.GetPulsePressureBaseline(PressureUnit::mmHg) << "mmHg expected " << expectedPatient.GetPulsePressureBaseline(PressureUnit::mmHg) << "mmHg";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasRespirationRateBaseline() && GeneralMath::PercentDifference(expectedPatient.GetRespirationRateBaseline(FrequencyUnit::Per_min), setupPatient.GetRespirationRateBaseline(FrequencyUnit::Per_min)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Respiration rate baseline mismatch : " << setupPatient.GetRespirationRateBaseline(FrequencyUnit::Per_min) << "bpm expected " << expectedPatient.GetRespirationRateBaseline(FrequencyUnit::Per_min) << "bpm";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasSystolicArterialPressureBaseline() && GeneralMath::PercentDifference(expectedPatient.GetSystolicArterialPressureBaseline(PressureUnit::mmHg), setupPatient.GetSystolicArterialPressureBaseline(PressureUnit::mmHg)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Systolic arterial pressure baseline mismatch : " << setupPatient.GetSystolicArterialPressureBaseline(PressureUnit::mmHg) << "mmHg expected " << expectedPatient.GetSystolicArterialPressureBaseline(PressureUnit::mmHg) << "mmHg";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasTidalVolumeBaseline() && GeneralMath::PercentDifference(expectedPatient.GetTidalVolumeBaseline(VolumeUnit::L), setupPatient.GetTidalVolumeBaseline(VolumeUnit::L)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Tidal volume baseline mismatch : " << setupPatient.GetTidalVolumeBaseline(VolumeUnit::L) << "L expected " << expectedPatient.GetTidalVolumeBaseline(VolumeUnit::L) << "L";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasHeartRateMaximum() && GeneralMath::PercentDifference(expectedPatient.GetHeartRateMaximum(FrequencyUnit::Per_min), setupPatient.GetHeartRateMaximum(FrequencyUnit::Per_min)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Heart rate maximum mismatch : " << setupPatient.GetHeartRateMaximum(FrequencyUnit::Per_min) << "bpm expected " << expectedPatient.GetHeartRateMaximum(FrequencyUnit::Per_min) << "bpm";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasHeartRateMinimum() && GeneralMath::PercentDifference(expectedPatient.GetHeartRateMinimum(FrequencyUnit::Per_min), setupPatient.GetHeartRateMinimum(FrequencyUnit::Per_min)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Heart rate minimum mismatch : " << setupPatient.GetHeartRateMinimum(FrequencyUnit::Per_min) << "bpm expected " << expectedPatient.GetHeartRateMinimum(FrequencyUnit::Per_min) << "bpm";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasExpiratoryReserveVolume() && GeneralMath::PercentDifference(expectedPatient.GetExpiratoryReserveVolume(VolumeUnit::L), setupPatient.GetExpiratoryReserveVolume(VolumeUnit::L)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Expiratory reserve volume mismatch : " << setupPatient.GetExpiratoryReserveVolume(VolumeUnit::L) << "L expected " << expectedPatient.GetExpiratoryReserveVolume(VolumeUnit::L) << "L";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasFunctionalResidualCapacity() && GeneralMath::PercentDifference(expectedPatient.GetFunctionalResidualCapacity(VolumeUnit::L), setupPatient.GetFunctionalResidualCapacity(VolumeUnit::L)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Functional residual capactiy mismatch : " << setupPatient.GetFunctionalResidualCapacity(VolumeUnit::L) << "L expected " << expectedPatient.GetFunctionalResidualCapacity(VolumeUnit::L) << "L";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasInspiratoryCapacity() && GeneralMath::PercentDifference(expectedPatient.GetInspiratoryCapacity(VolumeUnit::L), setupPatient.GetInspiratoryCapacity(VolumeUnit::L)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Inspiratory capacity mismatch : " << setupPatient.GetInspiratoryCapacity(VolumeUnit::L) << "L expected " << expectedPatient.GetInspiratoryCapacity(VolumeUnit::L) << "L";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasInspiratoryReserveVolume() && GeneralMath::PercentDifference(expectedPatient.GetInspiratoryReserveVolume(VolumeUnit::L), setupPatient.GetInspiratoryReserveVolume(VolumeUnit::L)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Inspiratory reserve volume mismatch : " << setupPatient.GetInspiratoryReserveVolume(VolumeUnit::L) << "L expected " << expectedPatient.GetInspiratoryReserveVolume(VolumeUnit::L) << "L";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasResidualVolume() && GeneralMath::PercentDifference(expectedPatient.GetResidualVolume(VolumeUnit::L), setupPatient.GetResidualVolume(VolumeUnit::L)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Residual volume mismatch : " << setupPatient.GetResidualVolume(VolumeUnit::L) << "L expected " << expectedPatient.GetResidualVolume(VolumeUnit::L) << "L";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasTotalLungCapacity() && GeneralMath::PercentDifference(expectedPatient.GetTotalLungCapacity(VolumeUnit::L), setupPatient.GetTotalLungCapacity(VolumeUnit::L)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Total lung capacity mismatch : " << setupPatient.GetTotalLungCapacity(VolumeUnit::L) << "L expected " << expectedPatient.GetTotalLungCapacity(VolumeUnit::L) << "L";
      errs.push_back(ss.str());
    }
    if (expectedPatient.HasVitalCapacity() && GeneralMath::PercentDifference(expectedPatient.GetVitalCapacity(VolumeUnit::L), setupPatient.GetVitalCapacity(VolumeUnit::L)) > tol)
    {
      ss.str("");
      ss.clear();
      ss << "Vital capacity mismatch : " << setupPatient.GetVitalCapacity(VolumeUnit::L) << "L expected " << expectedPatient.GetVitalCapacity(VolumeUnit::L) << "L";
      errs.push_back(ss.str());
    }

    return errs.empty();
  }

END_NAMESPACE_EX
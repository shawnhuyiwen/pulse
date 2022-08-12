/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PVGenerator.h"
#include "cdm/patient/SEPatient.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarLength.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerPressure.h"
#include "engine/human_adult/whole_body/Engine.h"

#include <map>

// Bounds from: https://pulse.kitware.com/_patient_methodology.html
// And: https://pulse.kitware.com/_cardiovascular_methodology.html#cardiovascular-validation-resting

#define minAge_yr 18
#define maxAge_yr 65
#define minMaleHeight_cm 163
#define maxMaleHeight_cm 190
#define minFemaleHeight_cm 151
#define maxFemaleHeight_cm 175.5
#define minBMI 16
#define maxBMI 29.9
#define minHR_bpm 60
#define maxHR_bpm 100
#define minSystolic_mmHg 90
#define maxSystolic_mmHg 120
#define minDiastolic_mmHg 60
#define maxDiastolic_mmHg 80
#define minMAP_mmHg 70
#define maxMAP_mmHg 100
#define minPulsePressure_mmHg 30
#define maxPulsePressure_mmHg 60

namespace pulse::study::patient_variability
{
  // Bounds from: https://pulse.kitware.com/_patient_methodology.html
  PVGenerator::PVGenerator(Logger* logger) : Loggable(logger),
    Age_yr(minAge_yr, maxAge_yr),
    HeightMale_cm(minMaleHeight_cm, maxMaleHeight_cm),
    HeightFemale_cm(minFemaleHeight_cm, maxFemaleHeight_cm),
    BMI(minBMI, maxBMI),
    HR_bpm(minHR_bpm, maxHR_bpm),
    MAP_mmHg(minMAP_mmHg, maxMAP_mmHg),
    PP_mmHg(minPulsePressure_mmHg, maxPulsePressure_mmHg)
  {
    ResetParameters();
  }
  PVGenerator::~PVGenerator()
  {
  }

  // Reset ParameterSpace
  void PVGenerator::ResetParameters()
  {
    Age_yr.AdjustBounds(minAge_yr, maxAge_yr);
    HeightMale_cm.AdjustBounds(minMaleHeight_cm, maxMaleHeight_cm);
    HeightFemale_cm.AdjustBounds(minFemaleHeight_cm, maxFemaleHeight_cm);
    BMI.AdjustBounds(minBMI, maxBMI);
    HR_bpm.AdjustBounds(minHR_bpm, maxHR_bpm);
    MAP_mmHg.AdjustBounds(minMAP_mmHg, maxMAP_mmHg);
    PP_mmHg.AdjustBounds(minPulsePressure_mmHg, maxPulsePressure_mmHg);

    Age_yr.Reset();
    HeightMale_cm.Reset();
    HeightFemale_cm.Reset();
    BMI.Reset();
    HR_bpm.Reset();
    MAP_mmHg.Reset();
    PP_mmHg.Reset();
  }

  bool PVGenerator::AdjustMeanArterialPressureBounds(double targetPulsePressure_mmHg)
  {
    bool found;

    double minDiastolic = minDiastolic_mmHg;
    double minSystolic;
    double minMAP=0;
    found = false;
    for (; minDiastolic < maxDiastolic_mmHg; minDiastolic++)
    {
      minSystolic = minDiastolic + targetPulsePressure_mmHg;
      minMAP = (minSystolic + (2. * minDiastolic)) / 3.;
      if (IsValidBloodPressure_mmHg(minMAP, minSystolic, minDiastolic))
      {
        found = true;
        break;
      }
    }
    if (!found) return false;

    double maxSystolic = maxSystolic_mmHg;
    double maxDiastolic;
    double maxMAP=0;
    found = false;
    for (; maxSystolic >= minSystolic_mmHg; maxSystolic--)
    {
      maxDiastolic = maxSystolic - targetPulsePressure_mmHg;
      maxMAP = (maxSystolic + (2. * maxDiastolic)) / 3.;
      if (IsValidBloodPressure_mmHg(maxMAP, maxSystolic, maxDiastolic))
      {
        found = true;
        break;
      }
    }
    if (!found) return false;

    MAP_mmHg.AdjustBounds(minMAP, maxMAP);
    return true;
  }
  bool PVGenerator::AdjustPulsePressureBounds(double targetMAP_mmHg)
  {
    bool found = true;
    double maxPulsePressure = maxPulsePressure_mmHg;
    for (; maxPulsePressure >= minPulsePressure_mmHg; maxPulsePressure -= 0.5)
    {
      double systolic = maxPulsePressure + (((3 * targetMAP_mmHg) - maxPulsePressure) / 3);
      double diastolic = systolic - maxPulsePressure;
      if (IsValidBloodPressure_mmHg(targetMAP_mmHg, systolic, diastolic))
      {
        found = true;
        break;
      }
    }
    if (!found) return false;

    found = true;
    double minPulsePressure = minPulsePressure_mmHg;
    for (; minPulsePressure <= maxPulsePressure_mmHg; minPulsePressure += 0.5)
    {
      double systolic = minPulsePressure + (((3 * targetMAP_mmHg) - minPulsePressure) / 3);
      double diastolic = systolic - minPulsePressure;
      if (IsValidBloodPressure_mmHg(targetMAP_mmHg, systolic, diastolic))
      {
        found = true;
        break;
      }
    }
    if (!found) return false;

    PP_mmHg.AdjustBounds(minPulsePressure, maxPulsePressure);
    return true;
  }
  bool PVGenerator::IsValidBloodPressure_mmHg(double map, double systolic, double diastolic)
  {
    return (systolic >= MAX(minSystolic_mmHg, 6.0/5.0 * map) && systolic <= maxSystolic_mmHg &&
            diastolic >= minDiastolic_mmHg && diastolic <= MIN(maxDiastolic_mmHg, 9.0/10.0 * map));
  }

  bool PVGenerator::TestPatientCombo(Logger* logger, ePatient_Sex sex,
    double age_yr, double height_cm, double weight_kg,
    double hr_bpm, double systolic_mmHg, double diastolic_mmHg)
  {
    auto lvl = logger->GetLogLevel();
    logger->SetLogLevel(Logger::Level::Error);
    // Ensure patient is valid
    SEPatient test(logger);
    test.SetSex(sex);
    test.GetAge().SetValue(age_yr, TimeUnit::yr);
    test.GetHeight().SetValue(height_cm, LengthUnit::cm);
    test.GetWeight().SetValue(weight_kg, MassUnit::kg);
    test.GetHeartRateBaseline().SetValue(hr_bpm, FrequencyUnit::Per_min);
    test.GetSystolicArterialPressureBaseline().SetValue(systolic_mmHg, PressureUnit::mmHg);
    test.GetDiastolicArterialPressureBaseline().SetValue(diastolic_mmHg, PressureUnit::mmHg);
    bool b = pulse::human_adult_whole_body::SetupPatient(test);
    logger->SetLogLevel(lvl);
    return b;
  }

  pulse::study::bind::patient_variability::PatientStateData* PVGenerator::AddPatientToList(PatientStateListData& pList,
    const ePatient_Sex sex, unsigned int age_yr, double height_cm, double weight_kg, double bmi,
    double hr_bpm, double map_mmHg, double pp_mmHg, double systolic_mmHg, double diastolic_mmHg,
    double runDuration_s, const std::string& full_dir_path)
  {
    m_MaxNumPatients++;
    Info("Creating patient: " + full_dir_path);

    if (!IsValidBloodPressure_mmHg(map_mmHg, systolic_mmHg, diastolic_mmHg))
    {
      Error("Invalid blood pressure for " + full_dir_path + " : " + pulse::cdm::to_string(systolic_mmHg) + "/" + pulse::cdm::to_string(diastolic_mmHg));
      return nullptr;
    }

    if (!TestPatientCombo(GetLogger(), sex, age_yr, height_cm, weight_kg, hr_bpm, systolic_mmHg, diastolic_mmHg))
    {
      Error("The invalid patient was: " + full_dir_path);
      return nullptr;
    }

    m_TotalRuns++;
    m_TotalPatients++;
    pulse::study::bind::patient_variability::PatientStateData* patientData = pList.add_patient();
    patientData->set_id(m_TotalRuns);
    patientData->set_sex((pulse::cdm::bind::PatientData_eSex)sex);
    patientData->set_age_yr(age_yr);
    patientData->set_bmi(bmi);
    patientData->set_heartrate_bpm(hr_bpm);
    patientData->set_height_cm(height_cm);
    patientData->set_meanarterialpressure_mmhg(map_mmHg);
    patientData->set_pulsepressure_mmhg(pp_mmHg);

    patientData->set_weight_kg(weight_kg);
    patientData->set_diastolicarterialpressure_mmhg(diastolic_mmHg);
    patientData->set_systolicarterialpressure_mmhg(systolic_mmHg);
    patientData->set_outputbasefilename(full_dir_path);
    patientData->set_maxsimulationtime_s(runDuration_s); // Generate 2 mins of data
    patientData->mutable_validation();// Create a validation object to fill
    
    return patientData;
  }

  void PVGenerator::GenerateIndividualParamaterVariabilityPatientList(PatientStateListData& pList)
  {
    m_MaxNumPatients = 0;
    m_TotalPatients = 0;
    m_TotalRuns = 0;


    std::map<ePatient_Sex, std::string> sexes = { {ePatient_Sex::Male, "male"}, {ePatient_Sex::Female, "female"} };
    for (auto sex : sexes)
    {
      // Adjust parameter space to standard patient based on settings
      SEPatient patient(GetLogger());
      if (sex.first == ePatient_Sex::Male)
      {
        patient.SerializeFromFile("./patients/StandardMale.json");
      }
      else if (sex.first == ePatient_Sex::Female)
      {
        patient.SerializeFromFile("./patients/StandardFemale.json");
      }

      double age_yr = patient.GetAge(TimeUnit::yr);
      double height_cm = patient.GetHeight(LengthUnit::cm);
      double height_m = patient.GetHeight(LengthUnit::m);
      double weight_kg = patient.GetWeight(MassUnit::kg);
      double bmi = weight_kg / (height_m * height_m);
      double hr_bpm = patient.GetHeartRateBaseline(FrequencyUnit::Per_min);
      double systolic_mmHg = patient.GetSystolicArterialPressureBaseline(PressureUnit::mmHg);
      double diastolic_mmHg = patient.GetDiastolicArterialPressureBaseline(PressureUnit::mmHg);
      double map_mmHg = (systolic_mmHg + 2.0 * diastolic_mmHg) / 3.0;
      double pp_mmHg = systolic_mmHg - diastolic_mmHg;
      Parameter& Height_cm = sex.first == ePatient_Sex::Male ? HeightMale_cm : HeightFemale_cm;

      std::string sex_dir = "/" + sex.second;
      if (IncludeStandardPatients)
      {
        std::string dir = sex_dir+"/standard";
        AddPatientToList(pList, sex.first, (unsigned int)age_yr, height_cm, weight_kg, bmi,
          hr_bpm, map_mmHg, pp_mmHg, systolic_mmHg, diastolic_mmHg, 120, dir);
      }

      for (auto v : Age_yr.Values())
      {
        std::string dir = sex_dir+"/age_yr" + pulse::cdm::to_string(v);
        AddPatientToList(pList, sex.first, (unsigned int)v, height_cm, weight_kg, bmi,
          hr_bpm, map_mmHg, pp_mmHg, systolic_mmHg, diastolic_mmHg, 120, dir);
      }

      for (auto v : Height_cm.Values())
      {
        std::string dir = sex_dir+"/height_cm" + pulse::cdm::to_string(v);
        AddPatientToList(pList, sex.first, (unsigned int)age_yr, v, weight_kg, bmi,
          hr_bpm, map_mmHg, pp_mmHg, systolic_mmHg, diastolic_mmHg, 120, dir);
      }

      for (auto v : BMI.Values())
      {
        std::string dir = sex_dir+"/bmi" + pulse::cdm::to_string(v);
        // Caclulate weight (kg) = BMI * m2
        double w = bmi * height_m * height_m;
        AddPatientToList(pList, sex.first, (unsigned int)age_yr, height_cm, w, v,
          hr_bpm, map_mmHg, pp_mmHg, systolic_mmHg, diastolic_mmHg, 120, dir);
      }

      for (auto v : HR_bpm.Values())
      {
        std::string dir = sex_dir+"/hr_bpm" + pulse::cdm::to_string(v);
        AddPatientToList(pList, sex.first, (unsigned int)age_yr, height_cm, weight_kg, bmi,
          v, map_mmHg, pp_mmHg, systolic_mmHg, diastolic_mmHg, 120, dir);
      }

      AdjustMeanArterialPressureBounds(pp_mmHg);
      for (auto v : MAP_mmHg.Values())
      {
        std::string dir = sex_dir+"/map_mmHg" + pulse::cdm::to_string(v);
        double s = pp_mmHg + (((3.0 * v) - pp_mmHg) / 3.0);
        double d = s - pp_mmHg;
        AddPatientToList(pList, sex.first, (unsigned int)age_yr, height_cm, weight_kg, bmi,
          hr_bpm, v, pp_mmHg, s, d, 120, dir);
      }

      AdjustPulsePressureBounds(map_mmHg);
      for (auto v : PP_mmHg.Values())
      {
        std::string dir = sex_dir+"/pp_mmHg" + pulse::cdm::to_string(v);
        double s = v + (((3.0 * map_mmHg) - v) / 3.0);
        double d = s - v;
        AddPatientToList(pList, sex.first, (unsigned int)age_yr, height_cm, weight_kg, bmi,
          hr_bpm, map_mmHg, v, s, d, 120, dir);
      }
    }
    Info("Created " + std::to_string(m_TotalPatients) + " out a total of " + std::to_string(m_MaxNumPatients) + " possible patients");
    Info("Created " + std::to_string(m_TotalRuns) + " total runs");
  }

  void PVGenerator::GenerateMultiVariableCombinationPatientList(PatientStateListData& pList)
  {
    m_MaxNumPatients = 0;
    m_TotalPatients = 0;
    m_TotalRuns = 0;

    std::map<ePatient_Sex, std::string> sexes = { {ePatient_Sex::Male, "male"}, {ePatient_Sex::Female, "female"} };
    for (auto sex : sexes)
    {
      ResetParameters();

      // Adjust parameter space to standard patient based on settings
      SEPatient standard(GetLogger());
      std::pair<double, double> standardBP_mmHg(0, 0);
      double standardMAP_mmHg = 0;
      double standardPP_mmHg = 0;
      if (IncludeStandardPatients)
      {
        if (sex.first == ePatient_Sex::Male)
        {
          standard.SerializeFromFile("./patients/StandardMale.json");
        }
        else if (sex.first == ePatient_Sex::Female)
        {
          standard.SerializeFromFile("./patients/StandardFemale.json");
        }
        if (!AddPatientParameters(standard))
        {
          Error("No data generated");
          return;
        }
        double stdSystolic_mmHg = standard.GetSystolicArterialPressureBaseline(PressureUnit::mmHg);
        double stdDiastolic_mmHg = standard.GetDiastolicArterialPressureBaseline(PressureUnit::mmHg);
        standardBP_mmHg = std::make_pair(stdSystolic_mmHg, stdDiastolic_mmHg);
        standardMAP_mmHg = (stdSystolic_mmHg + 2.0 * stdDiastolic_mmHg) / 3.0;
        standardPP_mmHg = stdSystolic_mmHg - stdDiastolic_mmHg;
      }

      std::string sex_dir = "/" + sex.second;
      Parameter& Height_cm = sex.first == ePatient_Sex::Male ? HeightMale_cm : HeightFemale_cm;

      for (auto age : Age_yr.Values())
      {
        unsigned int age_yr = (unsigned int)age;
        std::string age_dir = "/age_yr" + std::to_string(age_yr);

        for (auto height_cm : Height_cm.Values())
        {
          std::string height_dir = "/height_cm" + pulse::cdm::to_string(height_cm);

          for (auto bmi : BMI.Values())
          {
            std::string bmi_dir = "/bmi" + pulse::cdm::to_string(bmi);

            // BMI = kg/m2
            // kg = BMI * m2
            double height_m = Convert(height_cm, LengthUnit::cm, LengthUnit::m);

            // Caclulate weight (kg) from height (m) and BMI
            double weight_kg = bmi * height_m * height_m;

            for (auto hr_bpm : HR_bpm.Values())
            {
              std::string hr_dir = "/hr_bpm" + pulse::cdm::to_string(hr_bpm);

              for (auto map_mmHg : MAP_mmHg.Values())
              {
                std::string map_dir = "/map_mmHg" + pulse::cdm::to_string(map_mmHg);

                for (auto pp_mmHg : PP_mmHg.Values())
                {
                  std::string pp_dir = "/pp_mmHg" + pulse::cdm::to_string(pp_mmHg);
                  std::string full_dir_path = sex_dir + age_dir + height_dir + bmi_dir + hr_dir + map_dir + pp_dir;

                  double systolic_mmHg = pp_mmHg + (((3. * map_mmHg) - pp_mmHg) / 3.);
                  double diastolic_mmHg = systolic_mmHg - pp_mmHg;

                  switch (GenerateMode)
                  {
                  case Mode::Validation:
                  {
                    AddPatientToList(pList, sex.first, age_yr, height_cm, weight_kg, bmi, hr_bpm, map_mmHg, pp_mmHg, systolic_mmHg, diastolic_mmHg, 120, full_dir_path);
                    break;
                  }

                  case Mode::Hemorrhage:
                  {
                    GenerateHemorrhageOptions(pList, sex.first, age_yr, height_cm, weight_kg, bmi, hr_bpm, map_mmHg, pp_mmHg, systolic_mmHg, diastolic_mmHg, 2*60*60, full_dir_path);
                    break;
                  }
                  }
                }
              }
            }
          }
        }
      }
    }

    Info("Created " + std::to_string(m_TotalPatients) + " out a total of " + std::to_string(m_MaxNumPatients) + " possible patients");
    Info("Created " + std::to_string(m_TotalRuns) + " total runs");
  }

  // Creates a new list of patients, adding all hemorrhage variables to each patient in originalPatients
  void PVGenerator::GenerateHemorrhageOptions(PatientStateListData& pList,
    const ePatient_Sex sex, unsigned int age_yr, double height_cm, double weight_kg, double bmi,
    double hr_bpm, double map_mmHg, double pp_mmHg, double systolic_mmHg, double diastolic_mmHg,
    double runDuration_s, const std::string& full_dir_path)
  {

    std::vector<std::string> HemorrhageCompartments = { "RightArm", "RightLeg" };
    for (auto hemorrhageCompartment : HemorrhageCompartments)
    {
      std::string compartment_dir = +"/" + hemorrhageCompartment;

      int severityIdx, severityN = (int)((HemorrhageSeverityMax - HemorrhageSeverityMin) / HemorrhageSeverityStep);
      for (severityIdx = 0; severityIdx <= severityN; severityIdx++)
      {
        double severity = HemorrhageSeverityMin + HemorrhageSeverityStep * severityIdx;

        std::string severity_dir = "/severity" + pulse::cdm::to_string(severity);

        int triageTimeIdx, triageTimeN = (int)((HemorrhageTriageTimeMax_min - HemorrhageTriageTimeMin_min) / HemorrhageTriageTimeStep_min);
        for (triageTimeIdx = 0; triageTimeIdx <= triageTimeN; triageTimeIdx++)
        {
          m_TotalRuns++;
          double triageTime_min = HemorrhageTriageTimeMin_min + HemorrhageTriageTimeStep_min * triageTimeIdx;

          std::string triageTime_dir = "/triage_min" + pulse::cdm::to_string(triageTime_min);
          std::string hemorrhage_dir_path = full_dir_path + compartment_dir + severity_dir + triageTime_dir;

          auto patientData =
            AddPatientToList(pList, sex, age_yr, height_cm, weight_kg, bmi, hr_bpm, map_mmHg, pp_mmHg, systolic_mmHg, diastolic_mmHg, runDuration_s, hemorrhage_dir_path);

          auto hemorrhage = patientData->mutable_hemorrhage();
          hemorrhage->set_starttime_s(10);
          hemorrhage->set_compartment(hemorrhageCompartment);
          hemorrhage->set_severity(severity);
          hemorrhage->set_triagetime_s(triageTime_min * 60);
        }
      }
    }
  }

  // Add values to our ParameterSpace
  bool PVGenerator::AddPatientParameters(const SEPatient& patient)
  {
    Age_yr.Insert(patient.GetAge(TimeUnit::yr));

    double height_cm = patient.GetHeight(LengthUnit::cm);
    if (patient.GetSex() == ePatient_Sex::Male)
      HeightMale_cm.Insert(height_cm);
    else if (patient.GetSex() == ePatient_Sex::Female)
      HeightFemale_cm.Insert(height_cm);

    double height_m = patient.GetHeight(LengthUnit::m);
    double weight_kg = patient.GetWeight(MassUnit::kg);
    double bmi = weight_kg / (height_m * height_m);
    BMI.Insert(bmi);

    HR_bpm.Insert(patient.GetHeartRateBaseline(FrequencyUnit::Per_min));

    double systolic_mmHg = patient.GetSystolicArterialPressureBaseline(PressureUnit::mmHg);
    double diastolic_mmHg = patient.GetDiastolicArterialPressureBaseline(PressureUnit::mmHg);
    double map_mmHg = (systolic_mmHg + 2 * diastolic_mmHg) / 3.0;
    double pp_mmHg = systolic_mmHg - diastolic_mmHg;

    if (!AdjustMeanArterialPressureBounds(pp_mmHg))
    {
      Error("Unable to adjust bounds to provided patient");
      return false;
    }

    MAP_mmHg.Insert(map_mmHg);
    PP_mmHg.Insert(pp_mmHg);
    return true;
  }
}

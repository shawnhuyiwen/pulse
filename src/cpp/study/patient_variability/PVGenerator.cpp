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


namespace pulse::study::patient_variability
{
  // Bounds from: https://pulse.kitware.com/_patient_methodology.html
  PVGenerator::PVGenerator(Logger* logger) : Loggable(logger),
                                             Age_yr(18, 65),
                                             HeightMale_cm(163, 190),
                                             HeightFemale_cm(151, 175.5),
                                             BMI(16, 30),
                                             HR_bpm(60, 100),
                                             MAP_mmHg(70, 105),
                                             PP_mmHg(30, 60)
  {
  }
  PVGenerator::~PVGenerator()
  {
  }

  bool PVGenerator::TestPatientCombo(Logger* logger, ePatient_Sex sex,
                                     double age_yr, double height_cm, double weight_kg,
                                     double hr_bpm, double diastolic_mmHg, double systolic_mmHg)
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

  void PVGenerator::GeneratePatientList(PatientStateListData& pData)
  {
    int id = 0;
    m_MaxNumPatients = 0;
    m_TotalPatients = 0;
    m_TotalRuns = 0;

    std::map<ePatient_Sex, std::string> sexes = { {ePatient_Sex::Male, "male"}, {ePatient_Sex::Female, "female"} };
    for (auto sex : sexes)
    {
      // Adjust parameter space to standard patient based on settings
      SEPatient* patient = nullptr;
      SEPatientConfiguration pc;
      if (IncludeStandardPatients)
      {
        patient = &(pc.GetPatient());
        if (sex.first == ePatient_Sex::Male)
        {
          patient->SerializeFromFile("./patients/StandardMale.json");
        }
        else if (sex.first == ePatient_Sex::Female)
        {
          patient->SerializeFromFile("./patients/StandardFemale.json");
        }
        AddPatientParameters(*patient);
      }

      std::string sex_dir = "/" + sex.second;
      Parameter& Height_cm = sex.first == ePatient_Sex::Male ? HeightMale_cm : HeightFemale_cm;

      for (auto age: Age_yr.Values())
      {
        unsigned int age_yr = (unsigned int) age;
        std::string age_dir = "/age_yr" + std::to_string(age_yr);

        for (auto height_cm: Height_cm.Values())
        {
          std::string height_dir = "/height_cm" + std::to_string(height_cm);

          for (auto bmi: BMI.Values())
          {
            std::string bmi_dir = "/bmi" + pulse::cdm::to_string(bmi);

            // BMI = kg/m2
            // kg = BMI * m2
            double height_m = Convert(height_cm, LengthUnit::cm, LengthUnit::m);

            // Caclulate weight (kg) from height (m) and BMI
            double weight_kg = bmi * height_m * height_m;

            for (auto hr_bpm: HR_bpm.Values())
            {
              std::string hr_dir = "/hr_bpm" + std::to_string(hr_bpm);

              for (auto map_mmHg: MAP_mmHg.Values())
              {
                std::string map_dir = "/map_mmHg" + std::to_string(map_mmHg);

                for (auto pp_mmHg: PP_mmHg.Values())
                {
                  std::string pp_dir = "/pp_mmHg" + std::to_string(pp_mmHg);
                  std::string full_dir_path = sex_dir + age_dir + height_dir + bmi_dir + hr_dir + map_dir + pp_dir;

                  // systolic - diastolic = pulse pressure
                  // MAP = (systolic + 2 * diastolic) / 3
                  double diastolic_mmHg = (3 * map_mmHg - pp_mmHg) / 3.0;
                  double systolic_mmHg = pp_mmHg + diastolic_mmHg;

                  m_MaxNumPatients++;
                  // Test the patient combination
                  if (!TestPatientCombo(GetLogger(), sex.first, age_yr, height_cm, weight_kg, hr_bpm, diastolic_mmHg, systolic_mmHg))
                  {
                    Error("The invalid patient was: " + full_dir_path);
                    continue;
                  }
                  m_TotalPatients++;
                  Info("Creating patient: " + full_dir_path);

                  switch (Mode)
                  {
                  case Mode::Validation:
                  {
                    m_TotalRuns++;
                    auto patientData = pData.add_patient();
                    patientData->set_id(id++);
                    patientData->set_sex((pulse::cdm::bind::PatientData_eSex)sex.first);
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
                    patientData->set_maxsimulationtime_s(120); // Generate 2 mins of data
                    patientData->mutable_validation();// Create a validation object to fill
                    break;
                  }

                  case Mode::Hemorrhage:
                  {
                    GenerateHemorrhageOptions(pData, id, sex.first, age_yr, height_cm, weight_kg, bmi, hr_bpm, map_mmHg, pp_mmHg, diastolic_mmHg, systolic_mmHg, full_dir_path);
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
  void PVGenerator::GenerateHemorrhageOptions(PatientStateListData& pList, int& id,
    const ePatient_Sex sex, unsigned int age_yr, double height_cm, double weight_kg, double bmi,
    double hr_bpm, double map_mmHg, double pp_mmHg, double diastolic_mmHg, double systolic_mmHg,
    const std::string& full_dir_path)
  {

    std::vector<std::string> hemorrhageCompartments = { "RightArm", "RightLeg" };
    for (auto hemorrhageCompartment : hemorrhageCompartments)
    {
      std::string compartment_dir = + "/"+hemorrhageCompartment;

      int severityIdx, severityN = (int)((hemorrhageSeverityMax - hemorrhageSeverityMin) / hemorrhageSeverityStep);
      for (severityIdx = 0; severityIdx <= severityN; severityIdx++)
      {
        double severity = hemorrhageSeverityMin + hemorrhageSeverityStep * severityIdx;

        std::string severity_dir = "/severity" + pulse::cdm::to_string(severity);

        int triageTimeIdx, triageTimeN = (int)((hemorrhageTriageTimeMax_min - hemorrhageTriageTimeMin_min) / hemorrhageTriageTimeStep_min);
        for (triageTimeIdx = 0; triageTimeIdx <= triageTimeN; triageTimeIdx++)
        {
          m_TotalRuns++;
          double triageTime_min = hemorrhageTriageTimeMin_min + hemorrhageTriageTimeStep_min * triageTimeIdx;

          std::string triageTime_dir = "/triage_min" + pulse::cdm::to_string(triageTime_min);
          std::string hemorrhage_dir_path = full_dir_path + compartment_dir + severity_dir + triageTime_dir;

          auto patientData = pList.add_patient();
          patientData->set_id(id++);
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
          patientData->set_outputbasefilename(hemorrhage_dir_path);
          patientData->set_maxsimulationtime_s(2*60*60); // Generate 2 hours of data (max)

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
  void PVGenerator::AddPatientParameters(const SEPatient& patient)
  {
   
  }


}

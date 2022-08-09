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
  PVGenerator::PVGenerator(Logger* logger) : Loggable(logger)
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

    // Ensure all parameter bounds are within Pulse limits
    AdjustParameterSpaceBounds();

    for (auto sex : m_Parameters.sexes)
    {
      // Adjust parameter space to standard patient based on settings
      SEPatient* patient = nullptr;
      SEPatientConfiguration pc;
      if (m_IncludeStandardPatients)
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
      }

      // Generate the values we will iterate over
      GenerateParameterSpaceValues(patient);

      std::string sex_dir = "/" + sex.second;

      for (auto age: m_ParameterValues.ageRange_yr)
      {
        unsigned int age_yr = (unsigned int) age;
        std::string age_dir = "/age_yr" + std::to_string(age_yr);

        for (auto height_cm: m_ParameterValues.heightRange_cm[sex.first])
        {
          std::string height_dir = "/height_cm" + std::to_string(height_cm);

          for (auto bmi: m_ParameterValues.bmiRange)
          {
            std::string bmi_dir = "/bmi" + pulse::cdm::to_string(bmi);

            // BMI = kg/m2
            // kg = BMI * m2
            double height_m = Convert(height_cm, LengthUnit::cm, LengthUnit::m);

            // Caclulate weight (kg) from height (m) and BMI
            double weight_kg = bmi * height_m * height_m;

            for (auto hr_bpm: m_ParameterValues.hrRange_bpm)
            {
              std::string hr_dir = "/hr_bpm" + std::to_string(hr_bpm);

              for (auto map_mmHg: m_ParameterValues.mapRange_mmHg)
              {
                std::string map_dir = "/map_mmHg" + std::to_string(map_mmHg);

                for (auto pp_mmHg: m_ParameterValues.pulsePressureRange_mmHg)
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

                  switch (m_Mode)
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
    for (auto hemorrhageCompartment : m_Parameters.hemorrhageCompartments)
    {
      std::string compartment_dir = + "/"+hemorrhageCompartment;

      int severityIdx, severityN = (int)((m_Parameters.hemorrhageSeverityMax - m_Parameters.hemorrhageSeverityMin) / m_Parameters.hemorrhageSeverityStep);
      for (severityIdx = 0; severityIdx <= severityN; severityIdx++)
      {
        double severity = m_Parameters.hemorrhageSeverityMin + m_Parameters.hemorrhageSeverityStep * severityIdx;

        std::string severity_dir = "/severity" + pulse::cdm::to_string(severity);

        int triageTimeIdx, triageTimeN = (int)((m_Parameters.hemorrhageTriageTimeMax_min - m_Parameters.hemorrhageTriageTimeMin_min) / m_Parameters.hemorrhageTriageTimeStep_min);
        for (triageTimeIdx = 0; triageTimeIdx <= triageTimeN; triageTimeIdx++)
        {
          m_TotalRuns++;
          double triageTime_min = m_Parameters.hemorrhageTriageTimeMin_min + m_Parameters.hemorrhageTriageTimeStep_min * triageTimeIdx;

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

  // Adjusts parameter space to be within Pulse's limits.
  // Bounds from: https://pulse.kitware.com/_patient_methodology.html
  void PVGenerator::AdjustParameterSpaceBounds()
  {
    ParameterSpace p = m_Parameters;

    // Pulse defined limits
    // TODO: MAP and PP don't take into account `dia > 0.75sys` requirement
    const double ageMin_yr = 18;
    const double ageMax_yr = 65;
    const double bmiMin = 16;
    const double bmiMax = 30;
    const double hrMin_bpm = 60;
    const double hrMax_bpm = 100;
    const double heightMin_cm_male = 163;
    const double heightMin_cm_female = 151;
    const double heightMax_cm_male = 190;
    const double heightMax_cm_female = 175.5;
    const double mapMin_mmHg = 70;
    const double mapMax_mmHg = 93.333333;
    const double pulsePressureMin_mmHg = 10;
    const double pulsePressureMax_mmHg = 60;

    //AGE ---------------------------------------------------------------
    if (m_Parameters.ageMin_yr < ageMin_yr)
    {
      GetLogger()->Warning("Adjusting age (yr) minimum from " + std::to_string(m_Parameters.ageMin_yr) + " to: " + std::to_string(ageMin_yr));
      m_Parameters.ageMin_yr = ageMin_yr;
    }
    if (m_Parameters.ageMax_yr > ageMax_yr)
    {
      GetLogger()->Warning("Adjusting age (yr) maximum from " + std::to_string(m_Parameters.ageMax_yr) + " to: " + std::to_string(ageMax_yr));
      m_Parameters.ageMax_yr = ageMax_yr;
    }

    //BMI ---------------------------------------------------------------
    if (m_Parameters.bmiMin < bmiMin)
    {
      GetLogger()->Warning("Adjusting BMI minimum from " + std::to_string(m_Parameters.bmiMin) + " to: " + std::to_string(bmiMin));
      m_Parameters.bmiMin = bmiMin;
    }
    if (m_Parameters.bmiMax > bmiMax)
    {
      GetLogger()->Warning("Adjusting BMI maximum from " + std::to_string(m_Parameters.bmiMax) + " to: " + std::to_string(bmiMax));
      m_Parameters.bmiMax = bmiMax;
    }
    
    //HR ---------------------------------------------------------------
    if (m_Parameters.hrMin_bpm < hrMin_bpm)
    {
      GetLogger()->Warning("Adjusting HR (bpm) minimum from " + std::to_string(m_Parameters.hrMin_bpm) + " to: " + std::to_string(hrMin_bpm));
      m_Parameters.hrMin_bpm = hrMin_bpm;
    }
    if (m_Parameters.hrMax_bpm > hrMax_bpm)
    {
      GetLogger()->Warning("Adjusting HR (bpm) maximum from " + std::to_string(m_Parameters.hrMax_bpm) + " to: " + std::to_string(hrMax_bpm));
      m_Parameters.hrMax_bpm = hrMax_bpm;
    }

    //HEIGHT (MALE) ---------------------------------------------------------------
    if (m_Parameters.heightMin_cm[ePatient_Sex::Male] < heightMin_cm_male)
    {
      GetLogger()->Warning("Adjusting male height (cm) minimum from " + std::to_string(m_Parameters.heightMin_cm[ePatient_Sex::Male]) + " to: " + std::to_string(heightMin_cm_male));
      m_Parameters.heightMin_cm[ePatient_Sex::Male] = heightMin_cm_male;
    }
    if (m_Parameters.heightMax_cm[ePatient_Sex::Male] > heightMax_cm_male)
    {
      GetLogger()->Warning("Adjusting male height (cm) maximum from " + std::to_string(m_Parameters.heightMax_cm[ePatient_Sex::Male]) + " to: " + std::to_string(heightMax_cm_male));
      m_Parameters.heightMax_cm[ePatient_Sex::Male] = heightMax_cm_male;
    }
    //HEIGHT (FEMALE) ---------------------------------------------------------------
    if (m_Parameters.heightMin_cm[ePatient_Sex::Female] < heightMin_cm_female)
    {
      GetLogger()->Warning("Adjusting female height (cm) minimum from " + std::to_string(m_Parameters.heightMin_cm[ePatient_Sex::Female]) + " to: " + std::to_string(heightMin_cm_female));
      m_Parameters.heightMin_cm[ePatient_Sex::Female] = heightMin_cm_female;
    }
    if (m_Parameters.heightMax_cm[ePatient_Sex::Female] > heightMax_cm_female)
    {
      GetLogger()->Warning("Adjusting female height (cm) maximum from " + std::to_string(m_Parameters.heightMax_cm[ePatient_Sex::Female]) + " to: " + std::to_string(heightMax_cm_female));
      m_Parameters.heightMax_cm[ePatient_Sex::Female] = heightMax_cm_female;
    }

    //MAP ---------------------------------------------------------------
    if (m_Parameters.mapMin_mmHg < mapMin_mmHg)
    {
      GetLogger()->Warning("Adjusting MAP (mmHg) minimum from " + std::to_string(m_Parameters.mapMin_mmHg) + " to: " + std::to_string(mapMin_mmHg));
      m_Parameters.mapMin_mmHg = mapMin_mmHg;
    }
    if (m_Parameters.mapMax_mmHg > mapMax_mmHg)
    {
      GetLogger()->Warning("Adjusting MAP (mmHg) maximum from " + std::to_string(m_Parameters.mapMax_mmHg) + " to: " + std::to_string(mapMax_mmHg));
      m_Parameters.mapMax_mmHg = mapMax_mmHg;
    }

    //PULSE PRESSURE ---------------------------------------------------------------
    if (m_Parameters.pulsePressureMin_mmHg < pulsePressureMin_mmHg)
    {
      GetLogger()->Warning("Adjusting Pulse Pressure (mmHg) minimum from " + std::to_string(m_Parameters.pulsePressureMin_mmHg) + " to: " + std::to_string(pulsePressureMin_mmHg));
      m_Parameters.pulsePressureMin_mmHg = pulsePressureMin_mmHg;
    }
    if (m_Parameters.pulsePressureMax_mmHg > pulsePressureMax_mmHg)
    {
      GetLogger()->Warning("Adjusting Pulse Pressure (mmHg) maximum from " + std::to_string(m_Parameters.pulsePressureMax_mmHg) + " to: " + std::to_string(pulsePressureMax_mmHg));
      m_Parameters.pulsePressureMax_mmHg = pulsePressureMax_mmHg;
    }
  }

  // Generates parameter values to iterate over. Adjusts these values to inclue patient, if provided.
  void PVGenerator::GenerateParameterSpaceValues(const SEPatient* patient)
  {
    ParameterSpace p = m_Parameters;
    if (patient != nullptr)
    {
      if (patient->HasAge())
      {
        double age = patient->GetAge(TimeUnit::yr);
        if (!AdjustParameterBounds(p.ageMin_yr, p.ageMax_yr, p.ageStep_yr, age))
          GetLogger()->Error("Given patient age is not within given bounds, please adjust bounds to include this patient.");
      }
        
      if (patient->HasHeight())
      {
        double height = patient->GetHeight(LengthUnit::cm);
        if (!AdjustParameterBounds(p.heightMin_cm[patient->GetSex()], p.heightMax_cm[patient->GetSex()], p.heightStep_cm, height))
          GetLogger()->Error("Given patient height is not within given bounds, please adjust bounds to include this patient.");

        if (patient->HasWeight())
        {
          // BMI = kg/m2
          double bmi = patient->GetWeight(MassUnit::kg) / (patient->GetHeight(LengthUnit::m) * patient->GetHeight(LengthUnit::m));
          if (!AdjustParameterBounds(p.bmiMin, p.bmiMax, p.bmiStep, bmi))
            GetLogger()->Error("Given patient BMI is not within given bounds, please adjust bounds to include this patient.");
        }
      }

      if (patient->HasHeartRateBaseline())
      {
        double hr = patient->GetHeartRateBaseline(FrequencyUnit::Per_min);
        if (!AdjustParameterBounds(p.hrMin_bpm, p.hrMax_bpm, p.hrStep_bpm, hr))
            GetLogger()->Error("Given patient HR is not within given bounds, please adjust bounds to include this patient.");
      }
      
      if (patient->HasSystolicArterialPressureBaseline() && patient->HasDiastolicArterialPressureBaseline())
      {
        double systolic_mmHg = patient->GetSystolicArterialPressureBaseline(PressureUnit::mmHg);
        double diastolic_mmHg = patient->GetDiastolicArterialPressureBaseline(PressureUnit::mmHg);
        double map = (systolic_mmHg + 2 * diastolic_mmHg) / 3.0;
        double pp = systolic_mmHg - diastolic_mmHg;

        if (!AdjustParameterBounds(p.mapMin_mmHg, p.mapMax_mmHg, p.mapStep_mmHg, map))
          GetLogger()->Error("Given patient MAP is not within given bounds, please adjust bounds to include this patient.");

        if (!AdjustParameterBounds(p.pulsePressureMin_mmHg, p.pulsePressureMax_mmHg, p.pulsePressureStep_mmHg, pp))
          GetLogger()->Error("Given patient pulse pressure is not within given bounds, please adjust bounds to include this patient.");
      }
    }

    // Ensure given bounds are included for each parameter
    m_ParameterValues.ageRange_yr = GenerateParameterValues(p.ageMin_yr, p.ageMax_yr, p.ageStep_yr);
    if(p.ageMin_yr > m_Parameters.ageMin_yr)
      m_ParameterValues.ageRange_yr.push_front(m_Parameters.ageMin_yr);
    if(p.ageMax_yr < m_Parameters.ageMax_yr)
      m_ParameterValues.ageRange_yr.push_back(m_Parameters.ageMax_yr);

    m_ParameterValues.heightRange_cm[ePatient_Sex::Male] = GenerateParameterValues(p.heightMin_cm[ePatient_Sex::Male], p.heightMax_cm[ePatient_Sex::Male], p.heightStep_cm);
    if(p.heightMin_cm[ePatient_Sex::Male] > m_Parameters.heightMin_cm[ePatient_Sex::Male])
      m_ParameterValues.heightRange_cm[ePatient_Sex::Male].push_front(m_Parameters.heightMin_cm[ePatient_Sex::Male]);
    if(p.heightMax_cm[ePatient_Sex::Male] < m_Parameters.heightMax_cm[ePatient_Sex::Male])
      m_ParameterValues.heightRange_cm[ePatient_Sex::Male].push_back(m_Parameters.heightMax_cm[ePatient_Sex::Male]);
    m_ParameterValues.heightRange_cm[ePatient_Sex::Female] = GenerateParameterValues(p.heightMin_cm[ePatient_Sex::Female], p.heightMax_cm[ePatient_Sex::Female], p.heightStep_cm);
    if(p.heightMin_cm[ePatient_Sex::Female] > m_Parameters.heightMin_cm[ePatient_Sex::Female])
      m_ParameterValues.heightRange_cm[ePatient_Sex::Female].push_front(m_Parameters.heightMin_cm[ePatient_Sex::Female]);
    if(p.heightMax_cm[ePatient_Sex::Female] < m_Parameters.heightMax_cm[ePatient_Sex::Female])
      m_ParameterValues.heightRange_cm[ePatient_Sex::Female].push_back(m_Parameters.heightMax_cm[ePatient_Sex::Female]);

    m_ParameterValues.bmiRange = GenerateParameterValues(p.bmiMin, p.bmiMax, p.bmiStep);
    if(p.bmiMin > m_Parameters.bmiMin)
      m_ParameterValues.bmiRange.push_front(m_Parameters.bmiMin);
    if(p.bmiMax < m_Parameters.bmiMax)
      m_ParameterValues.bmiRange.push_back(m_Parameters.bmiMax);

    m_ParameterValues.hrRange_bpm = GenerateParameterValues(p.hrMin_bpm, p.hrMax_bpm, p.hrStep_bpm);
    if(p.hrMin_bpm > m_Parameters.hrMin_bpm)
      m_ParameterValues.hrRange_bpm.push_front(m_Parameters.hrMin_bpm);
    if(p.hrMax_bpm < m_Parameters.hrMax_bpm)
      m_ParameterValues.hrRange_bpm.push_back(m_Parameters.hrMax_bpm);

    m_ParameterValues.mapRange_mmHg = GenerateParameterValues(p.mapMin_mmHg, p.mapMax_mmHg, p.mapStep_mmHg);
    if(p.mapMin_mmHg > m_Parameters.mapMin_mmHg)
      m_ParameterValues.mapRange_mmHg.push_front(m_Parameters.mapMin_mmHg);
    if(p.mapMax_mmHg < m_Parameters.mapMax_mmHg)
      m_ParameterValues.mapRange_mmHg.push_back(m_Parameters.mapMax_mmHg);

    m_ParameterValues.pulsePressureRange_mmHg = GenerateParameterValues(p.pulsePressureMin_mmHg, p.pulsePressureMax_mmHg, p.pulsePressureStep_mmHg);
    if(p.pulsePressureMin_mmHg > m_Parameters.pulsePressureMin_mmHg)
      m_ParameterValues.pulsePressureRange_mmHg.push_front(m_Parameters.pulsePressureMin_mmHg);
    if(p.pulsePressureMax_mmHg < m_Parameters.pulsePressureMax_mmHg)
      m_ParameterValues.pulsePressureRange_mmHg.push_back(m_Parameters.pulsePressureMax_mmHg);
  }

  // Adjust parameter bounds to include given key value
  bool PVGenerator::AdjustParameterBounds(double& min, double& max, double step, double keyValue)
  {
    if (keyValue > max || keyValue < min)
      return false;
  
    double _min = keyValue;
    double _max = keyValue;

    // Increase until we are within a step of max value
    while (max - _max >= step)
    {
      _max += step;
    }

    // Decrease until we are within a step of min value
    while (_min - min >= step)
    {
      _min -= step;
    }

    min = _min;
    max = _max; 

    return true; 
  }

  // Generate list of values
  std::list<double> PVGenerator::GenerateParameterValues(double min, double max, double step)
  {
    std::list<double> values;

    int idx, n = (int)((max - min) / step);
    for (idx = 0; idx <= n; ++idx)
      values.push_back(min + step * idx);
  
    return values;
  }
}

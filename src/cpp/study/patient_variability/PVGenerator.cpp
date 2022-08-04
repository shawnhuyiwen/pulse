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


namespace pulse::study::patient_variability
{
  PVGenerator::PVGenerator(Logger* logger) : Loggable(logger)
  {
  }
  PVGenerator::~PVGenerator()
  {
  }

  void PVGenerator::GeneratePatientList(PatientStateListData& pData)
  {
    int id = 0;

    for (auto sex : parameters.sexes)
    {
      // Adjust parameter space if necessary
      ParameterSpace p = parameters;
      if (includeStandardPatients)
      {
        SEPatientConfiguration pc;
        SEPatient& patient = pc.GetPatient();
        if (sex.first == ePatient_Sex::Male)
        {
          patient.SerializeFromFile("./patients/StandardMale.json");
        }
        else if (sex.first == ePatient_Sex::Female)
        {
          patient.SerializeFromFile("./patients/StandardFemale.json");
        }
        p = AdjustParametersToPatient(patient);
      }

      // Adjust height range based on sex
      double heightMin_cm = p.heightMin_cm[sex.first];
      double heightMax_cm = p.heightMax_cm[sex.first];
      std::string sex_dir = "/" + sex.second;

      int ageIdx, ageN = (int)((p.ageMax_yr - p.ageMin_yr) / p.ageStep_yr);
      for (ageIdx = 0; ageIdx <= ageN; ageIdx++)
      {
        double age_yr = p.ageMin_yr + p.ageStep_yr * ageIdx;
        std::string age_dir = "/age_yr" + std::to_string(age_yr);

        int heightIdx, heightN = (int)((heightMax_cm - heightMin_cm) / p.heightStep_cm);
        for (heightIdx = 0; heightIdx <= heightN; heightIdx++)
        {
          double height_cm = heightMin_cm + p.heightStep_cm * heightIdx;
          std::string height_dir = "/height_cm" + std::to_string(height_cm);

          int bmiIdx, bmiN = (int)((p.bmiMax - p.bmiMin) / p.bmiStep);
          for (bmiIdx = 0; bmiIdx <= bmiN; bmiIdx++)
          {
            double bmi = p.bmiMin + p.bmiStep * bmiIdx;
            std::string bmi_dir = "/bmi" + pulse::cdm::to_string(bmi);

            // BMI = kg/m2
            // kg = BMI * m2
            double height_m = Convert(height_cm, LengthUnit::cm, LengthUnit::m);

            // Caclulate weight (kg) from height (m) and BMI
            double weight_kg = bmi * height_m * height_m;

            int hrIdx, hrN = (int)((p.hrMax_bpm - p.hrMin_bpm) / p.hrStep_bpm);
            for (hrIdx = 0; hrIdx <= hrN; hrIdx++)
            {
              double hr_bpm = p.hrMin_bpm + p.hrStep_bpm * hrIdx;
              std::string hr_dir = "/hr_bpm" + std::to_string(hr_bpm);

              int mapIdx, mapN = (int)((p.mapMax_mmHg - p.mapMin_mmHg) / p.mapStep_mmHg);
              for (mapIdx = 0; mapIdx <= mapN; mapIdx++)
              {
                double map_mmHg = p.mapMin_mmHg + p.mapStep_mmHg * mapIdx;
                std::string map_dir = "/map_mmHg" + std::to_string(map_mmHg);

                int ppIdx, ppN = (int)((p.pulsePressureMax_mmHg - p.pulsePressureMin_mmHg) / p.pulsePressureStep_mmHg);
                for (ppIdx = 0; ppIdx <= ppN; ppIdx++)
                {
                  double pp_mmHg = p.pulsePressureMin_mmHg + p.pulsePressureStep_mmHg * ppIdx;
                  std::string pp_dir = "/pp_mmHg" + std::to_string(pp_mmHg);
                  std::string full_dir_path = age_dir + bmi_dir + hr_dir + map_dir + pp_dir + sex_dir + height_dir;

                  // systolic - diastolic = pulse pressure
                  // MAP = (systolic + 2 * diastolic) / 3
                  double diastolic_mmHg = (3 * map_mmHg - pp_mmHg) / 3.0;
                  double systolic_mmHg = pp_mmHg + diastolic_mmHg;

                  switch (mode)
                  {
                  case Mode::Validation:
                  {
                    auto patientData = pData.add_patient();
                    patientData->set_id(id++);
                    patientData->set_sex((pulse::cdm::bind::PatientData_eSex)sex.first);
                    patientData->set_age_yr(age_yr);
                    patientData->set_height_cm(height_cm);
                    patientData->set_weight_kg(weight_kg);
                    patientData->set_bmi(bmi);
                    patientData->set_heartrate_bpm(hr_bpm);
                    patientData->set_diastolicarterialpressure_mmhg(diastolic_mmHg);
                    patientData->set_systolicarterialpressure_mmhg(systolic_mmHg);
                    patientData->set_outputbasefilename(full_dir_path);
                    patientData->set_maxsimulationtime_s(120); // Generate 2 mins of data
                    patientData->mutable_validation();// Create a validation object to fill
                    break;
                  }

                  case Mode::Hemorrhage:
                  {
                    GenerateHemorrhageOptions(pData, id, sex.first, age_yr, height_cm, weight_kg, bmi, hr_bpm, diastolic_mmHg, systolic_mmHg, p, full_dir_path);
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
  }

  // Creates a new list of patients, adding all hemorrhage variables to each patient in originalPatients
  void PVGenerator::GenerateHemorrhageOptions(PatientStateListData& pList, int& id,
    const ePatient_Sex sex, double age_yr, double height_cm, double weight_kg, double bmi,
    double hr_bpm, double diastolic_mmHg, double systolic_mmHg, const ParameterSpace& p, 
    const std::string& full_dir_path)
  {
    for (auto hemorrhageCompartment : p.hemorrhageCompartments)
    {
      std::string compartment_dir = + "/"+hemorrhageCompartment;

      int severityIdx, severityN = (int)((p.hemorrhageSeverityMax - p.hemorrhageSeverityMin) / p.hemorrhageSeverityStep);
      for (severityIdx = 0; severityIdx <= severityN; severityIdx++)
      {
        double severity = p.hemorrhageSeverityMin + p.hemorrhageSeverityStep * severityIdx;

        std::string severity_dir = "/severity" + pulse::cdm::to_string(severity);

        int triageTimeIdx, triageTimeN = (int)((p.hemorrhageTriageTimeMax_min - p.hemorrhageTriageTimeMin_min) / p.hemorrhageTriageTimeStep_min);
        for (triageTimeIdx = 0; triageTimeIdx <= triageTimeN; triageTimeIdx++)
        {
          double triageTime_min = p.hemorrhageTriageTimeMin_min + p.hemorrhageTriageTimeStep_min * triageTimeIdx;

          std::string triageTime_dir = "/triage_min" + pulse::cdm::to_string(triageTime_min);
          std::string hemorrhage_dir_path = full_dir_path + compartment_dir + severity_dir + triageTime_dir;

          auto patientData = pList.add_patient();
          patientData->set_id(id++);
          patientData->set_sex((pulse::cdm::bind::PatientData_eSex)sex);
          patientData->set_age_yr(age_yr);
          patientData->set_height_cm(height_cm);
          patientData->set_weight_kg(weight_kg);
          patientData->set_bmi(bmi);
          patientData->set_heartrate_bpm(hr_bpm);
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

  // Adjusts parameter space, within Pulse's limits, so that patient is included in parameter space.
  // Bounds from: https://pulse.kitware.com/_patient_methodology.html
  PVGenerator::ParameterSpace PVGenerator::AdjustParametersToPatient(const SEPatient& patient)
  {
    ParameterSpace p = parameters;

    if (patient.HasAge())
    {
      double age = patient.GetAge(TimeUnit::yr);
      AdjustParameter(p.ageMin_yr, p.ageMax_yr, p.ageStep_yr, age, 18, 65);
    }
      
    if (patient.HasHeight())
    {
      double height = patient.GetHeight(LengthUnit::cm);
      double heightLB = (patient.GetSex() == ePatient_Sex::Male) ? 163 : 151;
      double heightUB = (patient.GetSex() == ePatient_Sex::Male) ? 190 : 175.5;
      AdjustParameter(p.heightMin_cm[patient.GetSex()], p.heightMax_cm[patient.GetSex()], p.heightStep_cm, height, heightLB, heightUB);

      if (patient.HasWeight())
      {
        // BMI = kg/m2
        double bmi = patient.GetWeight(MassUnit::kg) / (patient.GetHeight(LengthUnit::m) * patient.GetHeight(LengthUnit::m));
        AdjustParameter(p.bmiMin, p.bmiMax, p.bmiStep, bmi, 16, 30);
      }
    }

    if (patient.HasHeartRateBaseline())
    {
      double hr = patient.GetHeartRateBaseline(FrequencyUnit::Per_min);
      AdjustParameter(p.hrMin_bpm, p.hrMax_bpm, p.hrStep_bpm, hr, 60, 100);
    }
    
    if (patient.HasSystolicArterialPressureBaseline() && patient.HasDiastolicArterialPressureBaseline())
    {
      double systolic_mmHg = patient.GetSystolicArterialPressureBaseline(PressureUnit::mmHg);
      double diastolic_mmHg = patient.GetDiastolicArterialPressureBaseline(PressureUnit::mmHg);
      double map = (systolic_mmHg + 2 * diastolic_mmHg) / 3.0;
      double pp = systolic_mmHg - diastolic_mmHg;

      AdjustParameter(p.mapMin_mmHg, p.mapMax_mmHg, p.mapStep_mmHg, map, 70, 93.333);
      AdjustParameter(p.pulsePressureMin_mmHg, p.pulsePressureMax_mmHg, p.pulsePressureStep_mmHg, pp, 10, 60);
    }

    return p;
  }

  // Adjust a single parameters range so that provided key value is included. Limited to [lowerBound, upperBound]
  void PVGenerator::AdjustParameter(double& min, double& max, double step, double keyValue, double lowerBound, double upperBound)
  {
    double _min = keyValue;
    double _max = keyValue;

    // Increase until we are within a half-step of previous max value or past it
    while (max - _max >= step/2.0)
    {
      _max += step;
    }
    // Ensure remain within Pulse's limits
    while(_max > upperBound)
    {
      _max -= step;
    }

    // Decrease until we are within a half-step of previous min value or past it
    while (_min - min >= step/2.0)
    {
      _min -= step;
    }
    // Ensure remain within Pulse's limits
    while(_min < lowerBound)
    {
      _min += step;
    }

    min = _min;
    max = _max;
  }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PVGenerator.h"
#include "cdm/patient/SEPatient.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarLength.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
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

  void PVGenerator::GeneratePatientList(pulse::study::bind::patient_variability::PatientStateListData& pData)
  {
    int id = 0;

    for( auto sex: sexes)
    {
      // Adjust height range based on sex
      unsigned int heightMin_cm = heightMaleMin_cm;
      unsigned int heightMax_cm = heightMaleMax_cm;
      std::string sex_dir = "/" + sex.second;
      if( sex.first == ePatient_Sex::Female )
      {
        heightMin_cm = heightFemaleMin_cm;
        heightMax_cm = heightFemaleMax_cm;
      }

      for( unsigned int age_yr = ageMin_yr; age_yr <= ageMax_yr; age_yr += ageStep_yr )
      {
        std::string age_dir = "/age_yr" + std::to_string(age_yr);

        for( unsigned int height_cm = heightMin_cm; height_cm <= heightMax_cm; height_cm += heightStep_cm )
        {
          std::string height_dir = "/height_cm" + std::to_string(height_cm);

          int bmiIdx, bmiN = (int)((bmiMax-bmiMin) / bmiStep);
          for( bmiIdx = 0; bmiIdx <= bmiN; bmiIdx++ )
          {
            double bmi = bmiMin + bmiStep * bmiIdx;
            
            std::string bmi_dir = "/bmi" + pulse::cdm::to_string(bmi);

            // BMI = kg/m2
            // kg = BMI * m2
            double height_m = Convert(height_cm, LengthUnit::cm, LengthUnit::m);

            // Caclulate weight (kg) from height (m) and BMI
            double weight_kg = bmi * height_m * height_m;

            for( unsigned int hr_bpm = hrMin_bpm; hr_bpm <= hrMax_bpm; hr_bpm += hrStep_bpm )
            {
              std::string hr_dir = "/hr_bpm" + std::to_string(hr_bpm);

              for( unsigned int map_mmHg = mapMin_mmHg; map_mmHg <= mapMax_mmHg; map_mmHg += mapStep_mmHg )
              {
                std::string map_dir = "/map_mmHg" + std::to_string(map_mmHg);

                for( unsigned int pp_mmHg = pulsePressureMin_mmHg; pp_mmHg <= pulsePressureMax_mmHg; pp_mmHg += pulsePressureStep_mmHg )
                {
                  std::string pp_dir = "/pp_mmHg" + std::to_string(pp_mmHg);
                  std::string full_dir_path = age_dir + bmi_dir + hr_dir + map_dir + pp_dir + sex_dir + height_dir;

                  // systolic - diastolic = pulse pressure
                  // MAP = (systolic + 2 * diastolic) / 3
                  double diastolic_mmHg = (3 * map_mmHg - pp_mmHg) / 3;
                  double systolic_mmHg  = pp_mmHg + diastolic_mmHg;

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
                }
              }
            }
          }
        }
      }
    }

    // Add hemorrhage option variables to all patients if enabled
    if(hemorrhageOptionEnabled)
    {
      id = 0;
      pData = AddHemorrhageOption(pData, id);
    }
  }

  // Creates a new list of patients, adding all hemorrhage variables to each patient in originalPatients
  pulse::study::bind::patient_variability::PatientStateListData PVGenerator::AddHemorrhageOption(pulse::study::bind::patient_variability::PatientStateListData& originalPatients, int& id)
  {
    pulse::study::bind::patient_variability::PatientStateListData patients;

    for (int i = 0; i < originalPatients.patient_size(); i++)
    {
      auto originalPatient = &(*originalPatients.mutable_patient())[i];
      std::string base_dir = originalPatient->outputbasefilename();

      for( auto hemorrhageCompartment: hemorrhageCompartments)
      {
        std::string compartment_dir = "/hemorrhageCompartment" + hemorrhageCompartment;

        int severityIdx, severityN = (int)((hemorrhageSeverityMax-hemorrhageSeverityMin) / hemorrhageSeverityStep);
        for( severityIdx = 0; severityIdx <= severityN; severityIdx++ )
        {
          double severity = hemorrhageSeverityMin + hemorrhageSeverityStep * severityIdx;

          std::string severity_dir = "/severity" + pulse::cdm::to_string(severity);

          int triageTimeIdx, triageTimeN = (int)((hemorrhageTriageTimeMax_min-hemorrhageTriageTimeMin_min) / hemorrhageTriageTimeStep_min);
          for( triageTimeIdx = 0; triageTimeIdx <= triageTimeN; triageTimeIdx++ )
          {
            double triageTime_min = hemorrhageTriageTimeMin_min + hemorrhageTriageTimeStep_min * triageTimeIdx;

            std::string triageTime_dir = "/triageTime_min" + pulse::cdm::to_string(triageTime_min);
            std::string hemorrhage_dir_path = base_dir + compartment_dir + severity_dir + triageTime_dir;

            auto patient = patients.add_patient();
            patient->CopyFrom(*originalPatient);

            auto hemorrhage = patient->mutable_hemorrhage();
            hemorrhage->set_enabled(true);
            hemorrhage->set_starttime_s(hemorrhageOption_startTime_s);
            hemorrhage->set_compartment(hemorrhageCompartment);
            hemorrhage->set_severity(severity);
            hemorrhage->set_starttime_s(hemorrhageOption_startTime_s);
            hemorrhage->set_triagetime_s(triageTime_min * 60);

            patient->set_id(id++);
            patient->set_outputbasefilename(hemorrhage_dir_path);
          }
        }
      }
    }

    return patients;
  }
}

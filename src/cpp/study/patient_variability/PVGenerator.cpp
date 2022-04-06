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

    enum Sex { Male=0, Female=1 };

    for( int sex = Male; sex <= Female; ++sex )
    {
      // Adjust height range based on sex
      unsigned int heightMin_cm = heightMaleMin_cm;
      unsigned int heightMax_cm = heightMaleMax_cm;
      std::string sex_dir = "/male";
      if( sex == Female )
      {
        heightMin_cm = heightFemaleMin_cm;
        heightMax_cm = heightFemaleMax_cm;
        sex_dir = "/female";
      }

      for( unsigned int age_yr = ageMin_yr; age_yr <= ageMax_yr; age_yr += ageStep_yr )
      {
        std::string age_dir = "/age_yr" + std::to_string(age_yr);

        for( unsigned int height_cm = heightMin_cm; height_cm <= heightMax_cm; height_cm += heightStep_cm )
        {
          std::string height_dir = "/height_cm" + std::to_string(height_cm);

          for( double bmi = bmiMin; bmi <= bmiMax; bmi += bmiStep )
          {
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
                  patientData->set_sex((pulse::cdm::bind::PatientData_eSex)sex);
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
  }

}
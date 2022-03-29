/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PVRunner.h"

using namespace pulse::study::patient_variability;

int main(int argc, char* argv[])
{
  enum Sex { Female, Male };

  unsigned int ageMin = 18;
  unsigned int ageMax = 65;
  unsigned int ageStep = 1;

  // Heights in inches
  unsigned int heightMin_male = 65;
  unsigned int heightMax_male = 73;
  unsigned int heightMin_female = 60;
  unsigned int heightMax_female = 67;
  unsigned int heightStep = 1;

  double bmiMin = 18.5;
  double bmiMax = 29;
  double bmiStep = 0.1;

  unsigned int hrMin = 60;
  unsigned int hrMax = 100;
  unsigned int hrStep = 1;

  unsigned int mapMin = 70;
  unsigned int mapMax = 100;
  unsigned int mapStep = 1;

  unsigned int pulsePressureMin = 30;
  unsigned int pulsePressureMax = 50;
  unsigned int pulsePressureStep = 1;

  unsigned long long int num_scenarios = 0;
  for( int sex = Female; sex <= Male; ++sex )
  {
    // Adjust height range based on sex
    unsigned int heightMin = heightMin_male;
    unsigned int heightMax = heightMax_male;
    if( sex == Female )
    {
      heightMin = heightMin_female;
      heightMax = heightMax_female;
    }

    for( unsigned int age = ageMin; age <= ageMax; age += ageStep )
    {
      for( unsigned int height = heightMin; height <= heightMax; height += heightStep )
      {
        for( unsigned int bmi = bmiMin; bmi <= bmiMax; bmi += bmiStep )
        {
          // Convert height to meters
          double height_meters = height * 0.0254;

          // Caclulate weight (kg) from height and BMI
          double weight = bmi * height_meters * height_meters;

          for( unsigned int hr = hrMin; hr <= hrMax; hr += hrStep )
          {
            for( unsigned int map = mapMin; map <= mapMax; map += mapStep )
            {
              for( unsigned int pp = pulsePressureMin; pp <= pulsePressureMax; pp += pulsePressureStep )
              {
                double diastolic = pp - 3 * map;
                double systolic = pp - diastolic;
                num_scenarios++;
                std::cout << "Scenario #" << num_scenarios << std::endl;
              }
            }
          }
        }
      }
    }
  }

  std::cout << "Number scenarios: " << num_scenarios << std::endl;

  return 0;
}
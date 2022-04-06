/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PVRunner.h"
#include "PVGenerator.h"

#include "cdm/utils/FileUtils.h"

using namespace pulse::study::patient_variability;

int main(int argc, char* argv[])
{
  bool clear = true;
  std::string mode = ""; // Default is to run the manual else below
  if (argc <= 1)
  {
    //mode = "full";
    mode = "solo";
    //mode = "test";
  }
  else
  {
    mode = argv[1];
  }
  std::string rootDir = "./test_results/patient_variability/";

  if(clear)
    DeleteDirectory(rootDir);

  pulse::study::bind::patient_variability::PatientStateListData patients;

  Logger log;
  log.LogToConsole(true);
  log.SetLogFile(rootDir +"PatientVariability.log");

  if(mode == "full")
  {
    PVGenerator pvg(&log);
    pvg.ageMin_yr = 18;
    pvg.ageMax_yr = 65;
    pvg.ageStep_yr = 10;

    pvg.heightMaleMin_cm = 165;
    pvg.heightMaleMax_cm = 186;
    pvg.heightFemaleMin_cm = 153;
    pvg.heightFemaleMax_cm = 170;
    pvg.heightStep_cm = 7;

    pvg.bmiMin = 18.5;
    pvg.bmiMax = 29;
    pvg.bmiStep = 3;

    pvg.hrMin_bpm = 60;
    pvg.hrMax_bpm = 100;
    pvg.hrStep_bpm = 15;

    pvg.mapMin_mmHg = 70;
    pvg.mapMax_mmHg = 100;
    pvg.mapStep_mmHg = 10;

    pvg.pulsePressureMin_mmHg = 30;
    pvg.pulsePressureMax_mmHg = 50;
    pvg.pulsePressureStep_mmHg = 10;

    pvg.GeneratePatientList(patients);
  }
  else if (mode == "solo")
  {
    PVGenerator pvg(&log);
    pvg.ageMin_yr = 18;
    pvg.ageMax_yr = 65;
    pvg.ageStep_yr = 50;

    pvg.heightMaleMin_cm = 165;
    pvg.heightMaleMax_cm = 186;
    pvg.heightFemaleMin_cm = 153;
    pvg.heightFemaleMax_cm = 170;
    pvg.heightStep_cm = 22;

    pvg.bmiMin = 18.5;
    pvg.bmiMax = 29;
    pvg.bmiStep = 30;

    pvg.hrMin_bpm = 60;
    pvg.hrMax_bpm = 100;
    pvg.hrStep_bpm = 150;

    pvg.mapMin_mmHg = 70;
    pvg.mapMax_mmHg = 100;
    pvg.mapStep_mmHg = 100;

    pvg.pulsePressureMin_mmHg = 30;
    pvg.pulsePressureMax_mmHg = 50;
    pvg.pulsePressureStep_mmHg = 100;

    pvg.GeneratePatientList(patients);
  }
  else if (mode == "test")
  {
    PVGenerator pvg(&log);
    pvg.ageMin_yr = 18;
    pvg.ageMax_yr = 65;
    pvg.ageStep_yr = 25;

    pvg.heightMaleMin_cm = 165;
    pvg.heightMaleMax_cm = 186;
    pvg.heightFemaleMin_cm = 153;
    pvg.heightFemaleMax_cm = 170;
    pvg.heightStep_cm = 8;

    pvg.bmiMin = 18.5;
    pvg.bmiMax = 29;
    pvg.bmiStep = 10;

    pvg.hrMin_bpm = 60;
    pvg.hrMax_bpm = 100;
    pvg.hrStep_bpm = 30;

    pvg.mapMin_mmHg = 70;
    pvg.mapMax_mmHg = 100;
    pvg.mapStep_mmHg = 40;

    pvg.pulsePressureMin_mmHg = 30;
    pvg.pulsePressureMax_mmHg = 50;
    pvg.pulsePressureStep_mmHg = 40;

    pvg.GeneratePatientList(patients);
  }

  PVRunner pvr(&log);
  return !pvr.Run(patients, rootDir);
}
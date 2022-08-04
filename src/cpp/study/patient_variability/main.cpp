/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PVRunner.h"
#include "PVGenerator.h"

#include "cdm/utils/FileUtils.h"
#include "io/protobuf/PBUtils.h"

using namespace pulse::study::patient_variability;

int main(int argc, char* argv[])
{
  bool clear                   = false;
  bool binary                  = false;
  bool generateOnly            = false;
  bool postProcessOnly         = false;
  bool validationMode          = false;
  bool hemorrhageMode          = false;
  bool useBaseline             = false;
  bool includeStandardPatients = false;
  std::string data = "solo";
  PVGenerator::Mode mode = PVGenerator::Mode::Validation;
  std::string rootDir = "./test_results/patient_variability/";

  // Process arguments
  for(int i = 1; i < argc; ++i)
  {
    // data
    if(!strcmp(argv[i], "full") || !strcmp(argv[i], "solo") || !strcmp(argv[i], "test"))
    {
      data = argv[i];
    }

    // Binary output
    if(!strcmp(argv[i], "-b") || !strcmp(argv[i], "--binary"))
    {
      binary = true;
    }

    // Clear
    if(!strcmp(argv[i], "-c") || !strcmp(argv[i], "--clear"))
    {
      clear = true;
    }

    // Only generate
    if (!strcmp(argv[i], "-g") || !strcmp(argv[i], "--generate"))
    {
      generateOnly = true;
    }

    // Only post-processing
    if(!strcmp(argv[i], "-p") || !strcmp(argv[i], "--post-process"))
    {
      postProcessOnly = true;
      clear = false;
    }

    // Validation RunMode
    if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--validation"))
    {
      validationMode = true;
      mode = PVGenerator::Mode::Validation;
    }
    // Hemorrhage RunMode
    if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--hemorrhage"))
    {
      hemorrhageMode = true;
      mode = PVGenerator::Mode::Hemorrhage;
    }

    // Use existing baseline results for comparison
    if(!strcmp(argv[i], "-u") || !strcmp(argv[i], "--use-baseline"))
    {
      useBaseline = true;
    }

    // Adjust parameters to include standard patients
    if(!strcmp(argv[i], "-i") || !strcmp(argv[i], "--include-standard-patients"))
    {
      includeStandardPatients = true;
    }
    
  }
  if (validationMode && hemorrhageMode)
  {
    std::cerr << "Can only select one run mode\n";
    return 1;
  }

  switch (mode)
  {
  case PVGenerator::Mode::Validation:
    rootDir += "validation/";
    break;
  case PVGenerator::Mode::Hemorrhage:
    rootDir += "hemorrhage/";
    break;
  }

  if(clear)
    DeleteDirectory(rootDir);

  Logger log;
  log.LogToConsole(true);
  log.SetLogFile(rootDir + "PatientVariability.log");

  pulse::study::bind::patient_variability::PatientStateListData patients;
  PVGenerator pvg(&log);
  pvg.mode = mode;
  pvg.includeStandardPatients = includeStandardPatients;

  if(data == "full")
  {
    pvg.parameters.ageMin_yr = 18;
    pvg.parameters.ageMax_yr = 65;
    pvg.parameters.ageStep_yr = 10;

    pvg.parameters.heightMin_cm[ePatient_Sex::Male] = 165;
    pvg.parameters.heightMax_cm[ePatient_Sex::Male] = 186;
    pvg.parameters.heightMin_cm[ePatient_Sex::Female] = 153;
    pvg.parameters.heightMax_cm[ePatient_Sex::Female] = 170;
    pvg.parameters.heightStep_cm = 7;

    pvg.parameters.bmiMin = 18.5;
    pvg.parameters.bmiMax = 29;
    pvg.parameters.bmiStep = 3;

    pvg.parameters.hrMin_bpm = 60;
    pvg.parameters.hrMax_bpm = 100;
    pvg.parameters.hrStep_bpm = 15;

    pvg.parameters.mapMin_mmHg = 70;
    pvg.parameters.mapMax_mmHg = 100;
    pvg.parameters.mapStep_mmHg = 10;

    pvg.parameters.pulsePressureMin_mmHg = 30;
    pvg.parameters.pulsePressureMax_mmHg = 50;
    pvg.parameters.pulsePressureStep_mmHg = 10;

    ////////////////////////
    // Hemorrhage Options //
    ////////////////////////

    pvg.parameters.hemorrhageSeverityMin = 0.25;
    pvg.parameters.hemorrhageSeverityMax = 1.0;
    pvg.parameters.hemorrhageSeverityStep = 0.25;

    pvg.parameters.hemorrhageTriageTimeMin_min = 1.0;
    pvg.parameters.hemorrhageTriageTimeMax_min = 5.0;
    pvg.parameters.hemorrhageTriageTimeStep_min = 1.0;
  }
  else if (data == "solo")
  {
    pvg.parameters.ageMin_yr = 18;
    pvg.parameters.ageMax_yr = 65;
    pvg.parameters.ageStep_yr = 50;

    pvg.parameters.heightMin_cm[ePatient_Sex::Male] = 165;
    pvg.parameters.heightMax_cm[ePatient_Sex::Male] = 186;
    pvg.parameters.heightMin_cm[ePatient_Sex::Female] = 153;
    pvg.parameters.heightMax_cm[ePatient_Sex::Female] = 170;
    pvg.parameters.heightStep_cm = 22;

    pvg.parameters.bmiMin = 18.5;
    pvg.parameters.bmiMax = 29;
    pvg.parameters.bmiStep = 30;

    pvg.parameters.hrMin_bpm = 60;
    pvg.parameters.hrMax_bpm = 100;
    pvg.parameters.hrStep_bpm = 150;

    pvg.parameters.mapMin_mmHg = 70;
    pvg.parameters.mapMax_mmHg = 100;
    pvg.parameters.mapStep_mmHg = 100;

    pvg.parameters.pulsePressureMin_mmHg = 30;
    pvg.parameters.pulsePressureMax_mmHg = 50;
    pvg.parameters.pulsePressureStep_mmHg = 100;

    ////////////////////////
    // Hemorrhage Options //
    ////////////////////////

    pvg.parameters.hemorrhageSeverityMin = 0.25;
    pvg.parameters.hemorrhageSeverityMax = 1.0;
    pvg.parameters.hemorrhageSeverityStep = 1.0;

    pvg.parameters.hemorrhageTriageTimeMin_min = 1.0;
    pvg.parameters.hemorrhageTriageTimeMax_min = 5.0;
    pvg.parameters.hemorrhageTriageTimeStep_min = 5.0;
  }
  else if (data == "test")
  {
    pvg.parameters.ageMin_yr = 18;
    pvg.parameters.ageMax_yr = 65;
    pvg.parameters.ageStep_yr = 25;

    pvg.parameters.heightMin_cm[ePatient_Sex::Male] = 165;
    pvg.parameters.heightMax_cm[ePatient_Sex::Male] = 186;
    pvg.parameters.heightMin_cm[ePatient_Sex::Female] = 153;
    pvg.parameters.heightMax_cm[ePatient_Sex::Female] = 170;
    pvg.parameters.heightStep_cm = 8;

    pvg.parameters.bmiMin = 18.5;
    pvg.parameters.bmiMax = 29;
    pvg.parameters.bmiStep = 10;

    pvg.parameters.hrMin_bpm = 60;
    pvg.parameters.hrMax_bpm = 100;
    pvg.parameters.hrStep_bpm = 30;

    pvg.parameters.mapMin_mmHg = 70;
    pvg.parameters.mapMax_mmHg = 100;
    pvg.parameters.mapStep_mmHg = 40;

    pvg.parameters.pulsePressureMin_mmHg = 30;
    pvg.parameters.pulsePressureMax_mmHg = 50;
    pvg.parameters.pulsePressureStep_mmHg = 40;

    ////////////////////////
    // Hemorrhage Options //
    ////////////////////////

    pvg.parameters.hemorrhageSeverityMin = 0.25;
    pvg.parameters.hemorrhageSeverityMax = 1.0;
    pvg.parameters.hemorrhageSeverityStep = 0.25;

    pvg.parameters.hemorrhageTriageTimeMin_min = 1.0;
    pvg.parameters.hemorrhageTriageTimeMax_min = 5.0;
    pvg.parameters.hemorrhageTriageTimeStep_min = 5.0;
  }

  pvg.GeneratePatientList(patients);
  if (generateOnly)
  {
    std::string patientFile = rootDir + "/patients.json";
    if (binary)
      patientFile = rootDir + "/patients.pbb";
    PBUtils::SerializeToFile(patients, patientFile, &log);
    return 0;
  }
    

  PVRunner pvr(rootDir, useBaseline, &log);
  pvr.PostProcessOnly = postProcessOnly;
  pvr.SerializationFormat = binary ? eSerializationFormat::BINARY : eSerializationFormat::JSON;
  return !pvr.Run(patients);
}

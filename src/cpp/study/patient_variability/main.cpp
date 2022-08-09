/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PVRunner.h"
#include "PVGenerator.h"

#include "cdm/properties/SEScalarLength.h"
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
  std::string data = "test";
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
    if(!strcmp(argv[i], "-n") || !strcmp(argv[i], "--no-baseline"))
    {
      useBaseline = false;
    }

    // Adjust parameters to exclude standard patients
    if(!strcmp(argv[i], "-x") || !strcmp(argv[i], "--exclude-standard-patients"))
    {
      includeStandardPatients = false;
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
  pvg.m_Mode = mode;
  pvg.m_IncludeStandardPatients = includeStandardPatients;

  if (data == "solo")
  {
    rootDir = "./test_results/PVRunner/";
    clear = true;
    uint32_t age_yr = 44;
    double   bmi    = 18.7099;
    double   hr_bpm = 92;
    double   height_cm = 180.34;
    double   map_mmHg = 87;
    double   pp_mmHg = 30.5;

    auto p = patients.add_patient();
    p->set_id(0);
    p->set_sex(pulse::cdm::bind::PatientData_eSex::PatientData_eSex_Male);
    p->set_age_yr(age_yr);
    p->set_bmi(bmi);
    p->set_heartrate_bpm(hr_bpm);
    p->set_height_cm(height_cm);
    p->set_meanarterialpressure_mmhg(map_mmHg);
    p->set_pulsepressure_mmhg(pp_mmHg);
    // Caclulate weight (kg) from height (m) and BMI
    double height_m = Convert(height_cm, LengthUnit::cm, LengthUnit::m);
    double weight_kg = bmi * height_m * height_m;
    p->set_weight_kg(weight_kg);
    // systolic - diastolic = pulse pressure
    // MAP = (systolic + 2 * diastolic) / 3
    double diastolic_mmHg = (3 * map_mmHg - pp_mmHg) / 3.0;
    double systolic_mmHg = pp_mmHg + diastolic_mmHg;
    p->set_diastolicarterialpressure_mmhg(diastolic_mmHg);
    p->set_systolicarterialpressure_mmhg(systolic_mmHg);

    p->set_outputbasefilename("solo/");
    p->set_maxsimulationtime_s(120); // Generate 2 mins of data
    p->mutable_validation();// Create a validation object to fill
  }
  else if(data == "full")
  {
    pvg.m_Parameters.ageMin_yr = 18;
    pvg.m_Parameters.ageMax_yr = 65;
    pvg.m_Parameters.ageStep_yr = 10;

    pvg.m_Parameters.heightMin_cm[ePatient_Sex::Male] = 165;
    pvg.m_Parameters.heightMax_cm[ePatient_Sex::Male] = 186;
    pvg.m_Parameters.heightMin_cm[ePatient_Sex::Female] = 153;
    pvg.m_Parameters.heightMax_cm[ePatient_Sex::Female] = 170;
    pvg.m_Parameters.heightStep_cm = 7;

    pvg.m_Parameters.bmiMin = 18.5;
    pvg.m_Parameters.bmiMax = 29;
    pvg.m_Parameters.bmiStep = 3;

    pvg.m_Parameters.hrMin_bpm = 60;
    pvg.m_Parameters.hrMax_bpm = 100;
    pvg.m_Parameters.hrStep_bpm = 15;

    pvg.m_Parameters.mapMin_mmHg = 70;
    pvg.m_Parameters.mapMax_mmHg = 100;
    pvg.m_Parameters.mapStep_mmHg = 10;

    pvg.m_Parameters.pulsePressureMin_mmHg = 30;
    pvg.m_Parameters.pulsePressureMax_mmHg = 50;
    pvg.m_Parameters.pulsePressureStep_mmHg = 10;

    ////////////////////////
    // Hemorrhage Options //
    ////////////////////////

    pvg.m_Parameters.hemorrhageSeverityMin = 0.25;
    pvg.m_Parameters.hemorrhageSeverityMax = 1.0;
    pvg.m_Parameters.hemorrhageSeverityStep = 0.25;

    pvg.m_Parameters.hemorrhageTriageTimeMin_min = 1.0;
    pvg.m_Parameters.hemorrhageTriageTimeMax_min = 5.0;
    pvg.m_Parameters.hemorrhageTriageTimeStep_min = 1.0;

    pvg.GeneratePatientList(patients);
  }
  else if (data == "test")
  {
    // Making the steps sizes waaay big
    // This should generate 3 values for each parameter
    // min, standard, max
    pvg.m_Parameters.ageMin_yr = 18;
    pvg.m_Parameters.ageMax_yr = 65;
    pvg.m_Parameters.ageStep_yr = 100;

    pvg.m_Parameters.heightMin_cm[ePatient_Sex::Male] = 165;
    pvg.m_Parameters.heightMax_cm[ePatient_Sex::Male] = 186;
    pvg.m_Parameters.heightMin_cm[ePatient_Sex::Female] = 153;
    pvg.m_Parameters.heightMax_cm[ePatient_Sex::Female] = 170;
    pvg.m_Parameters.heightStep_cm = 100;

    pvg.m_Parameters.bmiMin = 18.5;
    pvg.m_Parameters.bmiMax = 29;
    pvg.m_Parameters.bmiStep = 100;

    pvg.m_Parameters.hrMin_bpm = 60;
    pvg.m_Parameters.hrMax_bpm = 100;
    pvg.m_Parameters.hrStep_bpm = 100;

    pvg.m_Parameters.mapMin_mmHg = 70;
    pvg.m_Parameters.mapMax_mmHg = 100;
    pvg.m_Parameters.mapStep_mmHg = 100;

    pvg.m_Parameters.pulsePressureMin_mmHg = 30;
    pvg.m_Parameters.pulsePressureMax_mmHg = 50;
    pvg.m_Parameters.pulsePressureStep_mmHg = 100;

    ////////////////////////
    // Hemorrhage Options //
    ////////////////////////

    pvg.m_Parameters.hemorrhageSeverityMin = 0.25;
    pvg.m_Parameters.hemorrhageSeverityMax = 1.0;
    pvg.m_Parameters.hemorrhageSeverityStep = 0.25;

    pvg.m_Parameters.hemorrhageTriageTimeMin_min = 1.0;
    pvg.m_Parameters.hemorrhageTriageTimeMax_min = 5.0;
    pvg.m_Parameters.hemorrhageTriageTimeStep_min = 5.0;

    pvg.GeneratePatientList(patients);
  }

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

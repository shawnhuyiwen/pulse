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
  bool useBaseline             = true;
  bool includeStandardPatients = true;
  std::string data = "full";
  Mode mode = Mode::Validation;
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
      mode = Mode::Validation;
    }
    // Hemorrhage RunMode
    if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--hemorrhage"))
    {
      hemorrhageMode = true;
      mode = Mode::Hemorrhage;
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

  std::string modeDir = "";
  switch (mode)
  {
  case Mode::Validation:
    modeDir = "validation/";
    break;
  case Mode::Hemorrhage:
    modeDir = "hemorrhage/";
    break;
  }

  Logger log;
  log.LogToConsole(true);
  std::string logName = "PatientVariability.log";

  pulse::study::bind::patient_variability::PatientStateListData patients;
  PVGenerator pvg(&log);
  pvg.GenerateMode = mode;
  pvg.IncludeStandardPatients = includeStandardPatients;

  if (data == "solo")
  {
    rootDir += modeDir+"solo/";
    // I always want to run, so remove our "compiled" results of 1
    DeleteFile(rootDir+"patient_results.json");// If we are solo, always rerun
    if(clear)// Then we can delete the specific solo run results
      DeleteDirectory(rootDir);
    log.SetLogFile(rootDir + logName);

    /// male/age_yr18/height_cm163.000000/bmi16/hr_bpm100.000000/map_mmHg70.000000/pp_mmHg40.500000/bp_mmHg114.000000-73.500000
    uint32_t age_yr = 18;
    double   height_cm = 163;
    double   bmi    = 16;
    double   hr_bpm = 100;
    double   map_mmHg = 70;
    double   pp_mmHg = 40.5;
    double systolic_mmHg = 114;// Set to -1 if you want it computed
    double diastolic_mmHg = 73.5;// Set to -1 if you want it computed

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
    if(diastolic_mmHg<0)
      diastolic_mmHg = (3 * map_mmHg - pp_mmHg) / 3.0;
    if(systolic_mmHg<0)
      systolic_mmHg = pp_mmHg + diastolic_mmHg;
    p->set_diastolicarterialpressure_mmhg(diastolic_mmHg);
    p->set_systolicarterialpressure_mmhg(systolic_mmHg);

    p->set_outputbasefilename("solo/");
    p->set_maxsimulationtime_s(120); // Generate 2 mins of data
    p->mutable_validation();// Create a validation object to fill
  }
  else if(data == "full")
  {
    rootDir += modeDir+"full/";
    if(clear)
      DeleteDirectory(rootDir);
    log.SetLogFile(rootDir + logName);

    // The default min/max are the model bounds
    // So just increase the fidelity via step size
    pvg.Age_yr.AdjustStepSize(1);
    pvg.HeightMale_cm.AdjustStepSize(1);
    pvg.HeightFemale_cm.AdjustStepSize(1);
    pvg.BMI.AdjustStepSize(1);
    pvg.HR_bpm.AdjustStepSize(1);
    pvg.MAP_mmHg.AdjustStepSize(1);
    pvg.PP_mmHg.AdjustStepSize(1);

    ////////////////////////
    // Hemorrhage Options //
    ////////////////////////

    // Defaults are
    // HemorrhageSeverity(0.25,1.00,0.25)
    // HemorrhageTriageTime(1.0,5.0,1.0)

    pvg.GenerateIndividualParamaterVariabilityPatientList(patients);
  }
  else if (data == "test")
  {
    rootDir += modeDir+"test/";
    if(clear)
      DeleteDirectory(rootDir);
    log.SetLogFile(rootDir + logName);

    // This will use the defaults, 2 patients for each parameter

    ////////////////////////
    // Hemorrhage Options //
    ////////////////////////

    // Defaults are
    // HemorrhageSeverity(0.25,1.00,0.25)
    // HemorrhageTriageTime(1.0,5.0,1.0)

    pvg.GenerateIndividualParamaterVariabilityPatientList(patients);
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

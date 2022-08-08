/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include <map>
#include <limits>

#include "PulseEngine.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/patient/SEPatient.h"

PUSH_PROTO_WARNINGS
#include "pulse/cdm/bind/Patient.pb.h"
#include "pulse/study/bind/PatientVariability.pb.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>
POP_PROTO_WARNINGS
#include "io/protobuf/PBUtils.h"

namespace pulse::study::patient_variability
{
  class PVGenerator : public Loggable
  {
    using PatientStateListData = pulse::study::bind::patient_variability::PatientStateListData;
  public:
    PVGenerator(Logger* logger=nullptr);
    virtual ~PVGenerator();

    enum class Mode
    {
      Hemorrhage = 0,
      Validation
    };

    struct ParameterSpace
    {
      double ageMin_yr = 18;
      double ageMax_yr = 65;
      double ageStep_yr = 10;

      std::map<ePatient_Sex, double> heightMin_cm = { {ePatient_Sex::Male, 165}, {ePatient_Sex::Female, 153} };
      std::map<ePatient_Sex, double> heightMax_cm = { {ePatient_Sex::Male, 186}, {ePatient_Sex::Female, 170} };
      double heightStep_cm = 7;

      double bmiMin = 18.5;
      double bmiMax = 29;
      double bmiStep = 3;

      double hrMin_bpm = 60;
      double hrMax_bpm = 100;
      double hrStep_bpm = 15;

      double mapMin_mmHg = 70;
      double mapMax_mmHg = 100;
      double mapStep_mmHg = 10;

      double pulsePressureMin_mmHg = 30;
      double pulsePressureMax_mmHg = 50;
      double pulsePressureStep_mmHg = 10;

      std::map<ePatient_Sex, std::string> sexes = { {ePatient_Sex::Male, "male"}, {ePatient_Sex::Female, "female"} };

      //--------------------//
      // Hemorrhage options //
      //--------------------//
      
      std::vector<std::string> hemorrhageCompartments = { "RightArm", "RightLeg" };

      double hemorrhageSeverityMin = 0.25;
      double hemorrhageSeverityMax = 1.0;
      double hemorrhageSeverityStep = 0.25;

      double hemorrhageTriageTimeMin_min  = 1.0;
      double hemorrhageTriageTimeMax_min  = 20.0;
      double hemorrhageTriageTimeStep_min = 5.0;
    };

    void GeneratePatientList(PatientStateListData& pData);

    bool           m_IncludeStandardPatients = true;
    Mode           m_Mode = Mode::Validation;
    ParameterSpace m_Parameters;

  protected:
    void GenerateHemorrhageOptions(PatientStateListData& pList, int& id,
      const ePatient_Sex sex, double age_yr, double height_cm, double weight_kg, double bmi,
      double hr_bpm, double map_mmHg, double pp_mmHg, double diastolic_mmHg, double systolic_mmHg, const ParameterSpace& p,
      const std::string& full_dir_path);

    ParameterSpace AdjustParametersToPatient(const SEPatient& patient);
    void AdjustParameter(double& min, double& max, double step, double keyValue, double lowerBound=0, double upperBound=std::numeric_limits<double>::max());
  };
}

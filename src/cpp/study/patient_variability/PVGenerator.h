/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include <map>
#include <list>

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

    static bool TestPatientCombo(Logger*, ePatient_Sex sex,
                                 double age_yr, double height_cm, double weight_kg,
                                 double hr_bpm, double diastolic_mmHg, double systolic_mmHg);
    void GeneratePatientList(PatientStateListData& pData);

    bool           m_IncludeStandardPatients = true;
    Mode           m_Mode = Mode::Validation;
    ParameterSpace m_Parameters;

  protected:
    unsigned int m_MaxNumPatients; // This is the maximum number of patients for the set, not all combination may be valid, so the actual number of patients for a set could be less that this.
    unsigned int m_TotalPatients; // The actual number of patients for this data set, (NOT the number of runs, i.e. hemorrhage options can create more runs for each patient)
    unsigned int m_TotalRuns; // This is the actual number of runs to perform

    struct ParameterSpaceValues
    {
      std::list<double> ageRange_yr;
      std::map<ePatient_Sex, std::list<double>> heightRange_cm;
      std::list<double> bmiRange;
      std::list<double> hrRange_bpm;
      std::list<double> mapRange_mmHg;
      std::list<double> pulsePressureRange_mmHg;
    };
    ParameterSpaceValues m_ParameterValues;

    void GenerateHemorrhageOptions(PatientStateListData& pList, int& id,
      const ePatient_Sex sex, unsigned int age_yr, double height_cm, double weight_kg, double bmi,
      double hr_bpm, double map_mmHg, double pp_mmHg, double diastolic_mmHg, double systolic_mmHg,
      const std::string& full_dir_path);

    void AdjustParameterSpaceBounds();
    void GenerateParameterSpaceValues(const SEPatient* patient = nullptr);
    bool AdjustParameterBounds(double& min, double& max, double step, double keyValue);
    std::list<double> GenerateParameterValues(double min, double max, double step);
  };
}

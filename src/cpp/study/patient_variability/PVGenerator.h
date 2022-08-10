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
  enum class Mode
  {
    Hemorrhage = 0,
    Validation
  };

  class Parameter
  {
  public:
    Parameter(double lowerLimit, double upperLimit)
    {
      m_LowerLimit = lowerLimit;
      m_UpperLimit = upperLimit;
      Set(lowerLimit, upperLimit);
    }

    void Reset()
    {
      Set(m_Min, m_Max, m_StepSize);
    }
    void Set(double min, double max)
    {
      Set(min, max, max - min);
    }
    void Set(double min, double max, double stepSize)
    {
      if (min < m_LowerLimit)
      {
        min = m_LowerLimit;
        std::cerr << "Corrected invalid minimum to be the lowerLimit\n";
      }
      if (max > m_UpperLimit || max < 0)
      {
        max = m_UpperLimit;
        std::cerr << "Corrected invalid maximum to be the upperLimit\n";
      }
      if (stepSize > (m_UpperLimit - m_LowerLimit) || stepSize < 0)
      {
        stepSize = m_UpperLimit - m_LowerLimit;
        std::cerr << "Corrected invalid stepSize to be the upperLimit-lowerLimit\n";
      }

      m_Min = min;
      m_Max = max;
      m_StepSize = stepSize;
      m_Values.clear();
      int idx, n = (int)((m_Max - m_Min) / m_StepSize);
      for (idx = 0; idx <= n; ++idx)
      {
        m_Values.push_back(m_Min + m_StepSize * idx);
      }
    }
    void AdjustStepSize(double stepSize)
    {
      Set(m_Min, m_Max, stepSize);
    }
    void Insert(double v)
    {
      // Make sure v is not in our list and add it
      // Sort the list
    }

    double Min() const { return m_Min; }
    double Max() const { return m_Max; }
    double StepSize() const { return m_StepSize; }
    double LowerLimit() const { return m_LowerLimit; }
    double UpperLimit() const { return m_UpperLimit; }
    const std::vector<double>& Values() const { return m_Values; }

  protected:
    double m_Min, m_Max, m_StepSize;
    double m_LowerLimit, m_UpperLimit;
    std::vector<double> m_Values;
  };

  class PVGenerator : public Loggable
  {
    using PatientStateListData = pulse::study::bind::patient_variability::PatientStateListData;
  public:
    PVGenerator(Logger* logger=nullptr);
    virtual ~PVGenerator();

    static bool TestPatientCombo(Logger*, ePatient_Sex sex,
                                 double age_yr, double height_cm, double weight_kg,
                                 double hr_bpm, double diastolic_mmHg, double systolic_mmHg);
    void GeneratePatientList(PatientStateListData& pData);

    bool            IncludeStandardPatients = true;
    Mode            Mode = Mode::Validation;
    
    Parameter       Age_yr;
    Parameter       HeightMale_cm;
    Parameter       HeightFemale_cm;
    Parameter       BMI;
    Parameter       HR_bpm;
    Parameter       MAP_mmHg;
    Parameter       PP_mmHg;

    double hemorrhageSeverityMin = 0.25;
    double hemorrhageSeverityMax = 1.0;
    double hemorrhageSeverityStep = 0.25;

    double hemorrhageTriageTimeMin_min = 1.0;
    double hemorrhageTriageTimeMax_min = 20.0;
    double hemorrhageTriageTimeStep_min = 5.0;

  protected:
    unsigned int m_MaxNumPatients=0; // This is the maximum number of patients for the set, not all combination may be valid, so the actual number of patients for a set could be less that this.
    unsigned int m_TotalPatients=0; // The actual number of patients for this data set, (NOT the number of runs, i.e. hemorrhage options can create more runs for each patient)
    unsigned int m_TotalRuns=0; // This is the actual number of runs to perform

    void GenerateHemorrhageOptions(PatientStateListData& pList, int& id,
      const ePatient_Sex sex, unsigned int age_yr, double height_cm, double weight_kg, double bmi,
      double hr_bpm, double map_mmHg, double pp_mmHg, double diastolic_mmHg, double systolic_mmHg,
      const std::string& full_dir_path);

    void AddPatientParameters(const SEPatient& patient);
  };
}

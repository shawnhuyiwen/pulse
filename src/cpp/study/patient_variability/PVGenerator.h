/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#pragma once
#include "PulseEngine.h"
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
  public:
    PVGenerator(Logger* logger=nullptr);
    virtual ~PVGenerator();

    unsigned int ageMin_yr = 18;
    unsigned int ageMax_yr = 65;
    unsigned int ageStep_yr = 10;

    unsigned int heightMaleMin_cm = 165;
    unsigned int heightMaleMax_cm = 186;
    unsigned int heightFemaleMin_cm = 153;
    unsigned int heightFemaleMax_cm = 170;
    unsigned int heightStep_cm = 7;

    double bmiMin = 18.5;
    double bmiMax = 29;
    double bmiStep = 3;

    unsigned int hrMin_bpm = 60;
    unsigned int hrMax_bpm = 100;
    unsigned int hrStep_bpm = 15;

    unsigned int mapMin_mmHg = 70;
    unsigned int mapMax_mmHg = 100;
    unsigned int mapStep_mmHg = 10;

    unsigned int pulsePressureMin_mmHg = 30;
    unsigned int pulsePressureMax_mmHg = 50;
    unsigned int pulsePressureStep_mmHg = 10;

    void GeneratePatientList(pulse::study::bind::patient_variability::PatientStateListData& pData);
  };
}
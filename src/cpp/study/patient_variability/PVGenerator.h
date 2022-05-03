/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include <map>

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

    void GeneratePatientList(pulse::study::bind::patient_variability::PatientStateListData& pData);

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

    std::map<ePatient_Sex, std::string> sexes = { {ePatient_Sex::Male, "male"}, {ePatient_Sex::Female, "female"} };

    //--------------------//
    // Hemorrhage options //
    //--------------------//
    //run for two hours then death check and record vitals
    bool hemorrhageOptionEnabled = false;
    double hemorrhageOption_startTime_s = 10;
    
    std::vector<std::string> hemorrhageCompartments = { "RightArm", "RightLeg" };

    double hemorrhageSeverityMin = 0.25;
    double hemorrhageSeverityMax = 1.0;
    double hemorrhageSeverityStep = 0.25;

    double hemorrhageTriageTimeMin_min = 1.0;
    double hemorrhageTriageTimeMax_min = 5.0;
    double hemorrhageTriageTimeStep_min = 1.0;

  protected:
    pulse::study::bind::patient_variability::PatientStateListData AddHemorrhageOption(pulse::study::bind::patient_variability::PatientStateListData& originalPatient, int& id);
  };
}

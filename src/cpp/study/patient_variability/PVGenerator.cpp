/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PVGenerator.h"
#include "cdm/patient/SEPatient.h"
#include "cdm/io/protobuf/PBPatient.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarLength.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerPressure.h"
#include "engine/human_adult/whole_body/Engine.h"

#include <map>

using eFailure = pulse::study::bind::patient_variability::PatientStateData_eFailure;

// Bounds from: https://pulse.kitware.com/_patient_methodology.html
// And: https://pulse.kitware.com/_cardiovascular_methodology.html#cardiovascular-validation-resting

#define minAge_yr 18
#define maxAge_yr 65
#define minMaleHeight_cm 163
#define maxMaleHeight_cm 190
#define minFemaleHeight_cm 151
#define maxFemaleHeight_cm 175.5
#define minBMI 16
#define maxBMI 29.9
#define minMaleBFF 0.02
#define maxMaleBFF 0.25
#define minFemaleBFF 0.1
#define maxFemaleBFF 0.32
#define minHR_bpm 60
#define maxHR_bpm 100
#define minMAP_mmHg 70
#define maxMAP_mmHg 100
#define minPulsePressure_mmHg 30
#define maxPulsePressure_mmHg 60
#define minRR_bpm 8
#define maxRR_bpm 20

namespace pulse::study::patient_variability
{
  // Bounds from: https://pulse.kitware.com/_patient_methodology.html
  PVGenerator::PVGenerator(Logger* logger) : Loggable(logger),
    HemorrhageSeverity(0.1, 0.3, 0.1),
    HemorrhageTriageTime_min(1.0, 1.0)
  {
  }
  PVGenerator::~PVGenerator()
  {
  }

  // Reset ParameterSpace
  void PVGenerator::ResetParameters(ePatient_Sex sex)
  {
    // The third number is generally a compromise between standard and default
    Age_yr.Set(minAge_yr, maxAge_yr, 44.);
    HR_bpm.Set(minHR_bpm, maxHR_bpm, 72);
    MAP_mmHg.Set(minMAP_mmHg, maxMAP_mmHg, 87);
    PP_mmHg.Set(minPulsePressure_mmHg, maxPulsePressure_mmHg, 40.5);
    RR_bpm.Set(minRR_bpm, maxRR_bpm, 12);
    if (sex == ePatient_Sex::Male)
    {
      Height_cm.Set(minMaleHeight_cm, maxMaleHeight_cm, 180.34);
      BMI.Set(minBMI, maxBMI, 23.71);
      BFF.Set(minMaleBFF, maxMaleBFF, 0.21);
    }
    else
    {
      Height_cm.Set(minFemaleHeight_cm, maxFemaleHeight_cm, 162.56);
      BMI.Set(minBMI, maxBMI, 22.31);
      BFF.Set(minFemaleBFF, maxFemaleBFF, 0.28);
    }
  }

  void PVGenerator::GenerateMultiVariableCombinationPatientList(PatientStateListData& pList)
  {
    m_TotalRuns = 0;
    m_TotalPatients = 0;
    m_NumPatientsFailedToSetup = 0;

    // Each parameter we want to include in our permutations, has 3 options, min, max, standard/default
    std::vector<size_t> opts = { 2,2,2,2,2,2,2,2 }; // max index is 2
    std::vector<std::vector<size_t>> permutations;
    GeneralMath::Combinations(opts, permutations);

    std::vector<ePatient_Sex> sexes = { ePatient_Sex::Male, ePatient_Sex::Female };
    for (auto sex : sexes)
    {
      ResetParameters(sex);
      for (auto idxs : permutations)
      {
        SEPatient patient(GetLogger());
        patient.SetName("Patient" + std::to_string(m_TotalPatients));
        patient.SetSex(sex);

        patient.GetAge().SetValue(Age_yr.Values()[idxs[0]], TimeUnit::yr);
        patient.GetHeight().SetValue(Height_cm.Values()[idxs[1]], LengthUnit::cm);
        patient.GetBodyFatFraction().SetValue(BFF.Values()[idxs[2]]);
        patient.GetBodyMassIndex().SetValue(BMI.Values()[idxs[3]]);
        patient.GetHeartRateBaseline().SetValue(HR_bpm.Values()[idxs[4]], FrequencyUnit::Per_min);
        patient.GetMeanArterialPressureBaseline().SetValue(MAP_mmHg.Values()[idxs[5]], PressureUnit::mmHg);
        patient.GetPulsePressureBaseline().SetValue(PP_mmHg.Values()[idxs[6]], PressureUnit::mmHg);
        patient.GetRespirationRateBaseline().SetValue(RR_bpm.Values()[idxs[7]], FrequencyUnit::Per_min);

        CreatePatient(pList, patient, "/Patient" + std::to_string(m_TotalPatients));

        m_TotalPatients++;
      }
    }

    Info("Created " + std::to_string(m_TotalPatients) + " patients");
    Info("Created " + std::to_string(m_TotalRuns) + " total runs");
  }

  void PVGenerator::CreatePatient(PatientStateListData& pList, SEPatient& patient, const std::string& full_dir_path)
  {
    switch (GenerateMode)
    {
    case eMode::Validation:
    {
      AddPatientToList(pList, patient, 120, full_dir_path);
      break;
    }

    case eMode::Hemorrhage:
    {
      GenerateHemorrhageOptions(pList, patient, full_dir_path);
      break;
    }
    }
  }

  // Creates a new list of patients, adding all hemorrhage variables to each patient in originalPatients
  void PVGenerator::GenerateHemorrhageOptions(PatientStateListData& pList, SEPatient& patient, const std::string& full_dir_path)
  {
    std::vector<std::string> HemorrhageCompartments = { "RightArm", "RightLeg" };
    for (auto hemorrhageCompartment : HemorrhageCompartments)
    {
      std::string compartment_dir = +"/" + hemorrhageCompartment;

      for (auto severity : HemorrhageSeverity.Values())
      {
        std::string severity_dir = "/severity" + pulse::cdm::to_string(severity);

        for (auto triageTime_min : HemorrhageTriageTime_min.Values())
        {

          std::string triageTime_dir = "/triage_min" + pulse::cdm::to_string(triageTime_min);
          std::string hemorrhage_dir_path = full_dir_path + compartment_dir + severity_dir + triageTime_dir;

          auto patientData = AddPatientToList(pList, patient, 2 * 60 * 60, hemorrhage_dir_path);

          auto hemorrhage = patientData->mutable_hemorrhage();
          hemorrhage->set_starttime_s(10);
          hemorrhage->set_compartment(hemorrhageCompartment);
          hemorrhage->set_severity(severity);
          hemorrhage->set_triagetime_s(triageTime_min * 60);
        }
      }
    }
  }

  pulse::study::bind::patient_variability::PatientStateData* PVGenerator::AddPatientToList(PatientStateListData& pList,
    SEPatient& patient, double runDuration_s, const std::string& full_dir_path)
  {
    Info("Creating patient: " + full_dir_path);
    pulse::study::bind::patient_variability::PatientStateData* patientStateData = pList.add_patientstate();
    patientStateData->set_id(m_TotalRuns);
    patientStateData->set_outputbasefilename(full_dir_path);
    patientStateData->set_maxsimulationtime_s(runDuration_s); // Generate 2 mins of data
    patientStateData->mutable_validation();// Create a validation object to fill
    pulse::cdm::bind::PatientData* patientData = patientStateData->mutable_patient();
    PBPatient::Serialize(patient, *patientData);
    m_TotalRuns++;

    return patientStateData;
  }
}

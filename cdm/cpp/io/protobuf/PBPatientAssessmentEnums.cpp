/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "bind/cpp/cdm/PatientAssessments.pb.h"
#include "patient/assessments/SEPatientAssessment.h"
#include "patient/assessments/SEUrinalysisMicroscopic.h"

const std::string& ePatientAssessment_Type_Name(ePatientAssessment_Type m)
{
  return cdm::ePatientAssessmentType_Name((cdm::ePatientAssessmentType)m);
}

const std::string& eUrinalysis_MicroscopicObservationType_Name(eUrinalysis_MicroscopicObservationType m)
{
  return cdm::UrinalysisMicroscopicData::eObservationType_Name((cdm::UrinalysisMicroscopicData::eObservationType)m);
}

const std::string& eUrinalysis_MicroscopicObservationAmount_Name(eUrinalysis_MicroscopicObservationAmount m)
{
  return cdm::UrinalysisMicroscopicData::eObservationAmount_Name((cdm::UrinalysisMicroscopicData::eObservationAmount)m);
}
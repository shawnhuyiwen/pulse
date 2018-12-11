/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "bind/cpp/cdm/PatientAssessmentEnums.pb.h"
#include "patient/assessments/SEPatientAssessment.h"
#include "patient/assessments/SEUrinalysisMicroscopic.h"

const std::string& ePatientAssessment_Type_Name(ePatientAssessment_Type m)
{
  return cdm::ePatientAssessment_Type_Name((cdm::ePatientAssessment_Type)m);
}

const std::string& eUrinalysis_MicroscopicObservationType_Name(eUrinalysis_MicroscopicObservationType m)
{
  return cdm::eUrinalysis_MicroscopicObservationType_Name((cdm::eUrinalysis_MicroscopicObservationType)m);
}

const std::string& eUrinalysis_MicroscopicObservationAmount_Name(eUrinalysis_MicroscopicObservationAmount m)
{
  return cdm::eUrinalysis_MicroscopicObservationAmount_Name((cdm::eUrinalysis_MicroscopicObservationAmount)m);
}
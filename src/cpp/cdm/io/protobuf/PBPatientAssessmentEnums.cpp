/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/PatientAssessments.pb.h"
POP_PROTO_WARNINGS()
#include "patient/assessments/SEPatientAssessment.h"
#include "patient/assessments/SEUrinalysisMicroscopic.h"

const std::string& ePatientAssessment_Type_Name(ePatientAssessment_Type m)
{
  return CDM_BIND::ePatientAssessmentType_Name((CDM_BIND::ePatientAssessmentType)m);
}

const std::string& eUrinalysis_MicroscopicObservationType_Name(eUrinalysis_MicroscopicObservationType m)
{
  return CDM_BIND::UrinalysisMicroscopicData::eObservationType_Name((CDM_BIND::UrinalysisMicroscopicData::eObservationType)m);
}

const std::string& eUrinalysis_MicroscopicObservationAmount_Name(eUrinalysis_MicroscopicObservationAmount m)
{
  return CDM_BIND::UrinalysisMicroscopicData::eObservationAmount_Name((CDM_BIND::UrinalysisMicroscopicData::eObservationAmount)m);
}
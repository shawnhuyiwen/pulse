/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEPatientAssessmentRequest.h"
#include "bind/cdm/PatientActions.pb.h"
#include "bind/cdm/PatientAssessmentEnums.pb.h"

const std::string& ePatientAssessment_Type_Name(ePatientAssessment_Type m)
{
  return cdm::ePatientAssessment_Type_Name((cdm::ePatientAssessment_Type)m);
}

SEPatientAssessmentRequest::SEPatientAssessmentRequest() : SEPatientAction()
{
  m_Type=ePatientAssessment_Type::CompleteBloodCount;
}

SEPatientAssessmentRequest::~SEPatientAssessmentRequest()
{
  Clear();
}

void SEPatientAssessmentRequest::Clear()
{
  SEPatientAction::Clear();
  m_Type=ePatientAssessment_Type::CompleteBloodCount;
}

bool SEPatientAssessmentRequest::IsValid() const
{
  return SEPatientAction::IsValid();
}

bool SEPatientAssessmentRequest::IsActive() const
{
  return IsValid();
}

void SEPatientAssessmentRequest::Load(const cdm::PatientAssessmentRequestData& src, SEPatientAssessmentRequest& dst)
{
  SEPatientAssessmentRequest::Serialize(src, dst);
}
void SEPatientAssessmentRequest::Serialize(const cdm::PatientAssessmentRequestData& src, SEPatientAssessmentRequest& dst)
{
  SEPatientAction::Serialize(src.patientaction(), dst);
  dst.SetType((ePatientAssessment_Type)src.type());
}

cdm::PatientAssessmentRequestData* SEPatientAssessmentRequest::Unload(const SEPatientAssessmentRequest& src)
{
  cdm::PatientAssessmentRequestData* dst = new cdm::PatientAssessmentRequestData();
  SEPatientAssessmentRequest::Serialize(src, *dst);
  return dst;
}
void SEPatientAssessmentRequest::Serialize(const SEPatientAssessmentRequest& src, cdm::PatientAssessmentRequestData& dst)
{
  SEPatientAction::Serialize(src, *dst.mutable_patientaction());
  dst.set_type((cdm::ePatientAssessment_Type)src.m_Type);
}

ePatientAssessment_Type SEPatientAssessmentRequest::GetType() const
{
  return m_Type;
}
void SEPatientAssessmentRequest::SetType(ePatientAssessment_Type Type)
{
  m_Type = Type;
}
void SEPatientAssessmentRequest::ToString(std::ostream &str) const
{
  str << "Patient Action : Assessment Request"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str  << "\n\tType: "<< ePatientAssessment_Type_Name(GetType());
  str  << std::flush;
}
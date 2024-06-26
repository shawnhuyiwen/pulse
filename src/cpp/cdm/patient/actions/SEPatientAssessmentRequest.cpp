/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEPatientAssessmentRequest.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SEPatientAssessmentRequest::SEPatientAssessmentRequest(Logger* logger) : SEPatientAction(logger)
{
  m_Type=ePatientAssessment_Type::CompleteBloodCount;
}

SEPatientAssessmentRequest::~SEPatientAssessmentRequest()
{
  m_Type = ePatientAssessment_Type::CompleteBloodCount;
}

void SEPatientAssessmentRequest::Clear()
{
  SEPatientAction::Clear();
  m_Type=ePatientAssessment_Type::CompleteBloodCount;
}

void SEPatientAssessmentRequest::Copy(const SEPatientAssessmentRequest& src, bool /*preserveState*/)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEPatientAssessmentRequest::IsValid() const
{
  return SEPatientAction::IsValid();
}

bool SEPatientAssessmentRequest::IsActive() const
{
  return SEPatientAction::IsActive();
}
void SEPatientAssessmentRequest::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEPatientAssessmentRequest::GetScalar(const std::string& /*name*/)
{
  return nullptr;
}

ePatientAssessment_Type SEPatientAssessmentRequest::GetType() const
{
  return m_Type;
}
void SEPatientAssessmentRequest::SetType(ePatientAssessment_Type Type)
{
  m_Type = Type;
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEBrainInjury.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientActions.h"

SEBrainInjury::SEBrainInjury(Logger* logger) : SEPatientAction(logger)
{
  m_Severity=nullptr;
  m_Type = eBrainInjury_Type::Diffuse;
}

SEBrainInjury::~SEBrainInjury()
{
  SAFE_DELETE(m_Severity);
  m_Type = eBrainInjury_Type::Diffuse;
}

void SEBrainInjury::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_Severity);
  m_Type = eBrainInjury_Type::Diffuse;
}

void SEBrainInjury::Copy(const SEBrainInjury& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEBrainInjury::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}

bool SEBrainInjury::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return !m_Severity->IsZero();
}
void SEBrainInjury::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEBrainInjury::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
}

bool SEBrainInjury::HasSeverity() const
{
  return m_Severity==nullptr?false:m_Severity->IsValid();
}
SEScalar0To1& SEBrainInjury::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity=new SEScalar0To1();
  return *m_Severity;
}
double SEBrainInjury::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

eBrainInjury_Type SEBrainInjury::GetType() const
{
  return m_Type;
}
void SEBrainInjury::SetType(eBrainInjury_Type Type)
{
  m_Type = Type;
}

void SEBrainInjury::ToString(std::ostream &str) const
{
  str << "Patient Action : Brain Injury"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity: "; HasSeverity() ? str << *m_Severity : str << "Not Set";
  str << "\n\tType: "<< eBrainInjury_Type_Name(GetType());
  str << std::flush;
}
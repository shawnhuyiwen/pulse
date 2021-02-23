/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEDyspnea.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientActions.h"

SEDyspnea::SEDyspnea(Logger* logger) : SEPatientAction(logger)
{
  m_Severity=nullptr;
}

SEDyspnea::~SEDyspnea()
{
  SAFE_DELETE(m_Severity);
}

void SEDyspnea::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_Severity);
}

void SEDyspnea::Copy(const SEDyspnea& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEDyspnea::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}

bool SEDyspnea::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return !m_Severity->IsZero();
}
void SEDyspnea::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEDyspnea::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
}

bool SEDyspnea::HasSeverity() const
{
  return m_Severity==nullptr?false:m_Severity->IsValid();
}
SEScalar0To1& SEDyspnea::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity=new SEScalar0To1();
  return *m_Severity;
}
double SEDyspnea::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEDyspnea::ToString(std::ostream &str) const
{
  str << "Patient Action : Dyspnea"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity: "; HasSeverity() ? str << *m_Severity : str << "Not Set";
  str << std::flush;
}
/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SERespiratoryFatigue.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientActions.h"

SERespiratoryFatigue::SERespiratoryFatigue(Logger* logger) : SEPatientAction(logger)
{
  m_Severity=nullptr;
}

SERespiratoryFatigue::~SERespiratoryFatigue()
{
  SAFE_DELETE(m_Severity);
}

void SERespiratoryFatigue::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_Severity);
}

void SERespiratoryFatigue::Copy(const SERespiratoryFatigue& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SERespiratoryFatigue::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}

bool SERespiratoryFatigue::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return !m_Severity->IsZero();
}
void SERespiratoryFatigue::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SERespiratoryFatigue::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
}

bool SERespiratoryFatigue::HasSeverity() const
{
  return m_Severity==nullptr?false:m_Severity->IsValid();
}
SEScalar0To1& SERespiratoryFatigue::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity=new SEScalar0To1();
  return *m_Severity;
}
double SERespiratoryFatigue::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SERespiratoryFatigue::ToString(std::ostream &str) const
{
  str << "Patient Action : Respiratory Fatigue"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity: "; HasSeverity() ? str << *m_Severity : str << "Not Set";
  str << std::flush;
}
/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEApnea.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientActions.h"

SEApnea::SEApnea() : SEPatientAction()
{
  m_Severity=nullptr;
}

SEApnea::~SEApnea()
{
  Clear();
}

void SEApnea::Clear()
{
  SEPatientAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SEApnea::Copy(const SEApnea& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEApnea::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}

bool SEApnea::IsActive() const
{
  return IsValid() ? !m_Severity->IsZero() : false;
}

bool SEApnea::HasSeverity() const
{
  return m_Severity==nullptr?false:m_Severity->IsValid();
}
SEScalar0To1& SEApnea::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity=new SEScalar0To1();
  return *m_Severity;
}
double SEApnea::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEApnea::ToString(std::ostream &str) const
{
  str << "Patient Action : Apnea"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity: "; HasSeverity() ? str << *m_Severity : str << "Not Set";
  str << std::flush;
}
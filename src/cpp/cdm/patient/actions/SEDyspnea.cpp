/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEDyspnea.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientActions.h"

SEDyspnea::SEDyspnea() : SEPatientAction()
{
  m_Severity=nullptr;
}

SEDyspnea::~SEDyspnea()
{
  Clear();
}

void SEDyspnea::Clear()
{
  SEPatientAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SEDyspnea::Copy(const SEDyspnea& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEDyspnea::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}

bool SEDyspnea::IsActive() const
{
  return IsValid() ? !m_Severity->IsZero() : false;
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
/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEAcuteStress.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientActions.h"

SEAcuteStress::SEAcuteStress(Logger* logger) : SEPatientAction(logger)
{
  m_Severity=nullptr;
}

SEAcuteStress::~SEAcuteStress()
{
  Clear();
}

void SEAcuteStress::Clear()
{
  
  SEPatientAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SEAcuteStress::Copy(const SEAcuteStress& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEAcuteStress::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}

bool SEAcuteStress::IsActive() const
{
  return IsValid() ? !m_Severity->IsZero() : false;
}

bool SEAcuteStress::HasSeverity() const
{
  return m_Severity==nullptr?false:m_Severity->IsValid();
}
SEScalar0To1& SEAcuteStress::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity=new SEScalar0To1();
  return *m_Severity;
}
double SEAcuteStress::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEAcuteStress::ToString(std::ostream &str) const
{
  str << "Patient Action : Acute Stress"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity: "; HasSeverity() ? str << *m_Severity : str << "Not Set";
  str << std::flush;
}
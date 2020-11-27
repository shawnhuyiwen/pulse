/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/conditions/SEPulmonaryShunt.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientConditions.h"

SEPulmonaryShunt::SEPulmonaryShunt(Logger* logger) : SEPatientCondition(logger)
{
  m_Severity=nullptr;
}

SEPulmonaryShunt::~SEPulmonaryShunt()
{
  SAFE_DELETE(m_Severity);
}

void SEPulmonaryShunt::Clear()
{
  SEPatientCondition::Clear();
  INVALIDATE_PROPERTY(m_Severity);
}

void SEPulmonaryShunt::Copy(const SEPulmonaryShunt& src)
{
  PBPatientCondition::Copy(src, *this);
}

bool SEPulmonaryShunt::IsValid() const
{
  return HasSeverity();
}
bool SEPulmonaryShunt::IsActive() const
{
  if (!IsValid())
    return false;
  return GetSeverity() > 0;
}

bool SEPulmonaryShunt::HasSeverity() const
{
  return m_Severity==nullptr?false:m_Severity->IsValid();
}

SEScalar0To1& SEPulmonaryShunt::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity=new SEScalar0To1();
  return *m_Severity;
}
double SEPulmonaryShunt::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEPulmonaryShunt::ToString(std::ostream &str) const
{
  str << "Patient Condition : Pulmonary Shunt"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity: "; HasSeverity() ? str << m_Severity : str << "NaN";
  str << std::flush;
}
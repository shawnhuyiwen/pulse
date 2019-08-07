/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/conditions/SEAcuteRespiratoryDistressSyndrome.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientConditions.h"

SEAcuteRespiratoryDistressSyndrome::SEAcuteRespiratoryDistressSyndrome() : SEPatientCondition()
{
  m_Severity=nullptr;
}

SEAcuteRespiratoryDistressSyndrome::~SEAcuteRespiratoryDistressSyndrome()
{
  Clear();
}

void SEAcuteRespiratoryDistressSyndrome::Clear()
{
  SEPatientCondition::Clear();
  SAFE_DELETE(m_Severity);
}

void SEAcuteRespiratoryDistressSyndrome::Copy(const SEAcuteRespiratoryDistressSyndrome& src)
{
  PBPatientCondition::Copy(src, *this);
}

bool SEAcuteRespiratoryDistressSyndrome::IsValid() const
{
  return HasSeverity();
}
bool SEAcuteRespiratoryDistressSyndrome::IsActive() const
{
  if (!IsValid())
    return false;
  return GetSeverity() > 0;
}

bool SEAcuteRespiratoryDistressSyndrome::HasSeverity() const
{
  return m_Severity==nullptr?false:m_Severity->IsValid();
}

SEScalar0To1& SEAcuteRespiratoryDistressSyndrome::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity=new SEScalar0To1();
  return *m_Severity;
}
double SEAcuteRespiratoryDistressSyndrome::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEAcuteRespiratoryDistressSyndrome::ToString(std::ostream &str) const
{
  str << "Patient Condition : ARDS"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity: "; HasSeverity() ? str << m_Severity : str << "NaN";
  str << std::flush;
}
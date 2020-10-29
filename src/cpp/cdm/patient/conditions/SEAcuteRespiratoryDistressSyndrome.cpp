/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/conditions/SEAcuteRespiratoryDistressSyndrome.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientConditions.h"

SEAcuteRespiratoryDistressSyndrome::SEAcuteRespiratoryDistressSyndrome(Logger* logger) : SEPatientCondition(logger)
{
  m_Severity=nullptr;
  m_LeftLungAffected = nullptr;
  m_RightLungAffected = nullptr;
}

SEAcuteRespiratoryDistressSyndrome::~SEAcuteRespiratoryDistressSyndrome()
{
  SAFE_DELETE(m_Severity);
  SAFE_DELETE(m_LeftLungAffected);
  SAFE_DELETE(m_RightLungAffected);
}

void SEAcuteRespiratoryDistressSyndrome::Clear()
{
  SEPatientCondition::Clear();
  INVALIDATE_PROPERTY(m_Severity);
  INVALIDATE_PROPERTY(m_LeftLungAffected);
  INVALIDATE_PROPERTY(m_RightLungAffected);
}

void SEAcuteRespiratoryDistressSyndrome::Copy(const SEAcuteRespiratoryDistressSyndrome& src)
{
  PBPatientCondition::Copy(src, *this);
}

bool SEAcuteRespiratoryDistressSyndrome::IsValid() const
{
  return HasSeverity() && HasLeftLungAffected() && HasRightLungAffected();
}
bool SEAcuteRespiratoryDistressSyndrome::IsActive() const
{
  if (!IsValid())
    return false;
  if (GetSeverity() <= 0)
    return false;
  if (GetLeftLungAffected() <= 0 && GetRightLungAffected() <= 0)
    return false;
  return true;
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

bool SEAcuteRespiratoryDistressSyndrome::HasLeftLungAffected() const
{
  return m_LeftLungAffected == nullptr ? false : m_LeftLungAffected->IsValid();
}
SEScalar0To1& SEAcuteRespiratoryDistressSyndrome::GetLeftLungAffected()
{
  if (m_LeftLungAffected == nullptr)
    m_LeftLungAffected = new SEScalar0To1();
  return *m_LeftLungAffected;
}
double SEAcuteRespiratoryDistressSyndrome::GetLeftLungAffected() const
{
  if (m_LeftLungAffected == nullptr)
    return SEScalar::dNaN();
  return m_LeftLungAffected->GetValue();
}

bool SEAcuteRespiratoryDistressSyndrome::HasRightLungAffected() const
{
  return m_RightLungAffected == nullptr ? false : m_RightLungAffected->IsValid();
}
SEScalar0To1& SEAcuteRespiratoryDistressSyndrome::GetRightLungAffected()
{
  if (m_RightLungAffected == nullptr)
    m_RightLungAffected = new SEScalar0To1();
  return *m_RightLungAffected;
}
double SEAcuteRespiratoryDistressSyndrome::GetRightLungAffected() const
{
  if (m_RightLungAffected == nullptr)
    return SEScalar::dNaN();
  return m_RightLungAffected->GetValue();
}

void SEAcuteRespiratoryDistressSyndrome::ToString(std::ostream &str) const
{
  str << "Patient Condition : ARDS"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity: "; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << "\n\tLeftLungAffected: "; HasLeftLungAffected() ? str << *m_LeftLungAffected : str << "NaN";
  str << "\n\tRightLungAffected: "; HasRightLungAffected() ? str << *m_RightLungAffected : str << "NaN";
  str << std::flush;
}
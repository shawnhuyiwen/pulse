/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/conditions/SELobarPneumonia.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientConditions.h"

SELobarPneumonia::SELobarPneumonia(Logger* logger) : SEPatientCondition(logger)
{
  m_Severity = nullptr;
  m_LeftLungAffected = nullptr;
  m_RightLungAffected = nullptr;
}

SELobarPneumonia::~SELobarPneumonia()
{
  SAFE_DELETE(m_Severity);
  SAFE_DELETE(m_LeftLungAffected);
  SAFE_DELETE(m_RightLungAffected);
}

void SELobarPneumonia::Clear()
{
  SEPatientCondition::Clear();
  INVALIDATE_PROPERTY(m_Severity);
  INVALIDATE_PROPERTY(m_LeftLungAffected);
  INVALIDATE_PROPERTY(m_RightLungAffected);
}

void SELobarPneumonia::Copy(const SELobarPneumonia& src)
{
  PBPatientCondition::Copy(src, *this);
}

bool SELobarPneumonia::IsValid() const
{
  return HasSeverity() && HasLeftLungAffected() && HasRightLungAffected();
}

bool SELobarPneumonia::IsActive() const
{
  if (!IsValid())
    return false;
  if (GetSeverity() <= 0)
    return false;
  if (GetLeftLungAffected() <= 0 && GetRightLungAffected() <= 0)
    return false;
  return true;
}

bool SELobarPneumonia::HasSeverity() const
{
  return m_Severity==nullptr?false:m_Severity->IsValid();
}
SEScalar0To1& SELobarPneumonia::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity=new SEScalar0To1();
  return *m_Severity;
}
double SELobarPneumonia::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

bool SELobarPneumonia::HasLeftLungAffected() const
{
  return m_LeftLungAffected == nullptr ? false : m_LeftLungAffected->IsValid();
}
SEScalar0To1& SELobarPneumonia::GetLeftLungAffected()
{
  if (m_LeftLungAffected == nullptr)
    m_LeftLungAffected = new SEScalar0To1();
  return *m_LeftLungAffected;
}
double SELobarPneumonia::GetLeftLungAffected() const
{
  if (m_LeftLungAffected == nullptr)
    return SEScalar::dNaN();
  return m_LeftLungAffected->GetValue();
}

bool SELobarPneumonia::HasRightLungAffected() const
{
  return m_RightLungAffected == nullptr ? false : m_RightLungAffected->IsValid();
}
SEScalar0To1& SELobarPneumonia::GetRightLungAffected()
{
  if (m_RightLungAffected == nullptr)
    m_RightLungAffected = new SEScalar0To1();
  return *m_RightLungAffected;
}
double SELobarPneumonia::GetRightLungAffected() const
{
  if (m_RightLungAffected == nullptr)
    return SEScalar::dNaN();
  return m_RightLungAffected->GetValue();
}

void SELobarPneumonia::ToString(std::ostream &str) const
{
  str << "Patient Condition : Lobar Pneumonia"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity: "; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << "\n\tLeftLungAffected: "; HasLeftLungAffected() ? str << *m_LeftLungAffected : str << "NaN";
  str << "\n\tRightLungAffected: "; HasRightLungAffected() ? str << *m_RightLungAffected : str << "NaN";
  str << std::flush;
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/conditions/SEChronicObstructivePulmonaryDisease.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientConditions.h"

SEChronicObstructivePulmonaryDisease::SEChronicObstructivePulmonaryDisease(Logger* logger) : SEPatientCondition(logger)
{
  m_BronchitisSeverity = nullptr;
  m_EmphysemaSeverity = nullptr;
}

SEChronicObstructivePulmonaryDisease::~SEChronicObstructivePulmonaryDisease()
{
  SAFE_DELETE(m_BronchitisSeverity);
  SAFE_DELETE(m_EmphysemaSeverity);
}

void SEChronicObstructivePulmonaryDisease::Clear()
{
  SEPatientCondition::Clear();
  INVALIDATE_PROPERTY(m_BronchitisSeverity);
  INVALIDATE_PROPERTY(m_EmphysemaSeverity);
}

void SEChronicObstructivePulmonaryDisease::Copy(const SEChronicObstructivePulmonaryDisease& src)
{
  PBPatientCondition::Copy(src, *this);
}

bool SEChronicObstructivePulmonaryDisease::IsValid() const
{
  return HasBronchitisSeverity() || HasEmphysemaSeverity();
}

bool SEChronicObstructivePulmonaryDisease::IsActive() const
{
  if (!IsValid())
    return false;
  return GetBronchitisSeverity() > 0 || GetEmphysemaSeverity() > 0;
}

bool SEChronicObstructivePulmonaryDisease::HasBronchitisSeverity() const
{
  return m_BronchitisSeverity == nullptr ? false : m_BronchitisSeverity->IsValid();
}
SEScalar0To1& SEChronicObstructivePulmonaryDisease::GetBronchitisSeverity()
{
  if (m_BronchitisSeverity == nullptr)
    m_BronchitisSeverity = new SEScalar0To1();
  return *m_BronchitisSeverity;
}
double SEChronicObstructivePulmonaryDisease::GetBronchitisSeverity() const
{
  if (m_BronchitisSeverity == nullptr)
    return SEScalar::dNaN();
  return m_BronchitisSeverity->GetValue();
}

bool SEChronicObstructivePulmonaryDisease::HasEmphysemaSeverity() const
{
  return m_EmphysemaSeverity == nullptr ? false : m_EmphysemaSeverity->IsValid();
}
SEScalar0To1& SEChronicObstructivePulmonaryDisease::GetEmphysemaSeverity()
{
  if (m_EmphysemaSeverity == nullptr)
    m_EmphysemaSeverity = new SEScalar0To1();
  return *m_EmphysemaSeverity;
}
double SEChronicObstructivePulmonaryDisease::GetEmphysemaSeverity() const
{
  if (m_EmphysemaSeverity == nullptr)
    return SEScalar::dNaN();
  return m_EmphysemaSeverity->GetValue();
}

void SEChronicObstructivePulmonaryDisease::ToString(std::ostream &str) const
{
  str << "Patient Condition : COPD";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tBronchitisSeverity: "; HasBronchitisSeverity() ? str << *m_BronchitisSeverity : str << "NaN";
  str << "\n\tEmphysemaSeverity: "; HasEmphysemaSeverity() ? str << *m_EmphysemaSeverity : str << "NaN";
  str << std::flush;
}



/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/conditions/SEChronicObstructivePulmonaryDisease.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/io/protobuf/PBPatientConditions.h"

SEChronicObstructivePulmonaryDisease::SEChronicObstructivePulmonaryDisease(Logger* logger) : SEPatientCondition(logger)
{
  m_BronchitisSeverity = nullptr;
}

SEChronicObstructivePulmonaryDisease::~SEChronicObstructivePulmonaryDisease()
{
  Clear();
  SAFE_DELETE(m_BronchitisSeverity);
}

void SEChronicObstructivePulmonaryDisease::Clear()
{
  SEPatientCondition::Clear();
  INVALIDATE_PROPERTY(m_BronchitisSeverity);
  for (auto itr : m_EmphysemaSeverities)
    if (itr.second != nullptr)
      itr.second->Invalidate();
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
  return GetBronchitisSeverity() > 0 || HasEmphysemaSeverity();
}
void SEChronicObstructivePulmonaryDisease::Activate()
{
  SEPatientCondition::Activate();
  FillLungImpairmentMap(m_EmphysemaSeverities);
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
  for (auto itr : m_EmphysemaSeverities)
    if (itr.second != nullptr && itr.second->IsPositive())
      return true;
  return false;
}
LungImpairmentMap& SEChronicObstructivePulmonaryDisease::GetEmphysemaSeverities()
{
  return m_EmphysemaSeverities;
}
const LungImpairmentMap& SEChronicObstructivePulmonaryDisease::GetEmphysemaSeverities() const
{
  return m_EmphysemaSeverities;
}

bool SEChronicObstructivePulmonaryDisease::HasEmphysemaSeverity(eLungCompartment cmpt) const
{
  auto s = m_EmphysemaSeverities.find(cmpt);
  if (s == m_EmphysemaSeverities.end())
    return false;
  if (s->second == nullptr)
    return false;
  return s->second->IsValid();
}
SEScalar0To1& SEChronicObstructivePulmonaryDisease::GetEmphysemaSeverity(eLungCompartment cmpt)
{
  SEScalar0To1* s = m_EmphysemaSeverities[cmpt];
  if (s == nullptr)
  {
    s = new SEScalar0To1();
    m_EmphysemaSeverities[cmpt] = s;
  }
  return *s;
}
const SEScalar0To1* SEChronicObstructivePulmonaryDisease::GetEmphysemaSeverity(eLungCompartment cmpt) const
{
  auto s = m_EmphysemaSeverities.find(cmpt);
  if (s == m_EmphysemaSeverities.end())
    return nullptr;
  return s->second;
}


/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/conditions/SEPneumonia.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/io/protobuf/PBPatientConditions.h"

SEPneumonia::SEPneumonia(Logger* logger) : SEPatientCondition(logger)
{

}

SEPneumonia::~SEPneumonia()
{
  Clear();
  DELETE_MAP_SECOND(m_Severities);
}

void SEPneumonia::Clear()
{
  SEPatientCondition::Clear();
  for (auto itr : m_Severities)
    if (itr.second != nullptr)
      itr.second->Invalidate();
}

void SEPneumonia::Copy(const SEPneumonia& src)
{
  PBPatientCondition::Copy(src, *this);
}

bool SEPneumonia::IsValid() const
{
  return HasSeverity();
}

bool SEPneumonia::IsActive() const
{
  if (!IsValid())
    return false;
  if (!HasSeverity())
    return false;
  return true;
}

bool SEPneumonia::HasSeverity() const
{
  for (auto itr : m_Severities)
    if (itr.second != nullptr && itr.second->IsPositive())
      return true;
  return false;
}
LungImpairmentMap& SEPneumonia::GetSeverities()
{
  return m_Severities;
}
const LungImpairmentMap& SEPneumonia::GetSeverities() const
{
  return m_Severities;
}

bool SEPneumonia::HasSeverity(eLungCompartment cmpt) const
{
  auto s = m_Severities.find(cmpt);
  if (s == m_Severities.end())
    return false;
  if (s->second == nullptr)
    return false;
  return s->second->IsValid();
}
SEScalar0To1& SEPneumonia::GetSeverity(eLungCompartment cmpt)
{
  SEScalar0To1* s = m_Severities[cmpt];
  if (s == nullptr)
  {
    s = new SEScalar0To1();
    m_Severities[cmpt] = s;
  }
  return *s;
}
const SEScalar0To1* SEPneumonia::GetSeverity(eLungCompartment cmpt) const
{
  auto s = m_Severities.find(cmpt);
  if (s == m_Severities.end())
    return nullptr;
  return s->second;
}

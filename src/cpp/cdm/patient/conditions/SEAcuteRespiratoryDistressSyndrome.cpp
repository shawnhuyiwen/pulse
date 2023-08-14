/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/conditions/SEAcuteRespiratoryDistressSyndrome.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/io/protobuf/PBPatientConditions.h"

SEAcuteRespiratoryDistressSyndrome::SEAcuteRespiratoryDistressSyndrome(Logger* logger) : SEPatientCondition(logger)
{

}

SEAcuteRespiratoryDistressSyndrome::~SEAcuteRespiratoryDistressSyndrome()
{
  Clear();
  DELETE_MAP_SECOND(m_Severities);
}

void SEAcuteRespiratoryDistressSyndrome::Clear()
{
  SEPatientCondition::Clear();
  for (auto itr : m_Severities)
    if (itr.second != nullptr)
      itr.second->Invalidate();
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
  if (!HasSeverity())
    return false;
  return true;
}
void SEAcuteRespiratoryDistressSyndrome::Activate()
{
  SEPatientCondition::Activate();
  SERespiratorySystem::FillLungImpairmentMap(m_Severities);
}

bool SEAcuteRespiratoryDistressSyndrome::HasSeverity() const
{
  for (auto itr : m_Severities)
    if (itr.second != nullptr && itr.second->IsValid())
      return true;
  return false;
}
LungImpairmentMap& SEAcuteRespiratoryDistressSyndrome::GetSeverities()
{
  return m_Severities;
}
const LungImpairmentMap& SEAcuteRespiratoryDistressSyndrome::GetSeverities() const
{
  return m_Severities;
}

bool SEAcuteRespiratoryDistressSyndrome::HasSeverity(eLungCompartment cmpt) const
{
  auto s = m_Severities.find(cmpt);
  if (s == m_Severities.end())
    return false;
  if (s->second == nullptr)
    return false;
  return s->second->IsValid();
}
SEScalar0To1& SEAcuteRespiratoryDistressSyndrome::GetSeverity(eLungCompartment cmpt)
{
  SEScalar0To1* s = m_Severities[cmpt];
  if (s == nullptr)
  {
    s = new SEScalar0To1();
    m_Severities[cmpt] = s;
  }
  return *s;
}
const SEScalar0To1* SEAcuteRespiratoryDistressSyndrome::GetSeverity(eLungCompartment cmpt) const
{
  auto s = m_Severities.find(cmpt);
  if (s == m_Severities.end())
    return nullptr;
  return s->second;
}

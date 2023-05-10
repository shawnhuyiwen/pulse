/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEChronicObstructivePulmonaryDiseaseExacerbation.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SEChronicObstructivePulmonaryDiseaseExacerbation::SEChronicObstructivePulmonaryDiseaseExacerbation(Logger* logger) : SEPatientAction(logger)
{
  m_BronchitisSeverity = nullptr;
}

SEChronicObstructivePulmonaryDiseaseExacerbation::~SEChronicObstructivePulmonaryDiseaseExacerbation()
{
  SAFE_DELETE(m_BronchitisSeverity);
  DELETE_MAP_SECOND(m_EmphysemaSeverities);
}

void SEChronicObstructivePulmonaryDiseaseExacerbation::Clear()
{
  SEPatientAction::Clear();
  INVALIDATE_PROPERTY(m_BronchitisSeverity);
  for (auto itr : m_EmphysemaSeverities)
    if (itr.second != nullptr)
      itr.second->SetValue(0);
}

void SEChronicObstructivePulmonaryDiseaseExacerbation::Copy(const SEChronicObstructivePulmonaryDiseaseExacerbation& src, bool /*preserveState*/)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEChronicObstructivePulmonaryDiseaseExacerbation::IsValid() const
{
  return HasBronchitisSeverity() || HasEmphysemaSeverity();
}

bool SEChronicObstructivePulmonaryDiseaseExacerbation::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return GetBronchitisSeverity() > 0 || HasEmphysemaSeverity();
}
void SEChronicObstructivePulmonaryDiseaseExacerbation::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEChronicObstructivePulmonaryDiseaseExacerbation::GetScalar(const std::string& name)
{
  if (name.compare("BronchitisSeverity") == 0)
    return &GetBronchitisSeverity();
  return nullptr;
}

bool SEChronicObstructivePulmonaryDiseaseExacerbation::HasBronchitisSeverity() const
{
  return m_BronchitisSeverity == nullptr ? false : m_BronchitisSeverity->IsValid();
}
SEScalar0To1& SEChronicObstructivePulmonaryDiseaseExacerbation::GetBronchitisSeverity()
{
  if (m_BronchitisSeverity == nullptr)
    m_BronchitisSeverity = new SEScalar0To1();
  return *m_BronchitisSeverity;
}
double SEChronicObstructivePulmonaryDiseaseExacerbation::GetBronchitisSeverity() const
{
  if (m_BronchitisSeverity == nullptr)
    return SEScalar::dNaN();
  return m_BronchitisSeverity->GetValue();
}

bool SEChronicObstructivePulmonaryDiseaseExacerbation::HasEmphysemaSeverity() const
{
  for (auto itr : m_EmphysemaSeverities)
    if (itr.second != nullptr && itr.second->IsPositive())
      return true;
  return false;
}
LungImpairmentMap& SEChronicObstructivePulmonaryDiseaseExacerbation::GetEmphysemaSeverities()
{
  return m_EmphysemaSeverities;
}
const LungImpairmentMap& SEChronicObstructivePulmonaryDiseaseExacerbation::GetEmphysemaSeverities() const
{
  return m_EmphysemaSeverities;
}

bool SEChronicObstructivePulmonaryDiseaseExacerbation::HasEmphysemaSeverity(eLungCompartment cmpt) const
{
  auto s = m_EmphysemaSeverities.find(cmpt);
  if (s == m_EmphysemaSeverities.end())
    return false;
  if (s->second == nullptr)
    return false;
  return s->second->IsValid();
}
SEScalar0To1& SEChronicObstructivePulmonaryDiseaseExacerbation::GetEmphysemaSeverity(eLungCompartment cmpt)
{
  SEScalar0To1* s = m_EmphysemaSeverities[cmpt];
  if (s == nullptr)
  {
    s = new SEScalar0To1();
    m_EmphysemaSeverities[cmpt] = s;
    s->SetValue(0);
  }
  return *s;
}
const SEScalar0To1* SEChronicObstructivePulmonaryDiseaseExacerbation::GetEmphysemaSeverity(eLungCompartment cmpt) const
{
  auto s = m_EmphysemaSeverities.find(cmpt);
  if (s == m_EmphysemaSeverities.end())
    return nullptr;
  return s->second;
}

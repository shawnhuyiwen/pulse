/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "system/physiology/SEGastrointestinalSystem.h"
#include "patient/SENutrition.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarMassPerTime.h"

SEGastrointestinalSystem::SEGastrointestinalSystem(Logger* logger) : SESystem(logger)
{
  m_ChymeAbsorptionRate = nullptr;
  m_StomachContents     = nullptr;
}

SEGastrointestinalSystem::~SEGastrointestinalSystem()
{
  Clear();
}

void SEGastrointestinalSystem::Clear()
{
  SESystem::Clear();
  SAFE_DELETE(m_ChymeAbsorptionRate);
  SAFE_DELETE(m_StomachContents);
}


const SEScalar* SEGastrointestinalSystem::GetScalar(const std::string& name)
{
  if(name.compare("ChymeAbsorptionRate") == 0)
    return &GetChymeAbsorptionRate();

  size_t split = name.find('-');
  if (split != name.npos)
  {
    std::string child = name.substr(0, split);
    std::string prop = name.substr(split + 1, name.npos);
    if (child == "StomachContents")
      return GetStomachContents().GetScalar(prop);
  }
  return nullptr;
}

bool SEGastrointestinalSystem::HasChymeAbsorptionRate() const
{
  return m_ChymeAbsorptionRate == nullptr ? false : m_ChymeAbsorptionRate->IsValid();
}
SEScalarVolumePerTime& SEGastrointestinalSystem::GetChymeAbsorptionRate()
{
  if (m_ChymeAbsorptionRate == nullptr)
    m_ChymeAbsorptionRate = new SEScalarVolumePerTime();
  return *m_ChymeAbsorptionRate;
}
double SEGastrointestinalSystem::GetChymeAbsorptionRate(const VolumePerTimeUnit& unit) const
{
  if (m_ChymeAbsorptionRate == nullptr)
    return SEScalar::dNaN();
  return m_ChymeAbsorptionRate->GetValue(unit);
}

bool SEGastrointestinalSystem::HasStomachContents() const
{
  return m_StomachContents == nullptr ? false : true;
}
SENutrition& SEGastrointestinalSystem::GetStomachContents()
{
  if (m_StomachContents == nullptr)
    m_StomachContents = new SENutrition(GetLogger());
  return *m_StomachContents;
}
const SENutrition* SEGastrointestinalSystem::GetStomachContents() const
{
  return m_StomachContents;
}
void SEGastrointestinalSystem::RemoveStomachContents()
{
  SAFE_DELETE(m_StomachContents);
}

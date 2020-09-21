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
  m_WaterAbsorptionRate = nullptr;
  m_StomachContents     = nullptr;
}

SEGastrointestinalSystem::~SEGastrointestinalSystem()
{
  SAFE_DELETE(m_WaterAbsorptionRate);
  SAFE_DELETE(m_StomachContents);
}

void SEGastrointestinalSystem::Clear()
{
  SESystem::Clear();
  INVALIDATE_PROPERTY(m_WaterAbsorptionRate);
  if (m_StomachContents)
    m_StomachContents->Clear();
}


const SEScalar* SEGastrointestinalSystem::GetScalar(const std::string& name)
{
  if(name.compare("WaterAbsorptionRate") == 0)
    return &GetWaterAbsorptionRate();

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

bool SEGastrointestinalSystem::HasWaterAbsorptionRate() const
{
  return m_WaterAbsorptionRate == nullptr ? false : m_WaterAbsorptionRate->IsValid();
}
SEScalarVolumePerTime& SEGastrointestinalSystem::GetWaterAbsorptionRate()
{
  if (m_WaterAbsorptionRate == nullptr)
    m_WaterAbsorptionRate = new SEScalarVolumePerTime();
  return *m_WaterAbsorptionRate;
}
double SEGastrointestinalSystem::GetWaterAbsorptionRate(const VolumePerTimeUnit& unit) const
{
  if (m_WaterAbsorptionRate == nullptr)
    return SEScalar::dNaN();
  return m_WaterAbsorptionRate->GetValue(unit);
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

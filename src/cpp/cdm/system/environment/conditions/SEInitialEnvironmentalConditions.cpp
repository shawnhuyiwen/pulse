/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/environment/conditions/SEInitialEnvironmentalConditions.h"
#include "cdm/system/environment/SEEnvironmentalConditions.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceConcentration.h"
#include "cdm/substance/SESubstanceFraction.h"

#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarHeatConductancePerArea.h"
#include "cdm/properties/SEScalarLengthPerTime.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPower.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTemperature.h"
#include "cdm/properties/SEScalarHeatResistanceArea.h"
#include "cdm/io/protobuf/PBEnvironmentConditions.h"

SEInitialEnvironmentalConditions::SEInitialEnvironmentalConditions(Logger* logger) : SEEnvironmentCondition(logger)
{
  m_EnvironmentalConditions = nullptr;
  m_EnvironmentalConditionsFile = "";
}

SEInitialEnvironmentalConditions::~SEInitialEnvironmentalConditions()
{
  m_EnvironmentalConditionsFile = "";
  SAFE_DELETE(m_EnvironmentalConditions);
}

void SEInitialEnvironmentalConditions::Clear()
{
  SEEnvironmentCondition::Clear();
  m_EnvironmentalConditionsFile = "";
  if (m_EnvironmentalConditions)
    m_EnvironmentalConditions->Clear();
}

void SEInitialEnvironmentalConditions::Copy(const SEInitialEnvironmentalConditions& src, const SESubstanceManager& subMgr)
{// Using Bindings to make a copy
  PBEnvironmentCondition::Copy(src, *this, subMgr);
}

bool SEInitialEnvironmentalConditions::IsValid() const
{
  return (HasEnvironmentalConditions() || HasEnvironmentalConditionsFile());
}

bool SEInitialEnvironmentalConditions::IsActive() const
{
  return IsValid();
}

bool SEInitialEnvironmentalConditions::HasEnvironmentalConditions() const
{
  return m_EnvironmentalConditions != nullptr;
}
SEEnvironmentalConditions& SEInitialEnvironmentalConditions::GetEnvironmentalConditions()
{
  if (m_EnvironmentalConditions == nullptr)
    m_EnvironmentalConditions = new SEEnvironmentalConditions(GetLogger());
  return *m_EnvironmentalConditions;
}
const SEEnvironmentalConditions* SEInitialEnvironmentalConditions::GetEnvironmentalConditions() const
{
  return m_EnvironmentalConditions;
}

std::string SEInitialEnvironmentalConditions::GetEnvironmentalConditionsFile() const
{
  return m_EnvironmentalConditionsFile;
}
void SEInitialEnvironmentalConditions::SetEnvironmentalConditionsFile(const std::string& fileName)
{
  m_EnvironmentalConditionsFile = fileName;
}
bool SEInitialEnvironmentalConditions::HasEnvironmentalConditionsFile() const
{
  return !m_EnvironmentalConditionsFile.empty();
}

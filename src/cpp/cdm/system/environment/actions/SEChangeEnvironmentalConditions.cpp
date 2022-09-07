/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/environment/actions/SEChangeEnvironmentalConditions.h"
#include "cdm/system/environment/SEEnvironmentalConditions.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceFraction.h"
#include "cdm/substance/SESubstanceConcentration.h"
#include "cdm/substance/SESubstanceManager.h"

#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarHeatConductancePerArea.h"
#include "cdm/properties/SEScalarLengthPerTime.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPower.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTemperature.h"
#include "cdm/properties/SEScalarHeatResistanceArea.h"
#include "cdm/io/protobuf/PBEnvironmentActions.h"


SEChangeEnvironmentalConditions::SEChangeEnvironmentalConditions(Logger* logger) : SEEnvironmentAction(logger)
{
  m_EnvironmentalConditions = nullptr;
  m_EnvironmentalConditionsFile = "";
  m_MergeType = eMergeType::Append;
}

SEChangeEnvironmentalConditions::~SEChangeEnvironmentalConditions()
{
  m_EnvironmentalConditionsFile = "";
  SAFE_DELETE(m_EnvironmentalConditions);
  m_MergeType = eMergeType::Append;
}

void SEChangeEnvironmentalConditions::Clear()
{
  SEEnvironmentAction::Clear();
  m_EnvironmentalConditionsFile = "";
  if (m_EnvironmentalConditions)
    m_EnvironmentalConditions->Clear();
  m_MergeType = eMergeType::Append;
}

void SEChangeEnvironmentalConditions::Copy(const SEChangeEnvironmentalConditions& src, const SESubstanceManager& subMgr, bool /*preserveState*/)
{// Using Bindings to make a copy
  PBEnvironmentAction::Copy(src, *this, subMgr);
}

bool SEChangeEnvironmentalConditions::IsValid() const
{
  return SEEnvironmentAction::IsValid() && (HasEnvironmentalConditions() || HasEnvironmentalConditionsFile());
}

bool SEChangeEnvironmentalConditions::IsActive() const
{
  return SEEnvironmentAction::IsActive();
}
void SEChangeEnvironmentalConditions::Deactivate()
{
  SEEnvironmentAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEChangeEnvironmentalConditions::GetScalar(const std::string& name)
{
  return GetEnvironmentalConditions().GetScalar(name);
}

bool SEChangeEnvironmentalConditions::HasEnvironmentalConditions() const
{
  return m_EnvironmentalConditions != nullptr;
}
SEEnvironmentalConditions& SEChangeEnvironmentalConditions::GetEnvironmentalConditions()
{
  if (m_EnvironmentalConditions == nullptr)
    m_EnvironmentalConditions = new SEEnvironmentalConditions(GetLogger());
  return *m_EnvironmentalConditions;
}
const SEEnvironmentalConditions* SEChangeEnvironmentalConditions::GetEnvironmentalConditions() const
{
  return m_EnvironmentalConditions;
}

std::string SEChangeEnvironmentalConditions::GetEnvironmentalConditionsFile() const
{
  return m_EnvironmentalConditionsFile;
}
void SEChangeEnvironmentalConditions::SetEnvironmentalConditionsFile(const std::string& fileName)
{
  m_EnvironmentalConditionsFile = fileName;
}
bool SEChangeEnvironmentalConditions::HasEnvironmentalConditionsFile() const
{
  return !m_EnvironmentalConditionsFile.empty();
}

void SEChangeEnvironmentalConditions::SetMergeType(eMergeType m)
{
  m_MergeType = m;
}
eMergeType SEChangeEnvironmentalConditions::GetMergeType() const
{
  return m_MergeType;
}

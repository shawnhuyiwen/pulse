/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskConfiguration.h"
#include "cdm/system/equipment/bag_valve_mask/SEBagValveMask.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/substance/SESubstanceConcentration.h"
#include "cdm/substance/SESubstanceFraction.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/io/protobuf/PBEquipmentActions.h"

SEBagValveMaskConfiguration::SEBagValveMaskConfiguration(Logger* logger) : SEBagValveMaskAction(logger)
{
  m_ConfigurationFile = "";
  m_Configuration = nullptr;
  m_MergeType = eMergeType::Append;
}

SEBagValveMaskConfiguration::~SEBagValveMaskConfiguration()
{
  m_ConfigurationFile = "";
  SAFE_DELETE(m_Configuration);
  m_MergeType = eMergeType::Append;
}

void SEBagValveMaskConfiguration::Clear()
{
  SEBagValveMaskAction::Clear();
  m_ConfigurationFile = "";
  if (m_Configuration)
    m_Configuration->Clear();
  m_MergeType = eMergeType::Append;
}

void SEBagValveMaskConfiguration::Copy(const SEBagValveMaskConfiguration& src, const SESubstanceManager& subMgr, bool /*preserveState*/)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this, subMgr);
}

bool SEBagValveMaskConfiguration::IsValid() const
{
  return SEBagValveMaskAction::IsValid() && (HasConfiguration() || HasConfigurationFile());
}

bool SEBagValveMaskConfiguration::IsActive() const
{
  return SEBagValveMaskAction::IsActive();
}
void SEBagValveMaskConfiguration::Deactivate()
{
  SEBagValveMaskAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEBagValveMaskConfiguration::GetScalar(const std::string& name)
{
  return GetConfiguration().GetScalar(name);
}

bool SEBagValveMaskConfiguration::HasConfiguration() const
{
  return m_Configuration != nullptr;
}
SEBagValveMask& SEBagValveMaskConfiguration::GetConfiguration()
{
  if (m_Configuration == nullptr)
    m_Configuration = new SEBagValveMask(GetLogger());
  return *m_Configuration;
}
const SEBagValveMask* SEBagValveMaskConfiguration::GetConfiguration() const
{
  return m_Configuration;
}

std::string SEBagValveMaskConfiguration::GetConfigurationFile() const
{
  return m_ConfigurationFile;
}
void SEBagValveMaskConfiguration::SetConfigurationFile(const std::string& fileName)
{
  m_ConfigurationFile = fileName;
}
bool SEBagValveMaskConfiguration::HasConfigurationFile() const
{
  return !m_ConfigurationFile.empty();
}

void SEBagValveMaskConfiguration::SetMergeType(eMergeType m)
{
  m_MergeType = m;
}
eMergeType SEBagValveMaskConfiguration::GetMergeType() const
{
  return m_MergeType;
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.h"
#include "cdm/system/equipment/mechanical_ventilator/SEMechanicalVentilatorSettings.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/substance/SESubstanceConcentration.h"
#include "cdm/substance/SESubstanceFraction.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarVolumePerPressure.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/io/protobuf/PBEquipmentActions.h"

SEMechanicalVentilatorConfiguration::SEMechanicalVentilatorConfiguration(Logger* logger) : SEMechanicalVentilatorAction(logger)
{
  m_SettingsFile = "";
  m_Settings = nullptr;
  m_MergeType = eMergeType::Append;
}

SEMechanicalVentilatorConfiguration::~SEMechanicalVentilatorConfiguration()
{
  m_SettingsFile = "";
  SAFE_DELETE(m_Settings);
  m_MergeType = eMergeType::Append;
}

void SEMechanicalVentilatorConfiguration::Clear()
{
  SEMechanicalVentilatorAction::Clear();
  m_SettingsFile = "";
  if (m_Settings)
    m_Settings->Clear();
  m_MergeType = eMergeType::Append;
}

void SEMechanicalVentilatorConfiguration::Copy(const SEMechanicalVentilatorConfiguration& src, const SESubstanceManager& subMgr, bool /*preserveState*/)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this, subMgr);
}

bool SEMechanicalVentilatorConfiguration::IsValid() const
{
  return SEMechanicalVentilatorAction::IsValid() && (HasSettings() || HasSettingsFile());
}

bool SEMechanicalVentilatorConfiguration::IsActive() const
{
  return SEMechanicalVentilatorAction::IsActive();
}
void SEMechanicalVentilatorConfiguration::Deactivate()
{
  SEMechanicalVentilatorAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEMechanicalVentilatorConfiguration::GetScalar(const std::string& name)
{
  return GetSettings().GetScalar(name);
}

bool SEMechanicalVentilatorConfiguration::HasSettings() const
{
  return m_Settings != nullptr;
}
SEMechanicalVentilatorSettings& SEMechanicalVentilatorConfiguration::GetSettings()
{
  if (m_Settings == nullptr)
    m_Settings = new SEMechanicalVentilatorSettings(GetLogger());
  return *m_Settings;
}
const SEMechanicalVentilatorSettings* SEMechanicalVentilatorConfiguration::GetSettings() const
{
  return m_Settings;
}

std::string SEMechanicalVentilatorConfiguration::GetSettingsFile() const
{
  return m_SettingsFile;
}
void SEMechanicalVentilatorConfiguration::SetSettingsFile(const std::string& fileName)
{
  m_SettingsFile = fileName;
}
bool SEMechanicalVentilatorConfiguration::HasSettingsFile() const
{
  return !m_SettingsFile.empty();
}

void SEMechanicalVentilatorConfiguration::SetMergeType(eMergeType m)
{
  m_MergeType = m;
}
eMergeType SEMechanicalVentilatorConfiguration::GetMergeType() const
{
  return m_MergeType;
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineConfiguration.h"
#include "cdm/system/equipment/anesthesia_machine/SEAnesthesiaMachine.h"
#include "cdm/system/equipment/anesthesia_machine/SEAnesthesiaMachineChamber.h"
#include "cdm/system/equipment/anesthesia_machine/SEAnesthesiaMachineOxygenBottle.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/io/protobuf/PBEquipmentActions.h"

SEAnesthesiaMachineConfiguration::SEAnesthesiaMachineConfiguration(Logger* logger) : SEAnesthesiaMachineAction(logger)
{
  m_ConfigurationFile = "";
  m_Configuration = nullptr;
  m_MergeType = eMergeType::Append;
}

SEAnesthesiaMachineConfiguration::~SEAnesthesiaMachineConfiguration()
{
  m_ConfigurationFile = "";
  SAFE_DELETE(m_Configuration);
  m_MergeType = eMergeType::Append;
}

void SEAnesthesiaMachineConfiguration::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  m_ConfigurationFile = "";
  if (m_Configuration)
    m_Configuration->Clear();
  m_MergeType = eMergeType::Append;
}

void SEAnesthesiaMachineConfiguration::Copy(const SEAnesthesiaMachineConfiguration& src, const SESubstanceManager& subMgr, bool /*preserveState*/)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this, subMgr);
}

bool SEAnesthesiaMachineConfiguration::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && (HasConfiguration() || HasConfigurationFile());
}

bool SEAnesthesiaMachineConfiguration::IsActive() const
{
  return SEAnesthesiaMachineAction::IsActive();
}
void SEAnesthesiaMachineConfiguration::Deactivate()
{
  SEAnesthesiaMachineAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEAnesthesiaMachineConfiguration::GetScalar(const std::string& name)
{
  return GetConfiguration().GetScalar(name);
}

bool SEAnesthesiaMachineConfiguration::HasConfiguration() const
{
  return m_Configuration != nullptr;
}
SEAnesthesiaMachine& SEAnesthesiaMachineConfiguration::GetConfiguration()
{
  if (m_Configuration == nullptr)
    m_Configuration = new SEAnesthesiaMachine(GetLogger());
  return *m_Configuration;
}
const SEAnesthesiaMachine* SEAnesthesiaMachineConfiguration::GetConfiguration() const
{
  return m_Configuration;
}

std::string SEAnesthesiaMachineConfiguration::GetConfigurationFile() const
{
  return m_ConfigurationFile;
}
void SEAnesthesiaMachineConfiguration::SetConfigurationFile(const std::string& fileName)
{
  m_ConfigurationFile = fileName;
}
bool SEAnesthesiaMachineConfiguration::HasConfigurationFile() const
{
  return !m_ConfigurationFile.empty();
}

void SEAnesthesiaMachineConfiguration::SetMergeType(eMergeType m)
{
  m_MergeType = m;
}
eMergeType SEAnesthesiaMachineConfiguration::GetMergeType() const
{
  return m_MergeType;
}

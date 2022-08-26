/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/inhaler/actions/SEInhalerConfiguration.h"
#include "cdm/system/equipment/inhaler/SEInhaler.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/io/protobuf/PBEquipmentActions.h"

SEInhalerConfiguration::SEInhalerConfiguration(Logger* logger) : SEInhalerAction(logger)
{
  m_ConfigurationFile = "";
  m_Configuration = nullptr;
  m_MergeType = eMergeType::Append;
}

SEInhalerConfiguration::~SEInhalerConfiguration()
{
  m_ConfigurationFile = "";
  SAFE_DELETE(m_Configuration);
  m_MergeType = eMergeType::Append;
}

void SEInhalerConfiguration::Clear()
{
  SEInhalerAction::Clear();
  m_ConfigurationFile = "";
  if (m_Configuration)
    m_Configuration->Clear();
  m_MergeType = eMergeType::Append;
}

void SEInhalerConfiguration::Copy(const SEInhalerConfiguration& src, const SESubstanceManager& subMgr, bool /*preserveState*/)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this, subMgr);
}

bool SEInhalerConfiguration::IsValid() const
{
  return SEInhalerAction::IsValid() && (HasConfiguration() || HasConfigurationFile());
}

bool SEInhalerConfiguration::IsActive() const
{
  return SEInhalerAction::IsActive();
}
void SEInhalerConfiguration::Deactivate()
{
  SEInhalerAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEInhalerConfiguration::GetScalar(const std::string& name)
{
  return GetConfiguration().GetScalar(name);
}

bool SEInhalerConfiguration::HasConfiguration() const
{
  return m_Configuration != nullptr;
}
SEInhaler& SEInhalerConfiguration::GetConfiguration()
{
  if (m_Configuration == nullptr)
    m_Configuration = new SEInhaler(GetLogger());
  return *m_Configuration;
}
const SEInhaler* SEInhalerConfiguration::GetConfiguration() const
{
  return m_Configuration;
}

std::string SEInhalerConfiguration::GetConfigurationFile() const
{
  return m_ConfigurationFile;
}
void SEInhalerConfiguration::SetConfigurationFile(const std::string& fileName)
{
  m_ConfigurationFile = fileName;
}
bool SEInhalerConfiguration::HasConfigurationFile() const
{
  return !m_ConfigurationFile.empty();
}

void SEInhalerConfiguration::SetMergeType(eMergeType m)
{
  m_MergeType = m;
}
eMergeType SEInhalerConfiguration::GetMergeType() const
{
  return m_MergeType;
}

void SEInhalerConfiguration::ToString(std::ostream &str) const
{
  SEInhalerAction::ToString(str);
  if (HasConfigurationFile())
  {
    str << "\n\tConfiguration File: "; str << m_ConfigurationFile;
  }
  else if (HasConfiguration())
  {
    str << "\n\tState: " << eSwitch_Name(m_Configuration->GetState());
    str << "\n\tMetered Dose: "; m_Configuration->HasMeteredDose() ? str << m_Configuration->GetMeteredDose() : str << "NaN";
    str << "\n\tNozzle Loss: "; m_Configuration->HasNozzleLoss() ? str << m_Configuration->GetNozzleLoss() : str << "NaN";
    str << "\n\tSpacerVolume: "; m_Configuration->HasSpacerVolume() ? str << m_Configuration->GetSpacerVolume() : str << "NaN";
    str << "\n\tSubstance: "; m_Configuration->HasSubstance() ? str << m_Configuration->GetSubstance()->GetName() : str << "Not Set";
  }
  str << "\n\tMergeType: " << eMergeType_Name(m_MergeType);
  str << std::flush;
}

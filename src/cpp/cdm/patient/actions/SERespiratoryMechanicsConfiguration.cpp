/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SERespiratoryMechanicsConfiguration.h"
#include "cdm/system/physiology/SERespiratoryMechanics.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SERespiratoryMechanicsConfiguration::SERespiratoryMechanicsConfiguration(Logger* logger) : SEPatientAction(logger)
{
  m_SettingsFile = "";
  m_Settings = nullptr;
  m_AppliedRespiratoryCycle = eAppliedRespiratoryCycle::Expiratory;
  m_MergeType = eMergeType::Append;
}

SERespiratoryMechanicsConfiguration::~SERespiratoryMechanicsConfiguration()
{
  m_SettingsFile = "";
  SAFE_DELETE(m_Settings);
}

void SERespiratoryMechanicsConfiguration::Clear()
{
  SEPatientAction::Clear();
  m_SettingsFile = "";
  if (m_Settings)
    m_Settings->Clear();
  m_AppliedRespiratoryCycle = eAppliedRespiratoryCycle::Expiratory;
  m_MergeType = eMergeType::Append;
}

void SERespiratoryMechanicsConfiguration::Copy(const SERespiratoryMechanicsConfiguration& src, bool /*preserveState*/)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SERespiratoryMechanicsConfiguration::IsValid() const
{
  return SEPatientAction::IsValid() &&(HasSettings() || HasSettingsFile());
}

bool SERespiratoryMechanicsConfiguration::IsActive() const
{
  return SEPatientAction::IsActive();
}
void SERespiratoryMechanicsConfiguration::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SERespiratoryMechanicsConfiguration::GetScalar(const std::string& name)
{
  return GetSettings().GetScalar(name);
}

bool SERespiratoryMechanicsConfiguration::HasSettings() const
{
  return m_Settings != nullptr;
}
SERespiratoryMechanics& SERespiratoryMechanicsConfiguration::GetSettings()
{
  if (m_Settings == nullptr)
    m_Settings = new SERespiratoryMechanics(GetLogger());
  return *m_Settings;
}
const SERespiratoryMechanics* SERespiratoryMechanicsConfiguration::GetSettings() const
{
  return m_Settings;
}

std::string SERespiratoryMechanicsConfiguration::GetSettingsFile() const
{
  return m_SettingsFile;
}
void SERespiratoryMechanicsConfiguration::SetSettingsFile(const std::string& fileName)
{
  m_SettingsFile = fileName;
}
bool SERespiratoryMechanicsConfiguration::HasSettingsFile() const
{
  return !m_SettingsFile.empty();
}

void SERespiratoryMechanicsConfiguration::SetAppliedRespiratoryCycle(eAppliedRespiratoryCycle c)
{
  m_AppliedRespiratoryCycle = c;
}
eAppliedRespiratoryCycle SERespiratoryMechanicsConfiguration::GetAppliedRespiratoryCycle() const
{
  return m_AppliedRespiratoryCycle;
}

void SERespiratoryMechanicsConfiguration::SetMergeType(eMergeType m)
{
  m_MergeType = m;
}
eMergeType SERespiratoryMechanicsConfiguration::GetMergeType() const
{
  return m_MergeType;
}

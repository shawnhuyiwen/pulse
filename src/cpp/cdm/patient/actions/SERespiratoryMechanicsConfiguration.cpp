/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "CommonDataModel.h"
#include "patient/actions/SERespiratoryMechanicsConfiguration.h"
#include "system/physiology/SERespiratoryMechanics.h"
#include "io/protobuf/PBPatientActions.h"

SERespiratoryMechanicsConfiguration::SERespiratoryMechanicsConfiguration(Logger* logger) : SEPatientAction(logger)
{
  m_ConfigurationFile = "";
  m_Configuration = nullptr;
  m_AppliedRespiratoryCycle = eAppliedRespiratoryCycle::Expiratory;
  m_MergeType = eMergeType::Append;
}

SERespiratoryMechanicsConfiguration::~SERespiratoryMechanicsConfiguration()
{
  m_ConfigurationFile = "";
  SAFE_DELETE(m_Configuration);
}

void SERespiratoryMechanicsConfiguration::Clear()
{
  SEPatientAction::Clear();
  m_ConfigurationFile = "";
  if (m_Configuration)
    m_Configuration->Clear();
  m_AppliedRespiratoryCycle = eAppliedRespiratoryCycle::Expiratory;
  m_MergeType = eMergeType::Append;
}

void SERespiratoryMechanicsConfiguration::Copy(const SERespiratoryMechanicsConfiguration& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SERespiratoryMechanicsConfiguration::IsValid() const
{
  return SEPatientAction::IsValid() &&(HasConfiguration() || HasConfigurationFile());
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
  return GetConfiguration().GetScalar(name);
}

bool SERespiratoryMechanicsConfiguration::HasConfiguration() const
{
  return m_Configuration != nullptr;
}
SERespiratoryMechanics& SERespiratoryMechanicsConfiguration::GetConfiguration()
{
  if (m_Configuration == nullptr)
    m_Configuration = new SERespiratoryMechanics(GetLogger());
  return *m_Configuration;
}
const SERespiratoryMechanics* SERespiratoryMechanicsConfiguration::GetConfiguration() const
{
  return m_Configuration;
}

std::string SERespiratoryMechanicsConfiguration::GetConfigurationFile() const
{
  return m_ConfigurationFile;
}
void SERespiratoryMechanicsConfiguration::SetConfigurationFile(const std::string& fileName)
{
  m_ConfigurationFile = fileName;
}
bool SERespiratoryMechanicsConfiguration::HasConfigurationFile() const
{
  return !m_ConfigurationFile.empty();
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

void SERespiratoryMechanicsConfiguration::ToString(std::ostream& str) const
{
  str << "Respiratory Mechanics Configuration";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  if (HasConfigurationFile())
  {
    str << "\n\tConfiguration File: "; str << m_ConfigurationFile;
  }
  else if (HasConfiguration())
  {
    str << "\n";
    m_Configuration->ToString(str);
    str << "\n\tAppliedRespiratoryCycle: " << eAppliedRespiratoryCycle_Name(m_AppliedRespiratoryCycle);
    str << "\n\tMergeType: " << eMergeType_Name(m_MergeType);
  }
  str << std::flush;
}

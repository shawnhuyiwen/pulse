/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "scenario/SEScenario.h"
#include "engine/SEAction.h"
#include "engine/SECondition.h"
#include "engine/SEDataRequestManager.h"
#include "engine/SEPatientConfiguration.h"
#include "substance/SESubstanceManager.h"
#include "io/protobuf/PBScenario.h"
#include "io/protobuf/PBActions.h"

SEScenario::SEScenario(Logger* logger, std::string const& dataDir) : Loggable(logger)
{
  m_PatientConfiguration = nullptr;
  m_SubMgr = new SESubstanceManager(logger);
  m_SubMgr->LoadSubstanceDirectory(dataDir);
  m_DataRequestMgr = new SEDataRequestManager(logger);
  Clear();
}

SEScenario::~SEScenario()
{
  Clear();
  delete m_SubMgr;
  delete m_DataRequestMgr;
}

void SEScenario::Clear()
{
  m_Name = "";
  m_Description = "";
  m_EngineStateFile = "";
  SAFE_DELETE(m_PatientConfiguration);
  DELETE_VECTOR(m_Actions);
  m_DataRequestMgr->Clear();
}

bool SEScenario::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBScenario::SerializeToString(*this, output, m);
}
bool SEScenario::SerializeToFile(const std::string& filename, SerializationFormat m) const
{
  return PBScenario::SerializeToFile(*this, filename, m);
}
bool SEScenario::SerializeFromString(const std::string& src, SerializationFormat m)
{
  return PBScenario::SerializeFromString(src, *this, m);
}
bool SEScenario::SerializeFromFile(const std::string& filename, SerializationFormat m)
{
  return PBScenario::SerializeFromFile(filename, *this, m);
}

bool SEScenario::IsValid() const
{
  if (HasPatientConfiguration())
  {
    if (!m_PatientConfiguration->IsValid())
      return false;
  }
  if(m_Actions.size()==0)
    return false;
  return true;
}

std::string SEScenario::GetName() const
{
  return m_Name;
}
void SEScenario::SetName(const std::string& name)
{
  m_Name = name;
}
bool SEScenario::HasName() const
{
  return m_Name.empty()?false:true;
}
void SEScenario::InvalidateName()
{
  m_Name = "";
}

std::string SEScenario::GetDescription() const
{
  return m_Description;
}
void SEScenario::SetDescription(const std::string& desc)
{
  m_Description = desc;
}
bool SEScenario::HasDescription() const
{
  return m_Description.empty()?false:true;
}
void SEScenario::InvalidateDescription()
{
  m_Description = "";
}

std::string SEScenario::GetEngineStateFile() const
{
  return m_EngineStateFile;
}
void SEScenario::SetEngineStateFile(const std::string& file)
{
  InvalidatePatientConfiguration();
  m_EngineStateFile = file;
}
bool SEScenario::HasEngineStateFile() const
{
  return m_EngineStateFile.empty() ? false : true;
}
void SEScenario::InvalidateEngineStateFile()
{
  m_EngineStateFile = "";
}

SEPatientConfiguration& SEScenario::GetPatientConfiguration()
{
  InvalidateEngineStateFile();
  if (m_PatientConfiguration == nullptr)
    m_PatientConfiguration = new SEPatientConfiguration(*m_SubMgr);
  return *m_PatientConfiguration;
}
const SEPatientConfiguration* SEScenario::GetPatientConfiguration() const
{
  return m_PatientConfiguration;
}
bool SEScenario::HasPatientConfiguration() const
{
  return m_PatientConfiguration != nullptr;
}
void SEScenario::InvalidatePatientConfiguration()
{
  SAFE_DELETE(m_PatientConfiguration);
}

void SEScenario::AddAction(const SEAction& a)
{
  // Make a copy using the bindings
  m_Actions.push_back(PBAction::Copy(a,*m_SubMgr));
}
const std::vector<const SEAction*>& SEScenario::GetActions() const
{
  return m_Actions;
}


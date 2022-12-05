/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/scenario/SEScenario.h"
#include "cdm/engine/SEAction.h"
#include "cdm/engine/SECondition.h"
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/io/protobuf/PBScenario.h"
#include "cdm/utils/FileUtils.h"

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
  if (m_PatientConfiguration)
    m_PatientConfiguration->Clear();
  DELETE_VECTOR(m_Actions);
  m_DataRequestMgr->Clear();
  m_DataRequestFiles.clear();
}

void SEScenario::Copy(const SEScenario& src)
{
  PBScenario::Copy(src, *this);
}

bool SEScenario::SerializeToString(std::string& output, eSerializationFormat m) const
{
  return PBScenario::SerializeToString(*this, output, m);
}
bool SEScenario::SerializeToFile(const std::string& filename) const
{
  return PBScenario::SerializeToFile(*this, filename);
}
bool SEScenario::SerializeFromString(const std::string& src, eSerializationFormat m)
{
  return PBScenario::SerializeFromString(src, *this, m);
}
bool SEScenario::SerializeFromFile(const std::string& filename)
{
  return PBScenario::SerializeFromFile(filename, *this);
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

std::string SEScenario::GetEngineStateFile() const
{
  return m_EngineStateFile;
}
void SEScenario::SetEngineStateFile(const std::string& file)
{
  m_EngineStateFile = file;
}
bool SEScenario::HasEngineStateFile() const
{
  return m_EngineStateFile.empty() ? false : true;
}

SEPatientConfiguration& SEScenario::GetPatientConfiguration()
{
  if (m_PatientConfiguration == nullptr)
    m_PatientConfiguration = new SEPatientConfiguration(GetLogger());
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

void SEScenario::AddAction(const SEAction& a)
{
  // Make a copy using the bindings
  m_Actions.push_back(SEAction::Copy(a,*m_SubMgr));
}
const std::vector<SEAction*>& SEScenario::GetActions()
{
  return m_Actions;
}
const std::vector<const SEAction*> SEScenario::GetActions() const
{
  return std::vector<const SEAction*>(m_Actions.begin(), m_Actions.end());
}

void SEScenario::MakeAbsoluteDataRequestFiles(const std::string& search)
{
  for (std::string& drFile : m_DataRequestFiles)
  {
    std::string found;
    if (FindFileInFilePath(search, drFile, found))
      drFile = found;
    else
    {
      Error("Unable to locate file: " + drFile);
    }
  }
}

void SEScenario::MakeRelativeDataRequestFiles(const std::string& rootDir)
{
  for (std::string& drFile : m_DataRequestFiles)
  {
    std::string drFilename;
    SplitFilename(drFile, drFilename);
    drFile = rootDir + drFilename;
  }
}

bool SEScenario::ProcessDataRequestFiles(const std::string& search)
{
  bool err=false;
  MakeAbsoluteDataRequestFiles(search);
  auto itr = m_DataRequestFiles.begin();
  while (itr != m_DataRequestFiles.end())
  {
    std::string drFile = *itr;
    Info("Merging DataRequest File: " + drFile);
    if (!m_DataRequestMgr->MergeDataRequestFile(drFile))
    {
      err=true;
      Error("Unable to merge file: " + drFile);
      itr++;
    }
    else
      itr = m_DataRequestFiles.erase(itr);
  }
  return err;
}

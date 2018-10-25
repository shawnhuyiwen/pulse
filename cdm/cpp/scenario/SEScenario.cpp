/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "scenario/SEScenario.h"
#include "scenario/SEAction.h"
#include "scenario/SECondition.h"
#include "scenario/SEDataRequestManager.h"
#include "scenario/SEScenarioInitialParameters.h"
#include "substance/SESubstanceManager.h"
#include "io/protobuf/PBScenario.h"
#include "io/protobuf/PBActions.h"

SEScenario::SEScenario(SESubstanceManager& subMgr) : Loggable(subMgr.GetLogger()), m_SubMgr(subMgr)
{
  m_InitialParameters = nullptr;
  m_DataRequestMgr = new SEDataRequestManager(subMgr.GetLogger());
  Clear();
}

SEScenario::~SEScenario()
{
  Clear();
  delete m_DataRequestMgr;
}

void SEScenario::Clear()
{
  m_Name = "";
  m_Description = "";
  m_EngineStateFile = "";
  SAFE_DELETE(m_InitialParameters);
  DELETE_VECTOR(m_Actions);
  m_DataRequestMgr->Clear();
}

bool SEScenario::SerializeToString(std::string& output, SerializationMode m) const
{
  return PBScenario::SerializeToString(*this, output, m);
}
bool SEScenario::SerializeToFile(const std::string& filename, SerializationMode m) const
{
  return PBScenario::SerializeToFile(*this, filename, m);
}
bool SEScenario::SerializeFromString(const std::string& src, SerializationMode m)
{
  return PBScenario::SerializeFromString(src, *this, m);
}
bool SEScenario::SerializeFromFile(const std::string& filename, SerializationMode m)
{
  return PBScenario::SerializeFromFile(filename, *this, m);
}

bool SEScenario::IsValid() const
{
  if (HasInitialParameters())
  {
    if (!m_InitialParameters->IsValid())
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
  InvalidateInitialParameters();
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

SEScenarioInitialParameters& SEScenario::GetInitialParameters()
{
  InvalidateEngineStateFile();
  if (m_InitialParameters == nullptr)
    m_InitialParameters = new SEScenarioInitialParameters(m_SubMgr);
  return *m_InitialParameters;
}
const SEScenarioInitialParameters* SEScenario::GetInitialParameters() const
{
  return m_InitialParameters;
}
bool SEScenario::HasInitialParameters() const
{
  return m_InitialParameters != nullptr;
}
void SEScenario::InvalidateInitialParameters()
{
  SAFE_DELETE(m_InitialParameters);
}

void SEScenario::AddAction(const SEAction& a)
{
  // Make a copy using the bindings
  m_Actions.push_back(PBAction::Copy(a,m_SubMgr));
}
const std::vector<const SEAction*>& SEScenario::GetActions() const
{
  return m_Actions;
}


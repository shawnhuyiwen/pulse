/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEAction.h"
#include "engine/SEActionManager.h"
#include "patient/actions/SEPatientAction.h"
#include "engine/SEPatientActionCollection.h"
#include "system/environment/actions/SEEnvironmentAction.h"
#include "engine/SEEnvironmentActionCollection.h"
#include "system/equipment/SEEquipmentAction.h"
#include "engine/SEEquipmentActionCollection.h"
#include "substance/SESubstanceManager.h"
#include "io/protobuf/PBEngine.h"

SEActionManager::SEActionManager(Logger* logger) : Loggable(logger)
{
  m_PatientActions = new SEPatientActionCollection(logger);
  m_EnvironmentActions = new SEEnvironmentActionCollection(logger);
  m_EquipmentActions = new SEEquipmentActionCollection(logger);
}

SEActionManager::~SEActionManager()
{
  SAFE_DELETE(m_PatientActions);
  SAFE_DELETE(m_EnvironmentActions);
  SAFE_DELETE(m_EquipmentActions);
}

void SEActionManager::Clear()
{
  m_PatientActions->Clear();
  m_EnvironmentActions->Clear();
  m_EquipmentActions->Clear();
}

bool SEActionManager::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBEngine::SerializeToString(*this, output, m);
}
bool SEActionManager::SerializeToFile(const std::string& filename) const
{
  return PBEngine::SerializeToFile(*this, filename);
}
bool SEActionManager::SerializeFromString(const std::string& src, SerializationFormat m, SESubstanceManager& subMgr)
{
  return PBEngine::SerializeFromString(src, *this, m, subMgr);
}
bool SEActionManager::SerializeFromFile(const std::string& filename, SESubstanceManager& subMgr)
{
  return PBEngine::SerializeFromFile(filename, *this, subMgr);
}

// A raw serialize method
// The caller is the owner of these actions in the provided vector
// Actions will not be processed into an action manager, only inserted into the list
// Which could result in an invalid action being in the list
// This was created to support passing invalid actions over socket or languages (specifically PulseJNI)
// A hemorrhage with no flow rate isinvalid and used to turn off an existing hemorrhage
// So we need to serialize that invalid action in, and have it processed by the engine action manager
// So this method is intended to be a middle man between the socket/language client and an engine.
bool SEActionManager::SerializeFromString(const std::string& src, std::vector<SEAction*>& dst, SerializationFormat m, const SESubstanceManager& subMgr)
{
  return PBEngine::SerializeFromString(src, dst, m, subMgr);
}

bool SEActionManager::ProcessAction(const SEAction& action, SESubstanceManager& subMgr)
{
  if (!action.IsValid())
  {
    m_ss << "Ignoring invalid action : " << action;
    Error(m_ss);
    return false;
  }
  bool bRet = true;

  const SEPatientAction* pa = dynamic_cast<const SEPatientAction*>(&action);
  if (pa != nullptr)
    bRet = m_PatientActions->ProcessAction(*pa, subMgr);

  const SEEnvironmentAction* ea = dynamic_cast<const SEEnvironmentAction*>(&action);
  if (ea != nullptr)
    bRet = m_EnvironmentActions->ProcessAction(*ea, subMgr);

  const SEEquipmentAction* ia = dynamic_cast<const SEEquipmentAction*>(&action);
  if (ia != nullptr)
    bRet = m_EquipmentActions->ProcessAction(*ia, subMgr);

  if (!bRet)
  {
    m_ss << "Not including action due to processing error : " << action;
    Error(m_ss);
  }
  return bRet;
}

void SEActionManager::GetAllActions(std::vector<const SEAction*>& actions) const
{
  m_PatientActions->GetAllActions(actions);
  m_EnvironmentActions->GetAllActions(actions);
  m_EquipmentActions->GetAllActions(actions);
}

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

SEActionManager::SEActionManager(SESubstanceManager& substances) : Loggable(substances.GetLogger()), 
                                                                   m_Substances(substances)
{
  m_PatientActions = new SEPatientActionCollection(substances);
  m_EnvironmentActions = new SEEnvironmentActionCollection(substances);
  m_EquipmentActions = new SEEquipmentActionCollection(substances);
}

SEActionManager::~SEActionManager()
{
  Clear();
  delete(m_PatientActions);
  delete(m_EnvironmentActions);
  delete(m_EquipmentActions);
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
bool SEActionManager::SerializeToFile(const std::string& filename, SerializationFormat m) const
{
  return PBEngine::SerializeToFile(*this, filename, m);
}
bool SEActionManager::SerializeFromString(const std::string& src, SerializationFormat m)
{
  return PBEngine::SerializeFromString(src, *this, m);
}
bool SEActionManager::SerializeFromFile(const std::string& filename, SerializationFormat m)
{
  return PBEngine::SerializeFromFile(filename, *this, m);
}

// A raw serialize method
// The caller is the owner of these actions in the provided vector
// Actions will not be processed into an action manager, only inserted into the list
// Which could result in an invalid action being in the list
// This was created to support passing invalid actions over socket or languages (specifically PulseJNI)
// A hemorrhage with no flow rate isinvalid and used to turn off an existing hemorrhage
// So we need to serialize that invalid action in, and have it processed by the engine action manager
// So this method is intended to be a middle man between the socket/language client and an engine.
bool SEActionManager::SerializeFromString(const std::string& src, std::vector<SEAction*>& dst, SerializationFormat m, SESubstanceManager& subMgr)
{
  return PBEngine::SerializeFromString(src, dst, m, subMgr);
}

bool SEActionManager::ProcessAction(const SEAction& action)
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
    bRet = m_PatientActions->ProcessAction(*pa);

  const SEEnvironmentAction* ea = dynamic_cast<const SEEnvironmentAction*>(&action);
  if (ea != nullptr)
    bRet = m_EnvironmentActions->ProcessAction(*ea);

  const SEEquipmentAction* ia = dynamic_cast<const SEEquipmentAction*>(&action);
  if (ia != nullptr)
    bRet = m_EquipmentActions->ProcessAction(*ia);

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




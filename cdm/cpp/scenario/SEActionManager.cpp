/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "scenario/SEAction.h"
#include "scenario/SEActionManager.h"
#include "patient/actions/SEPatientAction.h"
#include "scenario/SEPatientActionCollection.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineAction.h"
#include "scenario/SEAnesthesiaMachineActionCollection.h"
#include "system/environment/actions/SEEnvironmentAction.h"
#include "scenario/SEEnvironmentActionCollection.h"
#include "system/equipment/inhaler/actions/SEInhalerAction.h"
#include "scenario/SEInhalerActionCollection.h"
#include "substance/SESubstanceManager.h"
#include "io/protobuf/PBScenario.h"

SEActionManager::SEActionManager(SESubstanceManager& substances) : Loggable(substances.GetLogger()), 
                                                                   m_Substances(substances)
{
  m_PatientActions = new SEPatientActionCollection(substances);
  m_AnesthesiaMachineActions = new SEAnesthesiaMachineActionCollection(substances);
  m_EnvironmentActions = new SEEnvironmentActionCollection(substances);
  m_InhalerActions = new SEInhalerActionCollection(substances);
}

SEActionManager::~SEActionManager()
{
  Clear();
  delete(m_PatientActions);
  delete(m_EnvironmentActions);
  delete(m_AnesthesiaMachineActions);
  delete(m_InhalerActions);
}

void SEActionManager::Clear()
{
  m_PatientActions->Clear();
  m_AnesthesiaMachineActions->Clear();
  m_EnvironmentActions->Clear();
  m_InhalerActions->Clear();
}

bool SEActionManager::SerializeToString(std::string& output, SerializationMode m) const
{
  return PBScenario::SerializeToString(*this, output, m);
}
bool SEActionManager::SerializeToFile(const std::string& filename, SerializationMode m) const
{
  return PBScenario::SerializeToFile(*this, filename, m);
}
bool SEActionManager::SerializeFromString(const std::string& src, SerializationMode m)
{
  return PBScenario::SerializeFromString(src, *this, m);
}
bool SEActionManager::SerializeFromFile(const std::string& filename, SerializationMode m)
{
  return PBScenario::SerializeFromFile(filename, *this, m);
}

bool SEActionManager::ProcessAction(const SEAction& action)
{
  if (!action.IsValid())
  {
    m_ss << "Ignoring invalid action : " << action;
    Error(m_ss);
    return false;
  }
  bool bRet;

  const SEPatientAction* pa = dynamic_cast<const SEPatientAction*>(&action);
  if (pa != nullptr)
    bRet = m_PatientActions->ProcessAction(*pa);

  const SEEnvironmentAction* ea = dynamic_cast<const SEEnvironmentAction*>(&action);
  if (ea != nullptr)
    bRet = m_EnvironmentActions->ProcessAction(*ea);

  const SEAnesthesiaMachineAction* aa = dynamic_cast<const SEAnesthesiaMachineAction*>(&action);
  if (aa != nullptr)
    bRet = m_AnesthesiaMachineActions->ProcessAction(*aa);

  const SEInhalerAction* ia = dynamic_cast<const SEInhalerAction*>(&action);
  if (ia != nullptr)
    bRet = m_InhalerActions->ProcessAction(*ia);

  if (!bRet)
  {
    m_ss << "Not including action due to processing error : " << action;
    Error(m_ss);
  }
  return bRet;
}

void SEActionManager::GetActiveActions(std::vector<const SEAction*>& actions) const
{
  m_PatientActions->GetActiveActions(actions);
  m_EnvironmentActions->GetActiveActions(actions);
  m_AnesthesiaMachineActions->GetActiveActions(actions);
  m_InhalerActions->GetActiveActions(actions);
}




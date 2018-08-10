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
#include "bind/cdm/Scenario.pb.h"
#include "substance/SESubstanceManager.h"

SEActionManager::SEActionManager(SESubstanceManager& substances) : Loggable(substances.GetLogger()), 
                                                                   m_Substances(substances)
{
  m_PatientActions = new SEPatientActionCollection(substances);
  m_AnesthesiaMachineActions = new SEAnesthesiaMachineActionCollection(substances);
  m_EnvironmentActions = new SEEnvironmentActionCollection(substances);
  m_InhalerActions = new SEInhalerActionCollection(substances);

  m_ProcessedActions = new cdm::ActionListData();
}

SEActionManager::~SEActionManager()
{
  Clear();
  delete(m_PatientActions);
  delete(m_EnvironmentActions);
  delete(m_AnesthesiaMachineActions);
  delete(m_InhalerActions);

  delete(m_ProcessedActions);
}

void SEActionManager::Clear()
{
  m_PatientActions->Clear();
  m_AnesthesiaMachineActions->Clear();
  m_EnvironmentActions->Clear();
  m_InhalerActions->Clear();
  m_ProcessedActions->Clear();
}

void SEActionManager::Load(const cdm::ActionListData& src, SEActionManager& dst)
{
  SEActionManager::Serialize(src, dst);
}
void SEActionManager::Serialize(const cdm::ActionListData& src, SEActionManager& dst)
{
  for (int i = 0; i < src.anyaction_size(); i++)
  {
    SEAction* a = SEAction::Load(src.anyaction()[i], dst.m_Substances);
    dst.ProcessAction(*a);
    delete a;
  }
}

cdm::ActionListData* SEActionManager::Unload(const SEActionManager& src)
{
  cdm::ActionListData* dst = new cdm::ActionListData();
  SEActionManager::Serialize(src, *dst);
  return dst;
}
void SEActionManager::Serialize(const SEActionManager& src, cdm::ActionListData& dst)
{
  SEPatientActionCollection::Serialize(*src.m_PatientActions, dst);
  SEEnvironmentActionCollection::Serialize(*src.m_EnvironmentActions, dst);
  SEAnesthesiaMachineActionCollection::Serialize(*src.m_AnesthesiaMachineActions, dst);
  SEInhalerActionCollection::Serialize(*src.m_InhalerActions, dst);
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
  // If you want to save out a scenario of your dynamic engine instance, use this
  cdm::AnyActionData* aData = nullptr;// m_ProcessedActions.add_anyaction();

  const SEPatientAction* pa = dynamic_cast<const SEPatientAction*>(&action);
  if (pa != nullptr)
    bRet = m_PatientActions->ProcessAction(*pa, aData == nullptr ? nullptr : aData->mutable_patientaction());

  const SEEnvironmentAction* ea = dynamic_cast<const SEEnvironmentAction*>(&action);
  if (ea != nullptr)
    bRet = m_EnvironmentActions->ProcessAction(*ea, aData == nullptr ? nullptr : aData->mutable_environmentaction());

  const SEAnesthesiaMachineAction* aa = dynamic_cast<const SEAnesthesiaMachineAction*>(&action);
  if (aa != nullptr)
    bRet = m_AnesthesiaMachineActions->ProcessAction(*aa, aData == nullptr ? nullptr : aData->mutable_anesthesiamachineaction());

  const SEInhalerAction* ia = dynamic_cast<const SEInhalerAction*>(&action);
  if (ia != nullptr)
    bRet = m_InhalerActions->ProcessAction(*ia, aData == nullptr ? nullptr : aData->mutable_inhaleraction());

  if (!bRet)
  {
    m_ss << "Not including action due to processing error : " << action;
    Error(m_ss);
    m_ProcessedActions->mutable_anyaction()->RemoveLast();
  }
  return bRet;
}




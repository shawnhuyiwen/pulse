/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEEnvironmentActionCollection.h"
#include "system/environment/SEEnvironment.h"
#include "system/environment/actions/SEChangeEnvironmentalConditions.h"
#include "system/environment/actions/SEThermalApplication.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstanceFraction.h"
#include "properties/SEScalarLengthPerTime.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarHeatConductancePerArea.h"
#include "properties/SEScalarHeatResistanceArea.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"

SEEnvironmentActionCollection::SEEnvironmentActionCollection(SESubstanceManager& subMgr) : m_SubMgr(subMgr), Loggable(subMgr.GetLogger())
{
  m_ChangeEnvironmentalConditions = nullptr;
  m_ThermalApplication = nullptr;
}

SEEnvironmentActionCollection::~SEEnvironmentActionCollection()
{
  Clear();
}

void SEEnvironmentActionCollection::Clear()
{
  RemoveChangeEnvironmentalConditions();
  RemoveThermalApplication();
}

bool SEEnvironmentActionCollection::ProcessAction(const SEEnvironmentAction& action)
{
  const SEChangeEnvironmentalConditions* conditions = dynamic_cast<const SEChangeEnvironmentalConditions*>(&action);
  if (conditions != nullptr)
  {
    GetChangeEnvironmentalConditions().Copy(*conditions, m_SubMgr, true);
    m_ChangeEnvironmentalConditions->Activate();
    if (!m_ChangeEnvironmentalConditions->IsActive())
      RemoveChangeEnvironmentalConditions();
    return true;
  }

  const SEThermalApplication *thermal = dynamic_cast<const SEThermalApplication*>(&action);
  if (thermal != nullptr)
  {
    GetThermalApplication().Copy(*thermal, true);
    m_ThermalApplication->Activate();
    if (!m_ThermalApplication->IsActive())
      RemoveThermalApplication();
    return true;
  }

  /// \error Unsupported Action
  Error("Unsupported Environment Action");
  return false;
}

bool SEEnvironmentActionCollection::HasChangeEnvironmentalConditions() const
{
  return m_ChangeEnvironmentalConditions == nullptr ? false : m_ChangeEnvironmentalConditions->IsActive();
}
SEChangeEnvironmentalConditions& SEEnvironmentActionCollection::GetChangeEnvironmentalConditions()
{
  if (m_ChangeEnvironmentalConditions == nullptr)
    m_ChangeEnvironmentalConditions = new SEChangeEnvironmentalConditions();
  return *m_ChangeEnvironmentalConditions;
}
const SEChangeEnvironmentalConditions* SEEnvironmentActionCollection::GetChangeEnvironmentalConditions() const
{
  return m_ChangeEnvironmentalConditions;
}
void SEEnvironmentActionCollection::RemoveChangeEnvironmentalConditions()
{
  if (m_ChangeEnvironmentalConditions)
    m_ChangeEnvironmentalConditions->Deactivate();
}

bool SEEnvironmentActionCollection::HasThermalApplication() const
{
  return m_ThermalApplication == nullptr ? false : m_ThermalApplication->IsActive();
}
SEThermalApplication& SEEnvironmentActionCollection::GetThermalApplication()
{
  if (m_ThermalApplication == nullptr)
    m_ThermalApplication = new SEThermalApplication();
  return *m_ThermalApplication;
}
const SEThermalApplication* SEEnvironmentActionCollection::GetThermalApplication() const
{
  return m_ThermalApplication;
}
void SEEnvironmentActionCollection::RemoveThermalApplication()
{
  if (m_ThermalApplication)
    m_ThermalApplication->Deactivate();
}

void SEEnvironmentActionCollection::GetAllActions(std::vector<const SEAction*>& actions) const
{
  if (HasChangeEnvironmentalConditions())
    actions.push_back(GetChangeEnvironmentalConditions());
  if (HasThermalApplication())
    actions.push_back(GetThermalApplication());
}

const SEScalar* SEEnvironmentActionCollection::GetScalar(const std::string& actionName, const std::string& cmptName, const std::string& substance, const std::string& property)
{
  if (actionName == "ChangeEnvironmentalConditions")
    return GetChangeEnvironmentalConditions().GetScalar(property);
  if (actionName == "ThermalApplication")
    return GetThermalApplication().GetScalar(property);

  return nullptr;
}

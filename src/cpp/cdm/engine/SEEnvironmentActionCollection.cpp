/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/engine/SEEnvironmentActionCollection.h"
#include "cdm/system/environment/SEEnvironment.h"
#include "cdm/system/environment/actions/SEChangeEnvironmentalConditions.h"
#include "cdm/system/environment/actions/SEThermalApplication.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/substance/SESubstanceFraction.h"
#include "cdm/properties/SEScalarLengthPerTime.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarHeatConductancePerArea.h"
#include "cdm/properties/SEScalarHeatResistanceArea.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarTemperature.h"

SEEnvironmentActionCollection::SEEnvironmentActionCollection(SESubstanceManager& subMgr) : Loggable(subMgr.GetLogger()), m_SubMgr(subMgr)
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

const SEScalar* SEEnvironmentActionCollection::GetScalar(const std::string& actionName, const std::string& /*cmptName*/, const std::string& /*substance*/, const std::string& property)
{
  if (actionName == "ChangeEnvironmentalConditions")
    return GetChangeEnvironmentalConditions().GetScalar(property);
  if (actionName == "ThermalApplication")
    return GetThermalApplication().GetScalar(property);

  return nullptr;
}

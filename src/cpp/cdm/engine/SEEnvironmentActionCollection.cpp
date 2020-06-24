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

SEEnvironmentActionCollection::SEEnvironmentActionCollection(SESubstanceManager& substances) : Loggable(substances.GetLogger()), m_Substances(substances)
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
    if (m_ChangeEnvironmentalConditions == nullptr)
      m_ChangeEnvironmentalConditions = new SEChangeEnvironmentalConditions(GetLogger());
    m_ChangeEnvironmentalConditions->Copy(*conditions, m_Substances);
    if (!m_ChangeEnvironmentalConditions->IsActive())
      RemoveChangeEnvironmentalConditions();
    return true;
  }

  const SEThermalApplication *thermal = dynamic_cast<const SEThermalApplication*>(&action);
  if (thermal != nullptr)
  {
    if (m_ThermalApplication == nullptr)
      m_ThermalApplication = new SEThermalApplication();
    m_ThermalApplication->Copy(*thermal);
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
  return m_ChangeEnvironmentalConditions == nullptr ? false : true;
}
SEChangeEnvironmentalConditions* SEEnvironmentActionCollection::GetChangeEnvironmentalConditions()
{
  return m_ChangeEnvironmentalConditions;
}
const SEChangeEnvironmentalConditions* SEEnvironmentActionCollection::GetChangeEnvironmentalConditions() const
{
  return m_ChangeEnvironmentalConditions;
}
void SEEnvironmentActionCollection::RemoveChangeEnvironmentalConditions()
{
  SAFE_DELETE(m_ChangeEnvironmentalConditions);
}

bool SEEnvironmentActionCollection::HasThermalApplication() const
{
  return m_ThermalApplication != nullptr;
}
SEThermalApplication* SEEnvironmentActionCollection::GetThermalApplication()
{
  return m_ThermalApplication;
}
const SEThermalApplication* SEEnvironmentActionCollection::GetThermalApplication() const
{
  return m_ThermalApplication;
}
void SEEnvironmentActionCollection::RemoveThermalApplication()
{
  SAFE_DELETE(m_ThermalApplication);
}

void SEEnvironmentActionCollection::GetAllActions(std::vector<const SEAction*>& actions) const
{
  if (HasChangeEnvironmentalConditions())
    actions.push_back(GetChangeEnvironmentalConditions());
  if (HasThermalApplication())
    actions.push_back(GetThermalApplication());
}
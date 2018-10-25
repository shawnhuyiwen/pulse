/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "scenario/SEConditionManager.h"
//Patient Conditions
#include "patient/conditions/SEChronicAnemia.h"
#include "patient/conditions/SEChronicObstructivePulmonaryDisease.h"
#include "patient/conditions/SEChronicHeartFailure.h"
#include "patient/conditions/SEChronicRenalStenosis.h"
#include "patient/conditions/SEChronicVentricularSystolicDysfunction.h"
#include "patient/conditions/SEConsumeMeal.h"
#include "patient/conditions/SELobarPneumonia.h"
#include "patient/conditions/SEChronicPericardialEffusion.h"
#include "patient/conditions/SEImpairedAlveolarExchange.h"
// Environment Conditions
#include "system/environment/conditions/SEInitialEnvironmentConditions.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "io/protobuf/PBScenario.h"

SEConditionManager::SEConditionManager(SESubstanceManager& substances) : Loggable(substances.GetLogger()), m_Substances(substances)
{
  m_Anemia = nullptr;
  m_COPD = nullptr;
  m_ChronicVentricularSystolicDysfunction = nullptr;
  m_RenalStenosis = nullptr;
  m_ConsumeMeal = nullptr;
  m_LobarPneumonia = nullptr;
  m_PericardialEffusion = nullptr;
  m_ImpairedAlveolarExchange = nullptr;
  m_InitialEnvironmentConditions = nullptr;
}

SEConditionManager::~SEConditionManager()
{
  Clear();
}

void SEConditionManager::Clear()
{
  SAFE_DELETE(m_Anemia);
  SAFE_DELETE(m_COPD);
  SAFE_DELETE(m_ChronicVentricularSystolicDysfunction);
  SAFE_DELETE(m_RenalStenosis);
  SAFE_DELETE(m_ConsumeMeal);
  SAFE_DELETE(m_LobarPneumonia);
  SAFE_DELETE(m_PericardialEffusion);
  SAFE_DELETE(m_ImpairedAlveolarExchange);
  SAFE_DELETE(m_InitialEnvironmentConditions);
}

bool SEConditionManager::SerializeToString(std::string& output, SerializationMode m) const
{
  return PBScenario::SerializeToString(*this, output, m);
}
bool SEConditionManager::SerializeToFile(const std::string& filename, SerializationMode m) const
{
  return PBScenario::SerializeToFile(*this, filename, m);
}
bool SEConditionManager::SerializeFromString(const std::string& src, SerializationMode m)
{
  return PBScenario::SerializeFromString(src, *this, m);
}
bool SEConditionManager::SerializeFromFile(const std::string& filename, SerializationMode m)
{
  return PBScenario::SerializeFromFile(filename, *this, m);
}

bool SEConditionManager::ProcessCondition(const SECondition& condition)
{
  if (!condition.IsValid())
  {
    m_ss << "Ignoring invalid condition : " << condition;
    Error(m_ss);
    return false;
  }

  if (dynamic_cast<const SEPatientCondition*>(&condition) != nullptr)
  {

    const SEChronicAnemia* a = dynamic_cast<const SEChronicAnemia*>(&condition);
    if (a != nullptr)
    {
      if (HasChronicAnemia())
      {
        Error("Cannot have multiple Anemia conditions");
        return false;
      }
      m_Anemia = new SEChronicAnemia();
      m_Anemia->Copy(*a);
      return true;
    }

    const SEChronicObstructivePulmonaryDisease* copd = dynamic_cast<const SEChronicObstructivePulmonaryDisease*>(&condition);
    if (copd != nullptr)
    {
      if (HasChronicObstructivePulmonaryDisease())
      {
        Error("Cannot have multiple COPD conditions");
        return false;
      }
      m_COPD = new SEChronicObstructivePulmonaryDisease();
      m_COPD->Copy(*copd);
      return true;
    }

    const SEChronicHeartFailure* h = dynamic_cast<const SEChronicHeartFailure*>(&condition);
    if (h != nullptr)
    {
      if (HasChronicHeartFailure())
      {
        Error("Cannot have multiple Heart Failure conditions");
        return false;
      }
      const SEChronicVentricularSystolicDysfunction* vsd = dynamic_cast<const SEChronicVentricularSystolicDysfunction*>(&condition);
      if (vsd != nullptr)
      {
        m_ChronicVentricularSystolicDysfunction = new SEChronicVentricularSystolicDysfunction();
        m_ChronicVentricularSystolicDysfunction->Copy(*vsd);
        return true;
      }
      Error("Unknown Heart Failure condition");
      return false;
    }

    const SEChronicPericardialEffusion* pe = dynamic_cast<const SEChronicPericardialEffusion*>(&condition);
    if (pe != nullptr)
    {
      if (HasChronicPericardialEffusion())
      {
        Error("Cannot have multiple Pericardial Effusion conditions");
        return false;
      }
      m_PericardialEffusion = new SEChronicPericardialEffusion();
      m_PericardialEffusion->Copy(*pe);
      return true;
    }

    const SEChronicRenalStenosis* r = dynamic_cast<const SEChronicRenalStenosis*>(&condition);
    if (r != nullptr)
    {
      if (HasChronicRenalStenosis())
      {
        Error("Cannot have multiple Renal Stenosis conditions");
        return false;
      }
      m_RenalStenosis = new SEChronicRenalStenosis();
      m_RenalStenosis->Copy(*r);
      return true;
    }

    const SEConsumeMeal* g = dynamic_cast<const SEConsumeMeal*>(&condition);
    if (g != nullptr)
    {
      if (HasConsumeMeal())
      {
        Error("Cannot have multiple Gut Nutrient conditions");
        return false;
      }
      m_ConsumeMeal = new SEConsumeMeal();
      m_ConsumeMeal->Copy(*g);
      return true;
    }

    const SEImpairedAlveolarExchange* iae = dynamic_cast<const SEImpairedAlveolarExchange*>(&condition);
    if (iae != nullptr)
    {
      if (HasImpairedAlveolarExchange())
      {
        Error("Cannot have multiple Impaired Alveolar Exchange conditions");
        return false;
      }
      m_ImpairedAlveolarExchange = new SEImpairedAlveolarExchange();
      m_ImpairedAlveolarExchange->Copy(*iae);
      return true;
    }

    const SELobarPneumonia* lp = dynamic_cast<const SELobarPneumonia*>(&condition);
    if (lp != nullptr)
    {
      if (HasLobarPneumonia())
      {
        Error("Cannot have multiple Lobar Pneumonia conditions");
        return false;
      }
      m_LobarPneumonia = new SELobarPneumonia();
      m_LobarPneumonia->Copy(*lp);
      return true;
    }
  }

  if (dynamic_cast<const SEEnvironmentCondition*>(&condition) != nullptr)
  {
    const SEInitialEnvironmentConditions* ie = dynamic_cast<const SEInitialEnvironmentConditions*>(&condition);
    if (ie != nullptr)
    {
      if (HasInitialEnvironmentConditions())
      {
        Error("Cannot have multiple Initial Environment conditions");
        return false;
      }
      m_InitialEnvironmentConditions = new SEInitialEnvironmentConditions(m_Substances);
      m_InitialEnvironmentConditions->Copy(*ie);
      return true;
    }
  }
  
  /// \error Unsupported Condition
  Error("Unsupported Condition");
  return false;
}

bool SEConditionManager::HasChronicAnemia() const
{
  return m_Anemia == nullptr ? false : m_Anemia->IsValid();
}
SEChronicAnemia* SEConditionManager::GetChronicAnemia()
{
  return m_Anemia;
}
const SEChronicAnemia* SEConditionManager::GetChronicAnemia() const
{
  return m_Anemia;
}

bool SEConditionManager::HasChronicObstructivePulmonaryDisease() const
{
  return m_COPD == nullptr ? false : m_COPD->IsValid();
}
SEChronicObstructivePulmonaryDisease* SEConditionManager::GetChronicObstructivePulmonaryDisease()
{
  return m_COPD;
}
const SEChronicObstructivePulmonaryDisease* SEConditionManager::GetChronicObstructivePulmonaryDisease() const
{
  return m_COPD;
}

bool SEConditionManager::HasChronicHeartFailure() const
{
  return m_ChronicVentricularSystolicDysfunction != nullptr;
}
bool SEConditionManager::HasChronicVentricularSystolicDysfunction() const
{
  return m_ChronicVentricularSystolicDysfunction == nullptr ? false : m_ChronicVentricularSystolicDysfunction->IsValid();
}
SEChronicVentricularSystolicDysfunction* SEConditionManager::GetChronicVentricularSystolicDysfunction()
{
  return m_ChronicVentricularSystolicDysfunction;
}
const SEChronicVentricularSystolicDysfunction* SEConditionManager::GetChronicVentricularSystolicDysfunction() const
{
  return m_ChronicVentricularSystolicDysfunction;
}

bool SEConditionManager::HasChronicPericardialEffusion() const
{
  return m_PericardialEffusion == nullptr ? false : m_PericardialEffusion->IsValid();
}
SEChronicPericardialEffusion* SEConditionManager::GetChronicPericardialEffusion()
{
  return m_PericardialEffusion;
}
const SEChronicPericardialEffusion* SEConditionManager::GetChronicPericardialEffusion() const
{
  return m_PericardialEffusion;
}

bool SEConditionManager::HasChronicRenalStenosis() const
{
  return m_RenalStenosis == nullptr ? false : m_RenalStenosis->IsValid();
}
SEChronicRenalStenosis* SEConditionManager::GetChronicRenalStenosis()
{
  return m_RenalStenosis;
}
const SEChronicRenalStenosis* SEConditionManager::GetChronicRenalStenosis() const
{
  return m_RenalStenosis;
}


bool SEConditionManager::HasConsumeMeal() const
{
  return m_ConsumeMeal == nullptr ? false : m_ConsumeMeal->IsValid();
}
SEConsumeMeal* SEConditionManager::GetConsumeMeal()
{
  return m_ConsumeMeal;
}
const SEConsumeMeal* SEConditionManager::GetConsumeMeal() const
{
  return m_ConsumeMeal;
}

bool SEConditionManager::HasImpairedAlveolarExchange() const
{
  return m_ImpairedAlveolarExchange == nullptr ? false : m_ImpairedAlveolarExchange->IsValid();
}
SEImpairedAlveolarExchange* SEConditionManager::GetImpairedAlveolarExchange()
{
  return m_ImpairedAlveolarExchange;
}
const SEImpairedAlveolarExchange* SEConditionManager::GetImpairedAlveolarExchange() const
{
  return m_ImpairedAlveolarExchange;
}

bool SEConditionManager::HasLobarPneumonia() const
{
  return m_LobarPneumonia == nullptr ? false : m_LobarPneumonia->IsValid();
}
SELobarPneumonia* SEConditionManager::GetLobarPneumonia()
{
  return m_LobarPneumonia;
}
const SELobarPneumonia* SEConditionManager::GetLobarPneumonia() const
{
  return m_LobarPneumonia;
}

bool SEConditionManager::HasInitialEnvironmentConditions() const
{
  return m_InitialEnvironmentConditions == nullptr ? false : m_InitialEnvironmentConditions->IsValid();
}
SEInitialEnvironmentConditions* SEConditionManager::GetInitialEnvironmentConditions()
{
  return m_InitialEnvironmentConditions;
}
const SEInitialEnvironmentConditions* SEConditionManager::GetInitialEnvironmentConditions() const
{
  return m_InitialEnvironmentConditions;
}

void SEConditionManager::GetActiveConditions(std::vector<const SECondition*>& conditions) const
{
  if (HasChronicAnemia())
    conditions.push_back(GetChronicAnemia());
  if (HasChronicObstructivePulmonaryDisease())
    conditions.push_back(GetChronicObstructivePulmonaryDisease());
  if (HasChronicVentricularSystolicDysfunction())
    conditions.push_back(GetChronicVentricularSystolicDysfunction());
  if (HasChronicPericardialEffusion())
    conditions.push_back(GetChronicPericardialEffusion());
  if (HasChronicRenalStenosis())
    conditions.push_back(GetChronicRenalStenosis());
  if (HasConsumeMeal())
    conditions.push_back(GetConsumeMeal());
  if (HasImpairedAlveolarExchange())
    conditions.push_back(GetImpairedAlveolarExchange());
  if (HasLobarPneumonia())
    conditions.push_back(GetLobarPneumonia());
  if (HasInitialEnvironmentConditions())
    conditions.push_back(GetInitialEnvironmentConditions());
}
/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEConditionManager.h"
   //Patient Conditions
#include "patient/conditions/SEAcuteRespiratoryDistressSyndrome.h"
#include "patient/conditions/SEChronicAnemia.h"
#include "patient/conditions/SEChronicObstructivePulmonaryDisease.h"
#include "patient/conditions/SEChronicHeartFailure.h"
#include "patient/conditions/SEChronicPericardialEffusion.h"
#include "patient/conditions/SEChronicRenalStenosis.h"
#include "patient/conditions/SEChronicVentricularSystolicDysfunction.h"
#include "patient/conditions/SEConsumeMeal.h"
#include "patient/conditions/SEImpairedAlveolarExchange.h"
#include "patient/conditions/SELobarPneumonia.h"
#include "patient/conditions/SEPulmonaryFibrosis.h"
#include "patient/conditions/SEPulmonaryShunt.h"
#include "patient/conditions/SESepsis.h"
// Environment Conditions
#include "system/environment/conditions/SEInitialEnvironmentalConditions.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "io/protobuf/PBEngine.h"

SEConditionManager::SEConditionManager(SESubstanceManager& substances) : Loggable(substances.GetLogger()), m_Substances(substances)
{
  m_ARDS = nullptr;
  m_Anemia = nullptr;
  m_COPD = nullptr;
  m_ChronicVentricularSystolicDysfunction = nullptr;
  m_RenalStenosis = nullptr;
  m_ConsumeMeal = nullptr;
  m_LobarPneumonia = nullptr;
  m_PericardialEffusion = nullptr;
  m_PulmonaryFibrosis = nullptr;
  m_PulmonaryShunt = nullptr;
  m_ImpairedAlveolarExchange = nullptr;
  m_InitialEnvironmentalConditions = nullptr;
  m_Sepsis = nullptr;
}

SEConditionManager::~SEConditionManager()
{
  Clear();
}

void SEConditionManager::Clear()
{
  SAFE_DELETE(m_ARDS);
  SAFE_DELETE(m_Anemia);
  SAFE_DELETE(m_COPD);
  SAFE_DELETE(m_ChronicVentricularSystolicDysfunction);
  SAFE_DELETE(m_RenalStenosis);
  SAFE_DELETE(m_ConsumeMeal);
  SAFE_DELETE(m_LobarPneumonia);
  SAFE_DELETE(m_PericardialEffusion);
  SAFE_DELETE(m_PulmonaryFibrosis);
  SAFE_DELETE(m_PulmonaryShunt);
  SAFE_DELETE(m_ImpairedAlveolarExchange);
  SAFE_DELETE(m_InitialEnvironmentalConditions);
  SAFE_DELETE(m_Sepsis);
}

void SEConditionManager::Copy(const SEConditionManager& src)
{
  Clear();
  if (src.IsEmpty())
    return;

  std::vector<const SECondition*> conditions;
  src.GetAllConditions(conditions);
  for (const SECondition* c : conditions)
  {
    if (!ProcessCondition(*c))
      Error("Could not process " + c->GetName());
  }
}

bool SEConditionManager::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBEngine::SerializeToString(*this, output, m);
}
bool SEConditionManager::SerializeToFile(const std::string& filename, SerializationFormat m) const
{
  return PBEngine::SerializeToFile(*this, filename, m);
}
bool SEConditionManager::SerializeFromString(const std::string& src, SerializationFormat m)
{
  return PBEngine::SerializeFromString(src, *this, m);
}
bool SEConditionManager::SerializeFromFile(const std::string& filename, SerializationFormat m)
{
  return PBEngine::SerializeFromFile(filename, *this, m);
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

    const SEAcuteRespiratoryDistressSyndrome* ards = dynamic_cast<const SEAcuteRespiratoryDistressSyndrome*>(&condition);
    if (ards != nullptr)
    {
      if (HasAcuteRespiratoryDistressSyndrome())
      {
        Error("Cannot have multiple ARDS conditions");
        return false;
      }
      m_ARDS = new SEAcuteRespiratoryDistressSyndrome();
      m_ARDS->Copy(*ards);
      return true;
    }

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

    const SEPulmonaryFibrosis* pf = dynamic_cast<const SEPulmonaryFibrosis*>(&condition);
    if (pf != nullptr)
    {
      if (HasPulmonaryFibrosis())
      {
        Error("Cannot have multiple Pulmonary Fibrosis conditions");
        return false;
      }
      m_PulmonaryFibrosis = new SEPulmonaryFibrosis();
      m_PulmonaryFibrosis->Copy(*pf);
      return true;
    }

    const SEPulmonaryShunt* ps = dynamic_cast<const SEPulmonaryShunt*>(&condition);
    if (ps != nullptr)
    {
      if (HasPulmonaryShunt())
      {
        Error("Cannot have multiple Pulmonary Shunt conditions");
        return false;
      }
      m_PulmonaryShunt = new SEPulmonaryShunt();
      m_PulmonaryShunt->Copy(*ps);
      return true;
    }

    const SESepsis* s = dynamic_cast<const SESepsis*>(&condition);
    if (s != nullptr)
    {
      if (HasSepsis())
      {
        Error("Cannot have multiple Lobar Pneumonia conditions");
        return false;
      }
      m_Sepsis = new SESepsis();
      m_Sepsis->Copy(*s);
      return true;
    }
  }

  if (dynamic_cast<const SEEnvironmentCondition*>(&condition) != nullptr)
  {
    const SEInitialEnvironmentalConditions* ie = dynamic_cast<const SEInitialEnvironmentalConditions*>(&condition);
    if (ie != nullptr)
    {
      if (HasInitialEnvironmentalConditions())
      {
        Error("Cannot have multiple Initial Environmental conditions");
        return false;
      }
      m_InitialEnvironmentalConditions = new SEInitialEnvironmentalConditions(m_Substances);
      m_InitialEnvironmentalConditions->Copy(*ie);
      return true;
    }
  }

  /// \error Unsupported Condition
  Error("Unsupported Condition");
  return false;
}

bool SEConditionManager::HasAcuteRespiratoryDistressSyndrome() const
{
  return m_ARDS == nullptr ? false : m_ARDS->IsValid();
}
SEAcuteRespiratoryDistressSyndrome* SEConditionManager::GetAcuteRespiratoryDistressSyndrome()
{
  return m_ARDS;
}
const SEAcuteRespiratoryDistressSyndrome* SEConditionManager::GetAcuteRespiratoryDistressSyndrome() const
{
  return m_ARDS;
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

bool SEConditionManager::HasPulmonaryFibrosis() const
{
  return m_PulmonaryFibrosis == nullptr ? false : m_PulmonaryFibrosis->IsValid();
}
SEPulmonaryFibrosis* SEConditionManager::GetPulmonaryFibrosis()
{
  return m_PulmonaryFibrosis;
}
const SEPulmonaryFibrosis* SEConditionManager::GetPulmonaryFibrosis() const
{
  return m_PulmonaryFibrosis;
}

bool SEConditionManager::HasPulmonaryShunt() const
{
  return m_PulmonaryShunt == nullptr ? false : m_PulmonaryShunt->IsValid();
}
SEPulmonaryShunt* SEConditionManager::GetPulmonaryShunt()
{
  return m_PulmonaryShunt;
}
const SEPulmonaryShunt* SEConditionManager::GetPulmonaryShunt() const
{
  return m_PulmonaryShunt;
}

bool SEConditionManager::HasSepsis() const
{
  return m_Sepsis == nullptr ? false : m_Sepsis->IsValid();
}
SESepsis* SEConditionManager::GetSepsis()
{
  return m_Sepsis;
}
const SESepsis* SEConditionManager::GetSepsis() const
{
  return m_Sepsis;
}

bool SEConditionManager::HasInitialEnvironmentalConditions() const
{
  return m_InitialEnvironmentalConditions == nullptr ? false : m_InitialEnvironmentalConditions->IsValid();
}
SEInitialEnvironmentalConditions* SEConditionManager::GetInitialEnvironmentalConditions()
{
  return m_InitialEnvironmentalConditions;
}
const SEInitialEnvironmentalConditions* SEConditionManager::GetInitialEnvironmentalConditions() const
{
  return m_InitialEnvironmentalConditions;
}

void SEConditionManager::GetAllConditions(std::vector<const SECondition*>& conditions) const
{
  if (HasAcuteRespiratoryDistressSyndrome())
    conditions.push_back(GetAcuteRespiratoryDistressSyndrome());
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
  if (HasPulmonaryFibrosis())
    conditions.push_back(GetPulmonaryFibrosis());
  if (HasPulmonaryShunt())
    conditions.push_back(GetPulmonaryShunt());
  if (HasSepsis())
    conditions.push_back(GetSepsis());

  if (HasInitialEnvironmentalConditions())
    conditions.push_back(GetInitialEnvironmentalConditions());
}

bool SEConditionManager::IsEmpty() const
{
  if (HasAcuteRespiratoryDistressSyndrome())
    return false;
  if (HasChronicAnemia())
    return false;
  if (HasChronicObstructivePulmonaryDisease())
    return false;
  if (HasChronicVentricularSystolicDysfunction())
    return false;
  if (HasChronicPericardialEffusion())
    return false;
  if (HasChronicRenalStenosis())
    return false;
  if (HasConsumeMeal())
    return false;
  if (HasImpairedAlveolarExchange())
    return false;
  if (HasLobarPneumonia())
    return false;
  if (HasPulmonaryFibrosis())
    return false;
  if (HasPulmonaryShunt())
    return false;
  if (HasSepsis())
    return false;

  if (HasInitialEnvironmentalConditions())
    return false;
  return true;
}
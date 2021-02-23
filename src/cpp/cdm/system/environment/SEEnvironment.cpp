/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "system/environment/SEEnvironment.h"
#include "system/environment/SEActiveConditioning.h"
#include "system/environment/SEAppliedTemperature.h"
#include "system/environment/SEEnvironmentalConditions.h"
#include "system/environment/actions/SEChangeEnvironmentalConditions.h"
#include "system/environment/conditions/SEInitialEnvironmentalConditions.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarHeatConductancePerArea.h"
#include "properties/SEScalarHeatResistanceArea.h"
#include "properties/SEScalarLengthPerTime.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPower.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"

SEEnvironment::SEEnvironment(Logger* logger) : SESystem(logger)
{
  m_ActiveHeating = nullptr;
  m_ActiveCooling = nullptr;
  m_AppliedTemperature = nullptr; 
  m_EnvironmentalConditions = nullptr;
  m_ConvectiveHeatLoss = nullptr;
  m_ConvectiveHeatTranferCoefficient = nullptr;
  m_EvaporativeHeatLoss = nullptr;
  m_EvaporativeHeatTranferCoefficient = nullptr;
  m_RadiativeHeatLoss = nullptr;
  m_RadiativeHeatTranferCoefficient = nullptr;
  m_RespirationHeatLoss = nullptr;
  m_SkinHeatLoss = nullptr;
}

SEEnvironment::~SEEnvironment()
{
  SAFE_DELETE(m_ActiveHeating);
  SAFE_DELETE(m_ActiveCooling);
  SAFE_DELETE(m_AppliedTemperature);
  SAFE_DELETE(m_EnvironmentalConditions);
  SAFE_DELETE(m_ConvectiveHeatLoss);
  SAFE_DELETE(m_ConvectiveHeatTranferCoefficient);
  SAFE_DELETE(m_EvaporativeHeatLoss);
  SAFE_DELETE(m_EvaporativeHeatTranferCoefficient);
  SAFE_DELETE(m_RadiativeHeatLoss);
  SAFE_DELETE(m_RadiativeHeatTranferCoefficient);
  SAFE_DELETE(m_RespirationHeatLoss);
  SAFE_DELETE(m_SkinHeatLoss);;
}

void SEEnvironment::Clear()
{
  SESystem::Clear();
  if (m_ActiveHeating)
    m_ActiveHeating->Clear();
  if (m_ActiveCooling)
    m_ActiveCooling->Clear();
  if (m_AppliedTemperature)
    m_AppliedTemperature->Clear();
  if (m_EnvironmentalConditions)
    m_EnvironmentalConditions->Clear();

  INVALIDATE_PROPERTY(m_ConvectiveHeatLoss);
  INVALIDATE_PROPERTY(m_ConvectiveHeatTranferCoefficient);
  INVALIDATE_PROPERTY(m_EvaporativeHeatLoss);
  INVALIDATE_PROPERTY(m_EvaporativeHeatTranferCoefficient);
  INVALIDATE_PROPERTY(m_RadiativeHeatLoss);
  INVALIDATE_PROPERTY(m_RadiativeHeatTranferCoefficient);
  INVALIDATE_PROPERTY(m_RespirationHeatLoss);
  INVALIDATE_PROPERTY(m_SkinHeatLoss);
}

const SEScalar* SEEnvironment::GetScalar(const std::string& name)
{
  if (name.compare("ConvectiveHeatLoss") == 0)
    return &GetConvectiveHeatLoss();
  if (name.compare("ConvectiveHeatTranferCoefficient") == 0)
    return &GetConvectiveHeatTranferCoefficient();
  if (name.compare("EvaporativeHeatLoss") == 0)
    return &GetEvaporativeHeatLoss();
  if (name.compare("EvaporativeHeatTranferCoefficient") == 0)
    return &GetEvaporativeHeatTranferCoefficient();
  if (name.compare("RadiativeHeatLoss") == 0)
    return &GetRadiativeHeatLoss();
  if (name.compare("RadiativeHeatTranferCoefficient") == 0)
    return &GetRadiativeHeatTranferCoefficient();
  if (name.compare("RespirationHeatLoss") == 0)
    return &GetRespirationHeatLoss();
  if (name.compare("SkinHeatLoss") == 0)
    return &GetSkinHeatLoss();

  size_t split = name.find('-');
  if (split != name.npos)
  {
    std::string child = name.substr(0, split);
    std::string prop = name.substr(split + 1, name.npos);
    if (child == "EnvironmentalConditions")
      return GetEnvironmentalConditions().GetScalar(prop);
    if (child == "ActiveHeating")
      return GetActiveHeating().GetScalar(prop);
    if (child == "ActiveCooling")
      return GetActiveCooling().GetScalar(prop);
    if (child == "AppliedTemperature")
      return GetAppliedTemperature().GetScalar(prop);
  }  
  return nullptr;
}

bool SEEnvironment::ProcessChange(SEInitialEnvironmentalConditions& change, SESubstanceManager& subMgr)
{
  // If we have data then we merge it, if a file was provided
  // we reset and set the environment to the file, so we only have the file data
  if (change.HasEnvironmentalConditions())
    GetEnvironmentalConditions().Merge(change.GetEnvironmentalConditions(), subMgr);
  else if (change.HasEnvironmentalConditionsFile())
  {
    // Update the condition with the file contents
    std::string cfg_file = change.GetEnvironmentalConditionsFile();
    if (!change.GetEnvironmentalConditions().SerializeFromFile(cfg_file, subMgr))
    {
      /// \error Unable to read Configuration Action file
      Error("Could not read provided SEInitialEnvironment file", "SEEnvironment::ProcessChange");
      return false;
    }
    GetEnvironmentalConditions().Merge(change.GetEnvironmentalConditions(), subMgr);
  }
  StateChange();
  return true;
}

bool SEEnvironment::ProcessChange(SEChangeEnvironmentalConditions& change, SESubstanceManager& subMgr)
{
  // If we have data then we merge it, if a file was provided
  // we reset and set the environment to the file, so we only have the file data
  if (change.HasEnvironmentalConditionsFile())
  {
    // Update the action with the file contents
    std::string cfg_file = change.GetEnvironmentalConditionsFile();
    if (!change.GetEnvironmentalConditions().SerializeFromFile(cfg_file, subMgr))
    {
      /// \error Unable to read Configuration Action file
      Error("Could not read provided Environmental Conditions file", "SEEnvironment::ProcessChange");
      return false;
    }
    GetEnvironmentalConditions().Merge(change.GetEnvironmentalConditions(), subMgr);
  }
  else if (change.HasEnvironmentalConditions())
    GetEnvironmentalConditions().Merge(change.GetEnvironmentalConditions(), subMgr);
  StateChange();
  return true;
}

bool SEEnvironment::HasActiveHeating() const
{
  return m_ActiveHeating != nullptr;
}
SEActiveConditioning& SEEnvironment::GetActiveHeating()
{
  if (m_ActiveHeating == nullptr)
    m_ActiveHeating = new SEActiveConditioning(GetLogger());
  return *m_ActiveHeating;
}
const SEActiveConditioning* SEEnvironment::GetActiveHeating() const
{
  return m_ActiveHeating;
}
void SEEnvironment::RemoveActiveHeating()
{
  SAFE_DELETE(m_ActiveHeating);
}

bool SEEnvironment::HasActiveCooling() const
{
  return m_ActiveCooling != nullptr;
}
SEActiveConditioning& SEEnvironment::GetActiveCooling()
{
  if (m_ActiveCooling == nullptr)
    m_ActiveCooling = new SEActiveConditioning(GetLogger());
  return *m_ActiveCooling;
}
const SEActiveConditioning* SEEnvironment::GetActiveCooling() const
{
  return m_ActiveCooling;
}
void SEEnvironment::RemoveActiveCooling()
{
  SAFE_DELETE(m_ActiveCooling);
}

bool SEEnvironment::HasAppliedTemperature() const
{
  return m_AppliedTemperature != nullptr;
}
SEAppliedTemperature& SEEnvironment::GetAppliedTemperature()
{
  if (m_AppliedTemperature == nullptr)
    m_AppliedTemperature = new SEAppliedTemperature(GetLogger());
  return *m_AppliedTemperature;
}
const SEAppliedTemperature* SEEnvironment::GetAppliedTemperature() const
{
  return m_AppliedTemperature;
}
void SEEnvironment::RemoveAppliedTemperature()
{
  SAFE_DELETE(m_AppliedTemperature);
}

bool SEEnvironment::HasEnvironmentalConditions() const
{
  return m_EnvironmentalConditions != nullptr;
}
SEEnvironmentalConditions& SEEnvironment::GetEnvironmentalConditions()
{
  if (m_EnvironmentalConditions == nullptr)
    m_EnvironmentalConditions = new SEEnvironmentalConditions(GetLogger());
  return *m_EnvironmentalConditions;
}
const SEEnvironmentalConditions* SEEnvironment::GetEnvironmentalConditions() const
{
  return m_EnvironmentalConditions;
}
void SEEnvironment::RemoveEnvironmentalConditions()
{
  SAFE_DELETE(m_EnvironmentalConditions);
}

bool SEEnvironment::HasConvectiveHeatLoss() const
{
  return m_ConvectiveHeatLoss == nullptr ? false : m_ConvectiveHeatLoss->IsValid();
}
SEScalarPower& SEEnvironment::GetConvectiveHeatLoss()
{
  if (m_ConvectiveHeatLoss == nullptr)
    m_ConvectiveHeatLoss = new SEScalarPower();
  return *m_ConvectiveHeatLoss;
}
double SEEnvironment::GetConvectiveHeatLoss(const PowerUnit& unit) const
{
  if (m_ConvectiveHeatLoss == nullptr)
    return SEScalar::dNaN();
  return m_ConvectiveHeatLoss->GetValue(unit);
}

bool SEEnvironment::HasConvectiveHeatTranferCoefficient() const
{
  return m_ConvectiveHeatTranferCoefficient == nullptr ? false : m_ConvectiveHeatTranferCoefficient->IsValid();
}
SEScalarHeatConductancePerArea& SEEnvironment::GetConvectiveHeatTranferCoefficient()
{
  if (m_ConvectiveHeatTranferCoefficient == nullptr)
    m_ConvectiveHeatTranferCoefficient = new SEScalarHeatConductancePerArea();
  return *m_ConvectiveHeatTranferCoefficient;
}
double SEEnvironment::GetConvectiveHeatTranferCoefficient(const HeatConductancePerAreaUnit& unit) const
{
  if (m_ConvectiveHeatTranferCoefficient == nullptr)
    return SEScalar::dNaN();
  return m_ConvectiveHeatTranferCoefficient->GetValue(unit);
}

bool SEEnvironment::HasEvaporativeHeatLoss() const
{
  return m_EvaporativeHeatLoss == nullptr ? false : m_EvaporativeHeatLoss->IsValid();
}
SEScalarPower& SEEnvironment::GetEvaporativeHeatLoss()
{
  if (m_EvaporativeHeatLoss == nullptr)
    m_EvaporativeHeatLoss = new SEScalarPower();
  return *m_EvaporativeHeatLoss;
}
double SEEnvironment::GetEvaporativeHeatLoss(const PowerUnit& unit) const
{
  if (m_EvaporativeHeatLoss == nullptr)
    return SEScalar::dNaN();
  return m_EvaporativeHeatLoss->GetValue(unit);
}

bool SEEnvironment::HasEvaporativeHeatTranferCoefficient() const
{
  return m_EvaporativeHeatTranferCoefficient == nullptr ? false : m_EvaporativeHeatTranferCoefficient->IsValid();
}
SEScalarHeatConductancePerArea& SEEnvironment::GetEvaporativeHeatTranferCoefficient()
{
  if (m_EvaporativeHeatTranferCoefficient == nullptr)
    m_EvaporativeHeatTranferCoefficient = new SEScalarHeatConductancePerArea();
  return *m_EvaporativeHeatTranferCoefficient;
}
double SEEnvironment::GetEvaporativeHeatTranferCoefficient(const HeatConductancePerAreaUnit& unit) const
{
  if (m_EvaporativeHeatTranferCoefficient == nullptr)
    return SEScalar::dNaN();
  return m_EvaporativeHeatTranferCoefficient->GetValue(unit);
}

bool SEEnvironment::HasRadiativeHeatLoss() const
{
  return m_RadiativeHeatLoss == nullptr ? false : m_RadiativeHeatLoss->IsValid();
}
SEScalarPower& SEEnvironment::GetRadiativeHeatLoss()
{
  if (m_RadiativeHeatLoss == nullptr)
    m_RadiativeHeatLoss = new SEScalarPower();
  return *m_RadiativeHeatLoss;
}
double SEEnvironment::GetRadiativeHeatLoss(const PowerUnit& unit) const
{
  if (m_RadiativeHeatLoss == nullptr)
    return SEScalar::dNaN();
  return m_RadiativeHeatLoss->GetValue(unit);
}

bool SEEnvironment::HasRadiativeHeatTranferCoefficient() const
{
  return m_RadiativeHeatTranferCoefficient == nullptr ? false : m_RadiativeHeatTranferCoefficient->IsValid();
}
SEScalarHeatConductancePerArea& SEEnvironment::GetRadiativeHeatTranferCoefficient()
{
  if (m_RadiativeHeatTranferCoefficient == nullptr)
    m_RadiativeHeatTranferCoefficient = new SEScalarHeatConductancePerArea();
  return *m_RadiativeHeatTranferCoefficient;
}
double SEEnvironment::GetRadiativeHeatTranferCoefficient(const HeatConductancePerAreaUnit& unit) const
{
  if (m_RadiativeHeatTranferCoefficient == nullptr)
    return SEScalar::dNaN();
  return m_RadiativeHeatTranferCoefficient->GetValue(unit);
}

bool SEEnvironment::HasRespirationHeatLoss() const
{
  return m_RespirationHeatLoss == nullptr ? false : m_RespirationHeatLoss->IsValid();
}
SEScalarPower& SEEnvironment::GetRespirationHeatLoss()
{
  if (m_RespirationHeatLoss == nullptr)
    m_RespirationHeatLoss = new SEScalarPower();
  return *m_RespirationHeatLoss;
}
double SEEnvironment::GetRespirationHeatLoss(const PowerUnit& unit) const
{
  if (m_RespirationHeatLoss == nullptr)
    return SEScalar::dNaN();
  return m_RespirationHeatLoss->GetValue(unit);
}

bool SEEnvironment::HasSkinHeatLoss() const
{
  return m_SkinHeatLoss == nullptr ? false : m_SkinHeatLoss->IsValid();
}
SEScalarPower& SEEnvironment::GetSkinHeatLoss()
{
  if (m_SkinHeatLoss == nullptr)
    m_SkinHeatLoss = new SEScalarPower();
  return *m_SkinHeatLoss;
}
double SEEnvironment::GetSkinHeatLoss(const PowerUnit& unit) const
{
  if (m_SkinHeatLoss == nullptr)
    return SEScalar::dNaN();
  return m_SkinHeatLoss->GetValue(unit);
}

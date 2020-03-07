/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBEnvironmentActions.h"
#include "io/protobuf/PBEnvironment.h"
#include "io/protobuf/PBActions.h"
#include "bind/EnvironmentActions.pb.h"
#include "system/environment/actions/SEChangeEnvironmentalConditions.h"
#include "system/environment/actions/SEThermalApplication.h"
#include "substance/SESubstanceManager.h"

void PBEnvironmentAction::Serialize(const CDM_BIND::EnvironmentActionData& src, SEEnvironmentAction& dst)
{
  PBAction::Serialize(src.action(), dst);
}
void PBEnvironmentAction::Serialize(const SEEnvironmentAction& src, CDM_BIND::EnvironmentActionData& dst)
{
  PBAction::Serialize(src, *dst.mutable_action());
}

void PBEnvironmentAction::Load(const CDM_BIND::ChangeEnvironmentalConditionsData& src, SEChangeEnvironmentalConditions& dst)
{
  PBEnvironmentAction::Serialize(src, dst);
}
void PBEnvironmentAction::Serialize(const CDM_BIND::ChangeEnvironmentalConditionsData& src, SEChangeEnvironmentalConditions& dst)
{
  PBEnvironmentAction::Serialize(src.environmentaction(), dst);
  if (src.has_environmentalconditions())
    PBEnvironment::Load(src.environmentalconditions(), dst.GetEnvironmentalConditions());
  else
    dst.SetEnvironmentalConditionsFile(src.environmentalconditionsfile());
}
CDM_BIND::ChangeEnvironmentalConditionsData* PBEnvironmentAction::Unload(const SEChangeEnvironmentalConditions& src)
{
  CDM_BIND::ChangeEnvironmentalConditionsData* dst = new CDM_BIND::ChangeEnvironmentalConditionsData();
  PBEnvironmentAction::Serialize(src, *dst);
  return dst;
}
void PBEnvironmentAction::Serialize(const SEChangeEnvironmentalConditions& src, CDM_BIND::ChangeEnvironmentalConditionsData& dst)
{
  PBEnvironmentAction::Serialize(src, *dst.mutable_environmentaction());
  if (src.HasEnvironmentalConditions())
    dst.set_allocated_environmentalconditions(PBEnvironment::Unload(*src.m_EnvironmentalConditions));
  else if (src.HasEnvironmentalConditionsFile())
    dst.set_environmentalconditionsfile(src.m_EnvironmentalConditionsFile);
}
void PBEnvironmentAction::Copy(const SEChangeEnvironmentalConditions& src, SEChangeEnvironmentalConditions& dst)
{
  CDM_BIND::ChangeEnvironmentalConditionsData data;
  PBEnvironmentAction::Serialize(src, data);
  PBEnvironmentAction::Serialize(data, dst);
}

void PBEnvironmentAction::Load(const CDM_BIND::ThermalApplicationData& src, SEThermalApplication& dst)
{
  PBEnvironmentAction::Serialize(src, dst);
}
void PBEnvironmentAction::Serialize(const CDM_BIND::ThermalApplicationData& src, SEThermalApplication& dst)
{
  // Set this before our super class tells us to Clear if the action wants us to keep our current data
  dst.m_ClearContents = !src.appendtoprevious();
  PBEnvironmentAction::Serialize(src.environmentaction(), dst);
  if (src.has_activecooling())
    PBEnvironment::Load(src.activecooling(), dst.GetActiveCooling());
  if (src.has_activeheating())
    PBEnvironment::Load(src.activeheating(), dst.GetActiveHeating());
  if (src.has_appliedtemperature())
    PBEnvironment::Load(src.appliedtemperature(), dst.GetAppliedTemperature());
  dst.m_ClearContents = true;
}
CDM_BIND::ThermalApplicationData* PBEnvironmentAction::Unload(const SEThermalApplication& src)
{
  CDM_BIND::ThermalApplicationData* dst = new CDM_BIND::ThermalApplicationData();
  PBEnvironmentAction::Serialize(src, *dst);
  return dst;
}
void PBEnvironmentAction::Serialize(const SEThermalApplication& src, CDM_BIND::ThermalApplicationData& dst)
{
  PBEnvironmentAction::Serialize(src, *dst.mutable_environmentaction());
  if (src.HasActiveHeating())
    dst.set_allocated_activeheating(PBEnvironment::Unload(*src.m_ActiveHeating));
  if (src.HasActiveCooling())
    dst.set_allocated_activecooling(PBEnvironment::Unload(*src.m_ActiveCooling));
  if (src.HasAppliedTemperature())
    dst.set_allocated_appliedtemperature(PBEnvironment::Unload(*src.m_AppliedTemperature));
}
void PBEnvironmentAction::Copy(const SEThermalApplication& src, SEThermalApplication& dst)
{
  CDM_BIND::ThermalApplicationData data;
  PBEnvironmentAction::Serialize(src, data);
  PBEnvironmentAction::Serialize(data, dst);
}

SEEnvironmentAction* PBEnvironmentAction::Load(const CDM_BIND::AnyEnvironmentActionData& any, SESubstanceManager& subMgr)
{
  switch (any.Action_case())
  {
  case CDM_BIND::AnyEnvironmentActionData::ActionCase::kChangeEnvironmentalConditions:
  {
    SEChangeEnvironmentalConditions* a = new SEChangeEnvironmentalConditions(subMgr);
    PBEnvironmentAction::Load(any.changeenvironmentalconditions(), *a);
    return a;
  }
  case CDM_BIND::AnyEnvironmentActionData::ActionCase::kThermalApplication:
  {
    SEThermalApplication* a = new SEThermalApplication();
    PBEnvironmentAction::Load(any.thermalapplication(), *a);
    return a;
  }
  }
  subMgr.Error("Unknown action type : " + any.Action_case());
  return nullptr;
}
CDM_BIND::AnyEnvironmentActionData* PBEnvironmentAction::Unload(const SEEnvironmentAction& action)
{
  CDM_BIND::AnyEnvironmentActionData* any = new CDM_BIND::AnyEnvironmentActionData();
  const SEChangeEnvironmentalConditions* cec = dynamic_cast<const SEChangeEnvironmentalConditions*>(&action);
  if (cec != nullptr)
  {
    any->set_allocated_changeenvironmentalconditions(PBEnvironmentAction::Unload(*cec));
    return any;
  }
  const SEThermalApplication* ta = dynamic_cast<const SEThermalApplication*>(&action);
  if (ta != nullptr)
  {
    any->set_allocated_thermalapplication(PBEnvironmentAction::Unload(*ta));
    return any;
  }
  delete any;
  return nullptr;
}
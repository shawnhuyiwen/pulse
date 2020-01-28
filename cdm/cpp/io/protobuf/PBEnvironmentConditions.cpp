/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBEnvironmentConditions.h"
#include "io/protobuf/PBEnvironment.h"
#include "io/protobuf/PBConditions.h"
#include "bind/EnvironmentConditions.pb.h"
#include "system/environment/SEEnvironment.h"
#include "system/environment/conditions/SEInitialEnvironmentConditions.h"
#include "substance/SESubstanceManager.h"

void PBEnvironmentCondition::Serialize(const CDM_BIND::EnvironmentConditionData& src, SEEnvironmentCondition& dst)
{
  PBCondition::Serialize(src.condition(), dst);
}
void PBEnvironmentCondition::Serialize(const SEEnvironmentCondition& src, CDM_BIND::EnvironmentConditionData& dst)
{
  PBCondition::Serialize(src, *dst.mutable_condition());
}
SEEnvironmentCondition* PBEnvironmentCondition::Load(const CDM_BIND::AnyEnvironmentConditionData& any, SESubstanceManager& subMgr)
{
  switch (any.Condition_case())
  {
    case CDM_BIND::AnyEnvironmentConditionData::ConditionCase::kInitialEnvironmentConditions:
    {
      SEInitialEnvironmentConditions* a = new SEInitialEnvironmentConditions(subMgr);
      PBEnvironmentCondition::Load(any.initialenvironmentconditions(), *a);
      return a;
    }
  }
  subMgr.Error("Unknown action type : " + any.Condition_case());
  return nullptr;
}
CDM_BIND::AnyEnvironmentConditionData* PBEnvironmentCondition::Unload(const SEEnvironmentCondition& action)
{
  CDM_BIND::AnyEnvironmentConditionData* any = new CDM_BIND::AnyEnvironmentConditionData();
  const SEInitialEnvironmentConditions* cec = dynamic_cast<const SEInitialEnvironmentConditions*>(&action);
  if (cec != nullptr)
  {
    any->set_allocated_initialenvironmentconditions(PBEnvironmentCondition::Unload(*cec));
    return any;
  }
  delete any;
  return nullptr;
}

void PBEnvironmentCondition::Load(const CDM_BIND::InitialEnvironmentConditionsData& src, SEInitialEnvironmentConditions& dst)
{
  PBEnvironmentCondition::Serialize(src, dst);
}
void PBEnvironmentCondition::Serialize(const CDM_BIND::InitialEnvironmentConditionsData& src, SEInitialEnvironmentConditions& dst)
{
  PBEnvironmentCondition::Serialize(src.environmentcondition(), dst);
  if (src.has_conditions())
    PBEnvironment::Load(src.conditions(), dst.GetConditions());
  else
    dst.SetConditionsFile(src.conditionsfile());
}
CDM_BIND::InitialEnvironmentConditionsData* PBEnvironmentCondition::Unload(const SEInitialEnvironmentConditions& src)
{
  CDM_BIND::InitialEnvironmentConditionsData* dst = new CDM_BIND::InitialEnvironmentConditionsData();
  PBEnvironmentCondition::Serialize(src, *dst);
  return dst;
}
void PBEnvironmentCondition::Serialize(const SEInitialEnvironmentConditions& src, CDM_BIND::InitialEnvironmentConditionsData& dst)
{
  PBEnvironmentCondition::Serialize(src, *dst.mutable_environmentcondition());
  if (src.HasConditions())
    dst.set_allocated_conditions(PBEnvironment::Unload(*src.m_Conditions));
  else if (src.HasConditionsFile())
    dst.set_conditionsfile(src.m_ConditionsFile);
}
void PBEnvironmentCondition::Copy(const SEInitialEnvironmentConditions& src, SEInitialEnvironmentConditions& dst)
{
  CDM_BIND::InitialEnvironmentConditionsData data;
  PBEnvironmentCondition::Serialize(src, data);
  PBEnvironmentCondition::Serialize(data, dst);
}

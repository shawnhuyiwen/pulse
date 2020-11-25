/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/EnvironmentConditions.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBEnvironmentConditions.h"
#include "io/protobuf/PBEnvironment.h"
#include "io/protobuf/PBConditions.h"
#include "system/environment/SEEnvironment.h"
#include "system/environment/conditions/SEInitialEnvironmentalConditions.h"
#include "substance/SESubstanceManager.h"

void PBEnvironmentCondition::Serialize(const CDM_BIND::EnvironmentConditionData& src, SEEnvironmentCondition& dst)
{
  PBCondition::Serialize(src.condition(), dst);
}
void PBEnvironmentCondition::Serialize(const SEEnvironmentCondition& src, CDM_BIND::EnvironmentConditionData& dst)
{
  PBCondition::Serialize(src, *dst.mutable_condition());
}
SEEnvironmentCondition* PBEnvironmentCondition::Load(const CDM_BIND::AnyEnvironmentConditionData& any, const SESubstanceManager& subMgr)
{
  switch (any.Condition_case())
  {
    case CDM_BIND::AnyEnvironmentConditionData::ConditionCase::kInitialEnvironmentalConditions:
    {
      SEInitialEnvironmentalConditions* a = new SEInitialEnvironmentalConditions(subMgr.GetLogger());
      PBEnvironmentCondition::Load(any.initialenvironmentalconditions(), *a, subMgr);
      return a;
    }
  }
  subMgr.Error("Unknown action type : " + any.Condition_case());
  return nullptr;
}
CDM_BIND::AnyEnvironmentConditionData* PBEnvironmentCondition::Unload(const SEEnvironmentCondition& action)
{
  CDM_BIND::AnyEnvironmentConditionData* any = new CDM_BIND::AnyEnvironmentConditionData();
  const SEInitialEnvironmentalConditions* cec = dynamic_cast<const SEInitialEnvironmentalConditions*>(&action);
  if (cec != nullptr)
  {
    any->set_allocated_initialenvironmentalconditions(PBEnvironmentCondition::Unload(*cec));
    return any;
  }
  delete any;
  return nullptr;
}

void PBEnvironmentCondition::Load(const CDM_BIND::InitialEnvironmentalConditionsData& src, SEInitialEnvironmentalConditions& dst, const SESubstanceManager& subMgr)
{
  dst.Clear();
  PBEnvironmentCondition::Serialize(src, dst, subMgr);
}
void PBEnvironmentCondition::Serialize(const CDM_BIND::InitialEnvironmentalConditionsData& src, SEInitialEnvironmentalConditions& dst, const SESubstanceManager& subMgr)
{
  PBEnvironmentCondition::Serialize(src.environmentcondition(), dst);
  if (!src.environmentalconditionsfile().empty())
    dst.SetEnvironmentalConditionsFile(src.environmentalconditionsfile());
  else if(src.has_environmentalconditions())
    PBEnvironment::Load(src.environmentalconditions(), dst.GetEnvironmentalConditions(), subMgr);
}
CDM_BIND::InitialEnvironmentalConditionsData* PBEnvironmentCondition::Unload(const SEInitialEnvironmentalConditions& src)
{
  CDM_BIND::InitialEnvironmentalConditionsData* dst = new CDM_BIND::InitialEnvironmentalConditionsData();
  PBEnvironmentCondition::Serialize(src, *dst);
  return dst;
}
void PBEnvironmentCondition::Serialize(const SEInitialEnvironmentalConditions& src, CDM_BIND::InitialEnvironmentalConditionsData& dst)
{
  PBEnvironmentCondition::Serialize(src, *dst.mutable_environmentcondition());
  if (src.HasEnvironmentalConditionsFile())
    dst.set_environmentalconditionsfile(src.m_EnvironmentalConditionsFile);
  else if (src.HasEnvironmentalConditions())
    dst.set_allocated_environmentalconditions(PBEnvironment::Unload(*src.m_EnvironmentalConditions));
}
void PBEnvironmentCondition::Copy(const SEInitialEnvironmentalConditions& src, SEInitialEnvironmentalConditions& dst, const SESubstanceManager& subMgr)
{
  dst.Clear();
  CDM_BIND::InitialEnvironmentalConditionsData data;
  PBEnvironmentCondition::Serialize(src, data);
  PBEnvironmentCondition::Serialize(data, dst, subMgr);
}

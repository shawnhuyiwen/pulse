/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBConditions.h"
#include "io/protobuf/PBPatientConditions.h"
#include "io/protobuf/PBEnvironmentConditions.h"
#include "bind/cdm/Conditions.pb.h"
#include "bind/cdm/Scenario.pb.h"
#include "patient/conditions/SEPatientCondition.h"
#include "system/environment/conditions/SEEnvironmentCondition.h"
#include "substance/SESubstanceManager.h"


SECondition* PBCondition::Load(const cdm::AnyConditionData& condition, SESubstanceManager& subMgr)
{
  switch (condition.Condition_case())
  {
  case cdm::AnyConditionData::kPatientCondition:
    return PBPatientCondition::Load(condition.patientcondition(), subMgr);
  case cdm::AnyConditionData::kEnvironmentCondition:
    return PBEnvironmentCondition::Load(condition.environmentcondition(), subMgr);
  }
  subMgr.Error("Unknown Condition");
  return nullptr;
}
cdm::AnyConditionData* PBCondition::Unload(const SECondition& condition)
{
  cdm::AnyConditionData* any = new cdm::AnyConditionData();
  const SEPatientCondition* pc = dynamic_cast<const SEPatientCondition*>(&condition);
  if (pc != nullptr)
  {
    any->set_allocated_patientcondition(PBPatientCondition::Unload(*pc));
    return any;
  }
  const SEEnvironmentCondition* ec = dynamic_cast<const SEEnvironmentCondition*>(&condition);
  if (ec != nullptr)
  {
    any->set_allocated_environmentcondition(PBEnvironmentCondition::Unload(*ec));
    return any;
  }
  condition.Error("Unsupported Condition");
  delete any;
  return nullptr;
}

void PBCondition::Serialize(const cdm::ConditionData& src, SECondition& dst)
{
  dst.Clear();
  dst.SetComment(src.comment());
}
void PBCondition::Serialize(const SECondition& src, cdm::ConditionData& dst)
{
  dst.set_comment(src.m_Comment);
}
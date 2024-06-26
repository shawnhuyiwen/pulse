/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
PUSH_PROTO_WARNINGS
#include "pulse/cdm/bind/Conditions.pb.h"
#include "pulse/cdm/bind/Scenario.pb.h"
POP_PROTO_WARNINGS
#include "cdm/io/protobuf/PBConditions.h"
#include "cdm/io/protobuf/PBPatientConditions.h"
#include "cdm/io/protobuf/PBEnvironmentConditions.h"
#include "cdm/io/protobuf/PBUtils.h"
#include "cdm/patient/conditions/SEPatientCondition.h"
#include "cdm/system/environment/conditions/SEEnvironmentCondition.h"
#include "cdm/substance/SESubstanceManager.h"


SECondition* PBCondition::Load(const CDM_BIND::AnyConditionData& condition, const SESubstanceManager& subMgr)
{
  switch (condition.Condition_case())
  {
  case CDM_BIND::AnyConditionData::kPatientCondition:
    return PBPatientCondition::Load(condition.patientcondition(), subMgr);
  case CDM_BIND::AnyConditionData::kEnvironmentCondition:
    return PBEnvironmentCondition::Load(condition.environmentcondition(), subMgr);
  case CDM_BIND::AnyConditionData::CONDITION_NOT_SET:
    subMgr.Warning("AnyConditionData is empty...was that intended?");
    return nullptr;
  }
  subMgr.Error("Unknown Condition");
  return nullptr;
}
CDM_BIND::AnyConditionData* PBCondition::Unload(const SECondition& condition)
{
  CDM_BIND::AnyConditionData* any = new CDM_BIND::AnyConditionData();
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

void PBCondition::Serialize(const CDM_BIND::ConditionData& src, SECondition& dst)
{
  dst.SetComment(src.comment());
}
void PBCondition::Serialize(const SECondition& src, CDM_BIND::ConditionData& dst)
{
  dst.set_comment(src.m_Comment);
}

bool PBCondition::SerializeToString(const SECondition& a, std::string& dst, eSerializationFormat fmt)
{
  auto* bind = Unload(a);
  return PBUtils::SerializeToString(*bind, dst, fmt, nullptr);
}
SECondition* PBCondition::SerializeFromString(const std::string& src, eSerializationFormat fmt, const SESubstanceManager& subMgr)
{
  CDM_BIND::AnyConditionData bind;
  PBUtils::SerializeFromString(src, bind, fmt, subMgr.GetLogger());
  return PBCondition::Load(bind, subMgr);
}

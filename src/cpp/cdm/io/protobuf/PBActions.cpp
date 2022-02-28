/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
PUSH_PROTO_WARNINGS
#include "pulse/cdm/bind/Actions.pb.h"
#include "pulse/cdm/bind/Scenario.pb.h"
POP_PROTO_WARNINGS
#include "cdm/io/protobuf/PBActions.h"
#include "cdm/io/protobuf/PBPatientActions.h"
#include "cdm/io/protobuf/PBEnvironmentActions.h"
#include "cdm/io/protobuf/PBEquipmentActions.h"
#include "cdm/io/protobuf/PBProperties.h"
#include "cdm/patient/actions/SEPatientAction.h"
#include "cdm/system/environment/actions/SEEnvironmentAction.h"
#include "cdm/system/equipment/SEEquipmentAction.h"
#include "cdm/engine/SEAdvanceTime.h"
#include "cdm/engine/SESerializeState.h"
#include "cdm/engine/SEOverrides.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/properties/SEScalarTime.h"

const std::string& eAppliedRespiratoryCycle_Name(eAppliedRespiratoryCycle c)
{
  return CDM_BIND::eAppliedRespiratoryCycle_Name((CDM_BIND::eAppliedRespiratoryCycle)c);
}

const std::string& eMergeType_Name(eMergeType m)
{
  return CDM_BIND::eMergeType_Name((CDM_BIND::eMergeType)m);
}

SEAction* PBAction::Load(const CDM_BIND::AnyActionData& action, const SESubstanceManager& subMgr)
{
  switch (action.Action_case())
  {
  case CDM_BIND::AnyActionData::kPatientAction:
    return PBPatientAction::Load(action.patientaction(), subMgr);
  case CDM_BIND::AnyActionData::kEnvironmentAction:
    return PBEnvironmentAction::Load(action.environmentaction(), subMgr);
  case CDM_BIND::AnyActionData::kEquipmentAction:
    return PBEquipmentAction::Load(action.equipmentaction(), subMgr);
  case CDM_BIND::AnyActionData::kAdvanceTime:
  {
    SEAdvanceTime* a = new SEAdvanceTime();
    PBAction::Load(action.advancetime(), *a);
    return a;
  }
  case CDM_BIND::AnyActionData::kSerialize:
  {
    SESerializeState* a = new SESerializeState();
    PBAction::Load(action.serialize(), *a);
    return a;
  }
  case CDM_BIND::AnyActionData::kOverrides:
  {
    SEOverrides* a = new SEOverrides();
    PBAction::Load(action.overrides(), *a);
    return a;
  }
  }
  subMgr.Error("Unknown Action");
  return nullptr;
}
CDM_BIND::AnyActionData* PBAction::Unload(const SEAction& action)
{
  CDM_BIND::AnyActionData* any = new CDM_BIND::AnyActionData();

  const SEAdvanceTime* adv = dynamic_cast<const SEAdvanceTime*>(&action);
  if (adv != nullptr)
  {
    any->set_allocated_advancetime(PBAction::Unload(*adv));
    return any;
  }
  const SESerializeState* ss = dynamic_cast<const SESerializeState*>(&action);
  if (ss != nullptr)
  {
    any->set_allocated_serialize(PBAction::Unload(*ss));
    return any;
  }
  const SEOverrides* o = dynamic_cast<const SEOverrides*>(&action);
  if (o != nullptr)
  {
    any->set_allocated_overrides(PBAction::Unload(*o));
    return any;
  }

  const SEPatientAction* pa = dynamic_cast<const SEPatientAction*>(&action);
  if (pa != nullptr)
  {
    any->set_allocated_patientaction(PBPatientAction::Unload(*pa));
    return any;
  }
  const SEEnvironmentAction* ea = dynamic_cast<const SEEnvironmentAction*>(&action);
  if (ea != nullptr)
  {
    any->set_allocated_environmentaction(PBEnvironmentAction::Unload(*ea));
    return any;
  }
  const SEEquipmentAction* eqa = dynamic_cast<const SEEquipmentAction*>(&action);
  if (eqa != nullptr)
  {
    any->set_allocated_equipmentaction(PBEquipmentAction::Unload(*eqa));
    return any;
  }
  action.Error("Unsupported Action");
  delete any;
  return nullptr;
}
SEAction* PBAction::Copy(const SEAction& a, const SESubstanceManager& subMgr)
{
  CDM_BIND::AnyActionData* data = PBAction::Unload(a);
  SEAction* copy = PBAction::Load(*data, subMgr);
  delete data;
  return copy;
}

void PBAction::Serialize(const CDM_BIND::ActionData& src, SEAction& dst)
{
  dst.SetComment(src.comment());
}
void PBAction::Serialize(const SEAction& src, CDM_BIND::ActionData& dst)
{
  dst.set_comment(src.m_Comment);
}

void PBAction::Load(const CDM_BIND::AdvanceTimeData& src, SEAdvanceTime& dst)
{
  dst.Clear();
  PBAction::Serialize(src, dst);
}
void PBAction::Serialize(const CDM_BIND::AdvanceTimeData& src, SEAdvanceTime& dst)
{
  PBAction::Serialize(src.action(), dst);
  if (src.has_time())
    PBProperty::Load(src.time(), dst.GetTime());
}
CDM_BIND::AdvanceTimeData* PBAction::Unload(const SEAdvanceTime& src)
{
  CDM_BIND::AdvanceTimeData* dst = new CDM_BIND::AdvanceTimeData();
  PBAction::Serialize(src, *dst);
  return dst;
}
void PBAction::Serialize(const SEAdvanceTime& src, CDM_BIND::AdvanceTimeData& dst)
{
  PBAction::Serialize(src, *dst.mutable_action());
  if (src.HasTime())
    dst.set_allocated_time(PBProperty::Unload(*src.m_Time));
}

void PBAction::Load(const CDM_BIND::SerializeStateData& src, SESerializeState& dst)
{
  dst.Clear();
  PBAction::Serialize(src, dst);
}
void PBAction::Serialize(const CDM_BIND::SerializeStateData& src, SESerializeState& dst)
{
  PBAction::Serialize(src.action(), dst);
  dst.SetType((eSerialization_Type)src.type());
  dst.SetFilename(src.filename());
}
CDM_BIND::SerializeStateData* PBAction::Unload(const SESerializeState& src)
{
  CDM_BIND::SerializeStateData* dst = new CDM_BIND::SerializeStateData();
  PBAction::Serialize(src, *dst);
  return dst;
}
void PBAction::Serialize(const SESerializeState& src, CDM_BIND::SerializeStateData& dst)
{
  PBAction::Serialize(src, *dst.mutable_action());

  dst.set_type((CDM_BIND::SerializeStateData::eType)src.m_Type);
  if (src.HasFilename())
    dst.set_filename(src.m_Filename);
}

void PBAction::Load(const CDM_BIND::OverridesData& src, SEOverrides& dst)
{
  dst.Clear();
  PBAction::Serialize(src, dst);
}
void PBAction::Serialize(const CDM_BIND::OverridesData& src, SEOverrides& dst)
{
  PBAction::Serialize(src.action(), dst);
  for (auto const& [name, o] : src.scalarproperties())
    dst.GetScalarProperties()[name] = SEScalarPair(o.value(), o.unit());
}
CDM_BIND::OverridesData* PBAction::Unload(const SEOverrides& src)
{
  CDM_BIND::OverridesData* dst = new CDM_BIND::OverridesData();
  PBAction::Serialize(src, *dst);
  return dst;
}
void PBAction::Serialize(const SEOverrides& src, CDM_BIND::OverridesData& dst)
{
  PBAction::Serialize(src, *dst.mutable_action());
  for (auto& [name, o] : src.GetScalarProperties())
  {
    CDM_BIND::ScalarData& scalar = 
      (*dst.mutable_scalarproperties())[name];
    scalar.set_value(o.value);
    scalar.set_unit(o.unit);
  }
}
void PBAction::Copy(const SEOverrides& src, SEOverrides& dst)
{
  dst.Clear();
  CDM_BIND::OverridesData data;
  PBAction::Serialize(src, data);
  PBAction::Serialize(data, dst);
}

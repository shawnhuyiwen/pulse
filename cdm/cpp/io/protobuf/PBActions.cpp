/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBActions.h"
#include "io/protobuf/PBPatientActions.h"
#include "io/protobuf/PBEnvironmentActions.h"
#include "io/protobuf/PBAnesthesiaMachineActions.h"
#include "io/protobuf/PBInhalerActions.h"
#include "io/protobuf/PBProperties.h"
#include "bind/cpp/cdm/Actions.pb.h"
#include "bind/cpp/cdm/Scenario.pb.h"
#include "patient/actions/SEPatientAction.h"
#include "system/environment/actions/SEEnvironmentAction.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineAction.h"
#include "system/equipment/inhaler/actions/SEInhalerAction.h"
#include "engine/SEAdvanceTime.h"
#include "engine/SESerializeState.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalarTime.h"

SEAction* PBAction::Load(const cdm::AnyActionData& action, SESubstanceManager& subMgr)
{
  switch (action.Action_case())
  {
  case cdm::AnyActionData::kPatientAction:
    return PBPatientAction::Load(action.patientaction(), subMgr);
  case cdm::AnyActionData::kEnvironmentAction:
    return PBEnvironmentAction::Load(action.environmentaction(), subMgr);
  case cdm::AnyActionData::kAnesthesiaMachineAction:
    return PBAnesthesiaMachineAction::Load(action.anesthesiamachineaction(), subMgr);
  case cdm::AnyActionData::kInhalerAction:
    return PBInhalerAction::Load(action.inhaleraction(), subMgr);
  case cdm::AnyActionData::kAdvanceTime:
  {
    SEAdvanceTime* a = new SEAdvanceTime();
    PBAction::Load(action.advancetime(), *a);
    return a;
  }
  case cdm::AnyActionData::kSerialize:
  {
    SESerializeState* a = new SESerializeState();
    PBAction::Load(action.serialize(), *a);
    return a;
  }
  }
  subMgr.Error("Unknown Action");
  return nullptr;
}
cdm::AnyActionData* PBAction::Unload(const SEAction& action)
{
  cdm::AnyActionData* any = new cdm::AnyActionData();

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
  const SEAnesthesiaMachineAction* aa = dynamic_cast<const SEAnesthesiaMachineAction*>(&action);
  if (aa != nullptr)
  {
    any->set_allocated_anesthesiamachineaction(PBAnesthesiaMachineAction::Unload(*aa));
    return any;
  }
  const SEInhalerAction* ia = dynamic_cast<const SEInhalerAction*>(&action);
  if (ia != nullptr)
  {
    any->set_allocated_inhaleraction(PBInhalerAction::Unload(*ia));
    return any;
  }
  action.Error("Unsupported Action");
  delete any;
  return nullptr;
}
SEAction* PBAction::Copy(const SEAction& a, SESubstanceManager& subMgr)
{
  cdm::AnyActionData* data = PBAction::Unload(a);
  SEAction* copy = PBAction::Load(*data, subMgr);
  delete data;
  return copy;
}

void PBAction::Serialize(const cdm::ActionData& src, SEAction& dst)
{
  dst.Clear();
  dst.SetComment(src.comment());
}
void PBAction::Serialize(const SEAction& src, cdm::ActionData& dst)
{
  dst.set_comment(src.m_Comment);
}

void PBAction::Load(const cdm::AdvanceTimeData& src, SEAdvanceTime& dst)
{
  PBAction::Serialize(src, dst);
}
void PBAction::Serialize(const cdm::AdvanceTimeData& src, SEAdvanceTime& dst)
{
  dst.Clear();
  if (src.has_time())
    PBProperty::Load(src.time(), dst.GetTime());
}
cdm::AdvanceTimeData* PBAction::Unload(const SEAdvanceTime& src)
{
  cdm::AdvanceTimeData* dst = new cdm::AdvanceTimeData();
  PBAction::Serialize(src, *dst);
  return dst;
}
void PBAction::Serialize(const SEAdvanceTime& src, cdm::AdvanceTimeData& dst)
{
  if (src.HasTime())
    dst.set_allocated_time(PBProperty::Unload(*src.m_Time));
}

void PBAction::Load(const cdm::SerializeStateData& src, SESerializeState& dst)
{
  PBAction::Serialize(src, dst);
}
void PBAction::Serialize(const cdm::SerializeStateData& src, SESerializeState& dst)
{
  dst.Clear();
  dst.SetType((eSerialization_Type)src.type());
  dst.SetFilename(src.filename());
}
cdm::SerializeStateData* PBAction::Unload(const SESerializeState& src)
{
  cdm::SerializeStateData* dst = new cdm::SerializeStateData();
  PBAction::Serialize(src, *dst);
  return dst;
}
void PBAction::Serialize(const SESerializeState& src, cdm::SerializeStateData& dst)
{
  dst.set_type((cdm::eSerialization_Type)src.m_Type);
  if (src.HasFilename())
    dst.set_filename(src.m_Filename);
}
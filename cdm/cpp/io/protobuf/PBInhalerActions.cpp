/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBInhalerActions.h"
#include "io/protobuf/PBInhaler.h"
#include "io/protobuf/PBActions.h"
#include "bind/cpp/cdm/InhalerActions.pb.h"
#include "system/equipment/inhaler/actions/SEInhalerConfiguration.h"
#include "substance/SESubstanceManager.h"

void PBInhalerAction::Serialize(const cdm::InhalerActionData& src, SEInhalerAction& dst)
{
  PBAction::Serialize(src.action(), dst);
}
void PBInhalerAction::Serialize(const SEInhalerAction& src, cdm::InhalerActionData& dst)
{
  PBAction::Serialize(src, *dst.mutable_action());
}

void PBInhalerAction::Load(const cdm::InhalerConfigurationData& src, SEInhalerConfiguration& dst)
{
  PBInhalerAction::Serialize(src, dst);
}
void PBInhalerAction::Serialize(const cdm::InhalerConfigurationData& src, SEInhalerConfiguration& dst)
{
  PBInhalerAction::Serialize(src.inhaleraction(), dst);
  if (src.has_configuration())
    PBInhaler::Load(src.configuration(), dst.GetConfiguration());
  else
    dst.SetConfigurationFile(src.configurationfile());
}
cdm::InhalerConfigurationData* PBInhalerAction::Unload(const SEInhalerConfiguration& src)
{
  cdm::InhalerConfigurationData* dst = new cdm::InhalerConfigurationData();
  PBInhalerAction::Serialize(src, *dst);
  return dst;
}
void PBInhalerAction::Serialize(const SEInhalerConfiguration& src, cdm::InhalerConfigurationData& dst)
{
  PBInhalerAction::Serialize(src, *dst.mutable_inhaleraction());
  if (src.HasConfiguration())
    dst.set_allocated_configuration(PBInhaler::Unload(*src.m_Configuration));
  else if (src.HasConfigurationFile())
    dst.set_configurationfile(src.m_ConfigurationFile);
}
void PBInhalerAction::Copy(const SEInhalerConfiguration& src, SEInhalerConfiguration& dst)
{
  cdm::InhalerConfigurationData data;
  PBInhalerAction::Serialize(src, data);
  PBInhalerAction::Serialize(data, dst);
}

SEInhalerAction* PBInhalerAction::Load(const cdm::AnyInhalerActionData& any, SESubstanceManager& subMgr)
{
  switch (any.Action_case())
  {
    case cdm::AnyInhalerActionData::ActionCase::kConfiguration:
    {
      SEInhalerConfiguration* a = new SEInhalerConfiguration(subMgr);
      PBInhalerAction::Load(any.configuration(), *a);
      return a;
    }
  }
  subMgr.Error("Unknown action type : " + any.Action_case());
  return nullptr;
}
cdm::AnyInhalerActionData* PBInhalerAction::Unload(const SEInhalerAction& action)
{
  cdm::AnyInhalerActionData* any = new cdm::AnyInhalerActionData();
  const SEInhalerConfiguration* cec = dynamic_cast<const SEInhalerConfiguration*>(&action);
  if (cec != nullptr)
  {
    any->set_allocated_configuration(PBInhalerAction::Unload(*cec));
    return any;
  }
  delete any;
  return nullptr;
}
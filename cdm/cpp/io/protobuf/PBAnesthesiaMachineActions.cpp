/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBAnesthesiaMachineActions.h"
#include "io/protobuf/PBAnesthesiaMachine.h"
#include "io/protobuf/PBActions.h"
#include "io/protobuf/PBProperties.h"
#include "bind/cdm/AnesthesiaMachineActions.pb.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineAction.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineConfiguration.h"
#include "system/equipment/anesthesiamachine/actions/SEOxygenWallPortPressureLoss.h"
#include "system/equipment/anesthesiamachine/actions/SEOxygenTankPressureLoss.h"
#include "system/equipment/anesthesiamachine/actions/SEExpiratoryValveLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEExpiratoryValveObstruction.h"
#include "system/equipment/anesthesiamachine/actions/SEInspiratoryValveLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEInspiratoryValveObstruction.h"
#include "system/equipment/anesthesiamachine/actions/SEMaskLeak.h"
#include "system/equipment/anesthesiamachine/actions/SESodaLimeFailure.h"
#include "system/equipment/anesthesiamachine/actions/SETubeCuffLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEVaporizerFailure.h"
#include "system/equipment/anesthesiamachine/actions/SEVentilatorPressureLoss.h"
#include "system/equipment/anesthesiamachine/actions/SEYPieceDisconnect.h"
#include "substance/SESubstanceManager.h"

void PBAnesthesiaMachineAction::Serialize(const cdm::AnesthesiaMachineActionData& src, SEAnesthesiaMachineAction& dst)
{
  PBAction::Serialize(src.action(), dst);
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineAction& src, cdm::AnesthesiaMachineActionData& dst)
{
  PBAction::Serialize(src, *dst.mutable_action());
}

void PBAnesthesiaMachineAction::Load(const cdm::AnesthesiaMachineConfigurationData& src, SEAnesthesiaMachineConfiguration& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::AnesthesiaMachineConfigurationData& src, SEAnesthesiaMachineConfiguration& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_configuration())
    PBAnesthesiaMachine::Load(src.configuration(), dst.GetConfiguration());
  else
    dst.SetConfigurationFile(src.configurationfile());
}
cdm::AnesthesiaMachineConfigurationData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineConfiguration& src)
{
  cdm::AnesthesiaMachineConfigurationData* dst = new cdm::AnesthesiaMachineConfigurationData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineConfiguration& src, cdm::AnesthesiaMachineConfigurationData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasConfiguration())
    dst.set_allocated_configuration(PBAnesthesiaMachine::Unload(*src.m_Configuration));
  else if (src.HasConfigurationFile())
    dst.set_configurationfile(src.m_ConfigurationFile);
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineConfiguration& src, SEAnesthesiaMachineConfiguration& dst)
{
  cdm::AnesthesiaMachineConfigurationData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::ExpiratoryValveLeakData& src, SEExpiratoryValveLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::ExpiratoryValveLeakData& src, SEExpiratoryValveLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::ExpiratoryValveLeakData* PBAnesthesiaMachineAction::Unload(const SEExpiratoryValveLeak& src)
{
  cdm::ExpiratoryValveLeakData* dst = new cdm::ExpiratoryValveLeakData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEExpiratoryValveLeak& src, cdm::ExpiratoryValveLeakData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEExpiratoryValveLeak& src, SEExpiratoryValveLeak& dst)
{
  cdm::ExpiratoryValveLeakData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::ExpiratoryValveObstructionData& src, SEExpiratoryValveObstruction& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::ExpiratoryValveObstructionData& src, SEExpiratoryValveObstruction& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::ExpiratoryValveObstructionData* PBAnesthesiaMachineAction::Unload(const SEExpiratoryValveObstruction& src)
{
  cdm::ExpiratoryValveObstructionData* dst = new cdm::ExpiratoryValveObstructionData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEExpiratoryValveObstruction& src, cdm::ExpiratoryValveObstructionData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEExpiratoryValveObstruction& src, SEExpiratoryValveObstruction& dst)
{
  cdm::ExpiratoryValveObstructionData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::InspiratoryValveLeakData& src, SEInspiratoryValveLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::InspiratoryValveLeakData& src, SEInspiratoryValveLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::InspiratoryValveLeakData* PBAnesthesiaMachineAction::Unload(const SEInspiratoryValveLeak& src)
{
  cdm::InspiratoryValveLeakData* dst = new cdm::InspiratoryValveLeakData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEInspiratoryValveLeak& src, cdm::InspiratoryValveLeakData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEInspiratoryValveLeak& src, SEInspiratoryValveLeak& dst)
{
  cdm::InspiratoryValveLeakData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::InspiratoryValveObstructionData& src, SEInspiratoryValveObstruction& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::InspiratoryValveObstructionData& src, SEInspiratoryValveObstruction& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::InspiratoryValveObstructionData* PBAnesthesiaMachineAction::Unload(const SEInspiratoryValveObstruction& src)
{
  cdm::InspiratoryValveObstructionData* dst = new cdm::InspiratoryValveObstructionData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEInspiratoryValveObstruction& src, cdm::InspiratoryValveObstructionData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEInspiratoryValveObstruction& src, SEInspiratoryValveObstruction& dst)
{
  cdm::InspiratoryValveObstructionData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::MaskLeakData& src, SEMaskLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::MaskLeakData& src, SEMaskLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::MaskLeakData* PBAnesthesiaMachineAction::Unload(const SEMaskLeak& src)
{
  cdm::MaskLeakData* dst = new cdm::MaskLeakData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEMaskLeak& src, cdm::MaskLeakData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEMaskLeak& src, SEMaskLeak& dst)
{
  cdm::MaskLeakData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::OxygenTankPressureLossData& src, SEOxygenTankPressureLoss& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::OxygenTankPressureLossData& src, SEOxygenTankPressureLoss& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  dst.SetActive(src.state() == cdm::eSwitch::On ? true : false);
}
cdm::OxygenTankPressureLossData* PBAnesthesiaMachineAction::Unload(const SEOxygenTankPressureLoss& src)
{
  cdm::OxygenTankPressureLossData* dst = new cdm::OxygenTankPressureLossData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEOxygenTankPressureLoss& src, cdm::OxygenTankPressureLossData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  dst.set_state(src.IsActive() ? cdm::eSwitch::On : cdm::eSwitch::Off);
}
void PBAnesthesiaMachineAction::Copy(const SEOxygenTankPressureLoss& src, SEOxygenTankPressureLoss& dst)
{
  cdm::OxygenTankPressureLossData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::OxygenWallPortPressureLossData& src, SEOxygenWallPortPressureLoss& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::OxygenWallPortPressureLossData& src, SEOxygenWallPortPressureLoss& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  dst.SetActive(src.state() == cdm::eSwitch::On ? true : false);
}
cdm::OxygenWallPortPressureLossData* PBAnesthesiaMachineAction::Unload(const SEOxygenWallPortPressureLoss& src)
{
  cdm::OxygenWallPortPressureLossData* dst = new cdm::OxygenWallPortPressureLossData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEOxygenWallPortPressureLoss& src, cdm::OxygenWallPortPressureLossData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  dst.set_state(src.IsActive() ? cdm::eSwitch::On : cdm::eSwitch::Off);
}
void PBAnesthesiaMachineAction::Copy(const SEOxygenWallPortPressureLoss& src, SEOxygenWallPortPressureLoss& dst)
{
  cdm::OxygenWallPortPressureLossData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::SodaLimeFailureData& src, SESodaLimeFailure& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::SodaLimeFailureData& src, SESodaLimeFailure& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::SodaLimeFailureData* PBAnesthesiaMachineAction::Unload(const SESodaLimeFailure& src)
{
  cdm::SodaLimeFailureData* dst = new cdm::SodaLimeFailureData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SESodaLimeFailure& src, cdm::SodaLimeFailureData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SESodaLimeFailure& src, SESodaLimeFailure& dst)
{
  cdm::SodaLimeFailureData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::TubeCuffLeakData& src, SETubeCuffLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::TubeCuffLeakData& src, SETubeCuffLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::TubeCuffLeakData* PBAnesthesiaMachineAction::Unload(const SETubeCuffLeak& src)
{
  cdm::TubeCuffLeakData* dst = new cdm::TubeCuffLeakData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SETubeCuffLeak& src, cdm::TubeCuffLeakData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SETubeCuffLeak& src, SETubeCuffLeak& dst)
{
  cdm::TubeCuffLeakData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::VaporizerFailureData& src, SEVaporizerFailure& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::VaporizerFailureData& src, SEVaporizerFailure& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::VaporizerFailureData* PBAnesthesiaMachineAction::Unload(const SEVaporizerFailure& src)
{
  cdm::VaporizerFailureData* dst = new cdm::VaporizerFailureData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEVaporizerFailure& src, cdm::VaporizerFailureData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEVaporizerFailure& src, SEVaporizerFailure& dst)
{
  cdm::VaporizerFailureData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::VentilatorPressureLossData& src, SEVentilatorPressureLoss& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::VentilatorPressureLossData& src, SEVentilatorPressureLoss& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::VentilatorPressureLossData* PBAnesthesiaMachineAction::Unload(const SEVentilatorPressureLoss& src)
{
  cdm::VentilatorPressureLossData* dst = new cdm::VentilatorPressureLossData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEVentilatorPressureLoss& src, cdm::VentilatorPressureLossData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEVentilatorPressureLoss& src, SEVentilatorPressureLoss& dst)
{
  cdm::VentilatorPressureLossData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::YPieceDisconnectData& src, SEYPieceDisconnect& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::YPieceDisconnectData& src, SEYPieceDisconnect& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::YPieceDisconnectData* PBAnesthesiaMachineAction::Unload(const SEYPieceDisconnect& src)
{
  cdm::YPieceDisconnectData* dst = new cdm::YPieceDisconnectData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEYPieceDisconnect& src, cdm::YPieceDisconnectData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEYPieceDisconnect& src, SEYPieceDisconnect& dst)
{
  cdm::YPieceDisconnectData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

SEAnesthesiaMachineAction* PBAnesthesiaMachineAction::Load(const cdm::AnyAnesthesiaMachineActionData& any, SESubstanceManager& subMgr)
{
  switch (any.Action_case())
  {
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kConfiguration:
  {
    SEAnesthesiaMachineConfiguration* a = new SEAnesthesiaMachineConfiguration(subMgr);
    PBAnesthesiaMachineAction::Load(any.configuration(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kOxygenTankPressureLoss:
  {
    SEOxygenTankPressureLoss* a = new SEOxygenTankPressureLoss();
    PBAnesthesiaMachineAction::Load(any.oxygentankpressureloss(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kOxygenWallPortPressureLoss:
  {
    SEOxygenWallPortPressureLoss* a = new SEOxygenWallPortPressureLoss();
    PBAnesthesiaMachineAction::Load(any.oxygenwallportpressureloss(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kExpiratoryValveLeak:
  {
    SEExpiratoryValveLeak* a = new SEExpiratoryValveLeak();
    PBAnesthesiaMachineAction::Load(any.expiratoryvalveleak(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kExpiratoryValveObstruction:
  {
    SEExpiratoryValveObstruction* a = new SEExpiratoryValveObstruction();
    PBAnesthesiaMachineAction::Load(any.expiratoryvalveobstruction(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kInspiratoryValveLeak:
  {
    SEInspiratoryValveLeak* a = new SEInspiratoryValveLeak();
    PBAnesthesiaMachineAction::Load(any.inspiratoryvalveleak(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kInspiratoryValveObstruction:
  {
    SEInspiratoryValveObstruction* a = new SEInspiratoryValveObstruction();
    PBAnesthesiaMachineAction::Load(any.inspiratoryvalveobstruction(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kMaskLeak:
  {
    SEMaskLeak* a = new SEMaskLeak();
    PBAnesthesiaMachineAction::Load(any.maskleak(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kSodaLimeFailure:
  {
    SESodaLimeFailure* a = new SESodaLimeFailure();
    PBAnesthesiaMachineAction::Load(any.sodalimefailure(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kTubeCuffLeak:
  {
    SETubeCuffLeak* a = new SETubeCuffLeak();
    PBAnesthesiaMachineAction::Load(any.tubecuffleak(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kVaporizerFailure:
  {
    SEVaporizerFailure* a = new SEVaporizerFailure();
    PBAnesthesiaMachineAction::Load(any.vaporizerfailure(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kVentilatorPressureLoss:
  {
    SEVentilatorPressureLoss* a = new SEVentilatorPressureLoss();
    PBAnesthesiaMachineAction::Load(any.ventilatorpressureloss(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kYPieceDisconnect:
  {
    SEYPieceDisconnect* a = new SEYPieceDisconnect();
    PBAnesthesiaMachineAction::Load(any.ypiecedisconnect(), *a);
    return a;
  }
  }
  subMgr.Error("Unknown action type : " + any.Action_case());
  return nullptr;
}
cdm::AnyAnesthesiaMachineActionData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineAction& action)
{
  cdm::AnyAnesthesiaMachineActionData* any = new cdm::AnyAnesthesiaMachineActionData();
  const SEAnesthesiaMachineConfiguration* amc = dynamic_cast<const SEAnesthesiaMachineConfiguration*>(&action);
  if (amc != nullptr)
  {
    any->set_allocated_configuration(PBAnesthesiaMachineAction::Unload(*amc));
    return any;
  }

  const SEOxygenWallPortPressureLoss* owppl = dynamic_cast<const SEOxygenWallPortPressureLoss*>(&action);
  if (owppl != nullptr)
  {
    any->set_allocated_oxygenwallportpressureloss(PBAnesthesiaMachineAction::Unload(*owppl));
    return any;
  }

  const SEOxygenTankPressureLoss* otpl = dynamic_cast<const SEOxygenTankPressureLoss*>(&action);
  if (otpl != nullptr)
  {
    any->set_allocated_oxygentankpressureloss(PBAnesthesiaMachineAction::Unload(*otpl));
    return any;
  }

  const SEExpiratoryValveLeak* evl = dynamic_cast<const SEExpiratoryValveLeak*>(&action);
  if (evl != nullptr)
  {
    any->set_allocated_expiratoryvalveleak(PBAnesthesiaMachineAction::Unload(*evl));
    return any;
  }

  const SEExpiratoryValveObstruction* evo = dynamic_cast<const SEExpiratoryValveObstruction*>(&action);
  if (evo != nullptr)
  {
    any->set_allocated_expiratoryvalveobstruction(PBAnesthesiaMachineAction::Unload(*evo));
    return any;
  }

  const SEInspiratoryValveLeak* ivl = dynamic_cast<const SEInspiratoryValveLeak*>(&action);
  if (ivl != nullptr)
  {
    any->set_allocated_inspiratoryvalveleak(PBAnesthesiaMachineAction::Unload(*ivl));
    return any;
  }

  const SEInspiratoryValveObstruction* ivo = dynamic_cast<const SEInspiratoryValveObstruction*>(&action);
  if (ivo != nullptr)
  {
    any->set_allocated_inspiratoryvalveobstruction(PBAnesthesiaMachineAction::Unload(*ivo));
    return any;
  }

  const SEMaskLeak* ml = dynamic_cast<const SEMaskLeak*>(&action);
  if (ml != nullptr)
  {
    any->set_allocated_maskleak(PBAnesthesiaMachineAction::Unload(*ml));
    return any;
  }

  const SESodaLimeFailure* slf = dynamic_cast<const SESodaLimeFailure*>(&action);
  if (slf != nullptr)
  {
    any->set_allocated_sodalimefailure(PBAnesthesiaMachineAction::Unload(*slf));
    return any;
  }

  const SETubeCuffLeak* tcl = dynamic_cast<const SETubeCuffLeak*>(&action);
  if (tcl != nullptr)
  {
    any->set_allocated_tubecuffleak(PBAnesthesiaMachineAction::Unload(*tcl));
    return any;
  }

  const SEVaporizerFailure* vf = dynamic_cast<const SEVaporizerFailure*>(&action);
  if (vf != nullptr)
  {
    any->set_allocated_vaporizerfailure(PBAnesthesiaMachineAction::Unload(*vf));
    return any;
  }

  const SEVentilatorPressureLoss* vpl = dynamic_cast<const SEVentilatorPressureLoss*>(&action);
  if (vpl != nullptr)
  {
    any->set_allocated_ventilatorpressureloss(PBAnesthesiaMachineAction::Unload(*vpl));
    return any;
  }

  const SEYPieceDisconnect* ypd = dynamic_cast<const SEYPieceDisconnect*>(&action);
  if (ypd != nullptr)
  {
    any->set_allocated_ypiecedisconnect(PBAnesthesiaMachineAction::Unload(*ypd));
    return any;
  }
  delete any;
  return nullptr;
}
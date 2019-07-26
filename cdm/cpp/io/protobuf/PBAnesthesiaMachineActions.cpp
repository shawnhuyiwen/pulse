/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBAnesthesiaMachineActions.h"
#include "io/protobuf/PBAnesthesiaMachine.h"
#include "io/protobuf/PBActions.h"
#include "io/protobuf/PBProperties.h"
#include "bind/cpp/cdm/AnesthesiaMachineActions.pb.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineAction.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineConfiguration.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineOxygenWallPortPressureLoss.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineOxygenTankPressureLoss.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineExpiratoryValveLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineExpiratoryValveObstruction.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineInspiratoryValveLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineInspiratoryValveObstruction.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineMaskLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineSodaLimeFailure.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineTubeCuffLeak.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineVaporizerFailure.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineVentilatorPressureLoss.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineYPieceDisconnect.h"
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

void PBAnesthesiaMachineAction::Load(const cdm::AnesthesiaMachineExpiratoryValveLeakData& src, SEAnesthesiaMachineExpiratoryValveLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::AnesthesiaMachineExpiratoryValveLeakData& src, SEAnesthesiaMachineExpiratoryValveLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::AnesthesiaMachineExpiratoryValveLeakData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineExpiratoryValveLeak& src)
{
  cdm::AnesthesiaMachineExpiratoryValveLeakData* dst = new cdm::AnesthesiaMachineExpiratoryValveLeakData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineExpiratoryValveLeak& src, cdm::AnesthesiaMachineExpiratoryValveLeakData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineExpiratoryValveLeak& src, SEAnesthesiaMachineExpiratoryValveLeak& dst)
{
  cdm::AnesthesiaMachineExpiratoryValveLeakData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::AnesthesiaMachineExpiratoryValveObstructionData& src, SEAnesthesiaMachineExpiratoryValveObstruction& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::AnesthesiaMachineExpiratoryValveObstructionData& src, SEAnesthesiaMachineExpiratoryValveObstruction& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::AnesthesiaMachineExpiratoryValveObstructionData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineExpiratoryValveObstruction& src)
{
  cdm::AnesthesiaMachineExpiratoryValveObstructionData* dst = new cdm::AnesthesiaMachineExpiratoryValveObstructionData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineExpiratoryValveObstruction& src, cdm::AnesthesiaMachineExpiratoryValveObstructionData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineExpiratoryValveObstruction& src, SEAnesthesiaMachineExpiratoryValveObstruction& dst)
{
  cdm::AnesthesiaMachineExpiratoryValveObstructionData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::AnesthesiaMachineInspiratoryValveLeakData& src, SEAnesthesiaMachineInspiratoryValveLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::AnesthesiaMachineInspiratoryValveLeakData& src, SEAnesthesiaMachineInspiratoryValveLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::AnesthesiaMachineInspiratoryValveLeakData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineInspiratoryValveLeak& src)
{
  cdm::AnesthesiaMachineInspiratoryValveLeakData* dst = new cdm::AnesthesiaMachineInspiratoryValveLeakData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineInspiratoryValveLeak& src, cdm::AnesthesiaMachineInspiratoryValveLeakData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineInspiratoryValveLeak& src, SEAnesthesiaMachineInspiratoryValveLeak& dst)
{
  cdm::AnesthesiaMachineInspiratoryValveLeakData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::AnesthesiaMachineInspiratoryValveObstructionData& src, SEAnesthesiaMachineInspiratoryValveObstruction& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::AnesthesiaMachineInspiratoryValveObstructionData& src, SEAnesthesiaMachineInspiratoryValveObstruction& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::AnesthesiaMachineInspiratoryValveObstructionData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineInspiratoryValveObstruction& src)
{
  cdm::AnesthesiaMachineInspiratoryValveObstructionData* dst = new cdm::AnesthesiaMachineInspiratoryValveObstructionData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineInspiratoryValveObstruction& src, cdm::AnesthesiaMachineInspiratoryValveObstructionData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineInspiratoryValveObstruction& src, SEAnesthesiaMachineInspiratoryValveObstruction& dst)
{
  cdm::AnesthesiaMachineInspiratoryValveObstructionData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::AnesthesiaMachineMaskLeakData& src, SEAnesthesiaMachineMaskLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::AnesthesiaMachineMaskLeakData& src, SEAnesthesiaMachineMaskLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::AnesthesiaMachineMaskLeakData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineMaskLeak& src)
{
  cdm::AnesthesiaMachineMaskLeakData* dst = new cdm::AnesthesiaMachineMaskLeakData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineMaskLeak& src, cdm::AnesthesiaMachineMaskLeakData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineMaskLeak& src, SEAnesthesiaMachineMaskLeak& dst)
{
  cdm::AnesthesiaMachineMaskLeakData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::AnesthesiaMachineOxygenTankPressureLossData& src, SEAnesthesiaMachineOxygenTankPressureLoss& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::AnesthesiaMachineOxygenTankPressureLossData& src, SEAnesthesiaMachineOxygenTankPressureLoss& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  dst.SetActive(src.state() == cdm::eSwitch::On ? true : false);
}
cdm::AnesthesiaMachineOxygenTankPressureLossData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineOxygenTankPressureLoss& src)
{
  cdm::AnesthesiaMachineOxygenTankPressureLossData* dst = new cdm::AnesthesiaMachineOxygenTankPressureLossData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineOxygenTankPressureLoss& src, cdm::AnesthesiaMachineOxygenTankPressureLossData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  dst.set_state(src.IsActive() ? cdm::eSwitch::On : cdm::eSwitch::Off);
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineOxygenTankPressureLoss& src, SEAnesthesiaMachineOxygenTankPressureLoss& dst)
{
  cdm::AnesthesiaMachineOxygenTankPressureLossData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::AnesthesiaMachineOxygenWallPortPressureLossData& src, SEAnesthesiaMachineOxygenWallPortPressureLoss& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::AnesthesiaMachineOxygenWallPortPressureLossData& src, SEAnesthesiaMachineOxygenWallPortPressureLoss& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  dst.SetActive(src.state() == cdm::eSwitch::On ? true : false);
}
cdm::AnesthesiaMachineOxygenWallPortPressureLossData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineOxygenWallPortPressureLoss& src)
{
  cdm::AnesthesiaMachineOxygenWallPortPressureLossData* dst = new cdm::AnesthesiaMachineOxygenWallPortPressureLossData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineOxygenWallPortPressureLoss& src, cdm::AnesthesiaMachineOxygenWallPortPressureLossData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  dst.set_state(src.IsActive() ? cdm::eSwitch::On : cdm::eSwitch::Off);
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineOxygenWallPortPressureLoss& src, SEAnesthesiaMachineOxygenWallPortPressureLoss& dst)
{
  cdm::AnesthesiaMachineOxygenWallPortPressureLossData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::AnesthesiaMachineSodaLimeFailureData& src, SEAnesthesiaMachineSodaLimeFailure& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::AnesthesiaMachineSodaLimeFailureData& src, SEAnesthesiaMachineSodaLimeFailure& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::AnesthesiaMachineSodaLimeFailureData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineSodaLimeFailure& src)
{
  cdm::AnesthesiaMachineSodaLimeFailureData* dst = new cdm::AnesthesiaMachineSodaLimeFailureData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineSodaLimeFailure& src, cdm::AnesthesiaMachineSodaLimeFailureData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineSodaLimeFailure& src, SEAnesthesiaMachineSodaLimeFailure& dst)
{
  cdm::AnesthesiaMachineSodaLimeFailureData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::AnesthesiaMachineTubeCuffLeakData& src, SEAnesthesiaMachineTubeCuffLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::AnesthesiaMachineTubeCuffLeakData& src, SEAnesthesiaMachineTubeCuffLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::AnesthesiaMachineTubeCuffLeakData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineTubeCuffLeak& src)
{
  cdm::AnesthesiaMachineTubeCuffLeakData* dst = new cdm::AnesthesiaMachineTubeCuffLeakData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineTubeCuffLeak& src, cdm::AnesthesiaMachineTubeCuffLeakData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineTubeCuffLeak& src, SEAnesthesiaMachineTubeCuffLeak& dst)
{
  cdm::AnesthesiaMachineTubeCuffLeakData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::AnesthesiaMachineVaporizerFailureData& src, SEAnesthesiaMachineVaporizerFailure& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::AnesthesiaMachineVaporizerFailureData& src, SEAnesthesiaMachineVaporizerFailure& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::AnesthesiaMachineVaporizerFailureData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineVaporizerFailure& src)
{
  cdm::AnesthesiaMachineVaporizerFailureData* dst = new cdm::AnesthesiaMachineVaporizerFailureData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineVaporizerFailure& src, cdm::AnesthesiaMachineVaporizerFailureData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineVaporizerFailure& src, SEAnesthesiaMachineVaporizerFailure& dst)
{
  cdm::AnesthesiaMachineVaporizerFailureData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::AnesthesiaMachineVentilatorPressureLossData& src, SEAnesthesiaMachineVentilatorPressureLoss& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::AnesthesiaMachineVentilatorPressureLossData& src, SEAnesthesiaMachineVentilatorPressureLoss& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::AnesthesiaMachineVentilatorPressureLossData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineVentilatorPressureLoss& src)
{
  cdm::AnesthesiaMachineVentilatorPressureLossData* dst = new cdm::AnesthesiaMachineVentilatorPressureLossData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineVentilatorPressureLoss& src, cdm::AnesthesiaMachineVentilatorPressureLossData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineVentilatorPressureLoss& src, SEAnesthesiaMachineVentilatorPressureLoss& dst)
{
  cdm::AnesthesiaMachineVentilatorPressureLossData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const cdm::AnesthesiaMachineYPieceDisconnectData& src, SEAnesthesiaMachineYPieceDisconnect& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const cdm::AnesthesiaMachineYPieceDisconnectData& src, SEAnesthesiaMachineYPieceDisconnect& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
cdm::AnesthesiaMachineYPieceDisconnectData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineYPieceDisconnect& src)
{
  cdm::AnesthesiaMachineYPieceDisconnectData* dst = new cdm::AnesthesiaMachineYPieceDisconnectData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineYPieceDisconnect& src, cdm::AnesthesiaMachineYPieceDisconnectData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineYPieceDisconnect& src, SEAnesthesiaMachineYPieceDisconnect& dst)
{
  cdm::AnesthesiaMachineYPieceDisconnectData data;
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
    SEAnesthesiaMachineOxygenTankPressureLoss* a = new SEAnesthesiaMachineOxygenTankPressureLoss();
    PBAnesthesiaMachineAction::Load(any.oxygentankpressureloss(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kOxygenWallPortPressureLoss:
  {
    SEAnesthesiaMachineOxygenWallPortPressureLoss* a = new SEAnesthesiaMachineOxygenWallPortPressureLoss();
    PBAnesthesiaMachineAction::Load(any.oxygenwallportpressureloss(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kExpiratoryValveLeak:
  {
    SEAnesthesiaMachineExpiratoryValveLeak* a = new SEAnesthesiaMachineExpiratoryValveLeak();
    PBAnesthesiaMachineAction::Load(any.expiratoryvalveleak(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kExpiratoryValveObstruction:
  {
    SEAnesthesiaMachineExpiratoryValveObstruction* a = new SEAnesthesiaMachineExpiratoryValveObstruction();
    PBAnesthesiaMachineAction::Load(any.expiratoryvalveobstruction(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kInspiratoryValveLeak:
  {
    SEAnesthesiaMachineInspiratoryValveLeak* a = new SEAnesthesiaMachineInspiratoryValveLeak();
    PBAnesthesiaMachineAction::Load(any.inspiratoryvalveleak(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kInspiratoryValveObstruction:
  {
    SEAnesthesiaMachineInspiratoryValveObstruction* a = new SEAnesthesiaMachineInspiratoryValveObstruction();
    PBAnesthesiaMachineAction::Load(any.inspiratoryvalveobstruction(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kMaskLeak:
  {
    SEAnesthesiaMachineMaskLeak* a = new SEAnesthesiaMachineMaskLeak();
    PBAnesthesiaMachineAction::Load(any.maskleak(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kSodaLimeFailure:
  {
    SEAnesthesiaMachineSodaLimeFailure* a = new SEAnesthesiaMachineSodaLimeFailure();
    PBAnesthesiaMachineAction::Load(any.sodalimefailure(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kTubeCuffLeak:
  {
    SEAnesthesiaMachineTubeCuffLeak* a = new SEAnesthesiaMachineTubeCuffLeak();
    PBAnesthesiaMachineAction::Load(any.tubecuffleak(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kVaporizerFailure:
  {
    SEAnesthesiaMachineVaporizerFailure* a = new SEAnesthesiaMachineVaporizerFailure();
    PBAnesthesiaMachineAction::Load(any.vaporizerfailure(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kVentilatorPressureLoss:
  {
    SEAnesthesiaMachineVentilatorPressureLoss* a = new SEAnesthesiaMachineVentilatorPressureLoss();
    PBAnesthesiaMachineAction::Load(any.ventilatorpressureloss(), *a);
    return a;
  }
  case cdm::AnyAnesthesiaMachineActionData::ActionCase::kYPieceDisconnect:
  {
    SEAnesthesiaMachineYPieceDisconnect* a = new SEAnesthesiaMachineYPieceDisconnect();
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

  const SEAnesthesiaMachineOxygenWallPortPressureLoss* owppl = dynamic_cast<const SEAnesthesiaMachineOxygenWallPortPressureLoss*>(&action);
  if (owppl != nullptr)
  {
    any->set_allocated_oxygenwallportpressureloss(PBAnesthesiaMachineAction::Unload(*owppl));
    return any;
  }

  const SEAnesthesiaMachineOxygenTankPressureLoss* otpl = dynamic_cast<const SEAnesthesiaMachineOxygenTankPressureLoss*>(&action);
  if (otpl != nullptr)
  {
    any->set_allocated_oxygentankpressureloss(PBAnesthesiaMachineAction::Unload(*otpl));
    return any;
  }

  const SEAnesthesiaMachineExpiratoryValveLeak* evl = dynamic_cast<const SEAnesthesiaMachineExpiratoryValveLeak*>(&action);
  if (evl != nullptr)
  {
    any->set_allocated_expiratoryvalveleak(PBAnesthesiaMachineAction::Unload(*evl));
    return any;
  }

  const SEAnesthesiaMachineExpiratoryValveObstruction* evo = dynamic_cast<const SEAnesthesiaMachineExpiratoryValveObstruction*>(&action);
  if (evo != nullptr)
  {
    any->set_allocated_expiratoryvalveobstruction(PBAnesthesiaMachineAction::Unload(*evo));
    return any;
  }

  const SEAnesthesiaMachineInspiratoryValveLeak* ivl = dynamic_cast<const SEAnesthesiaMachineInspiratoryValveLeak*>(&action);
  if (ivl != nullptr)
  {
    any->set_allocated_inspiratoryvalveleak(PBAnesthesiaMachineAction::Unload(*ivl));
    return any;
  }

  const SEAnesthesiaMachineInspiratoryValveObstruction* ivo = dynamic_cast<const SEAnesthesiaMachineInspiratoryValveObstruction*>(&action);
  if (ivo != nullptr)
  {
    any->set_allocated_inspiratoryvalveobstruction(PBAnesthesiaMachineAction::Unload(*ivo));
    return any;
  }

  const SEAnesthesiaMachineMaskLeak* ml = dynamic_cast<const SEAnesthesiaMachineMaskLeak*>(&action);
  if (ml != nullptr)
  {
    any->set_allocated_maskleak(PBAnesthesiaMachineAction::Unload(*ml));
    return any;
  }

  const SEAnesthesiaMachineSodaLimeFailure* slf = dynamic_cast<const SEAnesthesiaMachineSodaLimeFailure*>(&action);
  if (slf != nullptr)
  {
    any->set_allocated_sodalimefailure(PBAnesthesiaMachineAction::Unload(*slf));
    return any;
  }

  const SEAnesthesiaMachineTubeCuffLeak* tcl = dynamic_cast<const SEAnesthesiaMachineTubeCuffLeak*>(&action);
  if (tcl != nullptr)
  {
    any->set_allocated_tubecuffleak(PBAnesthesiaMachineAction::Unload(*tcl));
    return any;
  }

  const SEAnesthesiaMachineVaporizerFailure* vf = dynamic_cast<const SEAnesthesiaMachineVaporizerFailure*>(&action);
  if (vf != nullptr)
  {
    any->set_allocated_vaporizerfailure(PBAnesthesiaMachineAction::Unload(*vf));
    return any;
  }

  const SEAnesthesiaMachineVentilatorPressureLoss* vpl = dynamic_cast<const SEAnesthesiaMachineVentilatorPressureLoss*>(&action);
  if (vpl != nullptr)
  {
    any->set_allocated_ventilatorpressureloss(PBAnesthesiaMachineAction::Unload(*vpl));
    return any;
  }

  const SEAnesthesiaMachineYPieceDisconnect* ypd = dynamic_cast<const SEAnesthesiaMachineYPieceDisconnect*>(&action);
  if (ypd != nullptr)
  {
    any->set_allocated_ypiecedisconnect(PBAnesthesiaMachineAction::Unload(*ypd));
    return any;
  }
  delete any;
  return nullptr;
}
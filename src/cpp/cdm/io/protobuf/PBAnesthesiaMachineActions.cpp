/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include "pulse/cdm/bind/AnesthesiaMachineActions.pb.h"
POP_PROTO_WARNINGS()
#include "io/protobuf/PBAnesthesiaMachineActions.h"
#include "io/protobuf/PBAnesthesiaMachine.h"
#include "io/protobuf/PBActions.h"
#include "io/protobuf/PBProperties.h"
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

void PBAnesthesiaMachineAction::Serialize(const CDM_BIND::AnesthesiaMachineActionData& src, SEAnesthesiaMachineAction& dst)
{
  PBAction::Serialize(src.action(), dst);
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineAction& src, CDM_BIND::AnesthesiaMachineActionData& dst)
{
  PBAction::Serialize(src, *dst.mutable_action());
}

void PBAnesthesiaMachineAction::Load(const CDM_BIND::AnesthesiaMachineConfigurationData& src, SEAnesthesiaMachineConfiguration& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const CDM_BIND::AnesthesiaMachineConfigurationData& src, SEAnesthesiaMachineConfiguration& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_configuration())
    PBAnesthesiaMachine::Load(src.configuration(), dst.GetConfiguration());
  else
    dst.SetConfigurationFile(src.configurationfile());
}
CDM_BIND::AnesthesiaMachineConfigurationData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineConfiguration& src)
{
  CDM_BIND::AnesthesiaMachineConfigurationData* dst = new CDM_BIND::AnesthesiaMachineConfigurationData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineConfiguration& src, CDM_BIND::AnesthesiaMachineConfigurationData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasConfiguration())
    dst.set_allocated_configuration(PBAnesthesiaMachine::Unload(*src.m_Configuration));
  else if (src.HasConfigurationFile())
    dst.set_configurationfile(src.m_ConfigurationFile);
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineConfiguration& src, SEAnesthesiaMachineConfiguration& dst)
{
  CDM_BIND::AnesthesiaMachineConfigurationData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const CDM_BIND::AnesthesiaMachineExpiratoryValveLeakData& src, SEAnesthesiaMachineExpiratoryValveLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const CDM_BIND::AnesthesiaMachineExpiratoryValveLeakData& src, SEAnesthesiaMachineExpiratoryValveLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
CDM_BIND::AnesthesiaMachineExpiratoryValveLeakData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineExpiratoryValveLeak& src)
{
  CDM_BIND::AnesthesiaMachineExpiratoryValveLeakData* dst = new CDM_BIND::AnesthesiaMachineExpiratoryValveLeakData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineExpiratoryValveLeak& src, CDM_BIND::AnesthesiaMachineExpiratoryValveLeakData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineExpiratoryValveLeak& src, SEAnesthesiaMachineExpiratoryValveLeak& dst)
{
  CDM_BIND::AnesthesiaMachineExpiratoryValveLeakData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const CDM_BIND::AnesthesiaMachineExpiratoryValveObstructionData& src, SEAnesthesiaMachineExpiratoryValveObstruction& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const CDM_BIND::AnesthesiaMachineExpiratoryValveObstructionData& src, SEAnesthesiaMachineExpiratoryValveObstruction& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
CDM_BIND::AnesthesiaMachineExpiratoryValveObstructionData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineExpiratoryValveObstruction& src)
{
  CDM_BIND::AnesthesiaMachineExpiratoryValveObstructionData* dst = new CDM_BIND::AnesthesiaMachineExpiratoryValveObstructionData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineExpiratoryValveObstruction& src, CDM_BIND::AnesthesiaMachineExpiratoryValveObstructionData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineExpiratoryValveObstruction& src, SEAnesthesiaMachineExpiratoryValveObstruction& dst)
{
  CDM_BIND::AnesthesiaMachineExpiratoryValveObstructionData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const CDM_BIND::AnesthesiaMachineInspiratoryValveLeakData& src, SEAnesthesiaMachineInspiratoryValveLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const CDM_BIND::AnesthesiaMachineInspiratoryValveLeakData& src, SEAnesthesiaMachineInspiratoryValveLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
CDM_BIND::AnesthesiaMachineInspiratoryValveLeakData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineInspiratoryValveLeak& src)
{
  CDM_BIND::AnesthesiaMachineInspiratoryValveLeakData* dst = new CDM_BIND::AnesthesiaMachineInspiratoryValveLeakData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineInspiratoryValveLeak& src, CDM_BIND::AnesthesiaMachineInspiratoryValveLeakData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineInspiratoryValveLeak& src, SEAnesthesiaMachineInspiratoryValveLeak& dst)
{
  CDM_BIND::AnesthesiaMachineInspiratoryValveLeakData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const CDM_BIND::AnesthesiaMachineInspiratoryValveObstructionData& src, SEAnesthesiaMachineInspiratoryValveObstruction& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const CDM_BIND::AnesthesiaMachineInspiratoryValveObstructionData& src, SEAnesthesiaMachineInspiratoryValveObstruction& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
CDM_BIND::AnesthesiaMachineInspiratoryValveObstructionData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineInspiratoryValveObstruction& src)
{
  CDM_BIND::AnesthesiaMachineInspiratoryValveObstructionData* dst = new CDM_BIND::AnesthesiaMachineInspiratoryValveObstructionData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineInspiratoryValveObstruction& src, CDM_BIND::AnesthesiaMachineInspiratoryValveObstructionData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineInspiratoryValveObstruction& src, SEAnesthesiaMachineInspiratoryValveObstruction& dst)
{
  CDM_BIND::AnesthesiaMachineInspiratoryValveObstructionData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const CDM_BIND::AnesthesiaMachineMaskLeakData& src, SEAnesthesiaMachineMaskLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const CDM_BIND::AnesthesiaMachineMaskLeakData& src, SEAnesthesiaMachineMaskLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
CDM_BIND::AnesthesiaMachineMaskLeakData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineMaskLeak& src)
{
  CDM_BIND::AnesthesiaMachineMaskLeakData* dst = new CDM_BIND::AnesthesiaMachineMaskLeakData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineMaskLeak& src, CDM_BIND::AnesthesiaMachineMaskLeakData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineMaskLeak& src, SEAnesthesiaMachineMaskLeak& dst)
{
  CDM_BIND::AnesthesiaMachineMaskLeakData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const CDM_BIND::AnesthesiaMachineOxygenTankPressureLossData& src, SEAnesthesiaMachineOxygenTankPressureLoss& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const CDM_BIND::AnesthesiaMachineOxygenTankPressureLossData& src, SEAnesthesiaMachineOxygenTankPressureLoss& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  dst.SetActive(src.state() == CDM_BIND::eSwitch::On ? true : false);
}
CDM_BIND::AnesthesiaMachineOxygenTankPressureLossData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineOxygenTankPressureLoss& src)
{
  CDM_BIND::AnesthesiaMachineOxygenTankPressureLossData* dst = new CDM_BIND::AnesthesiaMachineOxygenTankPressureLossData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineOxygenTankPressureLoss& src, CDM_BIND::AnesthesiaMachineOxygenTankPressureLossData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  dst.set_state(src.IsActive() ? CDM_BIND::eSwitch::On : CDM_BIND::eSwitch::Off);
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineOxygenTankPressureLoss& src, SEAnesthesiaMachineOxygenTankPressureLoss& dst)
{
  CDM_BIND::AnesthesiaMachineOxygenTankPressureLossData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const CDM_BIND::AnesthesiaMachineOxygenWallPortPressureLossData& src, SEAnesthesiaMachineOxygenWallPortPressureLoss& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const CDM_BIND::AnesthesiaMachineOxygenWallPortPressureLossData& src, SEAnesthesiaMachineOxygenWallPortPressureLoss& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  dst.SetActive(src.state() == CDM_BIND::eSwitch::On ? true : false);
}
CDM_BIND::AnesthesiaMachineOxygenWallPortPressureLossData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineOxygenWallPortPressureLoss& src)
{
  CDM_BIND::AnesthesiaMachineOxygenWallPortPressureLossData* dst = new CDM_BIND::AnesthesiaMachineOxygenWallPortPressureLossData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineOxygenWallPortPressureLoss& src, CDM_BIND::AnesthesiaMachineOxygenWallPortPressureLossData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  dst.set_state(src.IsActive() ? CDM_BIND::eSwitch::On : CDM_BIND::eSwitch::Off);
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineOxygenWallPortPressureLoss& src, SEAnesthesiaMachineOxygenWallPortPressureLoss& dst)
{
  CDM_BIND::AnesthesiaMachineOxygenWallPortPressureLossData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const CDM_BIND::AnesthesiaMachineSodaLimeFailureData& src, SEAnesthesiaMachineSodaLimeFailure& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const CDM_BIND::AnesthesiaMachineSodaLimeFailureData& src, SEAnesthesiaMachineSodaLimeFailure& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
CDM_BIND::AnesthesiaMachineSodaLimeFailureData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineSodaLimeFailure& src)
{
  CDM_BIND::AnesthesiaMachineSodaLimeFailureData* dst = new CDM_BIND::AnesthesiaMachineSodaLimeFailureData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineSodaLimeFailure& src, CDM_BIND::AnesthesiaMachineSodaLimeFailureData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineSodaLimeFailure& src, SEAnesthesiaMachineSodaLimeFailure& dst)
{
  CDM_BIND::AnesthesiaMachineSodaLimeFailureData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const CDM_BIND::AnesthesiaMachineTubeCuffLeakData& src, SEAnesthesiaMachineTubeCuffLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const CDM_BIND::AnesthesiaMachineTubeCuffLeakData& src, SEAnesthesiaMachineTubeCuffLeak& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
CDM_BIND::AnesthesiaMachineTubeCuffLeakData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineTubeCuffLeak& src)
{
  CDM_BIND::AnesthesiaMachineTubeCuffLeakData* dst = new CDM_BIND::AnesthesiaMachineTubeCuffLeakData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineTubeCuffLeak& src, CDM_BIND::AnesthesiaMachineTubeCuffLeakData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineTubeCuffLeak& src, SEAnesthesiaMachineTubeCuffLeak& dst)
{
  CDM_BIND::AnesthesiaMachineTubeCuffLeakData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const CDM_BIND::AnesthesiaMachineVaporizerFailureData& src, SEAnesthesiaMachineVaporizerFailure& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const CDM_BIND::AnesthesiaMachineVaporizerFailureData& src, SEAnesthesiaMachineVaporizerFailure& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
CDM_BIND::AnesthesiaMachineVaporizerFailureData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineVaporizerFailure& src)
{
  CDM_BIND::AnesthesiaMachineVaporizerFailureData* dst = new CDM_BIND::AnesthesiaMachineVaporizerFailureData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineVaporizerFailure& src, CDM_BIND::AnesthesiaMachineVaporizerFailureData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineVaporizerFailure& src, SEAnesthesiaMachineVaporizerFailure& dst)
{
  CDM_BIND::AnesthesiaMachineVaporizerFailureData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const CDM_BIND::AnesthesiaMachineVentilatorPressureLossData& src, SEAnesthesiaMachineVentilatorPressureLoss& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const CDM_BIND::AnesthesiaMachineVentilatorPressureLossData& src, SEAnesthesiaMachineVentilatorPressureLoss& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
CDM_BIND::AnesthesiaMachineVentilatorPressureLossData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineVentilatorPressureLoss& src)
{
  CDM_BIND::AnesthesiaMachineVentilatorPressureLossData* dst = new CDM_BIND::AnesthesiaMachineVentilatorPressureLossData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineVentilatorPressureLoss& src, CDM_BIND::AnesthesiaMachineVentilatorPressureLossData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineVentilatorPressureLoss& src, SEAnesthesiaMachineVentilatorPressureLoss& dst)
{
  CDM_BIND::AnesthesiaMachineVentilatorPressureLossData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

void PBAnesthesiaMachineAction::Load(const CDM_BIND::AnesthesiaMachineYPieceDisconnectData& src, SEAnesthesiaMachineYPieceDisconnect& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, dst);
}
void PBAnesthesiaMachineAction::Serialize(const CDM_BIND::AnesthesiaMachineYPieceDisconnectData& src, SEAnesthesiaMachineYPieceDisconnect& dst)
{
  PBAnesthesiaMachineAction::Serialize(src.anesthesiamachineaction(), dst);
  if (src.has_severity())
    PBProperty::Load(src.severity(), dst.GetSeverity());
}
CDM_BIND::AnesthesiaMachineYPieceDisconnectData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineYPieceDisconnect& src)
{
  CDM_BIND::AnesthesiaMachineYPieceDisconnectData* dst = new CDM_BIND::AnesthesiaMachineYPieceDisconnectData();
  PBAnesthesiaMachineAction::Serialize(src, *dst);
  return dst;
}
void PBAnesthesiaMachineAction::Serialize(const SEAnesthesiaMachineYPieceDisconnect& src, CDM_BIND::AnesthesiaMachineYPieceDisconnectData& dst)
{
  PBAnesthesiaMachineAction::Serialize(src, *dst.mutable_anesthesiamachineaction());
  if (src.HasSeverity())
    dst.set_allocated_severity(PBProperty::Unload(*src.m_Severity));
}
void PBAnesthesiaMachineAction::Copy(const SEAnesthesiaMachineYPieceDisconnect& src, SEAnesthesiaMachineYPieceDisconnect& dst)
{
  CDM_BIND::AnesthesiaMachineYPieceDisconnectData data;
  PBAnesthesiaMachineAction::Serialize(src, data);
  PBAnesthesiaMachineAction::Serialize(data, dst);
}

SEAnesthesiaMachineAction* PBAnesthesiaMachineAction::Load(const CDM_BIND::AnyAnesthesiaMachineActionData& any, SESubstanceManager& subMgr)
{
  switch (any.Action_case())
  {
  case CDM_BIND::AnyAnesthesiaMachineActionData::ActionCase::kConfiguration:
  {
    SEAnesthesiaMachineConfiguration* a = new SEAnesthesiaMachineConfiguration(subMgr);
    PBAnesthesiaMachineAction::Load(any.configuration(), *a);
    return a;
  }
  case CDM_BIND::AnyAnesthesiaMachineActionData::ActionCase::kOxygenTankPressureLoss:
  {
    SEAnesthesiaMachineOxygenTankPressureLoss* a = new SEAnesthesiaMachineOxygenTankPressureLoss();
    PBAnesthesiaMachineAction::Load(any.oxygentankpressureloss(), *a);
    return a;
  }
  case CDM_BIND::AnyAnesthesiaMachineActionData::ActionCase::kOxygenWallPortPressureLoss:
  {
    SEAnesthesiaMachineOxygenWallPortPressureLoss* a = new SEAnesthesiaMachineOxygenWallPortPressureLoss();
    PBAnesthesiaMachineAction::Load(any.oxygenwallportpressureloss(), *a);
    return a;
  }
  case CDM_BIND::AnyAnesthesiaMachineActionData::ActionCase::kExpiratoryValveLeak:
  {
    SEAnesthesiaMachineExpiratoryValveLeak* a = new SEAnesthesiaMachineExpiratoryValveLeak();
    PBAnesthesiaMachineAction::Load(any.expiratoryvalveleak(), *a);
    return a;
  }
  case CDM_BIND::AnyAnesthesiaMachineActionData::ActionCase::kExpiratoryValveObstruction:
  {
    SEAnesthesiaMachineExpiratoryValveObstruction* a = new SEAnesthesiaMachineExpiratoryValveObstruction();
    PBAnesthesiaMachineAction::Load(any.expiratoryvalveobstruction(), *a);
    return a;
  }
  case CDM_BIND::AnyAnesthesiaMachineActionData::ActionCase::kInspiratoryValveLeak:
  {
    SEAnesthesiaMachineInspiratoryValveLeak* a = new SEAnesthesiaMachineInspiratoryValveLeak();
    PBAnesthesiaMachineAction::Load(any.inspiratoryvalveleak(), *a);
    return a;
  }
  case CDM_BIND::AnyAnesthesiaMachineActionData::ActionCase::kInspiratoryValveObstruction:
  {
    SEAnesthesiaMachineInspiratoryValveObstruction* a = new SEAnesthesiaMachineInspiratoryValveObstruction();
    PBAnesthesiaMachineAction::Load(any.inspiratoryvalveobstruction(), *a);
    return a;
  }
  case CDM_BIND::AnyAnesthesiaMachineActionData::ActionCase::kMaskLeak:
  {
    SEAnesthesiaMachineMaskLeak* a = new SEAnesthesiaMachineMaskLeak();
    PBAnesthesiaMachineAction::Load(any.maskleak(), *a);
    return a;
  }
  case CDM_BIND::AnyAnesthesiaMachineActionData::ActionCase::kSodaLimeFailure:
  {
    SEAnesthesiaMachineSodaLimeFailure* a = new SEAnesthesiaMachineSodaLimeFailure();
    PBAnesthesiaMachineAction::Load(any.sodalimefailure(), *a);
    return a;
  }
  case CDM_BIND::AnyAnesthesiaMachineActionData::ActionCase::kTubeCuffLeak:
  {
    SEAnesthesiaMachineTubeCuffLeak* a = new SEAnesthesiaMachineTubeCuffLeak();
    PBAnesthesiaMachineAction::Load(any.tubecuffleak(), *a);
    return a;
  }
  case CDM_BIND::AnyAnesthesiaMachineActionData::ActionCase::kVaporizerFailure:
  {
    SEAnesthesiaMachineVaporizerFailure* a = new SEAnesthesiaMachineVaporizerFailure();
    PBAnesthesiaMachineAction::Load(any.vaporizerfailure(), *a);
    return a;
  }
  case CDM_BIND::AnyAnesthesiaMachineActionData::ActionCase::kVentilatorPressureLoss:
  {
    SEAnesthesiaMachineVentilatorPressureLoss* a = new SEAnesthesiaMachineVentilatorPressureLoss();
    PBAnesthesiaMachineAction::Load(any.ventilatorpressureloss(), *a);
    return a;
  }
  case CDM_BIND::AnyAnesthesiaMachineActionData::ActionCase::kYPieceDisconnect:
  {
    SEAnesthesiaMachineYPieceDisconnect* a = new SEAnesthesiaMachineYPieceDisconnect();
    PBAnesthesiaMachineAction::Load(any.ypiecedisconnect(), *a);
    return a;
  }
  }
  subMgr.Error("Unknown action type : " + any.Action_case());
  return nullptr;
}
CDM_BIND::AnyAnesthesiaMachineActionData* PBAnesthesiaMachineAction::Unload(const SEAnesthesiaMachineAction& action)
{
  CDM_BIND::AnyAnesthesiaMachineActionData* any = new CDM_BIND::AnyAnesthesiaMachineActionData();
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
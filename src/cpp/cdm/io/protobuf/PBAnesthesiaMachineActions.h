/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(AnesthesiaMachineAction)
CDM_BIND_DECL2(AnesthesiaMachineConfiguration)
CDM_BIND_DECL2(AnesthesiaMachineExpiratoryValveLeak)
CDM_BIND_DECL2(AnesthesiaMachineExpiratoryValveObstruction)
CDM_BIND_DECL2(AnesthesiaMachineInspiratoryValveLeak)
CDM_BIND_DECL2(AnesthesiaMachineInspiratoryValveObstruction)
CDM_BIND_DECL2(AnesthesiaMachineMaskLeak)
CDM_BIND_DECL2(AnesthesiaMachineOxygenTankPressureLoss)
CDM_BIND_DECL2(AnesthesiaMachineOxygenWallPortPressureLoss)
CDM_BIND_DECL2(AnesthesiaMachineSodaLimeFailure)
CDM_BIND_DECL2(AnesthesiaMachineTubeCuffLeak)
CDM_BIND_DECL2(AnesthesiaMachineVaporizerFailure)
CDM_BIND_DECL2(AnesthesiaMachineVentilatorPressureLoss)
CDM_BIND_DECL2(AnesthesiaMachineYPieceDisconnect)
CDM_BIND_DECL(AnyAnesthesiaMachineActionData)
class SESubstanceManager;

class CDM_DECL PBAnesthesiaMachineAction
{
public:

  /** Create a new action based on the binding object, load that data into the new action, and return said action */
  static SEAnesthesiaMachineAction* Load(const CDM_BIND::AnyAnesthesiaMachineActionData& any, SESubstanceManager& subMgr);
  /** Create a new bind object, unload the action, put that in the bind object, and return said bind object */
  static CDM_BIND::AnyAnesthesiaMachineActionData* Unload(const SEAnesthesiaMachineAction& action);
  static void Serialize(const CDM_BIND::AnesthesiaMachineActionData& src, SEAnesthesiaMachineAction& dst);
  static void Serialize(const SEAnesthesiaMachineAction& src, CDM_BIND::AnesthesiaMachineActionData& dst);

  static void Load(const CDM_BIND::AnesthesiaMachineConfigurationData& src, SEAnesthesiaMachineConfiguration& dst);
  static CDM_BIND::AnesthesiaMachineConfigurationData* Unload(const SEAnesthesiaMachineConfiguration& src);
  static void Serialize(const CDM_BIND::AnesthesiaMachineConfigurationData& src, SEAnesthesiaMachineConfiguration& dst);
  static void Serialize(const SEAnesthesiaMachineConfiguration& src, CDM_BIND::AnesthesiaMachineConfigurationData& dst);
  static void Copy(const SEAnesthesiaMachineConfiguration& src, SEAnesthesiaMachineConfiguration& dst);

  static void Load(const CDM_BIND::AnesthesiaMachineExpiratoryValveLeakData& src, SEAnesthesiaMachineExpiratoryValveLeak& dst);
  static CDM_BIND::AnesthesiaMachineExpiratoryValveLeakData* Unload(const SEAnesthesiaMachineExpiratoryValveLeak& src);
  static void Serialize(const CDM_BIND::AnesthesiaMachineExpiratoryValveLeakData& src, SEAnesthesiaMachineExpiratoryValveLeak& dst);
  static void Serialize(const SEAnesthesiaMachineExpiratoryValveLeak& src, CDM_BIND::AnesthesiaMachineExpiratoryValveLeakData& dst);
  static void Copy(const SEAnesthesiaMachineExpiratoryValveLeak& src, SEAnesthesiaMachineExpiratoryValveLeak& dst);

  static void Load(const CDM_BIND::AnesthesiaMachineExpiratoryValveObstructionData& src, SEAnesthesiaMachineExpiratoryValveObstruction& dst);
  static CDM_BIND::AnesthesiaMachineExpiratoryValveObstructionData* Unload(const SEAnesthesiaMachineExpiratoryValveObstruction& src);
  static void Serialize(const CDM_BIND::AnesthesiaMachineExpiratoryValveObstructionData& src, SEAnesthesiaMachineExpiratoryValveObstruction& dst);
  static void Serialize(const SEAnesthesiaMachineExpiratoryValveObstruction& src, CDM_BIND::AnesthesiaMachineExpiratoryValveObstructionData& dst);
  static void Copy(const SEAnesthesiaMachineExpiratoryValveObstruction& src, SEAnesthesiaMachineExpiratoryValveObstruction& dst);

  static void Load(const CDM_BIND::AnesthesiaMachineInspiratoryValveLeakData& src, SEAnesthesiaMachineInspiratoryValveLeak& dst);
  static CDM_BIND::AnesthesiaMachineInspiratoryValveLeakData* Unload(const SEAnesthesiaMachineInspiratoryValveLeak& src);
  static void Serialize(const CDM_BIND::AnesthesiaMachineInspiratoryValveLeakData& src, SEAnesthesiaMachineInspiratoryValveLeak& dst);
  static void Serialize(const SEAnesthesiaMachineInspiratoryValveLeak& src, CDM_BIND::AnesthesiaMachineInspiratoryValveLeakData& dst);
  static void Copy(const SEAnesthesiaMachineInspiratoryValveLeak& src, SEAnesthesiaMachineInspiratoryValveLeak& dst);

  static void Load(const CDM_BIND::AnesthesiaMachineInspiratoryValveObstructionData& src, SEAnesthesiaMachineInspiratoryValveObstruction& dst);
  static CDM_BIND::AnesthesiaMachineInspiratoryValveObstructionData* Unload(const SEAnesthesiaMachineInspiratoryValveObstruction& src);
  static void Serialize(const CDM_BIND::AnesthesiaMachineInspiratoryValveObstructionData& src, SEAnesthesiaMachineInspiratoryValveObstruction& dst);
  static void Serialize(const SEAnesthesiaMachineInspiratoryValveObstruction& src, CDM_BIND::AnesthesiaMachineInspiratoryValveObstructionData& dst);
  static void Copy(const SEAnesthesiaMachineInspiratoryValveObstruction& src, SEAnesthesiaMachineInspiratoryValveObstruction& dst);

  static void Load(const CDM_BIND::AnesthesiaMachineMaskLeakData& src, SEAnesthesiaMachineMaskLeak& dst);
  static CDM_BIND::AnesthesiaMachineMaskLeakData* Unload(const SEAnesthesiaMachineMaskLeak& src);
  static void Serialize(const CDM_BIND::AnesthesiaMachineMaskLeakData& src, SEAnesthesiaMachineMaskLeak& dst);
  static void Serialize(const SEAnesthesiaMachineMaskLeak& src, CDM_BIND::AnesthesiaMachineMaskLeakData& dst);
  static void Copy(const SEAnesthesiaMachineMaskLeak& src, SEAnesthesiaMachineMaskLeak& dst);

  static void Load(const CDM_BIND::AnesthesiaMachineOxygenTankPressureLossData& src, SEAnesthesiaMachineOxygenTankPressureLoss& dst);
  static CDM_BIND::AnesthesiaMachineOxygenTankPressureLossData* Unload(const SEAnesthesiaMachineOxygenTankPressureLoss& src);
  static void Serialize(const CDM_BIND::AnesthesiaMachineOxygenTankPressureLossData& src, SEAnesthesiaMachineOxygenTankPressureLoss& dst);
  static void Serialize(const SEAnesthesiaMachineOxygenTankPressureLoss& src, CDM_BIND::AnesthesiaMachineOxygenTankPressureLossData& dst);
  static void Copy(const SEAnesthesiaMachineOxygenTankPressureLoss& src, SEAnesthesiaMachineOxygenTankPressureLoss& dst);

  static void Load(const CDM_BIND::AnesthesiaMachineOxygenWallPortPressureLossData& src, SEAnesthesiaMachineOxygenWallPortPressureLoss& dst);
  static CDM_BIND::AnesthesiaMachineOxygenWallPortPressureLossData* Unload(const SEAnesthesiaMachineOxygenWallPortPressureLoss& src);
  static void Serialize(const CDM_BIND::AnesthesiaMachineOxygenWallPortPressureLossData& src, SEAnesthesiaMachineOxygenWallPortPressureLoss& dst);
  static void Serialize(const SEAnesthesiaMachineOxygenWallPortPressureLoss& src, CDM_BIND::AnesthesiaMachineOxygenWallPortPressureLossData& dst);
  static void Copy(const SEAnesthesiaMachineOxygenWallPortPressureLoss& src, SEAnesthesiaMachineOxygenWallPortPressureLoss& dst);

  static void Load(const CDM_BIND::AnesthesiaMachineSodaLimeFailureData& src, SEAnesthesiaMachineSodaLimeFailure& dst);
  static CDM_BIND::AnesthesiaMachineSodaLimeFailureData* Unload(const SEAnesthesiaMachineSodaLimeFailure& src);
  static void Serialize(const CDM_BIND::AnesthesiaMachineSodaLimeFailureData& src, SEAnesthesiaMachineSodaLimeFailure& dst);
  static void Serialize(const SEAnesthesiaMachineSodaLimeFailure& src, CDM_BIND::AnesthesiaMachineSodaLimeFailureData& dst);
  static void Copy(const SEAnesthesiaMachineSodaLimeFailure& src, SEAnesthesiaMachineSodaLimeFailure& dst);

  static void Load(const CDM_BIND::AnesthesiaMachineTubeCuffLeakData& src, SEAnesthesiaMachineTubeCuffLeak& dst);
  static CDM_BIND::AnesthesiaMachineTubeCuffLeakData* Unload(const SEAnesthesiaMachineTubeCuffLeak& src);
  static void Serialize(const CDM_BIND::AnesthesiaMachineTubeCuffLeakData& src, SEAnesthesiaMachineTubeCuffLeak& dst);
  static void Serialize(const SEAnesthesiaMachineTubeCuffLeak& src, CDM_BIND::AnesthesiaMachineTubeCuffLeakData& dst);
  static void Copy(const SEAnesthesiaMachineTubeCuffLeak& src, SEAnesthesiaMachineTubeCuffLeak& dst);

  static void Load(const CDM_BIND::AnesthesiaMachineVaporizerFailureData& src, SEAnesthesiaMachineVaporizerFailure& dst);
  static CDM_BIND::AnesthesiaMachineVaporizerFailureData* Unload(const SEAnesthesiaMachineVaporizerFailure& src);
  static void Serialize(const CDM_BIND::AnesthesiaMachineVaporizerFailureData& src, SEAnesthesiaMachineVaporizerFailure& dst);
  static void Serialize(const SEAnesthesiaMachineVaporizerFailure& src, CDM_BIND::AnesthesiaMachineVaporizerFailureData& dst);
  static void Copy(const SEAnesthesiaMachineVaporizerFailure& src, SEAnesthesiaMachineVaporizerFailure& dst);

  static void Load(const CDM_BIND::AnesthesiaMachineVentilatorPressureLossData& src, SEAnesthesiaMachineVentilatorPressureLoss& dst);
  static CDM_BIND::AnesthesiaMachineVentilatorPressureLossData* Unload(const SEAnesthesiaMachineVentilatorPressureLoss& src);
  static void Serialize(const CDM_BIND::AnesthesiaMachineVentilatorPressureLossData& src, SEAnesthesiaMachineVentilatorPressureLoss& dst);
  static void Serialize(const SEAnesthesiaMachineVentilatorPressureLoss& src, CDM_BIND::AnesthesiaMachineVentilatorPressureLossData& dst);
  static void Copy(const SEAnesthesiaMachineVentilatorPressureLoss& src, SEAnesthesiaMachineVentilatorPressureLoss& dst);

  static void Load(const CDM_BIND::AnesthesiaMachineYPieceDisconnectData& src, SEAnesthesiaMachineYPieceDisconnect& dst);
  static CDM_BIND::AnesthesiaMachineYPieceDisconnectData* Unload(const SEAnesthesiaMachineYPieceDisconnect& src);
  static void Serialize(const CDM_BIND::AnesthesiaMachineYPieceDisconnectData& src, SEAnesthesiaMachineYPieceDisconnect& dst);
  static void Serialize(const SEAnesthesiaMachineYPieceDisconnect& src, CDM_BIND::AnesthesiaMachineYPieceDisconnectData& dst);
  static void Copy(const SEAnesthesiaMachineYPieceDisconnect& src, SEAnesthesiaMachineYPieceDisconnect& dst);
};
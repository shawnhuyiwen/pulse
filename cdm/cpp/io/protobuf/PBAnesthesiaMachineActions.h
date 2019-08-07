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
  static SEAnesthesiaMachineAction* Load(const cdm::AnyAnesthesiaMachineActionData& any, SESubstanceManager& subMgr);
  /** Create a new bind object, unload the action, put that in the bind object, and return said bind object */
  static cdm::AnyAnesthesiaMachineActionData* Unload(const SEAnesthesiaMachineAction& action);
  static void Serialize(const cdm::AnesthesiaMachineActionData& src, SEAnesthesiaMachineAction& dst);
  static void Serialize(const SEAnesthesiaMachineAction& src, cdm::AnesthesiaMachineActionData& dst);

  static void Load(const cdm::AnesthesiaMachineConfigurationData& src, SEAnesthesiaMachineConfiguration& dst);
  static cdm::AnesthesiaMachineConfigurationData* Unload(const SEAnesthesiaMachineConfiguration& src);
  static void Serialize(const cdm::AnesthesiaMachineConfigurationData& src, SEAnesthesiaMachineConfiguration& dst);
  static void Serialize(const SEAnesthesiaMachineConfiguration& src, cdm::AnesthesiaMachineConfigurationData& dst);
  static void Copy(const SEAnesthesiaMachineConfiguration& src, SEAnesthesiaMachineConfiguration& dst);

  static void Load(const cdm::AnesthesiaMachineExpiratoryValveLeakData& src, SEAnesthesiaMachineExpiratoryValveLeak& dst);
  static cdm::AnesthesiaMachineExpiratoryValveLeakData* Unload(const SEAnesthesiaMachineExpiratoryValveLeak& src);
  static void Serialize(const cdm::AnesthesiaMachineExpiratoryValveLeakData& src, SEAnesthesiaMachineExpiratoryValveLeak& dst);
  static void Serialize(const SEAnesthesiaMachineExpiratoryValveLeak& src, cdm::AnesthesiaMachineExpiratoryValveLeakData& dst);
  static void Copy(const SEAnesthesiaMachineExpiratoryValveLeak& src, SEAnesthesiaMachineExpiratoryValveLeak& dst);

  static void Load(const cdm::AnesthesiaMachineExpiratoryValveObstructionData& src, SEAnesthesiaMachineExpiratoryValveObstruction& dst);
  static cdm::AnesthesiaMachineExpiratoryValveObstructionData* Unload(const SEAnesthesiaMachineExpiratoryValveObstruction& src);
  static void Serialize(const cdm::AnesthesiaMachineExpiratoryValveObstructionData& src, SEAnesthesiaMachineExpiratoryValveObstruction& dst);
  static void Serialize(const SEAnesthesiaMachineExpiratoryValveObstruction& src, cdm::AnesthesiaMachineExpiratoryValveObstructionData& dst);
  static void Copy(const SEAnesthesiaMachineExpiratoryValveObstruction& src, SEAnesthesiaMachineExpiratoryValveObstruction& dst);

  static void Load(const cdm::AnesthesiaMachineInspiratoryValveLeakData& src, SEAnesthesiaMachineInspiratoryValveLeak& dst);
  static cdm::AnesthesiaMachineInspiratoryValveLeakData* Unload(const SEAnesthesiaMachineInspiratoryValveLeak& src);
  static void Serialize(const cdm::AnesthesiaMachineInspiratoryValveLeakData& src, SEAnesthesiaMachineInspiratoryValveLeak& dst);
  static void Serialize(const SEAnesthesiaMachineInspiratoryValveLeak& src, cdm::AnesthesiaMachineInspiratoryValveLeakData& dst);
  static void Copy(const SEAnesthesiaMachineInspiratoryValveLeak& src, SEAnesthesiaMachineInspiratoryValveLeak& dst);

  static void Load(const cdm::AnesthesiaMachineInspiratoryValveObstructionData& src, SEAnesthesiaMachineInspiratoryValveObstruction& dst);
  static cdm::AnesthesiaMachineInspiratoryValveObstructionData* Unload(const SEAnesthesiaMachineInspiratoryValveObstruction& src);
  static void Serialize(const cdm::AnesthesiaMachineInspiratoryValveObstructionData& src, SEAnesthesiaMachineInspiratoryValveObstruction& dst);
  static void Serialize(const SEAnesthesiaMachineInspiratoryValveObstruction& src, cdm::AnesthesiaMachineInspiratoryValveObstructionData& dst);
  static void Copy(const SEAnesthesiaMachineInspiratoryValveObstruction& src, SEAnesthesiaMachineInspiratoryValveObstruction& dst);

  static void Load(const cdm::AnesthesiaMachineMaskLeakData& src, SEAnesthesiaMachineMaskLeak& dst);
  static cdm::AnesthesiaMachineMaskLeakData* Unload(const SEAnesthesiaMachineMaskLeak& src);
  static void Serialize(const cdm::AnesthesiaMachineMaskLeakData& src, SEAnesthesiaMachineMaskLeak& dst);
  static void Serialize(const SEAnesthesiaMachineMaskLeak& src, cdm::AnesthesiaMachineMaskLeakData& dst);
  static void Copy(const SEAnesthesiaMachineMaskLeak& src, SEAnesthesiaMachineMaskLeak& dst);

  static void Load(const cdm::AnesthesiaMachineOxygenTankPressureLossData& src, SEAnesthesiaMachineOxygenTankPressureLoss& dst);
  static cdm::AnesthesiaMachineOxygenTankPressureLossData* Unload(const SEAnesthesiaMachineOxygenTankPressureLoss& src);
  static void Serialize(const cdm::AnesthesiaMachineOxygenTankPressureLossData& src, SEAnesthesiaMachineOxygenTankPressureLoss& dst);
  static void Serialize(const SEAnesthesiaMachineOxygenTankPressureLoss& src, cdm::AnesthesiaMachineOxygenTankPressureLossData& dst);
  static void Copy(const SEAnesthesiaMachineOxygenTankPressureLoss& src, SEAnesthesiaMachineOxygenTankPressureLoss& dst);

  static void Load(const cdm::AnesthesiaMachineOxygenWallPortPressureLossData& src, SEAnesthesiaMachineOxygenWallPortPressureLoss& dst);
  static cdm::AnesthesiaMachineOxygenWallPortPressureLossData* Unload(const SEAnesthesiaMachineOxygenWallPortPressureLoss& src);
  static void Serialize(const cdm::AnesthesiaMachineOxygenWallPortPressureLossData& src, SEAnesthesiaMachineOxygenWallPortPressureLoss& dst);
  static void Serialize(const SEAnesthesiaMachineOxygenWallPortPressureLoss& src, cdm::AnesthesiaMachineOxygenWallPortPressureLossData& dst);
  static void Copy(const SEAnesthesiaMachineOxygenWallPortPressureLoss& src, SEAnesthesiaMachineOxygenWallPortPressureLoss& dst);

  static void Load(const cdm::AnesthesiaMachineSodaLimeFailureData& src, SEAnesthesiaMachineSodaLimeFailure& dst);
  static cdm::AnesthesiaMachineSodaLimeFailureData* Unload(const SEAnesthesiaMachineSodaLimeFailure& src);
  static void Serialize(const cdm::AnesthesiaMachineSodaLimeFailureData& src, SEAnesthesiaMachineSodaLimeFailure& dst);
  static void Serialize(const SEAnesthesiaMachineSodaLimeFailure& src, cdm::AnesthesiaMachineSodaLimeFailureData& dst);
  static void Copy(const SEAnesthesiaMachineSodaLimeFailure& src, SEAnesthesiaMachineSodaLimeFailure& dst);

  static void Load(const cdm::AnesthesiaMachineTubeCuffLeakData& src, SEAnesthesiaMachineTubeCuffLeak& dst);
  static cdm::AnesthesiaMachineTubeCuffLeakData* Unload(const SEAnesthesiaMachineTubeCuffLeak& src);
  static void Serialize(const cdm::AnesthesiaMachineTubeCuffLeakData& src, SEAnesthesiaMachineTubeCuffLeak& dst);
  static void Serialize(const SEAnesthesiaMachineTubeCuffLeak& src, cdm::AnesthesiaMachineTubeCuffLeakData& dst);
  static void Copy(const SEAnesthesiaMachineTubeCuffLeak& src, SEAnesthesiaMachineTubeCuffLeak& dst);

  static void Load(const cdm::AnesthesiaMachineVaporizerFailureData& src, SEAnesthesiaMachineVaporizerFailure& dst);
  static cdm::AnesthesiaMachineVaporizerFailureData* Unload(const SEAnesthesiaMachineVaporizerFailure& src);
  static void Serialize(const cdm::AnesthesiaMachineVaporizerFailureData& src, SEAnesthesiaMachineVaporizerFailure& dst);
  static void Serialize(const SEAnesthesiaMachineVaporizerFailure& src, cdm::AnesthesiaMachineVaporizerFailureData& dst);
  static void Copy(const SEAnesthesiaMachineVaporizerFailure& src, SEAnesthesiaMachineVaporizerFailure& dst);

  static void Load(const cdm::AnesthesiaMachineVentilatorPressureLossData& src, SEAnesthesiaMachineVentilatorPressureLoss& dst);
  static cdm::AnesthesiaMachineVentilatorPressureLossData* Unload(const SEAnesthesiaMachineVentilatorPressureLoss& src);
  static void Serialize(const cdm::AnesthesiaMachineVentilatorPressureLossData& src, SEAnesthesiaMachineVentilatorPressureLoss& dst);
  static void Serialize(const SEAnesthesiaMachineVentilatorPressureLoss& src, cdm::AnesthesiaMachineVentilatorPressureLossData& dst);
  static void Copy(const SEAnesthesiaMachineVentilatorPressureLoss& src, SEAnesthesiaMachineVentilatorPressureLoss& dst);

  static void Load(const cdm::AnesthesiaMachineYPieceDisconnectData& src, SEAnesthesiaMachineYPieceDisconnect& dst);
  static cdm::AnesthesiaMachineYPieceDisconnectData* Unload(const SEAnesthesiaMachineYPieceDisconnect& src);
  static void Serialize(const cdm::AnesthesiaMachineYPieceDisconnectData& src, SEAnesthesiaMachineYPieceDisconnect& dst);
  static void Serialize(const SEAnesthesiaMachineYPieceDisconnect& src, cdm::AnesthesiaMachineYPieceDisconnectData& dst);
  static void Copy(const SEAnesthesiaMachineYPieceDisconnect& src, SEAnesthesiaMachineYPieceDisconnect& dst);
};
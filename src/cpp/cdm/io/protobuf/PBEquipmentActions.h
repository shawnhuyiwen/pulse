/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(EquipmentAction)
CDM_BIND_DECL(AnyEquipmentActionData)
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
CDM_BIND_DECL2(InhalerAction)
CDM_BIND_DECL2(InhalerConfiguration)
CDM_BIND_DECL2(MechanicalVentilatorAction)
CDM_BIND_DECL2(MechanicalVentilatorConfiguration)
class SESubstanceManager;

class CDM_DECL PBEquipmentAction
{
public:

  /** Create a new action based on the binding object, load that data into the new action, and return said action */
  static SEEquipmentAction* Load(const CDM_BIND::AnyEquipmentActionData& any, const SESubstanceManager& subMgr);
  /** Create a new bind object, unload the action, put that in the bind object, and return said bind object */
  static CDM_BIND::AnyEquipmentActionData* Unload(const SEEquipmentAction& action);

  static void Serialize(const CDM_BIND::EquipmentActionData& src, SEEquipmentAction& dst);
  static void Serialize(const SEEquipmentAction& src, CDM_BIND::EquipmentActionData& dst);

  ////////////////////////
  // Anesthesia Machine //
  ////////////////////////

  static void Serialize(const CDM_BIND::AnesthesiaMachineActionData& src, SEAnesthesiaMachineAction& dst);
  static void Serialize(const SEAnesthesiaMachineAction& src, CDM_BIND::AnesthesiaMachineActionData& dst);

  static void Load(const CDM_BIND::AnesthesiaMachineConfigurationData& src, SEAnesthesiaMachineConfiguration& dst, const SESubstanceManager& subMgr);
  static CDM_BIND::AnesthesiaMachineConfigurationData* Unload(const SEAnesthesiaMachineConfiguration& src);
  static void Serialize(const CDM_BIND::AnesthesiaMachineConfigurationData& src, SEAnesthesiaMachineConfiguration& dst, const SESubstanceManager& subMgr);
  static void Serialize(const SEAnesthesiaMachineConfiguration& src, CDM_BIND::AnesthesiaMachineConfigurationData& dst);
  static void Copy(const SEAnesthesiaMachineConfiguration& src, SEAnesthesiaMachineConfiguration& dst, const SESubstanceManager& subMgr);

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

  /////////////
  // Inhaler //
  /////////////

  static void Serialize(const CDM_BIND::InhalerActionData& src, SEInhalerAction& dst);
  static void Serialize(const SEInhalerAction& src, CDM_BIND::InhalerActionData& dst);

  static void Load(const CDM_BIND::InhalerConfigurationData& src, SEInhalerConfiguration& dst, const SESubstanceManager& subMgr);
  static CDM_BIND::InhalerConfigurationData* Unload(const SEInhalerConfiguration& src);
  static void Serialize(const CDM_BIND::InhalerConfigurationData& src, SEInhalerConfiguration& dst, const SESubstanceManager& subMgr);
  static void Serialize(const SEInhalerConfiguration& src, CDM_BIND::InhalerConfigurationData& dst);
  static void Copy(const SEInhalerConfiguration& src, SEInhalerConfiguration& dst, const SESubstanceManager& subMgr);

  ///////////////////////////
  // Mechanical Ventilator //
  ///////////////////////////

  static void Serialize(const CDM_BIND::MechanicalVentilatorActionData& src, SEMechanicalVentilatorAction& dst);
  static void Serialize(const SEMechanicalVentilatorAction& src, CDM_BIND::MechanicalVentilatorActionData& dst);

  static void Load(const CDM_BIND::MechanicalVentilatorConfigurationData& src, SEMechanicalVentilatorConfiguration& dst, const SESubstanceManager& subMgr);
  static CDM_BIND::MechanicalVentilatorConfigurationData* Unload(const SEMechanicalVentilatorConfiguration& src);
  static void Serialize(const CDM_BIND::MechanicalVentilatorConfigurationData& src, SEMechanicalVentilatorConfiguration& dst, const SESubstanceManager& subMgr);
  static void Serialize(const SEMechanicalVentilatorConfiguration& src, CDM_BIND::MechanicalVentilatorConfigurationData& dst);
  static void Copy(const SEMechanicalVentilatorConfiguration& src, SEMechanicalVentilatorConfiguration& dst, const SESubstanceManager& subMgr);
};
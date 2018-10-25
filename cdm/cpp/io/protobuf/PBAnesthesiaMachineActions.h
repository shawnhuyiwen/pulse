/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(AnesthesiaMachineAction)
CDM_BIND_DECL2(AnesthesiaMachineConfiguration)
CDM_BIND_DECL2(ExpiratoryValveLeak)
CDM_BIND_DECL2(ExpiratoryValveObstruction)
CDM_BIND_DECL2(InspiratoryValveLeak)
CDM_BIND_DECL2(InspiratoryValveObstruction)
CDM_BIND_DECL2(MaskLeak)
CDM_BIND_DECL2(OxygenTankPressureLoss)
CDM_BIND_DECL2(OxygenWallPortPressureLoss)
CDM_BIND_DECL2(SodaLimeFailure)
CDM_BIND_DECL2(TubeCuffLeak)
CDM_BIND_DECL2(VaporizerFailure)
CDM_BIND_DECL2(VentilatorPressureLoss)
CDM_BIND_DECL2(YPieceDisconnect)
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

  static void Load(const cdm::ExpiratoryValveLeakData& src, SEExpiratoryValveLeak& dst);
  static cdm::ExpiratoryValveLeakData* Unload(const SEExpiratoryValveLeak& src);
  static void Serialize(const cdm::ExpiratoryValveLeakData& src, SEExpiratoryValveLeak& dst);
  static void Serialize(const SEExpiratoryValveLeak& src, cdm::ExpiratoryValveLeakData& dst);
  static void Copy(const SEExpiratoryValveLeak& src, SEExpiratoryValveLeak& dst);

  static void Load(const cdm::ExpiratoryValveObstructionData& src, SEExpiratoryValveObstruction& dst);
  static cdm::ExpiratoryValveObstructionData* Unload(const SEExpiratoryValveObstruction& src);
  static void Serialize(const cdm::ExpiratoryValveObstructionData& src, SEExpiratoryValveObstruction& dst);
  static void Serialize(const SEExpiratoryValveObstruction& src, cdm::ExpiratoryValveObstructionData& dst);
  static void Copy(const SEExpiratoryValveObstruction& src, SEExpiratoryValveObstruction& dst);

  static void Load(const cdm::InspiratoryValveLeakData& src, SEInspiratoryValveLeak& dst);
  static cdm::InspiratoryValveLeakData* Unload(const SEInspiratoryValveLeak& src);
  static void Serialize(const cdm::InspiratoryValveLeakData& src, SEInspiratoryValveLeak& dst);
  static void Serialize(const SEInspiratoryValveLeak& src, cdm::InspiratoryValveLeakData& dst);
  static void Copy(const SEInspiratoryValveLeak& src, SEInspiratoryValveLeak& dst);

  static void Load(const cdm::InspiratoryValveObstructionData& src, SEInspiratoryValveObstruction& dst);
  static cdm::InspiratoryValveObstructionData* Unload(const SEInspiratoryValveObstruction& src);
  static void Serialize(const cdm::InspiratoryValveObstructionData& src, SEInspiratoryValveObstruction& dst);
  static void Serialize(const SEInspiratoryValveObstruction& src, cdm::InspiratoryValveObstructionData& dst);
  static void Copy(const SEInspiratoryValveObstruction& src, SEInspiratoryValveObstruction& dst);

  static void Load(const cdm::MaskLeakData& src, SEMaskLeak& dst);
  static cdm::MaskLeakData* Unload(const SEMaskLeak& src);
  static void Serialize(const cdm::MaskLeakData& src, SEMaskLeak& dst);
  static void Serialize(const SEMaskLeak& src, cdm::MaskLeakData& dst);
  static void Copy(const SEMaskLeak& src, SEMaskLeak& dst);

  static void Load(const cdm::OxygenTankPressureLossData& src, SEOxygenTankPressureLoss& dst);
  static cdm::OxygenTankPressureLossData* Unload(const SEOxygenTankPressureLoss& src);
  static void Serialize(const cdm::OxygenTankPressureLossData& src, SEOxygenTankPressureLoss& dst);
  static void Serialize(const SEOxygenTankPressureLoss& src, cdm::OxygenTankPressureLossData& dst);
  static void Copy(const SEOxygenTankPressureLoss& src, SEOxygenTankPressureLoss& dst);

  static void Load(const cdm::OxygenWallPortPressureLossData& src, SEOxygenWallPortPressureLoss& dst);
  static cdm::OxygenWallPortPressureLossData* Unload(const SEOxygenWallPortPressureLoss& src);
  static void Serialize(const cdm::OxygenWallPortPressureLossData& src, SEOxygenWallPortPressureLoss& dst);
  static void Serialize(const SEOxygenWallPortPressureLoss& src, cdm::OxygenWallPortPressureLossData& dst);
  static void Copy(const SEOxygenWallPortPressureLoss& src, SEOxygenWallPortPressureLoss& dst);

  static void Load(const cdm::SodaLimeFailureData& src, SESodaLimeFailure& dst);
  static cdm::SodaLimeFailureData* Unload(const SESodaLimeFailure& src);
  static void Serialize(const cdm::SodaLimeFailureData& src, SESodaLimeFailure& dst);
  static void Serialize(const SESodaLimeFailure& src, cdm::SodaLimeFailureData& dst);
  static void Copy(const SESodaLimeFailure& src, SESodaLimeFailure& dst);

  static void Load(const cdm::TubeCuffLeakData& src, SETubeCuffLeak& dst);
  static cdm::TubeCuffLeakData* Unload(const SETubeCuffLeak& src);
  static void Serialize(const cdm::TubeCuffLeakData& src, SETubeCuffLeak& dst);
  static void Serialize(const SETubeCuffLeak& src, cdm::TubeCuffLeakData& dst);
  static void Copy(const SETubeCuffLeak& src, SETubeCuffLeak& dst);

  static void Load(const cdm::VaporizerFailureData& src, SEVaporizerFailure& dst);
  static cdm::VaporizerFailureData* Unload(const SEVaporizerFailure& src);
  static void Serialize(const cdm::VaporizerFailureData& src, SEVaporizerFailure& dst);
  static void Serialize(const SEVaporizerFailure& src, cdm::VaporizerFailureData& dst);
  static void Copy(const SEVaporizerFailure& src, SEVaporizerFailure& dst);

  static void Load(const cdm::VentilatorPressureLossData& src, SEVentilatorPressureLoss& dst);
  static cdm::VentilatorPressureLossData* Unload(const SEVentilatorPressureLoss& src);
  static void Serialize(const cdm::VentilatorPressureLossData& src, SEVentilatorPressureLoss& dst);
  static void Serialize(const SEVentilatorPressureLoss& src, cdm::VentilatorPressureLossData& dst);
  static void Copy(const SEVentilatorPressureLoss& src, SEVentilatorPressureLoss& dst);

  static void Load(const cdm::YPieceDisconnectData& src, SEYPieceDisconnect& dst);
  static cdm::YPieceDisconnectData* Unload(const SEYPieceDisconnect& src);
  static void Serialize(const cdm::YPieceDisconnectData& src, SEYPieceDisconnect& dst);
  static void Serialize(const SEYPieceDisconnect& src, cdm::YPieceDisconnectData& dst);
  static void Copy(const SEYPieceDisconnect& src, SEYPieceDisconnect& dst);
};
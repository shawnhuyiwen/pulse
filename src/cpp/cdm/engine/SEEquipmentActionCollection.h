/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEAction;
class SEEquipmentAction;
class SEAnesthesiaMachine;
class SEAnesthesiaMachineAction;
class SEAnesthesiaMachineConfiguration;
class SEAnesthesiaMachineOxygenTankPressureLoss;
class SEAnesthesiaMachineOxygenWallPortPressureLoss;
class SEAnesthesiaMachineExpiratoryValveLeak;
class SEAnesthesiaMachineExpiratoryValveObstruction;
class SEAnesthesiaMachineInspiratoryValveLeak;
class SEAnesthesiaMachineInspiratoryValveObstruction;
class SEAnesthesiaMachineMaskLeak;
class SEAnesthesiaMachineSodaLimeFailure;
class SEAnesthesiaMachineTubeCuffLeak;
class SEAnesthesiaMachineVaporizerFailure;
class SEAnesthesiaMachineVentilatorPressureLoss;
class SEAnesthesiaMachineYPieceDisconnect;
class SEInhaler;
class SEInhalerAction;
class SEInhalerConfiguration;
class SEMechanicalVentilator;
class SEMechanicalVentilatorAction;
class SEMechanicalVentilatorConfiguration;
class SESubstanceManager;
CDM_BIND_DECL(ActionListData)
CDM_BIND_DECL(AnyEquipmentActionData)

class CDM_DECL SEEquipmentActionCollection : public Loggable
{
  friend class PBEngine;//friend the serialization class
  friend class SEActionManager;
protected:
  SEEquipmentActionCollection(SESubstanceManager& subMgr);
public:
  ~SEEquipmentActionCollection();

  // STATE ACTION
  bool HasAnesthesiaMachineConfiguration() const;
  SEAnesthesiaMachineConfiguration& GetAnesthesiaMachineConfiguration();
  const SEAnesthesiaMachineConfiguration* GetAnesthesiaMachineConfiguration() const;
  void RemoveAnesthesiaMachineConfiguration();

  bool HasInhalerConfiguration() const;
  SEInhalerConfiguration& GetInhalerConfiguration();
  const SEInhalerConfiguration* GetInhalerConfiguration() const;
  void RemoveInhalerConfiguration();

  bool HasMechanicalVentilatorConfiguration() const;
  SEMechanicalVentilatorConfiguration& GetMechanicalVentilatorConfiguration();
  const SEMechanicalVentilatorConfiguration* GetMechanicalVentilatorConfiguration() const;
  void RemoveMechanicalVentilatorConfiguration();

  // INCIDENT ACTIONS

  bool HasAnesthesiaMachineOxygenTankPressureLoss() const;
  SEAnesthesiaMachineOxygenTankPressureLoss& GetAnesthesiaMachineOxygenTankPressureLoss();
  const SEAnesthesiaMachineOxygenTankPressureLoss* GetAnesthesiaMachineOxygenTankPressureLoss() const;
  void RemoveAnesthesiaMachineOxygenTankPressureLoss();

  bool HasAnesthesiaMachineOxygenWallPortPressureLoss() const;
  SEAnesthesiaMachineOxygenWallPortPressureLoss& GetAnesthesiaMachineOxygenWallPortPressureLoss();
  const SEAnesthesiaMachineOxygenWallPortPressureLoss* GetAnesthesiaMachineOxygenWallPortPressureLoss() const;
  void RemoveAnesthesiaMachineOxygenWallPortPressureLoss();

  // FAILURE ACTIONS

  bool HasAnesthesiaMachineExpiratoryValveLeak() const;
  SEAnesthesiaMachineExpiratoryValveLeak& GetAnesthesiaMachineExpiratoryValveLeak();
  const SEAnesthesiaMachineExpiratoryValveLeak* GetAnesthesiaMachineExpiratoryValveLeak() const;
  void RemoveAnesthesiaMachineExpiratoryValveLeak();

  bool HasAnesthesiaMachineExpiratoryValveObstruction() const;
  SEAnesthesiaMachineExpiratoryValveObstruction& GetAnesthesiaMachineExpiratoryValveObstruction();
  const SEAnesthesiaMachineExpiratoryValveObstruction* GetAnesthesiaMachineExpiratoryValveObstruction() const;
  void RemoveAnesthesiaMachineExpiratoryValveObstruction();

  bool HasAnesthesiaMachineInspiratoryValveLeak() const;
  SEAnesthesiaMachineInspiratoryValveLeak& GetAnesthesiaMachineInspiratoryValveLeak();
  const SEAnesthesiaMachineInspiratoryValveLeak* GetAnesthesiaMachineInspiratoryValveLeak() const;
  void RemoveAnesthesiaMachineInspiratoryValveLeak();

  bool HasAnesthesiaMachineInspiratoryValveObstruction() const;
  SEAnesthesiaMachineInspiratoryValveObstruction& GetAnesthesiaMachineInspiratoryValveObstruction();
  const SEAnesthesiaMachineInspiratoryValveObstruction* GetAnesthesiaMachineInspiratoryValveObstruction() const;
  void RemoveAnesthesiaMachineInspiratoryValveObstruction();

  bool HasAnesthesiaMachineMaskLeak() const;
  SEAnesthesiaMachineMaskLeak& GetAnesthesiaMachineMaskLeak();
  const SEAnesthesiaMachineMaskLeak* GetAnesthesiaMachineMaskLeak() const;
  void RemoveAnesthesiaMachineMaskLeak();

  bool HasAnesthesiaMachineSodaLimeFailure() const;
  SEAnesthesiaMachineSodaLimeFailure& GetAnesthesiaMachineSodaLimeFailure();
  const SEAnesthesiaMachineSodaLimeFailure* GetAnesthesiaMachineSodaLimeFailure() const;
  void RemoveAnesthesiaMachineSodaLimeFailure();

  bool HasAnesthesiaMachineTubeCuffLeak() const;
  SEAnesthesiaMachineTubeCuffLeak& GetAnesthesiaMachineTubeCuffLeak();
  const SEAnesthesiaMachineTubeCuffLeak* GetAnesthesiaMachineTubeCuffLeak() const;
  void RemoveAnesthesiaMachineTubeCuffLeak();

  bool HasAnesthesiaMachineVaporizerFailure() const;
  SEAnesthesiaMachineVaporizerFailure& GetAnesthesiaMachineVaporizerFailure();
  const SEAnesthesiaMachineVaporizerFailure* GetAnesthesiaMachineVaporizerFailure() const;
  void RemoveAnesthesiaMachineVaporizerFailure();

  bool HasAnesthesiaMachineVentilatorPressureLoss() const;
  SEAnesthesiaMachineVentilatorPressureLoss& GetAnesthesiaMachineVentilatorPressureLoss();
  const SEAnesthesiaMachineVentilatorPressureLoss* GetAnesthesiaMachineVentilatorPressureLoss() const;
  void RemoveAnesthesiaMachineVentilatorPressureLoss();

  bool HasAnesthesiaMachineYPieceDisconnect() const;
  SEAnesthesiaMachineYPieceDisconnect& GetAnesthesiaMachineYPieceDisconnect();
  const SEAnesthesiaMachineYPieceDisconnect* GetAnesthesiaMachineYPieceDisconnect() const;
  void RemoveAnesthesiaMachineYPieceDisconnect();

  void GetAllActions(std::vector<const SEAction*>& v) const;
  const SEScalar* GetScalar(const std::string& actionName, const std::string& cmptName, const std::string& substance, const std::string& property);
  
protected:
  void Clear();
  bool ProcessAction(const SEEquipmentAction& action);

  SESubstanceManager&                                          m_SubMgr;

  SEAnesthesiaMachineConfiguration*                            m_AnesthesiaMachineConfiguration;
  //Anesthesia Machine Incidents
  SEAnesthesiaMachineOxygenTankPressureLoss*                   m_AnesthesiaMachineOxygenTankPressureLoss;
  SEAnesthesiaMachineOxygenWallPortPressureLoss*               m_AnesthesiaMachineOxygenWallPortPressureLoss;
  //Anesthesia Machine Failures
  SEAnesthesiaMachineExpiratoryValveLeak*                      m_AnesthesiaMachineExpiratoryValveLeak;
  SEAnesthesiaMachineExpiratoryValveObstruction*               m_AnesthesiaMachineExpiratoryValveObstruction;
  SEAnesthesiaMachineInspiratoryValveLeak*                     m_AnesthesiaMachineInspiratoryValveLeak;
  SEAnesthesiaMachineInspiratoryValveObstruction*              m_AnesthesiaMachineInspiratoryValveObstruction;
  SEAnesthesiaMachineMaskLeak*                                 m_AnesthesiaMachineMaskLeak;
  SEAnesthesiaMachineSodaLimeFailure*                          m_AnesthesiaMachineSodaLimeFailure;
  SEAnesthesiaMachineTubeCuffLeak*                             m_AnesthesiaMachineTubeCuffLeak;
  SEAnesthesiaMachineVaporizerFailure*                         m_AnesthesiaMachineVaporizerFailure;
  SEAnesthesiaMachineVentilatorPressureLoss*                   m_AnesthesiaMachineVentilatorPressureLoss;
  SEAnesthesiaMachineYPieceDisconnect*                         m_AnesthesiaMachineYPieceDisconnect;


  SEInhalerConfiguration*                                      m_InhalerConfiguration;

  SEMechanicalVentilatorConfiguration*                         m_MechanicalVentilatorConfiguration;
};

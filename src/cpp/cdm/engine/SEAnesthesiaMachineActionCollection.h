/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEAction;
class SESubstanceManager;
class SEAnesthesiaMachine;
class SEAnesthesiaMachineAction;
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
class SEAnesthesiaMachineConfiguration;
CDM_BIND_DECL(ActionListData)
CDM_BIND_DECL(AnyAnesthesiaMachineActionData)

class CDM_DECL SEAnesthesiaMachineActionCollection : public Loggable
{
  friend class PBEngine;//friend the serialization class
  friend class SEActionManager;
protected:
  SEAnesthesiaMachineActionCollection(SESubstanceManager&);
public:
  ~SEAnesthesiaMachineActionCollection();

  // STATE ACTION
  bool HasConfiguration() const;
  SEAnesthesiaMachineConfiguration* GetConfiguration();
  const SEAnesthesiaMachineConfiguration* GetConfiguration() const;
  void RemoveConfiguration();

  // INCIDENT ACTIONS

  bool HasOxygenTankPressureLoss() const;
  SEAnesthesiaMachineOxygenTankPressureLoss* GetOxygenTankPressureLoss();
  const SEAnesthesiaMachineOxygenTankPressureLoss* GetOxygenTankPressureLoss() const;
  void RemoveOxygenTankPressureLoss();

  bool HasOxygenWallPortPressureLoss() const;
  SEAnesthesiaMachineOxygenWallPortPressureLoss* GetOxygenWallPortPressureLoss();
  const SEAnesthesiaMachineOxygenWallPortPressureLoss* GetOxygenWallPortPressureLoss() const;
  void RemoveOxygenWallPortPressureLoss();

  // FAILURE ACTIONS

  bool HasExpiratoryValveLeak() const;
  SEAnesthesiaMachineExpiratoryValveLeak* GetExpiratoryValveLeak();
  const SEAnesthesiaMachineExpiratoryValveLeak* GetExpiratoryValveLeak() const;
  void RemoveExpiratoryValveLeak();

  bool HasExpiratoryValveObstruction() const;
  SEAnesthesiaMachineExpiratoryValveObstruction* GetExpiratoryValveObstruction();
  const SEAnesthesiaMachineExpiratoryValveObstruction* GetExpiratoryValveObstruction() const;
  void RemoveExpiratoryValveObstruction();

  bool HasInspiratoryValveLeak() const;
  SEAnesthesiaMachineInspiratoryValveLeak* GetInspiratoryValveLeak();
  const SEAnesthesiaMachineInspiratoryValveLeak* GetInspiratoryValveLeak() const;
  void RemoveInspiratoryValveLeak();

  bool HasInspiratoryValveObstruction() const;
  SEAnesthesiaMachineInspiratoryValveObstruction* GetInspiratoryValveObstruction();
  const SEAnesthesiaMachineInspiratoryValveObstruction* GetInspiratoryValveObstruction() const;
  void RemoveInspiratoryValveObstruction();

  bool HasMaskLeak() const;
  SEAnesthesiaMachineMaskLeak* GetMaskLeak();
  const SEAnesthesiaMachineMaskLeak* GetMaskLeak() const;
  void RemoveMaskLeak();

  bool HasSodaLimeFailure() const;
  SEAnesthesiaMachineSodaLimeFailure* GetSodaLimeFailure();
  const SEAnesthesiaMachineSodaLimeFailure* GetSodaLimeFailure() const;
  void RemoveSodaLimeFailure();

  bool HasTubeCuffLeak() const;
  SEAnesthesiaMachineTubeCuffLeak* GetTubeCuffLeak();
  const SEAnesthesiaMachineTubeCuffLeak* GetTubeCuffLeak() const;
  void RemoveTubeCuffLeak();

  bool HasVaporizerFailure() const;
  SEAnesthesiaMachineVaporizerFailure* GetVaporizerFailure();
  const SEAnesthesiaMachineVaporizerFailure* GetVaporizerFailure() const;
  void RemoveVaporizerFailure();

  bool HasVentilatorPressureLoss() const;
  SEAnesthesiaMachineVentilatorPressureLoss* GetVentilatorPressureLoss();
  const SEAnesthesiaMachineVentilatorPressureLoss* GetVentilatorPressureLoss() const;
  void RemoveVentilatorPressureLoss();

  bool HasYPieceDisconnect() const;
  SEAnesthesiaMachineYPieceDisconnect* GetYPieceDisconnect();
  const SEAnesthesiaMachineYPieceDisconnect* GetYPieceDisconnect() const;
  void RemoveYPieceDisconnect();

  void GetAllActions(std::vector<const SEAction*>& v) const;
  
protected:
  void Clear();
  bool ProcessAction(const SEAnesthesiaMachineAction& action);

  SEAnesthesiaMachineConfiguration*           m_Configuration;
  //Anesthesia Machine Incidents
  SEAnesthesiaMachineOxygenTankPressureLoss*                   m_OxygenTankPressureLoss;
  SEAnesthesiaMachineOxygenWallPortPressureLoss*               m_OxygenWallPortPressureLoss;
  //Anesthesia Machine Failures
  SEAnesthesiaMachineExpiratoryValveLeak*                      m_ExpiratoryValveLeak;
  SEAnesthesiaMachineExpiratoryValveObstruction*               m_ExpiratoryValveObstruction;
  SEAnesthesiaMachineInspiratoryValveLeak*                     m_InspiratoryValveLeak;
  SEAnesthesiaMachineInspiratoryValveObstruction*              m_InspiratoryValveObstruction;
  SEAnesthesiaMachineMaskLeak*                                 m_MaskLeak;
  SEAnesthesiaMachineSodaLimeFailure*                          m_SodaLimeFailure;
  SEAnesthesiaMachineTubeCuffLeak*                             m_TubeCuffLeak;
  SEAnesthesiaMachineVaporizerFailure*                         m_VaporizerFailure;
  SEAnesthesiaMachineVentilatorPressureLoss*                   m_VentilatorPressureLoss;
  SEAnesthesiaMachineYPieceDisconnect*                         m_YPieceDisconnect;
  // General
  SESubstanceManager& m_Substances;
};

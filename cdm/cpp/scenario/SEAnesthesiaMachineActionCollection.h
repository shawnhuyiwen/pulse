/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

class SESubstanceManager;
class SEAnesthesiaMachine;
class SEAnesthesiaMachineAction;
class SEOxygenTankPressureLoss;
class SEOxygenWallPortPressureLoss;
class SEExpiratoryValveLeak;
class SEExpiratoryValveObstruction;
class SEInspiratoryValveLeak;
class SEInspiratoryValveObstruction;
class SEMaskLeak;
class SESodaLimeFailure;
class SETubeCuffLeak;
class SEVaporizerFailure;
class SEVentilatorPressureLoss;
class SEYPieceDisconnect;
class SEAnesthesiaMachineConfiguration;
CDM_BIND_DECL(ActionListData)
CDM_BIND_DECL(AnyAnesthesiaMachineActionData)

class CDM_DECL SEAnesthesiaMachineActionCollection : public Loggable
{
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
  SEOxygenTankPressureLoss* GetOxygenTankPressureLoss();
  const SEOxygenTankPressureLoss* GetOxygenTankPressureLoss() const;
  void RemoveOxygenTankPressureLoss();

  bool HasOxygenWallPortPressureLoss() const;
  SEOxygenWallPortPressureLoss* GetOxygenWallPortPressureLoss();
  const SEOxygenWallPortPressureLoss* GetOxygenWallPortPressureLoss() const;
  void RemoveOxygenWallPortPressureLoss();

  // FAILURE ACTIONS

  bool HasExpiratoryValveLeak() const;
  SEExpiratoryValveLeak* GetExpiratoryValveLeak();
  const SEExpiratoryValveLeak* GetExpiratoryValveLeak() const;
  void RemoveExpiratoryValveLeak();

  bool HasExpiratoryValveObstruction() const;
  SEExpiratoryValveObstruction* GetExpiratoryValveObstruction();
  const SEExpiratoryValveObstruction* GetExpiratoryValveObstruction() const;
  void RemoveExpiratoryValveObstruction();

  bool HasInspiratoryValveLeak() const;
  SEInspiratoryValveLeak* GetInspiratoryValveLeak();
  const SEInspiratoryValveLeak* GetInspiratoryValveLeak() const;
  void RemoveInspiratoryValveLeak();

  bool HasInspiratoryValveObstruction() const;
  SEInspiratoryValveObstruction* GetInspiratoryValveObstruction();
  const SEInspiratoryValveObstruction* GetInspiratoryValveObstruction() const;
  void RemoveInspiratoryValveObstruction();

  bool HasMaskLeak() const;
  SEMaskLeak* GetMaskLeak();
  const SEMaskLeak* GetMaskLeak() const;
  void RemoveMaskLeak();

  bool HasSodaLimeFailure() const;
  SESodaLimeFailure* GetSodaLimeFailure();
  const SESodaLimeFailure* GetSodaLimeFailure() const;
  void RemoveSodaLimeFailure();

  bool HasTubeCuffLeak() const;
  SETubeCuffLeak* GetTubeCuffLeak();
  const SETubeCuffLeak* GetTubeCuffLeak() const;
  void RemoveTubeCuffLeak();

  bool HasVaporizerFailure() const;
  SEVaporizerFailure* GetVaporizerFailure();
  const SEVaporizerFailure* GetVaporizerFailure() const;
  void RemoveVaporizerFailure();

  bool HasVentilatorPressureLoss() const;
  SEVentilatorPressureLoss* GetVentilatorPressureLoss();
  const SEVentilatorPressureLoss* GetVentilatorPressureLoss() const;
  void RemoveVentilatorPressureLoss();

  bool HasYPieceDisconnect() const;
  SEYPieceDisconnect* GetYPieceDisconnect();
  const SEYPieceDisconnect* GetYPieceDisconnect() const;
  void RemoveYPieceDisconnect();
  
protected:
  void Clear();
  static void Serialize(const SEAnesthesiaMachineActionCollection& src, cdm::ActionListData& dst);
  bool ProcessAction(const SEAnesthesiaMachineAction& action, cdm::AnyAnesthesiaMachineActionData* any);

  SEAnesthesiaMachineConfiguration*           m_Configuration;
  //Anesthesia Machine Incidents
  SEOxygenTankPressureLoss*                   m_OxygenTankPressureLoss;
  SEOxygenWallPortPressureLoss*               m_OxygenWallPortPressureLoss;
  //Anesthesia Machine Failures
  SEExpiratoryValveLeak*                      m_ExpiratoryValveLeak;
  SEExpiratoryValveObstruction*               m_ExpiratoryValveObstruction;
  SEInspiratoryValveLeak*                     m_InspiratoryValveLeak;
  SEInspiratoryValveObstruction*              m_InspiratoryValveObstruction;
  SEMaskLeak*                                 m_MaskLeak;
  SESodaLimeFailure*                          m_SodaLimeFailure;
  SETubeCuffLeak*                             m_TubeCuffLeak;
  SEVaporizerFailure*                         m_VaporizerFailure;
  SEVentilatorPressureLoss*                   m_VentilatorPressureLoss;
  SEYPieceDisconnect*                         m_YPieceDisconnect;
  // General
  SESubstanceManager& m_Substances;
};

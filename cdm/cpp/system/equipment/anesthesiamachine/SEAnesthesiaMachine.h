/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "system/SESystem.h"
class SEEventHandler;
class SESubstanceManager;
class SEAnesthesiaMachineChamber;
class SEAnesthesiaMachineOxygenBottle;
class SEAnesthesiaMachineConfiguration;
class SEGasSubstanceQuantity;
CDM_BIND_DECL(AnesthesiaMachineData)
PROTO_PUSH
#include "bind/cdm/AnesthesiaMachineEnums.pb.h"
PROTO_POP


class CDM_DECL SEAnesthesiaMachine : public SESystem
{
protected:
  friend SEAnesthesiaMachineConfiguration;
public:

  SEAnesthesiaMachine(SESubstanceManager& substances);
  virtual ~SEAnesthesiaMachine();

  virtual void Clear();

  static void Load(const cdm::AnesthesiaMachineData& src, SEAnesthesiaMachine& dst);
  static cdm::AnesthesiaMachineData* Unload(const SEAnesthesiaMachine& src);
protected:
  static void Serialize(const cdm::AnesthesiaMachineData& src, SEAnesthesiaMachine& dst);
  static void Serialize(const SEAnesthesiaMachine& src, cdm::AnesthesiaMachineData& dst);

  /** @name StateChange
  *   @brief - This method is called when ever there is a state change
  *            Specically a new file has been loaded, configuration action, or the system reset
  *            Engine specific methodology can then update their logic.
  */
  virtual void StateChange(){};
  virtual void Merge(const SEAnesthesiaMachine& from);
  virtual void ProcessConfiguration(const SEAnesthesiaMachineConfiguration& config);
public:

  bool LoadFile(const std::string& anesthesiaMachineFile);

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual const std::map<cdm::eAnesthesiaMachine_Event, bool>& GetEventStates() const { return m_EventState; }
  virtual void SetEvent(cdm::eAnesthesiaMachine_Event state, bool active, const SEScalarTime& time);
  virtual bool IsEventActive(cdm::eAnesthesiaMachine_Event state) const;
  virtual double GetEventDuration(cdm::eAnesthesiaMachine_Event type, const TimeUnit& unit) const;
  virtual void UpdateEvents(const SEScalarTime& timeStep);
  /** @name ForwardEvents
  *  @brief - Set a callback class to invoke when any event changes
  *  @details - Note that the handler callback can and will be called in the middle of a time step
  *             So system and compartment objects may not be completely up to date when called.
  *             Use the PhysiologyEngineInterface::SetEventHandler to ensure that all engine
  *             data is up to date at the time the callback is invoked
  */
  virtual void ForwardEvents(SEEventHandler* handler);

  virtual cdm::eAnesthesiaMachine_Connection GetConnection() const;
  virtual void SetConnection(cdm::eAnesthesiaMachine_Connection c);

  virtual bool HasInletFlow() const;
  virtual SEScalarVolumePerTime& GetInletFlow();
  virtual double GetInletFlow(const VolumePerTimeUnit& unit) const;
  
  virtual bool HasInspiratoryExpiratoryRatio() const;
  virtual SEScalar& GetInspiratoryExpiratoryRatio();
  virtual double GetInspiratoryExpiratoryRatio() const;
    
  virtual bool HasOxygenFraction() const;
  virtual SEScalar0To1& GetOxygenFraction();
  virtual double GetOxygenFraction() const;

  virtual cdm::eAnesthesiaMachine_OxygenSource GetOxygenSource() const;
  virtual void SetOxygenSource(cdm::eAnesthesiaMachine_OxygenSource name);

  virtual bool HasPositiveEndExpiredPressure() const;
  virtual SEScalarPressure& GetPositiveEndExpiredPressure();
  virtual double GetPositiveEndExpiredPressure(const PressureUnit& unit) const;
  
  virtual cdm::eAnesthesiaMachine_PrimaryGas GetPrimaryGas() const;
  virtual void SetPrimaryGas(cdm::eAnesthesiaMachine_PrimaryGas name);
  
  virtual bool HasRespiratoryRate() const;
  virtual SEScalarFrequency& GetRespiratoryRate();
  virtual double GetRespiratoryRate(const FrequencyUnit& unit) const;

  virtual bool HasReliefValvePressure() const;
  virtual SEScalarPressure& GetReliefValvePressure();
  virtual double GetReliefValvePressure(const PressureUnit& unit) const;

  virtual bool HasVentilatorPressure() const;
  virtual SEScalarPressure& GetVentilatorPressure();
  virtual double GetVentilatorPressure(const PressureUnit& unit) const;

  virtual bool HasLeftChamber() const;
  virtual SEAnesthesiaMachineChamber& GetLeftChamber();
  virtual const SEAnesthesiaMachineChamber* GetLeftChamber() const;
  virtual void RemoveLeftChamber();

  virtual bool HasRightChamber() const;
  virtual SEAnesthesiaMachineChamber& GetRightChamber();
  virtual const SEAnesthesiaMachineChamber* GetRightChamber() const;
  virtual void RemoveRightChamber();

  virtual bool HasOxygenBottleOne() const;
  virtual SEAnesthesiaMachineOxygenBottle& GetOxygenBottleOne();
  virtual const SEAnesthesiaMachineOxygenBottle* GetOxygenBottleOne() const;
  virtual void RemoveOxygenBottleOne();

  virtual bool HasOxygenBottleTwo() const;
  virtual SEAnesthesiaMachineOxygenBottle& GetOxygenBottleTwo();
  virtual const SEAnesthesiaMachineOxygenBottle* GetOxygenBottleTwo() const;
  virtual void RemoveOxygenBottleTwo();

protected:

  SEEventHandler*                                        m_EventHandler;
  std::map<cdm::eAnesthesiaMachine_Event, bool>          m_EventState;
  std::map<cdm::eAnesthesiaMachine_Event, double>        m_EventDuration_s;

  cdm::eAnesthesiaMachine_Connection                     m_Connection;
  SEScalarVolumePerTime*                                 m_InletFlow;
  SEScalar*                                              m_InspiratoryExpiratoryRatio;
  SEScalar0To1*                                          m_OxygenFraction;  
  cdm::eAnesthesiaMachine_OxygenSource                   m_OxygenSource;
  SEScalarPressure*                                      m_PositiveEndExpiredPressure;  
  cdm::eAnesthesiaMachine_PrimaryGas                     m_PrimaryGas;
  SEScalarFrequency*                                     m_RespiratoryRate;
  SEScalarPressure*                                      m_ReliefValvePressure;

  SEScalarPressure*                                      m_VentilatorPressure;

  SEAnesthesiaMachineChamber*                            m_LeftChamber;
  SEAnesthesiaMachineChamber*                            m_RightChamber;

  SEAnesthesiaMachineOxygenBottle*                       m_OxygenBottleOne;
  SEAnesthesiaMachineOxygenBottle*                       m_OxygenBottleTwo;

  SESubstanceManager&                                    m_Substances;
};

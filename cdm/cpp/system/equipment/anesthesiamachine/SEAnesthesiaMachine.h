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

// Keep enums in sync with appropriate schema/cdm/AnesthesiaMachineEnums.proto file !!
enum class eAnesthesiaMachine_Event { OxygenBottleOneExhausted = 0, OxygenBottleTwoExhausted, ReliefValveActive };
extern const std::string& eAnesthesiaMachine_Event_Name(eAnesthesiaMachine_Event m);

// Keep enums in sync with appropriate schema/cdm/AnesthesiaMachineEnums.proto file !!
enum class eAnesthesiaMachine_OxygenSource { NullSource = 0, NoSource, Wall, BottleOne, BottleTwo };
extern const std::string& eAnesthesiaMachine_OxygenSource_Name(eAnesthesiaMachine_OxygenSource m);

// Keep enums in sync with appropriate schema/cdm/AnesthesiaMachineEnums.proto file !!
enum class eAnesthesiaMachine_PrimaryGas { NullGas = 0, NoGas, Air, Nitrogen };
extern const std::string& eAnesthesiaMachine_PrimaryGas_Name(eAnesthesiaMachine_PrimaryGas m);

// Keep enums in sync with appropriate schema/cdm/AnesthesiaMachineEnums.proto file !!
enum class eAnesthesiaMachine_Connection { NullConnection = 0, Off, Mask, Tube };
extern const std::string& eAnesthesiaMachine_Connection_Name(eAnesthesiaMachine_Connection m);


class CDM_DECL SEAnesthesiaMachine : public SESystem
{
  friend class PBAnesthesiaMachine;//friend the serialization class
protected:
  friend SEAnesthesiaMachineConfiguration;
public:

  SEAnesthesiaMachine(SESubstanceManager& substances);
  virtual ~SEAnesthesiaMachine();

  virtual void Clear();

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename, SerializationFormat m) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m);
  bool SerializeFromFile(const std::string& filename, SerializationFormat m);

protected:

  /** @name StateChange
  *   @brief - This method is called when ever there is a state change
  *            Specically a new file has been loaded, configuration action, or the system reset
  *            Engine specific methodology can then update their logic.
  */
  virtual void StateChange(){};
  virtual void Merge(const SEAnesthesiaMachine& from);
  virtual void ProcessConfiguration(SEAnesthesiaMachineConfiguration& config);

public:

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual const std::map<eAnesthesiaMachine_Event, bool>& GetEventStates() const { return m_EventState; }
  virtual void SetEvent(eAnesthesiaMachine_Event state, bool active, const SEScalarTime& time);
  virtual bool IsEventActive(eAnesthesiaMachine_Event state) const;
  virtual double GetEventDuration(eAnesthesiaMachine_Event type, const TimeUnit& unit) const;
  virtual void UpdateEvents(const SEScalarTime& timeStep);
  /** @name ForwardEvents
  *  @brief - Set a callback class to invoke when any event changes
  *  @details - Note that the handler callback can and will be called in the middle of a time step
  *             So system and compartment objects may not be completely up to date when called.
  *             Use the PhysiologyEngineInterface::SetEventHandler to ensure that all engine
  *             data is up to date at the time the callback is invoked
  */
  virtual void ForwardEvents(SEEventHandler* handler);

  virtual eAnesthesiaMachine_Connection GetConnection() const;
  virtual void SetConnection(eAnesthesiaMachine_Connection c);

  virtual bool HasInletFlow() const;
  virtual SEScalarVolumePerTime& GetInletFlow();
  virtual double GetInletFlow(const VolumePerTimeUnit& unit) const;
  
  virtual bool HasInspiratoryExpiratoryRatio() const;
  virtual SEScalar& GetInspiratoryExpiratoryRatio();
  virtual double GetInspiratoryExpiratoryRatio() const;
    
  virtual bool HasOxygenFraction() const;
  virtual SEScalar0To1& GetOxygenFraction();
  virtual double GetOxygenFraction() const;

  virtual eAnesthesiaMachine_OxygenSource GetOxygenSource() const;
  virtual void SetOxygenSource(eAnesthesiaMachine_OxygenSource name);

  virtual bool HasPositiveEndExpiredPressure() const;
  virtual SEScalarPressure& GetPositiveEndExpiredPressure();
  virtual double GetPositiveEndExpiredPressure(const PressureUnit& unit) const;
  
  virtual eAnesthesiaMachine_PrimaryGas GetPrimaryGas() const;
  virtual void SetPrimaryGas(eAnesthesiaMachine_PrimaryGas name);
  
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
  std::map<eAnesthesiaMachine_Event, bool>               m_EventState;
  std::map<eAnesthesiaMachine_Event, double>             m_EventDuration_s;

  eAnesthesiaMachine_Connection                          m_Connection;
  SEScalarVolumePerTime*                                 m_InletFlow;
  SEScalar*                                              m_InspiratoryExpiratoryRatio;
  SEScalar0To1*                                          m_OxygenFraction;  
  eAnesthesiaMachine_OxygenSource                        m_OxygenSource;
  SEScalarPressure*                                      m_PositiveEndExpiredPressure;  
  eAnesthesiaMachine_PrimaryGas                          m_PrimaryGas;
  SEScalarFrequency*                                     m_RespiratoryRate;
  SEScalarPressure*                                      m_ReliefValvePressure;

  SEScalarPressure*                                      m_VentilatorPressure;

  SEAnesthesiaMachineChamber*                            m_LeftChamber;
  SEAnesthesiaMachineChamber*                            m_RightChamber;

  SEAnesthesiaMachineOxygenBottle*                       m_OxygenBottleOne;
  SEAnesthesiaMachineOxygenBottle*                       m_OxygenBottleTwo;

  SESubstanceManager&                                    m_Substances;
};

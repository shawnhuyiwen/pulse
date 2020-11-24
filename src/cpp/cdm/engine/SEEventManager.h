/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEScalarTime.h"

// Keep enums in sync with appropriate schema/cdm/Event.proto file !!
enum class eEvent
{
  Antidiuresis = 0,
  Asystole = 1,
  Bradycardia = 2,
  Bradypnea = 3,
  BrainOxygenDeficit = 4,
  CardiacArrest = 5,
  CardiogenicShock = 6,
  CardiovascularCollapse = 7,
  CriticalBrainOxygenDeficit = 8,
  Dehydration = 9,
  Diuresis = 10,
  Fasciculation = 11,
  Fatigue = 12,
  FunctionalIncontinence = 13,
  Hypercapnia = 14,
  Hyperglycemia = 15,
  Hyperthermia = 16,
  Hypoglycemia = 17,
  Hypothermia = 18,
  Hypoxia = 19,
  HypovolemicShock = 20,
  IntracranialHypertension = 21,
  IntracranialHypotension = 22,
  IrreversibleState = 23,
  Ketoacidosis = 24,
  LacticAcidosis = 25,
  MaximumPulmonaryVentilationRate = 26,
  MetabolicAcidosis = 27,
  MetabolicAlkalosis = 28,
  ModerateHyperoxemia = 29,
  ModerateHypocapnia = 30,
  MyocardiumOxygenDeficit = 31,
  Natriuresis = 32,
  NutritionDepleted = 33,
  RenalHypoperfusion = 34,
  RespiratoryAcidosis = 35,
  RespiratoryAlkalosis = 36,
  SevereHyperoxemia = 37,
  SevereHypocapnia = 38,
  StartOfCardiacCycle = 39,
  StartOfExhale = 40,
  StartOfInhale = 41,
  Tachycardia = 42,
  Tachypnea = 43,

  // Equipment
  AnesthesiaMachineOxygenBottleOneExhausted = 1000, 
  AnesthesiaMachineOxygenBottleTwoExhausted = 1001,
  AnesthesiaMachineReliefValveActive = 1002,
  SupplementalOxygenBottleExhausted = 1003,
  NonRebreatherMaskOxygenBagEmpty = 1004
};
extern const std::string& eEvent_Name(eEvent m);

class CDM_DECL SEEventHandler
{
public:
  SEEventHandler() {};
  virtual ~SEEventHandler() {};

  virtual void HandleEvent(eEvent e, bool active, const SEScalarTime* simTime = nullptr) = 0;
};

class CDM_DECL SEActiveEvent
{
public:
  SEActiveEvent(eEvent e, const SEScalarTime& duration);
  SEActiveEvent(eEvent e, double duration, const TimeUnit& unit);

  static bool SerializeToString(std::vector<const SEActiveEvent*>& active, std::string& output, SerializationFormat m, Logger* logger);
  static bool SerializeFromString(const std::string& src, std::vector<const SEActiveEvent*>& active, SerializationFormat m, Logger* logger);

  eEvent GetEvent() const { return m_Event; }
  const SEScalarTime& GetDuration() const { return m_Duration; }

protected:
  eEvent       m_Event;
  SEScalarTime m_Duration;
};

class CDM_DECL SEEventChange
{
public:
  SEEventChange(eEvent e, bool active, const SEScalarTime* simTime = nullptr);

  static bool SerializeToString(std::vector<const SEEventChange*>& changes, std::string& output, SerializationFormat m, Logger* logger);
  static bool SerializeFromString(const std::string& src, std::vector<const SEEventChange*>& changes, SerializationFormat m, Logger* logger);

  eEvent GetEvent() const { return m_Event; }
  bool   GetActive() const { return m_Active; }
  const SEScalarTime& GetSimTime() const { return m_SimTime; }

protected:
  bool         m_Active;
  eEvent       m_Event;
  SEScalarTime m_SimTime;
};

class CDM_DECL SEEventManager : public Loggable
{
public:
  SEEventManager(Logger* logger);
  virtual ~SEEventManager();

  void Clear();
  // Essentially a load, this will set the state to active and set its duration
  virtual void OverrideActiveState(eEvent state, const SEScalarTime& duration);

  virtual const std::map<eEvent, bool>& GetEventStates() const { return m_EventState; }
  virtual void SetEvent(eEvent e, bool active, const SEScalarTime& time);
  virtual bool IsEventActive(eEvent e) const;
  virtual double GetEventDuration(eEvent e, const TimeUnit& unit) const;
  virtual void UpdateEvents(const SEScalarTime& timeStep);
  virtual bool GetActiveEvents(std::vector<const SEActiveEvent*>& active) const;
  /** @name ForwardEvents
   *  @brief - Set a callback class to invoke when any event changes
   *  @details - Note that the handler callback can and will be called in the middle of a time step
   *             So system and compartment objects may not be completely up to date when called.
   *             Use the PhysiologyEngineInterface::SetEventHandler to ensure that all engine
   *             data is up to date at the time the callback is invoked
   */
  virtual void ForwardEvents(SEEventHandler* handler) const;
  virtual SEEventHandler* GetEventHandler() { return m_EventHandler; }

protected:

  std::stringstream        m_ss;
  mutable SEEventHandler*  m_EventHandler;
  std::map<eEvent, bool>   m_EventState;
  std::map<eEvent, double> m_EventDuration_s;
};

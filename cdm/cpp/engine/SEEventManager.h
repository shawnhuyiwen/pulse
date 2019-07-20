/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

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
  CriticalBrainOxygenDeficit = 7,
  Dehydration = 8,
  Diuresis = 9,
  Fasciculation = 10,
  Fatigue = 11,
  FunctionalIncontinence = 12,
  Hypercapnia = 13,
  Hyperglycemia = 14,
  Hyperthermia = 15,
  Hypoglycemia = 16,
  Hypothermia = 17,
  Hypoxia = 18,
  HypovolemicShock = 19,
  IntracranialHypertension = 20,
  IntracranialHypotension = 21,
  IrreversibleState = 22,
  Ketoacidosis = 23,
  LacticAcidosis = 24,
  MaximumPulmonaryVentilationRate = 25,
  MetabolicAcidosis = 26,
  MetabolicAlkalosis = 27,
  MildAcuteRespiratoryDistress = 28,
  ModerateAcuteRespiratoryDistress = 29,
  MyocardiumOxygenDeficit = 30,
  Natriuresis = 31,
  NutritionDepleted = 32,
  RenalHypoperfusion = 33,
  RespiratoryAcidosis = 34,
  RespiratoryAlkalosis = 35,
  StartOfCardiacCycle = 36,
  StartOfExhale = 37,
  StartOfInhale = 38,
  SevereAcuteRespiratoryDistress = 39,
  Tachycardia = 40,
  Tachypnea = 41,

  // Equipment
  AnesthesiaMachineOxygenBottleOneExhausted = 1000, 
  AnesthesiaMachineOxygenBottleTwoExhausted = 1001,
  AnesthesiaMachineReliefValveActive = 1002
};
extern const std::string& eEvent_Name(eEvent m);

class CDM_DECL SEEventHandler
{
public:
  SEEventHandler() {};
  virtual ~SEEventHandler() {};

  virtual void HandleEvent(eEvent type, bool active, const SEScalarTime* time = nullptr) = 0;
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

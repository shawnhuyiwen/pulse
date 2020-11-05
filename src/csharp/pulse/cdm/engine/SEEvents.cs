/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eEvent : int
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
  }

  public interface IEventHandler
  {
    void HandleEvent(SEEventChange change);
  }

  public class SEActiveEvent
  {
    public eEvent Event { get; }
    public SEScalarTime Duration { get; }

    public SEActiveEvent(eEvent e, SEScalarTime duration)
    {
      Event = e;
      Duration = new SEScalarTime(duration);
    }

    public new string ToString()
    {
      return Event + " has been active for " + Duration.ToString();
    }
  }

  public class SEEventChange
  {
    public eEvent Event { get; }
    public bool Active { get; }
    public SEScalarTime SimTime { get; }

    public SEEventChange(eEvent e, bool active, SEScalarTime simTime)
    {
      Event = e;
      Active = active;
      SimTime = new SEScalarTime(simTime);
    }

    public new string ToString()
    {
      string str;
      str = Event + " was " + Active;
      if (SimTime.IsValid())
        str += " @ " + SimTime.ToString();
      return str;
    }
  }
}

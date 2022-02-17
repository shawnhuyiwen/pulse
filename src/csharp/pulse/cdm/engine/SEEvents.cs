/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eEvent : int
  {
    Antidiuresis = 0,
    Bradycardia = 1,
    Bradypnea = 2,
    BrainOxygenDeficit = 3,
    CardiacArrest = 4,
    CardiogenicShock = 5,
    CardiovascularCollapse = 6,
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
    ModerateHyperoxemia = 28,
    ModerateHypocapnia = 29,
    MyocardiumOxygenDeficit = 30,
    Natriuresis = 31,
    NutritionDepleted = 32,
    RenalHypoperfusion = 33,
    RespiratoryAcidosis = 34,
    RespiratoryAlkalosis = 35,
    SevereHyperoxemia = 36,
    SevereHypocapnia = 37,
    StartOfCardiacCycle = 38,
    StartOfExhale = 39,
    StartOfInhale = 40,
    Tachycardia = 41,
    Tachypnea = 42,

    // Equipment
    AnesthesiaMachineOxygenBottleOneExhausted = 1000,
    AnesthesiaMachineOxygenBottleTwoExhausted = 1001,
    AnesthesiaMachineReliefValveActive = 1002,
    MechanicalVentilatorReliefValveActive = 1003,
    SupplementalOxygenBottleExhausted = 1004,
    NonRebreatherMaskOxygenBagEmpty = 1005
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

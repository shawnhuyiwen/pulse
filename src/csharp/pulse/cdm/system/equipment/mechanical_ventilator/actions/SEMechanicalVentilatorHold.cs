/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEMechanicalVentilatorHold : SEMechanicalVentilatorAction
  {
    protected eSwitch state;
    protected eAppliedRespiratoryCycle appliedRespiratoryCycle;

    public SEMechanicalVentilatorHold()
    {
      Clear();
    }

    public SEMechanicalVentilatorHold(SEMechanicalVentilatorHold other)
    {
      Copy(other);
    }

    public void copy(SEMechanicalVentilatorHold other)
    {
      base.Copy(other);
      state = other.state;
      appliedRespiratoryCycle = other.appliedRespiratoryCycle;
    }

    public override void Clear()
    {
      base.Clear();
      appliedRespiratoryCycle = eAppliedRespiratoryCycle.Expiratory;
      state = eSwitch.Off;
    }

    public override bool IsValid()
    {
      return true;
    }

    public eSwitch GetState()
    {
      return state;
    }
    public void SetState(eSwitch s)
    {
      state = s;
    }

    public eAppliedRespiratoryCycle GetAppliedRespiratoryCycle()
    {
      return appliedRespiratoryCycle;
    }
    public void SetAppliedRespiratoryCycle(eAppliedRespiratoryCycle c)
    {
      appliedRespiratoryCycle = c;
    }

    public override string ToString()
    {
      string str = "Mechanical Ventilator Hold";
      str += "\n\tState: " + this.state;
      str += "\n\tApplied Respiratory Cycle: " + this.appliedRespiratoryCycle;

      return str;
    }
  }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SEAnesthesiaMachineOxygenWallPortPressureLoss : SEAnesthesiaMachineAction
{
  protected eSwitch state;
  
  public SEAnesthesiaMachineOxygenWallPortPressureLoss()
  {
    state = eSwitch.On;
  }

  public override void Clear()
  {
    base.Clear();
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
    this.state = (s == eSwitch.NullSwitch) ? eSwitch.Off : s;
  }

  public override string ToString()
  {
    return "Oxygen Wall Port Pressure Loss"
        + "\n\tState: " + GetState();
  }
}

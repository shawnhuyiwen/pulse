/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SECardiacArrest : SEPatientAction
{
  protected eSwitch state;

  public SECardiacArrest()
  {
    state = eSwitch.Off;
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
    state = (s == eSwitch.NullSwitch) ? eSwitch.Off : s;
  }
}

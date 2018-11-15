/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/


public enum IntubationType : int
{
    Off = 0,
    Esophageal,
    LeftMainstem,
    RightMainstem,
    Tracheal
}

public class SEIntubation : SEPatientAction
{
  protected IntubationType type;
  
  public SEIntubation()
  {
    type = IntubationType.Off;
  }
  
  public override void Clear()
  {
    base.Clear();
    type = IntubationType.Off;
  }
  
  public override bool IsValid()
  {
    return type != IntubationType.Off;
  }
  
  public new IntubationType GetType()
  {
    return type;
  }
  public void SetType(IntubationType t)
  {
    type = t;
  }

}

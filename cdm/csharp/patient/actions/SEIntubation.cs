/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SEIntubation : SEPatientAction
{
  public enum eType : int
  {
    Off = 0,
    Esophageal,
    LeftMainstem,
    RightMainstem,
    Tracheal
  }

  protected eType type;
  
  public SEIntubation()
  {
    type = eType.Off;
  }
  
  public override void Clear()
  {
    base.Clear();
    type = eType.Off;
  }
  
  public override bool IsValid()
  {
    return type != eType.Off;
  }
  
  public new eType GetType()
  {
    return type;
  }
  public void SetType(eType t)
  {
    type = t;
  }

}

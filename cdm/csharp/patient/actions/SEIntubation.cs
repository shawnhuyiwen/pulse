/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/


public class IntubationType
{
    private IntubationType(string v) { Value = v; }
    protected string Value { get; }

    public static IntubationType Off { get { return new IntubationType("Off"); } }
    public static IntubationType Esophageal { get { return new IntubationType("Esophageal"); } }
    public static IntubationType LeftMainstem { get { return new IntubationType("LeftMainstem"); } }
    public static IntubationType RightMainstem { get { return new IntubationType("RightMainstem"); } }
    public static IntubationType Tracheal { get { return new IntubationType("Tracheal"); } }
}

public class SEIntubation : SEPatientAction
{
  protected IntubationType type;
  
  public SEIntubation()
  {
    type = null;
  }
  
  public override void Reset()
  {
    base.Reset();
    type = null;
  }
  
  public override bool IsValid()
  {
    return HasType();
  }
  
  public new IntubationType GetType()
  {
    return type;
  }
  public void SetType(IntubationType t)
  {
    type = t;
  }
  public bool HasType()
  {
    return type == null ? false : true;
  }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class MassPerAreaTimeUnit
{
  private MassPerAreaTimeUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static MassPerAreaTimeUnit g_Per_cm2_s { get { return new MassPerAreaTimeUnit("g/cm^2 s"); } }

  public static MassPerAreaTimeUnit FromString(string u)
  {
    if (u == MassPerAreaTimeUnit.g_Per_cm2_s.Value)
      return MassPerAreaTimeUnit.g_Per_cm2_s;

    throw new System.NotSupportedException(u + " is an unsupported MassPerAreaTimeUnit");
  }
}

public class SEScalarMassPerAreaTime : SEScalar
{
  protected MassPerAreaTimeUnit unit;

  public SEScalarMassPerAreaTime() : base()
  {

  }

  public SEScalarMassPerAreaTime(SEScalarMassPerAreaTime from)
  {
    Set(from);
  }

  public SEScalarMassPerAreaTime(double value, MassPerAreaTimeUnit unit)
  {
    SetValue(value, unit);
  }

  public void Set(SEScalarMassPerAreaTime from)
  {
    SetValue(from.value, from.unit);
  }

  public new void SetValue(double value)
  {
    throw new System.NotSupportedException("You must provide a unit");
  }

  public void SetValue(double value, MassPerAreaTimeUnit unit)
  {
    this.value = value;
    this.unit = unit;
  }

  public double GetValue(MassPerAreaTimeUnit unit)
  {
    throw new System.NotImplementedException();
  }

  public MassPerAreaTimeUnit GetUnit()
  {
    return unit;
  }
}

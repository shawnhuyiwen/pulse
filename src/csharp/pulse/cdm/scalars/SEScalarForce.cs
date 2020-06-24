/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class ForceUnit
{
  private ForceUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static ForceUnit N = new ForceUnit("N");
  public static ForceUnit lbf = new ForceUnit("lbf");
  public static ForceUnit dyn = new ForceUnit("dyn");

  public static ForceUnit FromString(string u)
  {
    if (u == ForceUnit.N.ToString())
      return ForceUnit.N;
    if (u == ForceUnit.lbf.ToString())
      return ForceUnit.lbf;
    if (u == ForceUnit.dyn.ToString())
      return ForceUnit.dyn;

    throw new System.ArgumentException(u + " is not a know force unit");
  }
}

public class SEScalarForce : SEScalar
{
  protected ForceUnit unit;

  public SEScalarForce() : base()
  {

  }

  public SEScalarForce(SEScalarForce from)
  {
    Set(from);
  }

  public SEScalarForce(double value, ForceUnit unit)
  {
    SetValue(value, unit);
  }

  public void Set(SEScalarForce from)
  {
    if (from == null)
    {
      Invalidate();
      return;
    }
    SetValue(from.value, from.unit);
  }

  public new void SetValue(double value)
  {
    throw new System.NotSupportedException("You must provide a unit");
  }

  public void SetValue(double value, ForceUnit unit)
  {
    this.value = value;
    this.unit = unit;
  }

  public double GetValue(ForceUnit unit)
  {
    throw new System.NotImplementedException();
  }

  public ForceUnit GetUnit()
  {
    return unit;
  }

  public new string ToString()
  {
    return this.value + "(" + this.unit.ToString() + ")";
  }
}

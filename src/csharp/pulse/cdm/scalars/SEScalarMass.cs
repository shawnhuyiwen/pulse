/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class MassUnit
{
  private MassUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static MassUnit g = new MassUnit("g");
  public static MassUnit ug = new MassUnit("ug");
  public static MassUnit mg = new MassUnit("mg");
  public static MassUnit kg = new MassUnit("kg");
  public static MassUnit lb = new MassUnit("lb");

  public static MassUnit FromString(string u)
  {
    if (u == MassUnit.g.ToString())
      return MassUnit.g;
    if (u == MassUnit.ug.ToString())
      return MassUnit.ug;
    if (u == MassUnit.mg.ToString())
      return MassUnit.mg;
    if (u == MassUnit.kg.ToString())
      return MassUnit.kg;
    if (u == MassUnit.lb.ToString())
      return MassUnit.lb;

    throw new System.ArgumentException(u + " is not a know mass unit");
  }
}

public class SEScalarMass : SEScalar
{
  protected MassUnit unit;

  public SEScalarMass() : base()
  {

  }

  public SEScalarMass(SEScalarMass from)
  {
    Set(from);
  }

  public SEScalarMass(double value, MassUnit unit)
  {
    SetValue(value, unit);
  }

  public void Set(SEScalarMass from)
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

  public void SetValue(double value, MassUnit unit)
  {
    this.value = value;
    this.unit = unit;
  }

  public double GetValue(MassUnit unit)
  {
    throw new System.NotImplementedException();
  }

  public MassUnit GetUnit()
  {
    return unit;
  }
}

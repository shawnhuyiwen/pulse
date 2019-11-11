/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class MassUnit
{
  private MassUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static MassUnit g { get { return new MassUnit("g"); } }
  public static MassUnit ug { get { return new MassUnit("ug"); } }
  public static MassUnit mg { get { return new MassUnit("mg"); } }
  public static MassUnit kg { get { return new MassUnit("kg"); } }
  public static MassUnit lb { get { return new MassUnit("lb"); } }

  public static MassUnit FromString(string u)
  {
    if (u == MassUnit.g.Value)
      return MassUnit.g;
    if (u == MassUnit.ug.Value)
      return MassUnit.ug;
    if (u == MassUnit.mg.Value)
      return MassUnit.mg;
    if (u == MassUnit.kg.Value)
      return MassUnit.kg;
    if (u == MassUnit.lb.Value)
      return MassUnit.lb;

    throw new System.NotSupportedException(u + " is an unsupported MassUnit");
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

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class LengthUnit
{
  private LengthUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static LengthUnit m { get { return new LengthUnit("m"); } }
  public static LengthUnit cm { get { return new LengthUnit("cm"); } }
  public static LengthUnit mm { get { return new LengthUnit("mm"); } }
  public static LengthUnit um { get { return new LengthUnit("um"); } }
  public static LengthUnit inch { get { return new LengthUnit("in"); } }
  public static LengthUnit ft { get { return new LengthUnit("ft"); } }

  public static LengthUnit FromString(string u)
  {
    if (u == LengthUnit.m.Value)
      return LengthUnit.m;
    if (u == LengthUnit.cm.Value)
      return LengthUnit.cm;
    if (u == LengthUnit.mm.Value)
      return LengthUnit.mm;
    if (u == LengthUnit.um.Value)
      return LengthUnit.um;
    if (u == LengthUnit.inch.Value)
      return LengthUnit.inch;
   if (u == LengthUnit.ft.Value)
      return LengthUnit.ft;

  throw new System.NotSupportedException(u+" is an unsupported LengthUnit");
  }
}

public class SEScalarLength : SEScalar
{
  protected LengthUnit unit;

  public SEScalarLength() : base()
  {

  }

  public SEScalarLength(SEScalarLength from)
  {
    Set(from);
  }

  public SEScalarLength(double value, LengthUnit unit)
  {
    SetValue(value, unit);
  }

  public void Set(SEScalarLength from)
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

  public void SetValue(double value, LengthUnit unit)
  {
    this.value = value;
    this.unit = unit;
  }

  public double GetValue(LengthUnit unit)
  {
    throw new System.NotImplementedException();
  }

  public LengthUnit GetUnit()
  {
    return unit;
  }

  public new string ToString()
  {
    return this.value + "(" + this.unit.ToString() + ")";
  }
}

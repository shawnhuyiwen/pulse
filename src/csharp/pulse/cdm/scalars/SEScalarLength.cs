/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class LengthUnit : Unit
  {
    private LengthUnit(string v) { Value = v; }

    public static LengthUnit m = new LengthUnit("m");
    public static LengthUnit cm = new LengthUnit("cm");
    public static LengthUnit mm = new LengthUnit("mm");
    public static LengthUnit um = new LengthUnit("um");
    public static LengthUnit inch = new LengthUnit("in");
    public static LengthUnit ft = new LengthUnit("ft");

    public static LengthUnit FromString(string u)
    {
      if (u == LengthUnit.m.ToString())
        return LengthUnit.m;
      if (u == LengthUnit.cm.ToString())
        return LengthUnit.cm;
      if (u == LengthUnit.mm.ToString())
        return LengthUnit.mm;
      if (u == LengthUnit.um.ToString())
        return LengthUnit.um;
      if (u == LengthUnit.inch.ToString())
        return LengthUnit.inch;
      if (u == LengthUnit.ft.ToString())
        return LengthUnit.ft;

      throw new System.ArgumentException(u + " is not a know length unit");
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
      return UnitConverter.Convert(this.value, this.unit, unit);
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
}

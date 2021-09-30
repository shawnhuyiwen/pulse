/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class MassPerTimeUnit : Unit
  {
    private MassPerTimeUnit(string v) : base(v) { }

    public static MassPerTimeUnit g_Per_s = new MassPerTimeUnit("g/s");
    public static MassPerTimeUnit g_Per_min = new MassPerTimeUnit("g/min");
    public static MassPerTimeUnit g_Per_day = new MassPerTimeUnit("g/day");
    public static MassPerTimeUnit mg_Per_s = new MassPerTimeUnit("mg/s");
    public static MassPerTimeUnit mg_Per_min = new MassPerTimeUnit("mg/min");
    public static MassPerTimeUnit ug_Per_s = new MassPerTimeUnit("ug/s");
    public static MassPerTimeUnit kg_Per_s = new MassPerTimeUnit("kg/s");
    public static MassPerTimeUnit ug_Per_min = new MassPerTimeUnit("ug/min");

    public static MassPerTimeUnit FromString(string u)
    {
      if (u == MassPerTimeUnit.g_Per_s.ToString())
        return MassPerTimeUnit.g_Per_s;
      if (u == MassPerTimeUnit.g_Per_min.ToString())
        return MassPerTimeUnit.g_Per_min;
      if (u == MassPerTimeUnit.g_Per_day.ToString())
        return MassPerTimeUnit.g_Per_day;
      if (u == MassPerTimeUnit.mg_Per_s.ToString())
        return MassPerTimeUnit.mg_Per_s;
      if (u == MassPerTimeUnit.mg_Per_min.ToString())
        return MassPerTimeUnit.mg_Per_min;
      if (u == MassPerTimeUnit.ug_Per_s.ToString())
        return MassPerTimeUnit.ug_Per_s;
      if (u == MassPerTimeUnit.kg_Per_s.ToString())
        return MassPerTimeUnit.kg_Per_s;
      if (u == MassPerTimeUnit.ug_Per_min.ToString())
        return MassPerTimeUnit.ug_Per_min;

      throw new System.ArgumentException(u + " is not a know mass per time unit");
    }

  }

  public class SEScalarMassPerTime : SEScalar
  {
    protected MassPerTimeUnit unit;

    public SEScalarMassPerTime() : base()
    {

    }

    public SEScalarMassPerTime(SEScalarMassPerTime from)
    {
      Set(from);
    }

    public SEScalarMassPerTime(double value, MassPerTimeUnit unit)
    {
      SetValue(value, unit);
    }

    public void Set(SEScalarMassPerTime from)
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

    public void SetValue(double value, MassPerTimeUnit unit)
    {
      this.value = value;
      this.unit = unit;
    }

    public double GetValue(MassPerTimeUnit unit)
    {
      return UnitConverter.Convert(this.value, this.unit, unit);
    }

    public MassPerTimeUnit GetUnit()
    {
      return unit;
    }

    public new string ToString()
    {
      return this.value + "(" + this.unit.ToString() + ")";
    }
  }
}

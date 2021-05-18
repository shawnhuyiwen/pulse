/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class TimeUnit : Unit
  {
    private TimeUnit(string v) { Value = v; }

    public static TimeUnit s = new TimeUnit("s");
    public static TimeUnit min = new TimeUnit("min");
    public static TimeUnit hr = new TimeUnit("hr");
    public static TimeUnit day = new TimeUnit("day");
    public static TimeUnit yr = new TimeUnit("yr");

    public static TimeUnit FromString(string u)
    {
      if (u == TimeUnit.s.ToString())
        return TimeUnit.s;
      if (u == TimeUnit.min.ToString())
        return TimeUnit.min;
      if (u == TimeUnit.hr.ToString())
        return TimeUnit.hr;
      if (u == TimeUnit.day.ToString())
        return TimeUnit.day;
      if (u == TimeUnit.yr.ToString())
        return TimeUnit.yr;

      throw new System.ArgumentException(u + " is not a know time unit");
    }
  }

  public class SEScalarTime : SEScalar
  {
    protected TimeUnit unit;

    public SEScalarTime() : base()
    {

    }

    public SEScalarTime(SEScalarTime from)
    {
      Set(from);
    }

    public SEScalarTime(double value, TimeUnit unit)
    {
      SetValue(value, unit);
    }

    public void Set(SEScalarTime from)
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

    public void SetValue(double value, TimeUnit unit)
    {
      this.value = value;
      this.unit = unit;
    }

    public double GetValue(TimeUnit unit)
    {
      return UnitConverter.Convert(this.value, this.unit, unit);
    }

    public TimeUnit GetUnit()
    {
      return unit;
    }

    public new string ToString()
    {
      return this.value + "(" + this.unit.ToString() + ")";
    }
  }
}

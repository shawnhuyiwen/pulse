/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class TimeUnit
{
  private TimeUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static TimeUnit s = new TimeUnit("s");
  public static TimeUnit min = new TimeUnit("min");
  public static TimeUnit hr = new TimeUnit("hr");
  public static TimeUnit day = new TimeUnit("day");
  public static TimeUnit yr = new TimeUnit("yr");

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
    throw new System.NotImplementedException();
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

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class TimeUnit
{
  private TimeUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static TimeUnit s { get { return new TimeUnit("s"); } }
  public static TimeUnit min { get { return new TimeUnit("min"); } }
  public static TimeUnit hr { get { return new TimeUnit("hr"); } }
  public static TimeUnit day { get { return new TimeUnit("day"); } }
  public static TimeUnit yr { get { return new TimeUnit("yr"); } }

  public static TimeUnit FromString(string u)
  {
    if (u == TimeUnit.s.Value)
      return TimeUnit.s;
    if (u == TimeUnit.min.Value)
      return TimeUnit.min;
    if (u == TimeUnit.hr.Value)
      return TimeUnit.hr;
    if (u == TimeUnit.day.Value)
      return TimeUnit.day;
    if (u == TimeUnit.yr.Value)
      return TimeUnit.yr;

    throw new System.NotSupportedException(u+" is an unsupported TimeUnit");
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

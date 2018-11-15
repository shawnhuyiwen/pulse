/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class TimeUnit
{
    private TimeUnit(string v) { Value = v; }
    protected string Value { get; }

    public static TimeUnit s { get { return new TimeUnit("s"); } }
    public static TimeUnit min { get { return new TimeUnit("min"); } }
    public static TimeUnit hr { get { return new TimeUnit("hr"); } }
    public static TimeUnit day { get { return new TimeUnit("day"); } }
    public static TimeUnit yr { get { return new TimeUnit("yr"); } }
}

public class SEScalarTime : SEScalar
{
    protected TimeUnit unit;

    public SEScalarTime() : base()
    {

    }

    public SEScalarTime(double value, TimeUnit unit)
    {
        SetValue(value, unit);
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
}

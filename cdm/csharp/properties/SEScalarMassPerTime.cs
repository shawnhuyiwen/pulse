/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class MassPerTimeUnit
{
    private MassPerTimeUnit(string v) { Value = v; }
    protected string Value { get; }

    public new string ToString() { return Value; }

    public static MassPerTimeUnit g_Per_s { get { return new MassPerTimeUnit("g/s"); } }
    public static MassPerTimeUnit g_Per_min { get { return new MassPerTimeUnit("g/min"); } }
    public static MassPerTimeUnit g_Per_day { get { return new MassPerTimeUnit("g/day"); } }
    public static MassPerTimeUnit mg_Per_s { get { return new MassPerTimeUnit("mg/s"); } }
    public static MassPerTimeUnit mg_Per_min { get { return new MassPerTimeUnit("mg/min"); } }
    public static MassPerTimeUnit ug_Per_s { get { return new MassPerTimeUnit("ug/s"); } }
    public static MassPerTimeUnit kg_Per_s { get { return new MassPerTimeUnit("kg/s"); } }
    public static MassPerTimeUnit ug_Per_min { get { return new MassPerTimeUnit("ug/min"); } }
}

public class SEScalarMassPerTime : SEScalar
{
    protected MassPerTimeUnit unit;

    public SEScalarMassPerTime() : base()
    {

    }

    public SEScalarMassPerTime(double value, MassPerTimeUnit unit)
    {
        SetValue(value, unit);
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
        throw new System.NotImplementedException();
    }

    public MassPerTimeUnit GetUnit()
    {
        return unit;
    }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class PressureUnit
{
    private PressureUnit(string v) { Value = v; }
    protected string Value { get; }

    public new string ToString() { return Value; }

    public static PressureUnit Pa { get { return new PressureUnit("Pa"); } }
    public static PressureUnit mmHg { get { return new PressureUnit("mmHg"); } }
    public static PressureUnit cmH2O { get { return new PressureUnit("cmH2O"); } }
    public static PressureUnit psi { get { return new PressureUnit("psi"); } }
    public static PressureUnit atm { get { return new PressureUnit("atm"); } }
}

public class SEScalarPressure : SEScalar
{
    protected PressureUnit unit;

    public SEScalarPressure() : base()
    {

    }

    public SEScalarPressure(double value, PressureUnit unit)
    {
        SetValue(value, unit);
    }

    public new void SetValue(double value)
    {
        throw new System.NotSupportedException("You must provide a unit");
    }

    public void SetValue(double value, PressureUnit unit)
    {
        this.value = value;
        this.unit = unit;
    }

    public double GetValue(PressureUnit unit)
    {
        throw new System.NotImplementedException();
    }

    public PressureUnit GetUnit()
    {
        return unit;
    }
}

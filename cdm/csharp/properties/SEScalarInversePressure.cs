/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class InversePressureUnit
{
    private InversePressureUnit(string v) { Value = v; }
    protected string Value { get; }

    public new string ToString() { return Value; }

    public static InversePressureUnit Inverse_Pa { get { return new InversePressureUnit("1/Pa"); } }
    public static InversePressureUnit Inverse_mmHg { get { return new InversePressureUnit("1/mmHg"); } }
    public static InversePressureUnit Inverse_cmH2O { get { return new InversePressureUnit("1/cmH2O"); } }
    public static InversePressureUnit Inverse_atm { get { return new InversePressureUnit("1/atm"); } }
}

public class SEScalarInversePressure : SEScalar
{
    protected InversePressureUnit unit;

    public SEScalarInversePressure() : base()
    {

    }

    public SEScalarInversePressure(double value, InversePressureUnit unit)
    {
        SetValue(value, unit);
    }

    public new void SetValue(double value)
    {
        throw new System.NotSupportedException("You must provide a unit");
    }

    public void SetValue(double value, InversePressureUnit unit)
    {
        this.value = value;
        this.unit = unit;
    }

    public double GetValue(InversePressureUnit unit)
    {
        throw new System.NotImplementedException();
    }

    public InversePressureUnit GetUnit()
    {
        return unit;
    }
}
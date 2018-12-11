/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class MassPerAmountUnit
{
    private MassPerAmountUnit(string v) { Value = v; }
    protected string Value { get; }

    public new string ToString() { return Value; }

    public static MassPerAmountUnit g_Per_ct { get { return new MassPerAmountUnit("g/ct"); } }
    public static MassPerAmountUnit g_Per_mol { get { return new MassPerAmountUnit("g/mol"); } }
    public static MassPerAmountUnit g_Per_mmol { get { return new MassPerAmountUnit("g/mmol"); } }
    public static MassPerAmountUnit g_Per_umol { get { return new MassPerAmountUnit("g/umol"); } }
    public static MassPerAmountUnit mg_Per_mol { get { return new MassPerAmountUnit("mg/mol"); } }
    public static MassPerAmountUnit mg_Per_mmol { get { return new MassPerAmountUnit("mg/mmol"); } }
    public static MassPerAmountUnit kg_Per_mol { get { return new MassPerAmountUnit("kg/mol"); } }
    public static MassPerAmountUnit ug_Per_mmol { get { return new MassPerAmountUnit("ug/mmol"); } }
    public static MassPerAmountUnit ug_Per_mol { get { return new MassPerAmountUnit("ug/mol"); } }
    public static MassPerAmountUnit ug_Per_ct { get { return new MassPerAmountUnit("ug/ct"); } }
    public static MassPerAmountUnit pg_Per_ct { get { return new MassPerAmountUnit("pg/ct"); } }
}

public class SEScalarMassPerAmount : SEScalar
{
    protected MassPerAmountUnit unit;

    public SEScalarMassPerAmount() : base()
    {

    }

    public SEScalarMassPerAmount(double value, MassPerAmountUnit unit)
    {
        SetValue(value, unit);
    }

    public new void SetValue(double value)
    {
        throw new System.NotSupportedException("You must provide a unit");
    }

    public void SetValue(double value, MassPerAmountUnit unit)
    {
        this.value = value;
        this.unit = unit;
    }

    public double GetValue(MassPerAmountUnit unit)
    {
        throw new System.NotImplementedException();
    }

    public MassPerAmountUnit GetUnit()
    {
        return unit;
    }
}

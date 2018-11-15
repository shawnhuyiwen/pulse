/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class MassPerVolumeUnit
{
    private MassPerVolumeUnit(string v) { Value = v; }
    protected string Value { get; }

    public static MassPerVolumeUnit ug_Per_mL { get { return new MassPerVolumeUnit("ug/mL"); } }
    public static MassPerVolumeUnit mg_Per_mL { get { return new MassPerVolumeUnit("mg/mL"); } }
    public static MassPerVolumeUnit ug_Per_L { get { return new MassPerVolumeUnit("ug/L"); } }
    public static MassPerVolumeUnit g_Per_mL { get { return new MassPerVolumeUnit("g/mL"); } }
    public static MassPerVolumeUnit g_Per_dL { get { return new MassPerVolumeUnit("g/dL"); } }
    public static MassPerVolumeUnit g_Per_m3 { get { return new MassPerVolumeUnit("g/m^3"); } }
    public static MassPerVolumeUnit mg_Per_m3 { get { return new MassPerVolumeUnit("mg/m^3"); } }
    public static MassPerVolumeUnit kg_Per_m3 { get { return new MassPerVolumeUnit("kg/m^3"); } }
}

public class SEScalarMassPerVolume : SEScalar
{
    protected MassPerVolumeUnit unit;

    public SEScalarMassPerVolume() : base()
    {

    }

    public SEScalarMassPerVolume(double value, MassPerVolumeUnit unit)
    {
        SetValue(value, unit);
    }

    public new void SetValue(double value)
    {
        throw new System.NotSupportedException("You must provide a unit");
    }

    public void SetValue(double value, MassPerVolumeUnit unit)
    {
        this.value = value;
        this.unit = unit;
    }

    public double GetValue(MassPerVolumeUnit unit)
    {
        throw new System.NotImplementedException();
    }

    public MassPerVolumeUnit GetUnit()
    {
        return unit;
    }
}

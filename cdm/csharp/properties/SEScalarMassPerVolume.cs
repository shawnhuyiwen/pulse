/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class MassPerVolumeUnit
{
  private MassPerVolumeUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static MassPerVolumeUnit g_Per_dL { get { return new MassPerVolumeUnit("g/dL"); } }
  public static MassPerVolumeUnit g_Per_cm3 { get { return new MassPerVolumeUnit("g/cm^3"); } }
  public static MassPerVolumeUnit g_Per_m3 { get { return new MassPerVolumeUnit("g/m^3"); } }
  public static MassPerVolumeUnit ug_Per_mL { get { return new MassPerVolumeUnit("ug/mL"); } }
  public static MassPerVolumeUnit mg_Per_m3 { get { return new MassPerVolumeUnit("mg/m^3"); } }
  public static MassPerVolumeUnit kg_Per_m3 { get { return new MassPerVolumeUnit("kg/m^3"); } }
  public static MassPerVolumeUnit ug_Per_L { get { return new MassPerVolumeUnit("ug/L"); } }
  public static MassPerVolumeUnit g_Per_L { get { return new MassPerVolumeUnit("g/L"); } }
  public static MassPerVolumeUnit g_Per_mL { get { return new MassPerVolumeUnit("g/mL"); } }
  public static MassPerVolumeUnit mg_Per_mL { get { return new MassPerVolumeUnit("mg/mL"); } }
  public static MassPerVolumeUnit mg_Per_L { get { return new MassPerVolumeUnit("mg/L"); } }
  public static MassPerVolumeUnit mg_Per_dL { get { return new MassPerVolumeUnit("mg/dL"); } }
  public static MassPerVolumeUnit kg_Per_mL { get { return new MassPerVolumeUnit("kg/mL"); } }
  public static MassPerVolumeUnit kg_Per_L { get { return new MassPerVolumeUnit("kg/L"); } }

  public static MassPerVolumeUnit FromString(string u)
  {
    if (u == MassPerVolumeUnit.g_Per_dL.Value)
      return MassPerVolumeUnit.g_Per_dL;
    if (u == MassPerVolumeUnit.g_Per_cm3.Value)
      return MassPerVolumeUnit.g_Per_cm3;
    if (u == MassPerVolumeUnit.g_Per_m3.Value)
      return MassPerVolumeUnit.g_Per_m3;
    if (u == MassPerVolumeUnit.ug_Per_mL.Value)
      return MassPerVolumeUnit.ug_Per_mL;
    if (u == MassPerVolumeUnit.mg_Per_m3.Value)
      return MassPerVolumeUnit.mg_Per_m3;
    if (u == MassPerVolumeUnit.kg_Per_m3.Value)
      return MassPerVolumeUnit.kg_Per_m3;
    if (u == MassPerVolumeUnit.ug_Per_L.Value)
      return MassPerVolumeUnit.ug_Per_L;
    if (u == MassPerVolumeUnit.g_Per_L.Value)
      return MassPerVolumeUnit.g_Per_L;
    if (u == MassPerVolumeUnit.g_Per_mL.Value)
      return MassPerVolumeUnit.g_Per_mL;
    if (u == MassPerVolumeUnit.mg_Per_mL.Value)
      return MassPerVolumeUnit.mg_Per_mL;
    if (u == MassPerVolumeUnit.mg_Per_L.Value)
      return MassPerVolumeUnit.mg_Per_L;
    if (u == MassPerVolumeUnit.mg_Per_dL.Value)
      return MassPerVolumeUnit.mg_Per_dL;
    if (u == MassPerVolumeUnit.kg_Per_mL.Value)
      return MassPerVolumeUnit.kg_Per_mL;
    if (u == MassPerVolumeUnit.kg_Per_L.Value)
      return MassPerVolumeUnit.kg_Per_L;

    throw new System.NotSupportedException(u + " is an unsupported MassPerVolumeUnit");
  }
}

public class SEScalarMassPerVolume : SEScalar
{
  protected MassPerVolumeUnit unit;

  public SEScalarMassPerVolume() : base()
  {

  }

  public SEScalarMassPerVolume(SEScalarMassPerVolume from)
  {
    Set(from);
  }

  public SEScalarMassPerVolume(double value, MassPerVolumeUnit unit)
  {
    SetValue(value, unit);
  }

  public void Set(SEScalarMassPerVolume from)
  {
    SetValue(from.value, from.unit);
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

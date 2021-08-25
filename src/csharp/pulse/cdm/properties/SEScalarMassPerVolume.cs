/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class MassPerVolumeUnit : Unit
  {
    private MassPerVolumeUnit(string v) : base(v) { }

    public static MassPerVolumeUnit g_Per_dL = new MassPerVolumeUnit("g/dL");
    public static MassPerVolumeUnit g_Per_cm3 = new MassPerVolumeUnit("g/cm^3");
    public static MassPerVolumeUnit g_Per_m3 = new MassPerVolumeUnit("g/m^3");
    public static MassPerVolumeUnit ug_Per_mL = new MassPerVolumeUnit("ug/mL");
    public static MassPerVolumeUnit mg_Per_m3 = new MassPerVolumeUnit("mg/m^3");
    public static MassPerVolumeUnit kg_Per_m3 = new MassPerVolumeUnit("kg/m^3");
    public static MassPerVolumeUnit ug_Per_L = new MassPerVolumeUnit("ug/L");
    public static MassPerVolumeUnit g_Per_L = new MassPerVolumeUnit("g/L");
    public static MassPerVolumeUnit g_Per_mL = new MassPerVolumeUnit("g/mL");
    public static MassPerVolumeUnit mg_Per_mL = new MassPerVolumeUnit("mg/mL");
    public static MassPerVolumeUnit mg_Per_L = new MassPerVolumeUnit("mg/L");
    public static MassPerVolumeUnit mg_Per_dL = new MassPerVolumeUnit("mg/dL");
    public static MassPerVolumeUnit kg_Per_mL = new MassPerVolumeUnit("kg/mL");
    public static MassPerVolumeUnit kg_Per_L = new MassPerVolumeUnit("kg/L");

    public static MassPerVolumeUnit FromString(string u)
    {
      if (u == MassPerVolumeUnit.g_Per_dL.ToString())
        return MassPerVolumeUnit.g_Per_dL;
      if (u == MassPerVolumeUnit.g_Per_cm3.ToString())
        return MassPerVolumeUnit.g_Per_cm3;
      if (u == MassPerVolumeUnit.g_Per_m3.ToString())
        return MassPerVolumeUnit.g_Per_m3;
      if (u == MassPerVolumeUnit.ug_Per_mL.ToString())
        return MassPerVolumeUnit.ug_Per_mL;
      if (u == MassPerVolumeUnit.mg_Per_m3.ToString())
        return MassPerVolumeUnit.mg_Per_m3;
      if (u == MassPerVolumeUnit.kg_Per_m3.ToString())
        return MassPerVolumeUnit.kg_Per_m3;
      if (u == MassPerVolumeUnit.ug_Per_L.ToString())
        return MassPerVolumeUnit.ug_Per_L;
      if (u == MassPerVolumeUnit.g_Per_L.ToString())
        return MassPerVolumeUnit.g_Per_L;
      if (u == MassPerVolumeUnit.g_Per_mL.ToString())
        return MassPerVolumeUnit.g_Per_mL;
      if (u == MassPerVolumeUnit.mg_Per_mL.ToString())
        return MassPerVolumeUnit.mg_Per_mL;
      if (u == MassPerVolumeUnit.mg_Per_L.ToString())
        return MassPerVolumeUnit.mg_Per_L;
      if (u == MassPerVolumeUnit.mg_Per_dL.ToString())
        return MassPerVolumeUnit.mg_Per_dL;
      if (u == MassPerVolumeUnit.kg_Per_mL.ToString())
        return MassPerVolumeUnit.kg_Per_mL;
      if (u == MassPerVolumeUnit.kg_Per_L.ToString())
        return MassPerVolumeUnit.kg_Per_L;

      throw new System.ArgumentException(u + " is not a know mass per volume unit");
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

    public void SetValue(double value, MassPerVolumeUnit unit)
    {
      this.value = value;
      this.unit = unit;
    }

    public double GetValue(MassPerVolumeUnit unit)
    {
      return UnitConverter.Convert(this.value, this.unit, unit);
    }

    public MassPerVolumeUnit GetUnit()
    {
      return unit;
    }

    public new string ToString()
    {
      return this.value + "(" + this.unit.ToString() + ")";
    }
  }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class VolumePerTimeMassUnit : Unit
  {
    private VolumePerTimeMassUnit(string v) { Value = v; }

    public static VolumePerTimeMassUnit L_Per_s_g = new VolumePerTimeMassUnit("L/s g");
    public static VolumePerTimeMassUnit mL_Per_s_g = new VolumePerTimeMassUnit("mL/s g");
    public static VolumePerTimeMassUnit mL_Per_min_kg = new VolumePerTimeMassUnit("mL/min kg");
    public static VolumePerTimeMassUnit mL_Per_s_kg = new VolumePerTimeMassUnit("mL/s kg");
    public static VolumePerTimeMassUnit uL_Per_min_kg = new VolumePerTimeMassUnit("uL/min kg");

    public static VolumePerTimeMassUnit FromString(string u)
    {
      if (u == VolumePerTimeMassUnit.L_Per_s_g.ToString())
        return VolumePerTimeMassUnit.L_Per_s_g;
      if (u == VolumePerTimeMassUnit.mL_Per_s_g.ToString())
        return VolumePerTimeMassUnit.mL_Per_s_g;
      if (u == VolumePerTimeMassUnit.mL_Per_min_kg.ToString())
        return VolumePerTimeMassUnit.mL_Per_min_kg;
      if (u == VolumePerTimeMassUnit.mL_Per_s_kg.ToString())
        return VolumePerTimeMassUnit.mL_Per_s_kg;
      if (u == VolumePerTimeMassUnit.uL_Per_min_kg.ToString())
        return VolumePerTimeMassUnit.uL_Per_min_kg;

      throw new System.ArgumentException(u + " is not a know volume per time mass unit");
    }
  }

  public class SEScalarVolumePerTimeMass : SEScalar
  {
    protected VolumePerTimeMassUnit unit;

    public SEScalarVolumePerTimeMass() : base()
    {

    }

    public SEScalarVolumePerTimeMass(SEScalarVolumePerTimeMass from)
    {
      Set(from);
    }

    public SEScalarVolumePerTimeMass(double value, VolumePerTimeMassUnit unit)
    {
      SetValue(value, unit);
    }

    public void Set(SEScalarVolumePerTimeMass from)
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

    public void SetValue(double value, VolumePerTimeMassUnit unit)
    {
      this.value = value;
      this.unit = unit;
    }

    public double GetValue(VolumePerTimeMassUnit unit)
    {
      return UnitConverter.Convert(this.value, this.unit, unit);
    }

    public VolumePerTimeMassUnit GetUnit()
    {
      return unit;
    }

    public new string ToString()
    {
      return this.value + "(" + this.unit.ToString() + ")";
    }
  }
}

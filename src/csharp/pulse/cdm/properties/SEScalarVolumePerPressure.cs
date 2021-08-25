/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class VolumePerPressureUnit : Unit
  {
    private VolumePerPressureUnit(string v) : base(v) { }

    public static VolumePerPressureUnit L_Per_Pa = new VolumePerPressureUnit("L/Pa");
    public static VolumePerPressureUnit L_Per_cmH2O = new VolumePerPressureUnit("L/cmH2O");
    public static VolumePerPressureUnit mL_Per_cmH2O = new VolumePerPressureUnit("mL/cmH2O");
    public static VolumePerPressureUnit m3_Per_Pa = new VolumePerPressureUnit("m^3/Pa");
    public static VolumePerPressureUnit mL_Per_mmHg = new VolumePerPressureUnit("mL/mmHg");

    public static VolumePerPressureUnit FromString(string u)
    {
      if (u == VolumePerPressureUnit.L_Per_Pa.ToString())
        return VolumePerPressureUnit.L_Per_Pa;
      if (u == VolumePerPressureUnit.L_Per_cmH2O.ToString())
        return VolumePerPressureUnit.L_Per_cmH2O;
      if (u == VolumePerPressureUnit.mL_Per_cmH2O.ToString())
        return VolumePerPressureUnit.mL_Per_cmH2O;
      if (u == VolumePerPressureUnit.m3_Per_Pa.ToString())
        return VolumePerPressureUnit.m3_Per_Pa;
      if (u == VolumePerPressureUnit.mL_Per_mmHg.ToString())
        return VolumePerPressureUnit.mL_Per_mmHg;

      throw new System.ArgumentException(u + " is not a know volume per  pressure unit");
    }
  }

  public class SEScalarVolumePerPressure : SEScalar
  {
    protected VolumePerPressureUnit unit;

    public SEScalarVolumePerPressure() : base()
    {

    }

    public SEScalarVolumePerPressure(SEScalarVolumePerPressure from)
    {
      Set(from);
    }

    public SEScalarVolumePerPressure(double value, VolumePerPressureUnit unit)
    {
      SetValue(value, unit);
    }

    public void Set(SEScalarVolumePerPressure from)
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

    public void SetValue(double value, VolumePerPressureUnit unit)
    {
      this.value = value;
      this.unit = unit;
    }

    public double GetValue(VolumePerPressureUnit unit)
    {
      return UnitConverter.Convert(this.value, this.unit, unit);
    }

    public VolumePerPressureUnit GetUnit()
    {
      return unit;
    }

    public new string ToString()
    {
      return this.value + "(" + this.unit.ToString() + ")";
    }
  }
}

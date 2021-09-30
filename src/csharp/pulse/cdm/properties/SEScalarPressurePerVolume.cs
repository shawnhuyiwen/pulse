/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class PressurePerVolumeUnit : Unit
  {
    private PressurePerVolumeUnit(string v) : base(v) { }

    public static PressurePerVolumeUnit mmHg_Per_mL = new PressurePerVolumeUnit("mmHg/mL");
    public static PressurePerVolumeUnit cmH2O_Per_mL = new PressurePerVolumeUnit("cmH2O/mL");
    public static PressurePerVolumeUnit cmH2O_Per_L = new PressurePerVolumeUnit("cmH2O/L");
    
    public static PressurePerVolumeUnit FromString(string u)
    {
      if (u == PressurePerVolumeUnit.mmHg_Per_mL.ToString())
        return PressurePerVolumeUnit.mmHg_Per_mL;
      if (u == PressurePerVolumeUnit.cmH2O_Per_mL.ToString())
        return PressurePerVolumeUnit.cmH2O_Per_mL;
      if (u == PressurePerVolumeUnit.cmH2O_Per_L.ToString())
        return PressurePerVolumeUnit.cmH2O_Per_L;

      throw new System.ArgumentException(u + " is not a know pressure per volume unit");
    }
  }

  public class SEScalarPressurePerVolume : SEScalar
  {
    protected PressurePerVolumeUnit unit;

    public SEScalarPressurePerVolume() : base()
    {

    }

    public SEScalarPressurePerVolume(SEScalarPressurePerVolume from)
    {
      Set(from);
    }

    public SEScalarPressurePerVolume(double value, PressurePerVolumeUnit unit)
    {
      SetValue(value, unit);
    }

    public void Set(SEScalarPressurePerVolume from)
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

    public void SetValue(double value, PressurePerVolumeUnit unit)
    {
      this.value = value;
      this.unit = unit;
    }

    public double GetValue(PressurePerVolumeUnit unit)
    {
      return UnitConverter.Convert(this.value, this.unit, unit);
    }

    public PressurePerVolumeUnit GetUnit()
    {
      return unit;
    }

    public new string ToString()
    {
      return this.value + "(" + this.unit.ToString() + ")";
    }
  }
}

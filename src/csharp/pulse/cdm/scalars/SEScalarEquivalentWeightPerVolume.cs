/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class EquivalentWeightPerVolumeUnit : Unit
  {
    private EquivalentWeightPerVolumeUnit(string v) { Value = v; }

    public static EquivalentWeightPerVolumeUnit Eq_Per_L = new EquivalentWeightPerVolumeUnit("Eq/L");
    public static EquivalentWeightPerVolumeUnit Eq_Per_mL = new EquivalentWeightPerVolumeUnit("Eq/mL");
    public static EquivalentWeightPerVolumeUnit mEq_Per_L = new EquivalentWeightPerVolumeUnit("mEq/L");
    public static EquivalentWeightPerVolumeUnit mEq_Per_mL = new EquivalentWeightPerVolumeUnit("mEq/mL");

    public static EquivalentWeightPerVolumeUnit FromString(string u)
    {
      if (u == EquivalentWeightPerVolumeUnit.Eq_Per_L.ToString())
        return EquivalentWeightPerVolumeUnit.Eq_Per_L;
      if (u == EquivalentWeightPerVolumeUnit.Eq_Per_mL.ToString())
        return EquivalentWeightPerVolumeUnit.Eq_Per_mL;
      if (u == EquivalentWeightPerVolumeUnit.mEq_Per_L.ToString())
        return EquivalentWeightPerVolumeUnit.mEq_Per_L;
      if (u == EquivalentWeightPerVolumeUnit.mEq_Per_mL.ToString())
        return EquivalentWeightPerVolumeUnit.mEq_Per_mL;

      throw new System.ArgumentException(u + " is not a know equivalent weight per volume unit");
    }
  }

  public class SEScalarEquivalentWeightPerVolume : SEScalar
  {
    protected EquivalentWeightPerVolumeUnit unit;

    public SEScalarEquivalentWeightPerVolume() : base()
    {

    }

    public SEScalarEquivalentWeightPerVolume(SEScalarEquivalentWeightPerVolume from)
    {
      Set(from);
    }

    public SEScalarEquivalentWeightPerVolume(double value, EquivalentWeightPerVolumeUnit unit)
    {
      SetValue(value, unit);
    }

    public void Set(SEScalarEquivalentWeightPerVolume from)
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

    public void SetValue(double value, EquivalentWeightPerVolumeUnit unit)
    {
      this.value = value;
      this.unit = unit;
    }

    public double GetValue(EquivalentWeightPerVolumeUnit unit)
    {
      return UnitConverter.Convert(this.value, this.unit, unit);
    }

    public EquivalentWeightPerVolumeUnit GetUnit()
    {
      return unit;
    }

    public new string ToString()
    {
      return this.value + "(" + this.unit.ToString() + ")";
    }
  }
}

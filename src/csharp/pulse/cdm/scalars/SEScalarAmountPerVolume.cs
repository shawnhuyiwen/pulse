/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class AmountPerVolumeUnit : Unit
  {
    private AmountPerVolumeUnit(string v) { Value = v; }

    public static AmountPerVolumeUnit mol_Per_L = new AmountPerVolumeUnit("mol/L");
    public static AmountPerVolumeUnit mol_Per_mL = new AmountPerVolumeUnit("mol/mL");
    public static AmountPerVolumeUnit mmol_Per_L = new AmountPerVolumeUnit("mmol/L");
    public static AmountPerVolumeUnit mmol_Per_mL = new AmountPerVolumeUnit("mmol/mL");
    public static AmountPerVolumeUnit pmol_Per_L = new AmountPerVolumeUnit("pmol/L");
    public static AmountPerVolumeUnit ct_Per_L = new AmountPerVolumeUnit("ct/L");
    public static AmountPerVolumeUnit ct_Per_uL = new AmountPerVolumeUnit("ct/uL");

    public static AmountPerVolumeUnit FromString(string u)
    {
      if (u == AmountPerVolumeUnit.mol_Per_L.ToString())
        return AmountPerVolumeUnit.mol_Per_L;
      if (u == AmountPerVolumeUnit.mol_Per_mL.ToString())
        return AmountPerVolumeUnit.mol_Per_mL;
      if (u == AmountPerVolumeUnit.mmol_Per_L.ToString())
        return AmountPerVolumeUnit.mmol_Per_L;
      if (u == AmountPerVolumeUnit.mmol_Per_mL.ToString())
        return AmountPerVolumeUnit.mmol_Per_mL;
      if (u == AmountPerVolumeUnit.pmol_Per_L.ToString())
        return AmountPerVolumeUnit.pmol_Per_L;
      if (u == AmountPerVolumeUnit.ct_Per_L.ToString())
        return AmountPerVolumeUnit.ct_Per_L;
      if (u == AmountPerVolumeUnit.ct_Per_uL.ToString())
        return AmountPerVolumeUnit.ct_Per_uL;

      throw new System.ArgumentException(u + " is not a know area unit");
    }
  }

  public class SEScalarAmountPerVolume : SEScalar
  {
    protected AmountPerVolumeUnit unit;

    public SEScalarAmountPerVolume() : base()
    {

    }

    public SEScalarAmountPerVolume(SEScalarAmountPerVolume from)
    {
      Set(from);
    }

    public SEScalarAmountPerVolume(double value, AmountPerVolumeUnit unit)
    {
      SetValue(value, unit);
    }

    public void Set(SEScalarAmountPerVolume from)
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

    public void SetValue(double value, AmountPerVolumeUnit unit)
    {
      this.value = value;
      this.unit = unit;
    }

    public double GetValue(AmountPerVolumeUnit unit)
    {
      return UnitConverter.Convert(this.value, this.unit, unit);
    }

    public AmountPerVolumeUnit GetUnit()
    {
      return unit;
    }

    public new string ToString()
    {
      return this.value + "(" + this.unit.ToString() + ")";
    }
  }
}

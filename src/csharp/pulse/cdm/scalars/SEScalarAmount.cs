/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class AmountUnit : Unit
  {
    private AmountUnit(string v) { Value = v; }

    public static AmountUnit mol = new AmountUnit("mol");
    public static AmountUnit pmol = new AmountUnit("pmol");

    public static AmountUnit FromString(string u)
    {
      if (u == AmountUnit.mol.ToString())
        return AmountUnit.mol;
      if (u == AmountUnit.pmol.ToString())
        return AmountUnit.pmol;

      throw new System.ArgumentException(u + " is not a know area unit");
    }
  }

  public class SEScalarAmount : SEScalar
  {
    protected AmountUnit unit;

    public SEScalarAmount() : base()
    {

    }

    public SEScalarAmount(SEScalarAmount from)
    {
      Set(from);
    }

    public SEScalarAmount(double value, AmountUnit unit)
    {
      SetValue(value, unit);
    }

    public void Set(SEScalarAmount from)
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

    public void SetValue(double value, AmountUnit unit)
    {
      this.value = value;
      this.unit = unit;
    }

    public double GetValue(AmountUnit unit)
    {
      return UnitConverter.Convert(this.value, this.unit, unit);
    }

    public AmountUnit GetUnit()
    {
      return unit;
    }

    public new string ToString()
    {
      return this.value + "(" + this.unit.ToString() + ")";
    }
  }
}

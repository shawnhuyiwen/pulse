/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class ElectricPotentialUnit : Unit
  {
    private ElectricPotentialUnit(string v) : base(v) { }

    public static ElectricPotentialUnit V = new ElectricPotentialUnit("V");
    public static ElectricPotentialUnit mV = new ElectricPotentialUnit("mV");

    public static ElectricPotentialUnit FromString(string u)
    {
      if (u == ElectricPotentialUnit.V.ToString())
        return ElectricPotentialUnit.V;
      if (u == ElectricPotentialUnit.mV.ToString())
        return ElectricPotentialUnit.mV;

      throw new System.ArgumentException(u + " is not a know ElectricPotential unit");
    }
  }

  public class SEScalarElectricPotential : SEScalar
  {
    protected ElectricPotentialUnit unit;

    public SEScalarElectricPotential() : base()
    {

    }

    public SEScalarElectricPotential(SEScalarElectricPotential from)
    {
      Set(from);
    }

    public SEScalarElectricPotential(double value, ElectricPotentialUnit unit)
    {
      SetValue(value, unit);
    }

    public void Set(SEScalarElectricPotential from)
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

    public void SetValue(double value, ElectricPotentialUnit unit)
    {
      this.value = value;
      this.unit = unit;
    }

    public double GetValue(ElectricPotentialUnit unit)
    {
      return UnitConverter.Convert(this.value, this.unit, unit);
    }

    public ElectricPotentialUnit GetUnit()
    {
      return unit;
    }

    public new string ToString()
    {
      return this.value + "(" + this.unit.ToString() + ")";
    }
  }
}

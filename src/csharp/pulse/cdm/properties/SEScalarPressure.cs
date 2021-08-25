/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class PressureUnit : Unit
  {
    private PressureUnit(string v) : base(v) { }

    public static PressureUnit Pa = new PressureUnit("Pa");
    public static PressureUnit mmHg = new PressureUnit("mmHg");
    public static PressureUnit cmH2O = new PressureUnit("cmH2O");
    public static PressureUnit psi = new PressureUnit("psi");
    public static PressureUnit atm = new PressureUnit("atm");

    public static PressureUnit FromString(string u)
    {
      if (u == PressureUnit.Pa.ToString())
        return PressureUnit.Pa;
      if (u == PressureUnit.mmHg.ToString())
        return PressureUnit.mmHg;
      if (u == PressureUnit.cmH2O.ToString())
        return PressureUnit.cmH2O;
      if (u == PressureUnit.psi.ToString())
        return PressureUnit.psi;
      if (u == PressureUnit.atm.ToString())
        return PressureUnit.atm;

      throw new System.ArgumentException(u + " is not a know pressure unit");
    }
  }

  public class SEScalarPressure : SEScalar
  {
    protected PressureUnit unit;

    public SEScalarPressure() : base()
    {

    }

    public SEScalarPressure(SEScalarPressure from)
    {
      Set(from);
    }

    public SEScalarPressure(double value, PressureUnit unit)
    {
      SetValue(value, unit);
    }

    public void Set(SEScalarPressure from)
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

    public void SetValue(double value, PressureUnit unit)
    {
      this.value = value;
      this.unit = unit;
    }

    public double GetValue(PressureUnit unit)
    {
      return UnitConverter.Convert(this.value, this.unit, unit);
    }

    public PressureUnit GetUnit()
    {
      return unit;
    }

    public new string ToString()
    {
      return this.value + "(" + this.unit.ToString() + ")";
    }
  }
}

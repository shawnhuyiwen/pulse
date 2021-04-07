/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class HeatResistanceAreaUnit : Unit
  {
    private HeatResistanceAreaUnit(string v) { Value = v; }

    public static HeatResistanceAreaUnit rsi = new HeatResistanceAreaUnit("rsi");
    public static HeatResistanceAreaUnit clo = new HeatResistanceAreaUnit("clo");
    public static HeatResistanceAreaUnit rValue = new HeatResistanceAreaUnit("rValue");
    public static HeatResistanceAreaUnit tog = new HeatResistanceAreaUnit("tog");

    public static HeatResistanceAreaUnit FromString(string u)
    {
      if (u == HeatResistanceAreaUnit.rsi.ToString())
        return HeatResistanceAreaUnit.rsi;
      if (u == HeatResistanceAreaUnit.clo.ToString())
        return HeatResistanceAreaUnit.clo;
      if (u == HeatResistanceAreaUnit.rValue.ToString())
        return HeatResistanceAreaUnit.rValue;
      if (u == HeatResistanceAreaUnit.tog.ToString())
        return HeatResistanceAreaUnit.tog;

      throw new System.ArgumentException(u + " is not a know heat resistance area unit");
    }
  }

  public class SEScalarHeatResistanceArea : SEScalar
  {
    protected HeatResistanceAreaUnit unit;
    public SEScalarHeatResistanceArea() : base()
    {

    }

    public SEScalarHeatResistanceArea(SEScalarHeatResistanceArea from)
    {
      Set(from);
    }
    public SEScalarHeatResistanceArea(double value, HeatResistanceAreaUnit unit)
    {
      SetValue(value, unit);
    }

    public void Set(SEScalarHeatResistanceArea from)
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

    public void SetValue(double value, HeatResistanceAreaUnit unit)
    {
      this.value = value;
      this.unit = unit;
    }

    public double GetValue(HeatResistanceAreaUnit unit)
    {
      return UnitConverter.Convert(this.value, this.unit, unit);
    }

    public HeatResistanceAreaUnit GetUnit()
    {
      return unit;
    }

    public new string ToString()
    {
      return this.value + "(" + this.unit.ToString() + ")";
    }
  }
}

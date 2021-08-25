/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class FrequencyUnit : Unit
  {
    private FrequencyUnit(string v) : base(v) { }

    public static FrequencyUnit Per_min = new FrequencyUnit("1/min");
    public static FrequencyUnit Per_s = new FrequencyUnit("1/s");
    public static FrequencyUnit Hz = new FrequencyUnit("Hz");

    public static FrequencyUnit FromString(string u)
    {
      if (u == FrequencyUnit.Per_min.ToString())
        return FrequencyUnit.Per_min;
      if (u == FrequencyUnit.Per_s.ToString())
        return FrequencyUnit.Per_s;
      if (u == FrequencyUnit.Hz.ToString())
        return FrequencyUnit.Hz;

      throw new System.ArgumentException(u + " is not a know frequency unit");
    }
  }

  public class SEScalarFrequency : SEScalar
  {
    protected FrequencyUnit unit;

    public SEScalarFrequency() : base()
    {

    }

    public SEScalarFrequency(SEScalarFrequency from)
    {
      Set(from);
    }

    public SEScalarFrequency(double value, FrequencyUnit unit)
    {
      SetValue(value, unit);
    }

    public void Set(SEScalarFrequency from)
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

    public void SetValue(double value, FrequencyUnit unit)
    {
      this.value = value;
      this.unit = unit;
    }

    public double GetValue(FrequencyUnit unit)
    {
      return UnitConverter.Convert(this.value, this.unit, unit);
    }

    public FrequencyUnit GetUnit()
    {
      return unit;
    }

    public new string ToString()
    {
      return this.value + "(" + this.unit.ToString() + ")";
    }
  }
}

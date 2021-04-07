/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class LengthPerTimeUnit : Unit
  {
    private LengthPerTimeUnit(string v) { Value = v; }

    public static LengthPerTimeUnit m_Per_s = new LengthPerTimeUnit("m/s");
    public static LengthPerTimeUnit ft_Per_s = new LengthPerTimeUnit("ft/s");

    public static LengthPerTimeUnit FromString(string u)
    {
      if (u == LengthPerTimeUnit.m_Per_s.ToString())
        return LengthPerTimeUnit.m_Per_s;
      if (u == LengthPerTimeUnit.ft_Per_s.ToString())
        return LengthPerTimeUnit.ft_Per_s;

      throw new System.ArgumentException(u + " is not a know length per time unit");
    }
  }


  public class SEScalarLengthPerTime : SEScalar
  {
    protected LengthPerTimeUnit unit;

    public SEScalarLengthPerTime() : base()
    {

    }

    public SEScalarLengthPerTime(SEScalarLengthPerTime from)
    {
      Set(from);
    }

    public SEScalarLengthPerTime(double value, LengthPerTimeUnit unit)
    {
      SetValue(value, unit);
    }

    public void Set(SEScalarLengthPerTime from)
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

    public void SetValue(double value, LengthPerTimeUnit unit)
    {
      this.value = value;
      this.unit = unit;
    }


    public double GetValue(LengthPerTimeUnit unit)
    {
      return UnitConverter.Convert(this.value, this.unit, unit);
    }

    public LengthPerTimeUnit GetUnit()
    {
      return unit;
    }

    public new string ToString()
    {
      return this.value + "(" + this.unit.ToString() + ")";
    }
  }
}

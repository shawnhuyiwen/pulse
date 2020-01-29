/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class FrequencyUnit
{
  private FrequencyUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static FrequencyUnit Per_min = new FrequencyUnit("1/min");
  public static FrequencyUnit Per_s = new FrequencyUnit("1/s");
  public static FrequencyUnit Hz = new FrequencyUnit("Hz");
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
    throw new System.NotImplementedException();
  }

  public FrequencyUnit GetUnit()
  {
    return unit;
  }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class FrequencyUnit
{
  private FrequencyUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static FrequencyUnit Per_min { get { return new FrequencyUnit("1/min"); } }
  public static FrequencyUnit Per_s { get { return new FrequencyUnit("1/s"); } }
  public static FrequencyUnit Hz { get { return new FrequencyUnit("Hz"); } }

  public static FrequencyUnit FromString(string u)
  {
    if (u == FrequencyUnit.Per_min.Value)
      return FrequencyUnit.Per_min;
    if (u == FrequencyUnit.Per_s.Value)
      return FrequencyUnit.Per_s;
    if (u == FrequencyUnit.Hz.Value)
      return FrequencyUnit.Hz;

    throw new System.NotSupportedException(u + " is an unsupported FrequencyUnit");
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

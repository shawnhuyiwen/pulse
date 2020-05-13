/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class TemperatureUnit
{
  private TemperatureUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static TemperatureUnit C = new TemperatureUnit("degC");
  public static TemperatureUnit F = new TemperatureUnit("degF");
  public static TemperatureUnit K = new TemperatureUnit("K");
  public static TemperatureUnit R = new TemperatureUnit("degR");

  public static TemperatureUnit FromString(string u)
  {
    if (u == TemperatureUnit.C.ToString())
      return TemperatureUnit.C;
    if (u == TemperatureUnit.F.ToString())
      return TemperatureUnit.F;
    if (u == TemperatureUnit.K.ToString())
      return TemperatureUnit.K;
    if (u == TemperatureUnit.R.ToString())
      return TemperatureUnit.R;

    throw new System.ArgumentException(u + " is not a know temperature unit");
  }
}

public class SEScalarTemperature : SEScalar
{

  protected TemperatureUnit unit;

  public SEScalarTemperature() : base()
  {

  }

  public SEScalarTemperature(SEScalarTemperature from)
  {
    Set(from);
  }

  public SEScalarTemperature(double value, TemperatureUnit unit)
  {
    SetValue(value, unit);
  }

  public void Set(SEScalarTemperature from)
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

  public void SetValue(double value, TemperatureUnit unit)
  {
    this.value = value;
    this.unit = unit;
  }

  public double GetValue(TemperatureUnit unit)
  {
    throw new System.NotImplementedException();
  }

  public TemperatureUnit GetUnit()
  {
    return unit;
  }

  public new string ToString()
  {
    return this.value + "(" + this.unit.ToString() + ")";
  }
}

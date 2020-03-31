/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class PowerUnit
{
  private PowerUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static PowerUnit W = new PowerUnit("W");
  public static PowerUnit kcal_Per_s = new PowerUnit("kcal/s");
  public static PowerUnit kcal_Per_hr = new PowerUnit("kcal/hr");
  public static PowerUnit kcal_Per_day = new PowerUnit("kcal/day");
  public static PowerUnit J_Per_s = new PowerUnit("J/s");
  public static PowerUnit BTU_Per_hr = new PowerUnit("BTU/hr");

  public static PowerUnit FromString(string u)
  {
    if (u == PowerUnit.W.ToString())
      return PowerUnit.W;
    if (u == PowerUnit.kcal_Per_s.ToString())
      return PowerUnit.kcal_Per_s;
    if (u == PowerUnit.kcal_Per_hr.ToString())
      return PowerUnit.kcal_Per_hr;
    if (u == PowerUnit.kcal_Per_day.ToString())
      return PowerUnit.kcal_Per_day;
    if (u == PowerUnit.J_Per_s.ToString())
      return PowerUnit.J_Per_s;
    if (u == PowerUnit.BTU_Per_hr.ToString())
      return PowerUnit.BTU_Per_hr;

    throw new System.ArgumentException(u + " is not a know power unit");
  }
}

public class SEScalarPower : SEScalar
{
  protected PowerUnit unit;

  public SEScalarPower() : base()
  {

  }

  public SEScalarPower(SEScalarPower from)
  {
    Set(from);
  }

  public SEScalarPower(double value, PowerUnit unit)
  {
    SetValue(value, unit);
  }

  public void Set(SEScalarPower from)
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

  public void SetValue(double value, PowerUnit unit)
  {
    this.value = value;
    this.unit = unit;
  }

  public double GetValue(PowerUnit unit)
  {
    throw new System.NotImplementedException();
  }

  public PowerUnit GetUnit()
  {
    return unit;
  }

  public new string ToString()
  {
    return this.value + "(" + this.unit.ToString() + ")";
  }
}

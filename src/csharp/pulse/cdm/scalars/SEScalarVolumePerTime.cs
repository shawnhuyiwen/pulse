/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class VolumePerTimeUnit
{
  private VolumePerTimeUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static VolumePerTimeUnit L_Per_s = new VolumePerTimeUnit("L/s");
  public static VolumePerTimeUnit mL_Per_s = new VolumePerTimeUnit("mL/s");
  public static VolumePerTimeUnit L_Per_min = new VolumePerTimeUnit("L/min");
  public static VolumePerTimeUnit m3_Per_s = new VolumePerTimeUnit("m^3/s");
  public static VolumePerTimeUnit mL_Per_min = new VolumePerTimeUnit("mL/min");
  public static VolumePerTimeUnit mL_Per_day = new VolumePerTimeUnit("mL/day");

  public static VolumePerTimeUnit FromString(string u)
  {
    if (u == VolumePerTimeUnit.L_Per_s.ToString())
      return VolumePerTimeUnit.L_Per_s;
    if (u == VolumePerTimeUnit.mL_Per_s.ToString())
      return VolumePerTimeUnit.mL_Per_s;
    if (u == VolumePerTimeUnit.L_Per_min.ToString())
      return VolumePerTimeUnit.L_Per_min;
    if (u == VolumePerTimeUnit.m3_Per_s.ToString())
      return VolumePerTimeUnit.m3_Per_s;
    if (u == VolumePerTimeUnit.mL_Per_min.ToString())
      return VolumePerTimeUnit.mL_Per_min;
    if (u == VolumePerTimeUnit.mL_Per_day.ToString())
      return VolumePerTimeUnit.mL_Per_day;

    throw new System.ArgumentException(u + " is not a know volume per time unit");
  }
}

public class SEScalarVolumePerTime : SEScalar
{
  protected VolumePerTimeUnit unit;

  public SEScalarVolumePerTime() : base()
  {

  }

  public SEScalarVolumePerTime(SEScalarVolumePerTime from)
  {
    Set(from);
  }

  public SEScalarVolumePerTime(double value, VolumePerTimeUnit unit)
  {
    SetValue(value, unit);
  }

  public void Set(SEScalarVolumePerTime from)
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

  public void SetValue(double value, VolumePerTimeUnit unit)
  {
    this.value = value;
    this.unit = unit;
  }

  public double GetValue(VolumePerTimeUnit unit)
  {
    throw new System.NotImplementedException();
  }

  public VolumePerTimeUnit GetUnit()
  {
    return unit;
  }
}

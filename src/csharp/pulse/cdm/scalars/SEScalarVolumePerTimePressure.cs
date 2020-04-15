/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class VolumePerTimePressureUnit
{
  private VolumePerTimePressureUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static VolumePerTimePressureUnit L_Per_s_mmHg = new VolumePerTimePressureUnit("L/s mmHg");
  public static VolumePerTimePressureUnit mL_Per_s_mmHg = new VolumePerTimePressureUnit("mL/s mmHg");
  public static VolumePerTimePressureUnit L_Per_min_mmHg = new VolumePerTimePressureUnit("L/min mmHg");
  public static VolumePerTimePressureUnit mL_Per_min_mmHg = new VolumePerTimePressureUnit("mL/min mmHg");

  public static VolumePerTimePressureUnit FromString(string u)
  {
    if (u == VolumePerTimePressureUnit.L_Per_s_mmHg.ToString())
      return VolumePerTimePressureUnit.L_Per_s_mmHg;
    if (u == VolumePerTimePressureUnit.mL_Per_s_mmHg.ToString())
      return VolumePerTimePressureUnit.mL_Per_s_mmHg;
    if (u == VolumePerTimePressureUnit.L_Per_min_mmHg.ToString())
      return VolumePerTimePressureUnit.L_Per_min_mmHg;
    if (u == VolumePerTimePressureUnit.mL_Per_min_mmHg.ToString())
      return VolumePerTimePressureUnit.mL_Per_min_mmHg;

    throw new System.ArgumentException(u + " is not a know volume per time pressure unit");
  }
}

public class SEScalarVolumePerTimePressure : SEScalar
{
  protected VolumePerTimePressureUnit unit;

  public SEScalarVolumePerTimePressure() : base()
  {

  }

  public SEScalarVolumePerTimePressure(SEScalarVolumePerTimePressure from)
  {
    Set(from);
  }

  public SEScalarVolumePerTimePressure(double value, VolumePerTimePressureUnit unit)
  {
    SetValue(value, unit);
  }

  public void Set(SEScalarVolumePerTimePressure from)
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

  public void SetValue(double value, VolumePerTimePressureUnit unit)
  {
    this.value = value;
    this.unit = unit;
  }

  public double GetValue(VolumePerTimePressureUnit unit)
  {
    throw new System.NotImplementedException();
  }

  public VolumePerTimePressureUnit GetUnit()
  {
    return unit;
  }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class VolumePerTimeMassUnit
{
  private VolumePerTimeMassUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static VolumePerTimeMassUnit L_Per_s_g { get { return new VolumePerTimeMassUnit("L/s g"); } }
  public static VolumePerTimeMassUnit mL_Per_s_g { get { return new VolumePerTimeMassUnit("mL / s g"); } }
  public static VolumePerTimeMassUnit mL_Per_min_kg { get { return new VolumePerTimeMassUnit("mL/min kg"); } }
  public static VolumePerTimeMassUnit mL_Per_s_kg { get { return new VolumePerTimeMassUnit("mL/s kg"); } }
  public static VolumePerTimeMassUnit uL_Per_min_kg { get { return new VolumePerTimeMassUnit("uL/min kg"); } }

  public static VolumePerTimeMassUnit FromString(string u)
  {
    if (u == VolumePerTimeMassUnit.L_Per_s_g.Value)
      return VolumePerTimeMassUnit.L_Per_s_g;
    if (u == VolumePerTimeMassUnit.mL_Per_s_g.Value)
      return VolumePerTimeMassUnit.mL_Per_s_g;
    if (u == VolumePerTimeMassUnit.mL_Per_min_kg.Value)
      return VolumePerTimeMassUnit.mL_Per_min_kg;
    if (u == VolumePerTimeMassUnit.mL_Per_s_kg.Value)
      return VolumePerTimeMassUnit.mL_Per_s_kg;
    if (u == VolumePerTimeMassUnit.uL_Per_min_kg.Value)
      return VolumePerTimeMassUnit.uL_Per_min_kg;

    throw new System.NotSupportedException(u + " is an unsupported VolumePerTimeMassUnit");
  }
}

public class SEScalarVolumePerTimeMass : SEScalar
{
  protected VolumePerTimeMassUnit unit;

  public SEScalarVolumePerTimeMass() : base()
  {

  }

  public SEScalarVolumePerTimeMass(SEScalarVolumePerTimeMass from)
  {
    Set(from);
  }

  public SEScalarVolumePerTimeMass(double value, VolumePerTimeMassUnit unit)
  {
    SetValue(value, unit);
  }

  public void Set(SEScalarVolumePerTimeMass from)
  {
    SetValue(from.value, from.unit);
  }

  public new void SetValue(double value)
  {
    throw new System.NotSupportedException("You must provide a unit");
  }

  public void SetValue(double value, VolumePerTimeMassUnit unit)
  {
    this.value = value;
    this.unit = unit;
  }

  public double GetValue(VolumePerTimeMassUnit unit)
  {
    throw new System.NotImplementedException();
  }

  public VolumePerTimeMassUnit GetUnit()
  {
    return unit;
  }
}

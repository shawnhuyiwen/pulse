/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class VolumeUnit
{
  private VolumeUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static VolumeUnit L = new VolumeUnit("L");
  public static VolumeUnit mL = new VolumeUnit("mL");
  public static VolumeUnit dL = new VolumeUnit("dL");
  public static VolumeUnit m3 = new VolumeUnit("m^3");

}

public class SEScalarVolume : SEScalar
{
  protected VolumeUnit unit;

  public SEScalarVolume() : base()
  {

  }

  public SEScalarVolume(SEScalarVolume from)
  {
    Set(from);
  }

  public SEScalarVolume(double value, VolumeUnit unit)
  {
    SetValue(value, unit);
  }

  public void Set(SEScalarVolume from)
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

  public void SetValue(double value, VolumeUnit unit)
  {
    this.value = value;
    this.unit = unit;
  }

  public double GetValue(VolumeUnit unit)
  {
    throw new System.NotImplementedException();
  }

  public VolumeUnit GetUnit()
  {
    return unit;
  }
}

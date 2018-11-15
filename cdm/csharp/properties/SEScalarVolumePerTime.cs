/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class VolumePerTimeUnit
{
    private VolumePerTimeUnit(string v) { Value = v; }
    protected string Value { get; }

    public static VolumePerTimeUnit L_Per_s { get { return new VolumePerTimeUnit("L/s"); } }
    public static VolumePerTimeUnit mL_Per_s { get { return new VolumePerTimeUnit("mL/s"); } }
    public static VolumePerTimeUnit L_Per_min { get { return new VolumePerTimeUnit("L/min"); } }
    public static VolumePerTimeUnit m3_Per_s { get { return new VolumePerTimeUnit("m^3/s"); } }
    public static VolumePerTimeUnit mL_Per_min { get { return new VolumePerTimeUnit("mL/min"); } }
    public static VolumePerTimeUnit mL_Per_day { get { return new VolumePerTimeUnit("mL/day"); } }
}

public class SEScalarVolumePerTime : SEScalar
{
  protected VolumePerTimeUnit unit;

  public SEScalarVolumePerTime() : base()
  {

  }

  public SEScalarVolumePerTime(double value, VolumePerTimeUnit unit)
    {
        SetValue(value, unit);
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

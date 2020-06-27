/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class PressureTimePerVolumeUnit
{
  private PressureTimePerVolumeUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static PressureTimePerVolumeUnit cmH2O_s_Per_L = new PressureTimePerVolumeUnit("cmH2O s/L");
  public static PressureTimePerVolumeUnit cmH2O_s_Per_mL = new PressureTimePerVolumeUnit("cmH2O s/mL");
  public static PressureTimePerVolumeUnit mmHg_s_Per_mL = new PressureTimePerVolumeUnit("mmHg s/mL");
  public static PressureTimePerVolumeUnit mmHg_min_Per_mL = new PressureTimePerVolumeUnit("mmHg min/mL");
  public static PressureTimePerVolumeUnit mmHg_min_Per_L = new PressureTimePerVolumeUnit("mmHg min/L");
  public static PressureTimePerVolumeUnit Pa_s_Per_m3 = new PressureTimePerVolumeUnit("Pa s/m^3");

  public static PressureTimePerVolumeUnit FromString(string u)
  {
    if (u == PressureTimePerVolumeUnit.cmH2O_s_Per_L.ToString())
      return PressureTimePerVolumeUnit.cmH2O_s_Per_L;
    if (u == PressureTimePerVolumeUnit.cmH2O_s_Per_mL.ToString())
      return PressureTimePerVolumeUnit.cmH2O_s_Per_mL;
    if (u == PressureTimePerVolumeUnit.mmHg_s_Per_mL.ToString())
      return PressureTimePerVolumeUnit.mmHg_s_Per_mL;
    if (u == PressureTimePerVolumeUnit.mmHg_min_Per_mL.ToString())
      return PressureTimePerVolumeUnit.mmHg_min_Per_mL;
    if (u == PressureTimePerVolumeUnit.mmHg_min_Per_L.ToString())
      return PressureTimePerVolumeUnit.mmHg_min_Per_L;
    if (u == PressureTimePerVolumeUnit.Pa_s_Per_m3.ToString())
      return PressureTimePerVolumeUnit.Pa_s_Per_m3;

    throw new System.ArgumentException(u + " is not a know pressure time per volume unit");
  }
}

public class SEScalarPressureTimePerVolume : SEScalar
{
  protected PressureTimePerVolumeUnit unit;

  public SEScalarPressureTimePerVolume() : base()
  {

  }

  public SEScalarPressureTimePerVolume(SEScalarPressureTimePerVolume from)
  {
    Set(from);
  }

  public SEScalarPressureTimePerVolume(double value, PressureTimePerVolumeUnit unit)
  {
    SetValue(value, unit);
  }

  public void Set(SEScalarPressureTimePerVolume from)
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

  public void SetValue(double value, PressureTimePerVolumeUnit unit)
  {
    this.value = value;
    this.unit = unit;
  }

  public double GetValue(PressureTimePerVolumeUnit unit)
  {
    throw new System.NotImplementedException();
  }

  public PressureTimePerVolumeUnit GetUnit()
  {
    return unit;
  }

  public new string ToString()
  {
    return this.value + "(" + this.unit.ToString() + ")";
  }
}

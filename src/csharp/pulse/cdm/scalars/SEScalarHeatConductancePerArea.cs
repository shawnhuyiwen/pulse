/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class HeatConductancePerAreaUnit
{
  private HeatConductancePerAreaUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static HeatConductancePerAreaUnit W_Per_m2_K = new HeatConductancePerAreaUnit("W/m^2 K");
  public static HeatConductancePerAreaUnit W_Per_m2_C = new HeatConductancePerAreaUnit("W/m^2 degC");
  public static HeatConductancePerAreaUnit BTU_Per_hr_ft2_C = new HeatConductancePerAreaUnit("BTU/hr ft^2 degC");

  public static HeatConductancePerAreaUnit FromString(string u)
  {
    if (u == HeatConductancePerAreaUnit.W_Per_m2_K.ToString())
      return HeatConductancePerAreaUnit.W_Per_m2_K;
    if (u == HeatConductancePerAreaUnit.W_Per_m2_C.ToString())
      return HeatConductancePerAreaUnit.W_Per_m2_C;
    if (u == HeatConductancePerAreaUnit.BTU_Per_hr_ft2_C.ToString())
      return HeatConductancePerAreaUnit.BTU_Per_hr_ft2_C;

    throw new System.ArgumentException(u + " is not a know heat conductance per area unit");
  }
}

public class SEScalarHeatConductancePerArea : SEScalar
{

  protected HeatConductancePerAreaUnit unit;


  public SEScalarHeatConductancePerArea() : base()
  {

  }

  public SEScalarHeatConductancePerArea(SEScalarHeatConductancePerArea from)
  {
    Set(from);
  }

  public SEScalarHeatConductancePerArea(double value, HeatConductancePerAreaUnit unit)
  {
    SetValue(value, unit);
  }

  public void Set(SEScalarHeatConductancePerArea from)
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

  public void SetValue(double value, HeatConductancePerAreaUnit unit)
  {
    this.value = value;
    this.unit = unit;
  }

  public double GetValue(HeatConductancePerAreaUnit unit)
  {
    throw new System.NotImplementedException();
  }

  public HeatConductancePerAreaUnit GetUnit()
  {
    return unit;
  }

  public new string ToString()
  {
    return this.value + "(" + this.unit.ToString() + ")";
  }
}

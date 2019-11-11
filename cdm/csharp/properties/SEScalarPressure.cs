/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class PressureUnit
{
  private PressureUnit(string v) { Value = v; }
  protected string Value { get; }

  public new string ToString() { return Value; }

  public static PressureUnit Pa { get { return new PressureUnit("Pa"); } }
  public static PressureUnit mmHg { get { return new PressureUnit("mmHg"); } }
  public static PressureUnit cmH2O { get { return new PressureUnit("cmH2O"); } }
  public static PressureUnit psi { get { return new PressureUnit("psi"); } }
  public static PressureUnit atm { get { return new PressureUnit("atm"); } }

  public static PressureUnit FromString(string u)
  {
    if (u == PressureUnit.Pa.Value)
      return PressureUnit.Pa;
    if (u == PressureUnit.mmHg.Value)
      return PressureUnit.mmHg;
    if (u == PressureUnit.cmH2O.Value)
      return PressureUnit.cmH2O;
    if (u == PressureUnit.psi.Value)
      return PressureUnit.psi;
    if (u == PressureUnit.atm.Value)
      return PressureUnit.atm;

    throw new System.NotSupportedException(u + " is an unsupported PressureUnit");
  }
}

public class SEScalarPressure : SEScalar
{
  protected PressureUnit unit;

  public SEScalarPressure() : base()
  {

  }

  public SEScalarPressure(SEScalarPressure from)
  {
    Set(from);
  }

  public SEScalarPressure(double value, PressureUnit unit)
  {
    SetValue(value, unit);
  }

  public void Set(SEScalarPressure from)
  {
    SetValue(from.value, from.unit);
  }

  public new void SetValue(double value)
  {
    throw new System.NotSupportedException("You must provide a unit");
  }

  public void SetValue(double value, PressureUnit unit)
  {
    this.value = value;
    this.unit = unit;
  }

  public double GetValue(PressureUnit unit)
  {
    throw new System.NotImplementedException();
  }

  public PressureUnit GetUnit()
  {
    return unit;
  }
}

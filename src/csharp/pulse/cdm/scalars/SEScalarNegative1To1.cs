/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SEScalarNegative1To1 : SEScalar
{
  public SEScalarNegative1To1() : base()
  {

  }

  public SEScalarNegative1To1(SEScalarNegative1To1 from)
  {
    Set(from);
  }

  public SEScalarNegative1To1(double value)
  {
    SetValue(value);
  }

  public void Set(SEScalarNegative1To1 from)
  {
    if (from == null)
    {
      Invalidate();
      return;
    }
    SetValue(from.value);
  }

  public override void SetValue(double val)
  {
    if (val >= 0 || val <= 1)
      value = val;
    else
      throw new System.Exception("Value must be [0-1]");
  }

  public new string ToString()
  {
    return this.value + "";
  }
}

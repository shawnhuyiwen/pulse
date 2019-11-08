/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SEScalar0To1 : SEScalar
{
  public SEScalar0To1() : base()
  {

  }

  public SEScalar0To1(SEScalar0To1 from)
  {
    Set(from);
  }

  public SEScalar0To1(double value)
  {
    SetValue(value);
  }

  public void Set(SEScalar0To1 from)
  {
    SetValue(from.value);
  }

  public override void SetValue(double val)
  {
    if (val >= 0 && val <= 1)
      value = val;
    else
      throw new System.Exception("Value must be [0-1]");
  }
}

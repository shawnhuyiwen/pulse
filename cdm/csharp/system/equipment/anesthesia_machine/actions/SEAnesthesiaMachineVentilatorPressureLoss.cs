/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SEAnesthesiaMachineVentilatorPressureLoss : SEAnesthesiaMachineAction
{
  protected SEScalar0To1 severity;
  
  public SEAnesthesiaMachineVentilatorPressureLoss()
  {
    severity = null;
  }
  public override void Clear()
  {
    base.Clear();
    if (severity != null)
      severity.Invalidate();
  }

  public override bool IsValid()
  {
    return HasSeverity();
  }

  public bool HasSeverity()
  {
    return severity == null ? false : severity.IsValid();
  }
  public SEScalar0To1 GetSeverity()
  {
    if (severity == null)
      severity = new SEScalar0To1();
    return severity;
  }

  public override string ToString()
  {
    if (severity != null)
      return "Ventilator Pressure Loss"
          + "\n\tSeverity: " + GetSeverity();
    else
      return "Ventilator Pressure Loss not specified properly";
  }
}

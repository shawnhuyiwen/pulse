/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
   
public class SEExpiratoryValveObstruction : SEAnesthesiaMachineAction
{
  protected SEScalar0To1 severity;
  
  public SEExpiratoryValveObstruction()
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
      return "Expiratory Valve Obstruction"
          + "\n\tSeverity: " + GetSeverity();
    else
      return "Expiratory Valve Obstruction not specified properly";
  }
}
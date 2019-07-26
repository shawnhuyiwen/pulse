/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SEAnesthesiaMachineTubeCuffLeak : SEAnesthesiaMachineAction
{
  protected SEScalar0To1 severity;
  
  public SEAnesthesiaMachineTubeCuffLeak()
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
      return "Tube Cuff Leak"
          + "\n\tSeverity: " + GetSeverity();
    else
      return "Tube Cuff Leak not specified properly";
  }
}

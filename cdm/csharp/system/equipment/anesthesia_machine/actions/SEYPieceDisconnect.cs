/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
   
public class SEYPieceDisconnect : SEAnesthesiaMachineAction
{
  protected SEScalar0To1 severity;
  
  public SEYPieceDisconnect()
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
      return "Y Piece Disconnect"
          + "\n\tSeverity: " + GetSeverity();
    else
      return "Y Piece Disconnect not specified properly";
  }
}
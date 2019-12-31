/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/


public class SEBrainInjury : SEPatientAction
{
  public enum eType : int
  {
    Diffuse = 0,
    LeftFocal,
    RightFocal
  }

  protected SEScalar0To1 severity;
  protected eType        injury_type;

  public SEBrainInjury()
  {
    severity = null;
    injury_type = eType.Diffuse;
  }

  public override void Clear()
  {
    base.Clear();
    if (severity != null)
      severity.Invalidate();
    injury_type = eType.Diffuse;
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

  public eType GetInjuryType() { return injury_type; }
  public void SetInjuryType(eType s) { injury_type = s; }
}

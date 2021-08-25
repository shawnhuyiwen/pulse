/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eBrainInjury_Type : int
  {
    Diffuse = 0,
    LeftFocal,
    RightFocal
  }

  public class SEBrainInjury : SEPatientAction
  {
    
    protected SEScalar0To1 severity;
    protected eBrainInjury_Type injury_type;

    public SEBrainInjury()
    {
      severity = null;
      injury_type = eBrainInjury_Type.Diffuse;
    }

    public override void Clear()
    {
      base.Clear();
      if (severity != null)
        severity.Invalidate();
      injury_type = eBrainInjury_Type.Diffuse;
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

    public eBrainInjury_Type GetInjuryType() { return injury_type; }
    public void SetInjuryType(eBrainInjury_Type s) { injury_type = s; }
  }
}

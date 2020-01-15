/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEAcuteRespiratoryDistressSyndromeExacerbation : SEPatientAction
  {
    protected SEScalar0To1 severity;
    protected SEScalar0To1 left_lung_affected;
    protected SEScalar0To1 right_lung_affected;

    public SEAcuteRespiratoryDistressSyndromeExacerbation()
    {
      severity = null;
      left_lung_affected = null;
      right_lung_affected = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (severity != null)
        severity.Invalidate();
      if (left_lung_affected != null)
        left_lung_affected.Invalidate();
      if (right_lung_affected != null)
        right_lung_affected.Invalidate();
    }

    public override bool IsValid()
    {
      return HasSeverity() && HasLeftLungAffected() && HasRightLungAffected();
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

    public bool HasLeftLungAffected()
    {
      return left_lung_affected == null ? false : left_lung_affected.IsValid();
    }
    public SEScalar0To1 GetLeftLungAffected()
    {
      if (left_lung_affected == null)
        left_lung_affected = new SEScalar0To1();
      return left_lung_affected;
    }

    public bool HasRightLungAffected()
    {
      return right_lung_affected == null ? false : right_lung_affected.IsValid();
    }
    public SEScalar0To1 GetRightLungAffected()
    {
      if (right_lung_affected == null)
        right_lung_affected = new SEScalar0To1();
      return right_lung_affected;
    }
  }
}

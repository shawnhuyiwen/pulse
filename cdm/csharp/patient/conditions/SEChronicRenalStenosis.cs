/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SEChronicRenalStenosis : SEPatientCondition
{
  protected SEScalar0To1 left_kidney_severity;
  protected SEScalar0To1 right_kidney_severity;

  public SEChronicRenalStenosis()
  {
    left_kidney_severity = null;
    right_kidney_severity = null;
  }

  public override void Clear()
  {
    base.Clear();
    if (left_kidney_severity != null)
      left_kidney_severity.Invalidate();
    if (right_kidney_severity != null)
      right_kidney_severity.Invalidate();
  }

  public override bool IsValid()
  {
    return HasLeftKidneySeverity() && HasRightKidneySeverity();
  }

  public bool HasLeftKidneySeverity()
  {
    return left_kidney_severity == null ? false : left_kidney_severity.IsValid();
  }
  public SEScalar0To1 GetLeftKidneySeverity()
  {
    if (left_kidney_severity == null)
      left_kidney_severity = new SEScalar0To1();
    return left_kidney_severity;
  }

  public bool HasRightKidneySeverity()
  {
    return right_kidney_severity == null ? false : right_kidney_severity.IsValid();
  }
  public SEScalar0To1 GetRightKidneySeverity()
  {
    if (right_kidney_severity == null)
      right_kidney_severity = new SEScalar0To1();
    return right_kidney_severity;
  }

}

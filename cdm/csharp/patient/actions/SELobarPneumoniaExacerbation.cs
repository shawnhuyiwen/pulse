/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SELobarPneumoniaExacerbation : SEPatientAction
{
  protected SEScalar0To1 severity;
  protected SEScalar0To1 leftLungAffected;
  protected SEScalar0To1 rightLungAffected;
  
  public SELobarPneumoniaExacerbation()
  {
    severity = null;
    leftLungAffected = null;
    rightLungAffected = null;
  }
  
  public override void Clear()
  {
    base.Clear();
    if (severity != null)
      severity.Invalidate();
    if (leftLungAffected != null)
      leftLungAffected.Invalidate();
    if (rightLungAffected != null)
      rightLungAffected.Invalidate();
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
    return leftLungAffected == null ? false : leftLungAffected.IsValid();
  }
  public SEScalar0To1 GetLeftLungAffected()
  {
    if (leftLungAffected == null)
      leftLungAffected = new SEScalar0To1();
    return leftLungAffected;
  }

    
  public bool HasRightLungAffected()
  {
    return rightLungAffected == null ? false : rightLungAffected.IsValid();
  }
  public SEScalar0To1 GetRightLungAffected()
  {
    if (rightLungAffected == null)
      rightLungAffected = new SEScalar0To1();
    return rightLungAffected;
  }
}

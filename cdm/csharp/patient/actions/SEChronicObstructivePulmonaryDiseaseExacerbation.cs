/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SEChronicObstructivePulmonaryDiseaseExacerbation : SEPatientAction
{
  protected SEScalar0To1 bronchitisSeverity;
  protected SEScalar0To1 emphysemaSeverity;

    public SEChronicObstructivePulmonaryDiseaseExacerbation()
  {
    bronchitisSeverity = null;
    emphysemaSeverity = null;
  }
  
  public override void Clear()
  {
    base.Clear();
    if (bronchitisSeverity != null)
      bronchitisSeverity.Invalidate();
    if (emphysemaSeverity != null)
      emphysemaSeverity.Invalidate();
  }
  
  public override bool IsValid()
  {
    return HasBronchitisSeverity() && HasEmphysemaSeverity();
  }
  
  public bool HasBronchitisSeverity()
  {
    return bronchitisSeverity == null ? false : bronchitisSeverity.IsValid();
  }
  public SEScalar0To1 GetBronchitisSeverity()
  {
    if (bronchitisSeverity == null)
      bronchitisSeverity = new SEScalar0To1();
    return bronchitisSeverity;
  }

  public bool HasEmphysemaSeverity()
  {
    return emphysemaSeverity == null ? false : emphysemaSeverity.IsValid();
  }
  public SEScalar0To1 GetEmphysemaSeverity()
  {
    if (emphysemaSeverity == null)
      emphysemaSeverity = new SEScalar0To1();
    return emphysemaSeverity;
  }
}

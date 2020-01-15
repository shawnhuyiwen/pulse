/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEChronicObstructivePulmonaryDiseaseExacerbation : SEPatientAction
  {
    protected SEScalar0To1 bronchitis_severity;
    protected SEScalar0To1 emphysema_severity;

    public SEChronicObstructivePulmonaryDiseaseExacerbation()
    {
      bronchitis_severity = null;
      emphysema_severity = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (bronchitis_severity != null)
        bronchitis_severity.Invalidate();
      if (emphysema_severity != null)
        emphysema_severity.Invalidate();
    }

    public override bool IsValid()
    {
      return HasBronchitisSeverity() && HasEmphysemaSeverity();
    }

    public bool HasBronchitisSeverity()
    {
      return bronchitis_severity == null ? false : bronchitis_severity.IsValid();
    }
    public SEScalar0To1 GetBronchitisSeverity()
    {
      if (bronchitis_severity == null)
        bronchitis_severity = new SEScalar0To1();
      return bronchitis_severity;
    }

    public bool HasEmphysemaSeverity()
    {
      return emphysema_severity == null ? false : emphysema_severity.IsValid();
    }
    public SEScalar0To1 GetEmphysemaSeverity()
    {
      if (emphysema_severity == null)
        emphysema_severity = new SEScalar0To1();
      return emphysema_severity;
    }
  }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEChestCompressionForceScale : SEPatientAction
  {
    protected SEScalar0To1 force_scale;
    protected SEScalarTime force_period;

    public SEChestCompressionForceScale()
    {
      force_scale = null;
      force_period = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (force_scale != null)
        force_scale.Invalidate();
      if (force_period != null)
        force_period.Invalidate();
    }

    public override bool IsValid()
    {
      return HasForceScale() && HasForcePeriod();
    }

    public bool HasForceScale()
    {
      return force_scale == null ? false : force_scale.IsValid();
    }
    public SEScalar0To1 GetForceScale()
    {
      if (force_scale == null)
        force_scale = new SEScalar0To1();
      return force_scale;
    }

    public bool HasForcePeriod()
    {
      return force_period == null ? false : force_period.IsValid();
    }
    public SEScalarTime GetForcePeriod()
    {
      if (force_period == null)
        force_period = new SEScalarTime();
      return force_period;
    }
  }
}

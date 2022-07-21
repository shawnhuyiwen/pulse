/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEChestCompressionAutomated : SEPatientAction
  {
    protected SEScalar0To1          applied_force_fraction;
    protected SEScalarFrequency     compression_frequency;
    protected SEScalarForce         force;
    protected SEScalar0To1          force_scale;

    public SEChestCompressionAutomated()
    {
      applied_force_fraction = null;
      compression_frequency = null;
      force = null;
      force_scale = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (force != null)
        force.Invalidate();
      if (force_scale != null)
        force_scale.Invalidate();
      if (applied_force_fraction != null)
        applied_force_fraction.Invalidate();
      if (compression_frequency != null)
        compression_frequency.Invalidate();
    }

    public override bool IsValid()
    {
      return (HasForce() || HasForceScale()) && HasCompressionFrequency();
    }

    public bool HasForce()
    {
      return force == null ? false : force.IsValid();
    }
    public SEScalarForce GetForce()
    {
      if (force == null)
        force = new SEScalarForce();
      return force;
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

    public bool HasAppliedForceFraction()
    {
      return applied_force_fraction == null ? false : applied_force_fraction.IsValid();
    }
    public SEScalar0To1 GetAppliedForceFraction()
    {
      if (applied_force_fraction == null)
        applied_force_fraction = new SEScalar0To1();
      return applied_force_fraction;
    }

    public bool HasCompressionFrequency()
    {
      return compression_frequency == null ? false : compression_frequency.IsValid();
    }
    public SEScalarFrequency GetCompressionFrequency()
    {
      if (compression_frequency == null)
        compression_frequency = new SEScalarFrequency();
      return compression_frequency;
    }
  }
}

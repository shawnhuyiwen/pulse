/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEChestCompression : SEPatientAction
  {
    protected SEScalarForce    force;
    protected SEScalarLength   depth;
    protected SEScalarTime     compression_period;

    public SEChestCompression()
    {
      force = null;
      depth = null;
      compression_period = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (force != null)
        force.Invalidate();
      if (depth != null)
        depth.Invalidate();
      if (compression_period != null)
        compression_period.Invalidate();
    }

    public override bool IsValid()
    {
      return (HasForce() || HasDepth()) && HasCompressionPeriod();
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

    public bool HasDepth()
    {
      return depth == null ? false : depth.IsValid();
    }
    public SEScalarLength GetDepth()
    {
      if (depth == null)
        depth = new SEScalarLength();
      return depth;
    }

    public bool HasCompressionPeriod()
    {
      return compression_period == null ? false : compression_period.IsValid();
    }
    public SEScalarTime GetCompressionPeriod()
    {
      if (compression_period == null)
        compression_period = new SEScalarTime();
      return compression_period;
    }
  }
}

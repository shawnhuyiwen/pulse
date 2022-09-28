/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEChestCompressionInstantaneous : SEPatientAction
  {
    protected SEScalarForce    force;
    protected SEScalarLength   depth;

    public SEChestCompressionInstantaneous()
    {
      force = null;
      depth = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (force != null)
        force.Invalidate();
      if (depth != null)
        depth.Invalidate();
    }

    public override bool IsValid()
    {
      return (HasForce() || HasDepth());
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

  }
}

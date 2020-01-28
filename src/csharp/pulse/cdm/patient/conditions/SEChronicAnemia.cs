/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEChronicAnemia : SEPatientCondition
  {
    protected SEScalar0To1 reduction_factor;

    public SEChronicAnemia()
    {
      reduction_factor = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (reduction_factor != null)
        reduction_factor.Invalidate();
    }

    public override bool IsValid()
    {
      return HasReductionFactor();
    }

    public bool HasReductionFactor()
    {
      return reduction_factor == null ? false : reduction_factor.IsValid();
    }
    public SEScalar0To1 GetReductionFactor()
    {
      if (reduction_factor == null)
        reduction_factor = new SEScalar0To1();
      return reduction_factor;
    }
  }
}

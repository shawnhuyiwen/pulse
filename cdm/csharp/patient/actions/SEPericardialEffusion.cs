/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEPericardialEffusion : SEPatientAction
  {
    protected SEScalarVolumePerTime effusion_rate;

    public SEPericardialEffusion()
    {
      effusion_rate = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (effusion_rate != null)
        effusion_rate.Invalidate();
    }

    public override bool IsValid()
    {
      return HasEffusionRate();
    }

    public bool HasEffusionRate()
    {
      return effusion_rate == null ? false : effusion_rate.IsValid();
    }
    public SEScalarVolumePerTime GetEffusionRate()
    {
      if (effusion_rate == null)
        effusion_rate = new SEScalarVolumePerTime();
      return effusion_rate;
    }
  }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEHemothorax : SEPatientAction
  {
    protected eSide side;
    protected SEScalar0To1 severity;
    protected SEScalarVolumePerTime flow_rate;
    protected SEScalarVolume target_volume;

    public SEHemothorax()
    {
      side = eSide.NullSide;
      severity = null;
      flow_rate = null;
      target_volume = null;
    }

    public override void Clear()
    {
      base.Clear();
      side = eSide.NullSide;
      if (severity != null)
        severity.Invalidate();
      if (flow_rate != null)
        flow_rate.Invalidate();
      if (target_volume != null)
        target_volume.Invalidate();
    }

    public override bool IsValid()
    {
      return HasSide() && (HasFlowRate() || HasSeverity());
    }

    public eSide GetSide()
    {
      return side;
    }
    public void SetSide(eSide side)
    {
      this.side = side;
    }
    public bool HasSide()
    {
      return side == eSide.NullSide ? false : true;
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

    public bool HasFlowRate()
    {
      return flow_rate == null ? false : flow_rate.IsValid();
    }
    public SEScalarVolumePerTime GetFlowRate()
    {
      if (flow_rate == null)
        flow_rate = new SEScalarVolumePerTime();
      return flow_rate;
    }

    public bool HasTargetVolume()
    {
      return target_volume == null ? false : target_volume.IsValid();
    }
    public SEScalarVolume GetTargetVolume()
    {
      if (target_volume == null)
        target_volume = new SEScalarVolume();
      return target_volume;
    }
  }
}

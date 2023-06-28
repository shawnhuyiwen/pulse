/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SETubeThoracostomy : SEPatientAction
  {
    protected eSide side;
    protected SEScalarVolumePerTime flow_rate;

    public SETubeThoracostomy()
    {
      side = eSide.NullSide;
      flow_rate = null;
    }

    public override void Clear()
    {
      base.Clear();
      side = eSide.NullSide;
      if (flow_rate != null)
        flow_rate.Invalidate();
    }

    public override bool IsValid()
    {
      return HasSide();
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
  }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEBagValveMaskInstantaneous : SEBagValveMaskAction
  {
    protected SEScalarVolumePerTime flow;
    protected SEScalarPressure pressure;

    public SEBagValveMaskInstantaneous()
    {
      flow = null;
      pressure = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (flow != null)
        flow.Invalidate();
      if (pressure != null)
        pressure.Invalidate();
    }

    public override bool IsValid()
    {
      return HasFlow() || HasPressure();
    }

    public bool HasFlow()
    {
      return flow == null ? false : flow.IsValid();
    }
    public SEScalarVolumePerTime GetFlow()
    {
      if (flow == null)
        flow = new SEScalarVolumePerTime();
      return flow;
    }

    public bool HasPressure()
    {
      return pressure == null ? false : pressure.IsValid();
    }
    public SEScalarPressure GetPressure()
    {
      if (pressure == null)
        pressure = new SEScalarPressure();
      return pressure;
    }

    public override string ToString()
    {
      return "Bag Valve Mask Instantaneous"
            + "\n\tFlow: " + (HasFlow() ? GetFlow().ToString() : "Not Provided")
            + "\n\tPressure: " + (HasPressure() ? GetPressure().ToString() : "Not Provided");
    }
  }
}

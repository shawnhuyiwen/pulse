/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SESegmentLinear : SESegment
  {
    protected SEScalarPressurePerVolume slope;
    protected SEScalarPressure          yIntercept;

    public SESegmentLinear() : base()
    {
      slope = null;
      yIntercept = null;
    }

    public SESegmentLinear(SESegment other)
    {
      Copy(other);
    }

    public void Copy(SESegmentLinear other)
    {
      base.Copy(other);
      if (other.HasSlope())
        GetSlope().Set(other.GetSlope());
      if (other.HasYIntercept())
        GetYIntercept().Set(other.GetYIntercept());
    }

    public override void Clear()
    {
      base.Clear();
      if (slope != null)
        slope.Invalidate();
      if (yIntercept != null)
        yIntercept.Invalidate();
    }

    public override bool IsValid()
    {
      if (!base.IsValid())
        return false;
      return HasSlope() && HasYIntercept();
    }

    public bool HasSlope()
    {
      return slope == null ? false : slope.IsValid();
    }
    public SEScalarPressurePerVolume GetSlope()
    {
      if (slope == null)
        slope = new SEScalarPressurePerVolume();
      return slope;
    }

    public bool HasYIntercept()
    {
      return yIntercept == null ? false : yIntercept.IsValid();
    }
    public SEScalarPressure GetYIntercept()
    {
      if (yIntercept == null)
        yIntercept = new SEScalarPressure();
      return yIntercept;
    }

    public override string ToString()
    {
      string str = base.ToString();
      str += "\n\tSlope: " + (HasSlope() ? GetSlope().ToString() : "Not Provided");
      str += "\n\tYIntercepte: " + (HasYIntercept() ? GetYIntercept().ToString() : "Not Provided");

      return str;
    }
  }
}

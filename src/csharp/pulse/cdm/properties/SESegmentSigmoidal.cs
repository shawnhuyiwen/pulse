/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SESegmentSigmoidal : SESegment
  {
    protected SEScalarPressure lowerCorner;
    protected SEScalarPressure upperCorner;
    protected SEScalarVolumePerPressure baselineCompliance;

    public SESegmentSigmoidal() : base()
    {
      lowerCorner = null;
      upperCorner = null;
      baselineCompliance = null;
    }

    public SESegmentSigmoidal(SESegment other)
    {
      Copy(other);
    }

    public void Copy(SESegmentSigmoidal other)
    {
      base.Copy(other);
      if (other.HasLowerCorner())
        GetLowerCorner().Set(other.GetLowerCorner());
      if (other.HasUpperCorner())
        GetUpperCorner().Set(other.GetUpperCorner());
      if (other.HasBaselineCompliance())
        GetBaselineCompliance().Set(other.GetBaselineCompliance());
    }

    public override void Clear()
    {
      base.Clear();
      if (lowerCorner != null)
        lowerCorner.Invalidate();
      if (upperCorner != null)
        upperCorner.Invalidate();
      if (baselineCompliance != null)
        baselineCompliance.Invalidate();
    }

    public override bool IsValid()
    {
      if (!base.IsValid())
        return false;
      return HasLowerCorner() && HasUpperCorner() &&
        HasBaselineCompliance();
    }

    public bool HasLowerCorner()
    {
      return lowerCorner == null ? false : lowerCorner.IsValid();
    }
    public SEScalarPressure GetLowerCorner()
    {
      if (lowerCorner == null)
        lowerCorner = new SEScalarPressure();
      return lowerCorner;
    }

    public bool HasUpperCorner()
    {
      return upperCorner == null ? false : upperCorner.IsValid();
    }
    public SEScalarPressure GetUpperCorner()
    {
      if (upperCorner == null)
        upperCorner = new SEScalarPressure();
      return upperCorner;
    }

    public bool HasBaselineCompliance()
    {
      return baselineCompliance == null ? false : baselineCompliance.IsValid();
    }
    public SEScalarVolumePerPressure GetBaselineCompliance()
    {
      if (baselineCompliance == null)
        baselineCompliance = new SEScalarVolumePerPressure();
      return baselineCompliance;
    }


    public override string ToString()
    {
      string str = base.ToString();
      str += "\n\tLowerCorner: " + (HasLowerCorner() ? GetLowerCorner().ToString() : "Not Provided");
      str += "\n\tUpperCorner: " + (HasUpperCorner() ? GetUpperCorner().ToString() : "Not Provided");
      str += "\n\tBaselineCompliance: " + (HasBaselineCompliance() ? GetBaselineCompliance().ToString() : "Not Provided");

      return str;
    }
  }
}

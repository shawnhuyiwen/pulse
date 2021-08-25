/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SESegmentConstant : SESegment
  {
    protected SEScalarVolumePerPressure compliance;

    public SESegmentConstant() : base()
    {
      compliance = null;
    }

    public SESegmentConstant(SESegment other)
    {
      Copy(other);
    }

    public void Copy(SESegmentConstant other)
    {
      base.Copy(other);
      if (other.HasCompliance())
        GetCompliance().Set(other.GetCompliance());
    }

    public override void Clear()
    {
      base.Clear();
      if (compliance != null)
        compliance.Invalidate();
    }

    public override bool IsValid()
    {
      if (!base.IsValid())
        return false;
      return HasCompliance();
    }

    public bool HasCompliance()
    {
      return compliance == null ? false : compliance.IsValid();
    }
    public SEScalarVolumePerPressure GetCompliance()
    {
      if (compliance == null)
        compliance = new SEScalarVolumePerPressure();
      return compliance;
    }

    public override string ToString()
    {
      string str = base.ToString();
      str += "\n\tCompliance: " + (HasCompliance() ? GetCompliance().ToString() : "Not Provided");

      return str;
    }
  }
}

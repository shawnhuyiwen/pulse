/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SESegment
  {
    protected SEScalarVolume beginVolume;
    protected SEScalarVolume endVolume;

    protected SESegment()
    {
      beginVolume = null;
      endVolume = null;
    }

    public SESegment(SESegment other)
    {
      Copy(other);
    }

    public void Copy(SESegment other)
    {
      if (other.HasBeginVolume())
        GetBeginVolume().Set(other.GetBeginVolume());
      if (other.HasEndVolume())
        GetEndVolume().Set(other.GetEndVolume());
    }

    public virtual void Clear()
    {
      if (beginVolume != null)
        beginVolume.Invalidate();
      if (endVolume != null)
        endVolume.Invalidate();
    }

    public virtual bool IsValid()
    {
      return HasBeginVolume() && HasEndVolume();
    }

    public bool HasBeginVolume()
    {
      return beginVolume == null ? false : beginVolume.IsValid();
    }
    public SEScalarVolume GetBeginVolume()
    {
      if (beginVolume == null)
        beginVolume = new SEScalarVolume();
      return beginVolume;
    }

    public bool HasEndVolume()
    {
      return endVolume == null ? false : endVolume.IsValid();
    }
    public SEScalarVolume GetEndVolume()
    {
      if (endVolume == null)
        endVolume = new SEScalarVolume();
      return endVolume;
    }

    public override string ToString()
    {
      string str="";
      str += "\n\tBeginVolume: " + (HasBeginVolume() ? GetBeginVolume().ToString() : "Not Provided");
      str += "\n\tEndVolume: " + (HasEndVolume() ? GetEndVolume().ToString() : "Not Provided");

      return str;
    }
  }
}

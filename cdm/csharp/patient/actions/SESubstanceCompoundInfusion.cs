/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SESubstanceCompoundInfusion : SEPatientAction
  {
    protected SEScalarVolume bag_volume;
    protected SEScalarVolumePerTime rate;
    protected string compound;

    public SESubstanceCompoundInfusion()
    {
      this.rate = null;
      this.bag_volume = null;
      this.compound = null;
    }

    public override void Clear()
    {
      base.Clear();
      this.compound = null;
      if (rate != null)
        rate.Invalidate();
      if (bag_volume != null)
        bag_volume.Invalidate();
    }

    public override bool IsValid()
    {
      return HasRate() && HasBagVolume() && HasSubstanceCompound();
    }

    public bool HasBagVolume()
    {
      return bag_volume == null ? false : bag_volume.IsValid();
    }
    public SEScalarVolume GetBagVolume()
    {
      if (bag_volume == null)
        bag_volume = new SEScalarVolume();
      return bag_volume;
    }

    public bool HasRate()
    {
      return rate == null ? false : rate.IsValid();
    }
    public SEScalarVolumePerTime GetRate()
    {
      if (rate == null)
        rate = new SEScalarVolumePerTime();
      return rate;
    }

    public bool HasSubstanceCompound() { return !string.IsNullOrEmpty(this.compound); }
    public string GetSubstanceCompound()
    {
      return compound;
    }
    public void SetSubstanceCompound(string s)
    {
      compound = s;
    }
  }
}

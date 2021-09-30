/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SESubstanceInfusion : SEPatientAction
  {
    protected SEScalarMassPerVolume concentration;
    protected SEScalarVolumePerTime rate;
    protected SEScalarVolume volume;
    protected string substance;

    public SESubstanceInfusion()
    {
      this.rate = null;
      this.concentration = null;
      this.volume = null;
      this.substance = null;
    }

    public override void Clear()
    {
      base.Clear();
      this.substance = null;
      if (rate != null)
        rate.Invalidate();
      if (concentration != null)
        concentration.Invalidate();
      if (volume != null)
        volume.Invalidate();
    }

    public override bool IsValid()
    {
      return HasRate() && HasConcentration() && HasSubstance();
    }

    public bool HasConcentration()
    {
      return concentration == null ? false : concentration.IsValid();
    }
    public SEScalarMassPerVolume GetConcentration()
    {
      if (concentration == null)
        concentration = new SEScalarMassPerVolume();
      return concentration;
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

    public bool HasVolume()
    {
      return volume == null ? false : volume.IsValid();
    }
    public SEScalarVolume GetVolume()
    {
      if (volume == null)
        volume = new SEScalarVolume();
      return volume;
    }

    public bool HasSubstance() { return !string.IsNullOrEmpty(substance); }
    public string GetSubstance()
    {
      return substance;
    }
    public void SetSubstance(string s)
    {
      substance = s;
    }
  }
}

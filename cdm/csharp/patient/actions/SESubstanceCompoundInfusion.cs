/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SESubstanceCompoundInfusion : SEPatientAction
{
    protected SEScalarVolume bagVolume;
    protected SEScalarVolumePerTime rate;
    protected string compound;

    public SESubstanceCompoundInfusion()
    {
        this.rate = null;
        this.bagVolume = null;
        this.compound = null;
    }

    public override void Clear()
    {
        base.Clear();
        this.compound = null;
        if (rate != null)
            rate.Invalidate();
        if (bagVolume != null)
            bagVolume.Invalidate();
    }

    public override bool IsValid()
    {
        return HasRate() && HasBagVolume() && HasSubstanceCompound();
    }

    public bool HasBagVolume()
    {
        return bagVolume == null ? false : bagVolume.IsValid();
    }
    public SEScalarVolume GetBagVolume()
    {
        if (bagVolume == null)
            bagVolume = new SEScalarVolume();
        return bagVolume;
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

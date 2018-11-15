/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SESubstanceInfusion : SEPatientAction
{
  protected SEScalarMassPerVolume concentration;
  protected SEScalarVolumePerTime rate;
  protected string substance;
  
  public SESubstanceInfusion(string substance)
  {
    this.rate = null;
    this.concentration = null;
    this.substance = substance;
  }
  
  public override void Reset()
  {
    base.Reset();
    if (rate != null)
      rate.Invalidate();
    if (concentration != null)
      concentration.Invalidate();
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
    return rate ==  null ? false : rate.IsValid();
  }
  public SEScalarVolumePerTime GetRate()
  {
    if (rate == null)
      rate = new SEScalarVolumePerTime();
    return rate;
  }
  
  public bool HasSubstance() { return !string.IsNullOrEmpty(substance); }
  public string GetSubstance()
  {
    return substance;
  }
}

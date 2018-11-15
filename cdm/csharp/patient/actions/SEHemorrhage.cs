/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SEHemorrhage : SEPatientAction
{
  protected string compartment;
  protected SEScalarVolumePerTime rate;
  
  public SEHemorrhage()
  {
    compartment = null;
    rate = null;
  }
  
  public override void Reset()
  {
    base.Reset();
    compartment = null;
    if (rate != null)
      rate.Invalidate();
  }
  
  public override bool IsValid()
  {
    return HasRate() && HasCompartment();
  }
  
  public string GetCompartment()
  {
    return compartment;
  }
  public void SetCompartment(string name)
  {
    compartment = name;
  }
  public bool HasCompartment()
  {
    return !string.IsNullOrEmpty(compartment);
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
}




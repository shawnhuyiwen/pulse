/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public enum HemorrhageType : int
{
  External = 0,
  Internal
}

public class SEHemorrhage : SEPatientAction
{
  protected string compartment;
  protected SEScalarVolumePerTime rate;
  protected HemorrhageType type;
  
  public SEHemorrhage()
  {
    compartment = null;
    rate = null;
    type = HemorrhageType.External;
  }
  
  public override void Clear()
  {
    base.Clear();
    compartment = null;
    if (rate != null)
      rate.Invalidate();
    type = HemorrhageType.External;
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

  public new HemorrhageType GetType()
  {
    return type;
  }
  public void SetType(HemorrhageType t)
  {
    type = t;
  }
}




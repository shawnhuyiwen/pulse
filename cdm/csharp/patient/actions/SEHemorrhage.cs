/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEHemorrhage : SEPatientAction
  {
    public enum eType : int
    {
      External = 0,
      Internal
    }

    protected string compartment;
    protected SEScalarVolumePerTime rate;
    protected eType type;

    public SEHemorrhage()
    {
      compartment = null;
      rate = null;
      type = eType.External;
    }

    public override void Clear()
    {
      base.Clear();
      compartment = null;
      if (rate != null)
        rate.Invalidate();
      type = eType.External;
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

    public new eType GetType()
    {
      return type;
    }
    public void SetType(eType t)
    {
      type = t;
    }
  }
}




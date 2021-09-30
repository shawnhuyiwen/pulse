/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEBagValveMaskAutomated : SEBagValveMaskAction
  {
    protected SEScalarFrequency breath_frequency;
    protected SEScalar inspiratory_expiratory_ratio;
    protected SEScalarPressure squeeze_pressure;
    protected SEScalarVolume squeeze_volume;

    public SEBagValveMaskAutomated()
    {
      breath_frequency = null;
      inspiratory_expiratory_ratio = null;
      squeeze_pressure = null;
      squeeze_volume = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (breath_frequency != null)
        breath_frequency.Invalidate();
      if (inspiratory_expiratory_ratio != null)
        inspiratory_expiratory_ratio.Invalidate();
      if (squeeze_pressure != null)
        squeeze_pressure.Invalidate();
      if (squeeze_volume != null)
        squeeze_volume.Invalidate();
    }

    public override bool IsValid()
    {
      return true;
    }

    public bool HasBreathFrequency()
    {
      return breath_frequency == null ? false : breath_frequency.IsValid();
    }
    public SEScalarFrequency GetBreathFrequency()
    {
      if (breath_frequency == null)
        breath_frequency = new SEScalarFrequency();
      return breath_frequency;
    }

    public bool HasInspiratoryExpiratoryRatio()
    {
      return inspiratory_expiratory_ratio == null ? false : inspiratory_expiratory_ratio.IsValid();
    }
    public SEScalar GetInspiratoryExpiratoryRatio()
    {
      if (inspiratory_expiratory_ratio == null)
        inspiratory_expiratory_ratio = new SEScalar();
      return inspiratory_expiratory_ratio;
    }

    public bool HasSqueezePressure()
    {
      return squeeze_pressure == null ? false : squeeze_pressure.IsValid();
    }
    public SEScalarPressure GetSqueezePressure()
    {
      if (squeeze_pressure == null)
        squeeze_pressure = new SEScalarPressure();
      return squeeze_pressure;
    }

    public bool HasSqueezeVolume()
    {
      return squeeze_volume == null ? false : squeeze_volume.IsValid();
    }
    public SEScalarVolume GetSqueezeVolume()
    {
      if (squeeze_volume == null)
        squeeze_volume = new SEScalarVolume();
      return squeeze_volume;
    }


    public override string ToString()
    {
      return "Bag Valve Mask Automated"
          + "\n\tBreathFrequency: " + (HasBreathFrequency() ? GetBreathFrequency().ToString() : "Not Provided")
          + "\n\tInspiratoryExpiratoryRatio: " + (HasInspiratoryExpiratoryRatio() ? GetInspiratoryExpiratoryRatio().ToString() : "Not Provided")
          + "\n\tSqueezePressure: " + (HasSqueezePressure() ? GetSqueezePressure().ToString() : "Not Provided")
          + "\n\tSqueezeVolume: " + (HasSqueezeVolume() ? GetSqueezeVolume().ToString() : "Not Provided");
    }
  }
}

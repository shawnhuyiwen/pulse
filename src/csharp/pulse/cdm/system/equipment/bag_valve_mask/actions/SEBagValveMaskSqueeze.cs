/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEBagValveMaskSqueeze : SEBagValveMaskAction
  {
    protected SEScalarPressure squeeze_pressure;
    protected SEScalarVolume squeeze_volume;
    protected SEScalarTime expiratory_period;
    protected SEScalarTime inspriatory_period;

    public SEBagValveMaskSqueeze()
    {
      squeeze_pressure = null;
      squeeze_volume = null;
      expiratory_period = null;
      inspriatory_period = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (squeeze_pressure != null)
        squeeze_pressure.Invalidate();
      if (squeeze_volume != null)
        squeeze_volume.Invalidate();
      if (expiratory_period != null)
        expiratory_period.Invalidate();
      if (inspriatory_period != null)
        inspriatory_period.Invalidate();
    }

    public override bool IsValid()
    {
      return true;
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

    public bool HasExpiratoryPeriod()
    {
      return expiratory_period == null ? false : expiratory_period.IsValid();
    }
    public SEScalarTime GetExpiratoryPeriod()
    {
      if (expiratory_period == null)
        expiratory_period = new SEScalarTime();
      return expiratory_period;
    }

    public bool HasInspiratoryPeriod()
    {
      return inspriatory_period == null ? false : inspriatory_period.IsValid();
    }
    public SEScalarTime GetInspiratoryPeriod()
    {
      if (inspriatory_period == null)
        inspriatory_period = new SEScalarTime();
      return inspriatory_period;
    }

    public override string ToString()
    {
        return "Bag Valve Mask Squeeze"
            + "\n\tSqueezePressure: " + (HasSqueezePressure() ? GetSqueezePressure().ToString() : "Not Provided")
            + "\n\tSqueezeVolume: " + (HasSqueezeVolume() ? GetSqueezeVolume().ToString() : "Not Provided")
            + "\n\tExpiratoryPeriod: " + (HasExpiratoryPeriod() ? GetExpiratoryPeriod().ToString() : "Not Provided")
            + "\n\tInspiratoryPeriod: " + (HasInspiratoryPeriod() ? GetInspiratoryPeriod().ToString() : "Not Provided");
    }
  }
}

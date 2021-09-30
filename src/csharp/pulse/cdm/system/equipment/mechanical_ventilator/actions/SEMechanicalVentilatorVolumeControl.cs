/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eMechanicalVentilator_VolumeControlMode : int
  {
    AssistedControl = 0,
    ContinuousMandatoryVentilation
  }
  public class SEMechanicalVentilatorVolumeControl : SEMechanicalVentilatorMode
  {
    protected eMechanicalVentilator_VolumeControlMode mode;
    protected SEScalarVolumePerTime                   flow;
    protected SEScalar0To1                            fractionInspiredOxygen;
    protected SEScalarTime                            inspiratoryPeriod;
    protected SEScalarPressure                        positiveEndExpiredPressure;
    protected SEScalarFrequency                       respirationRate;
    protected SEScalarVolume                          tidalVolume;

    public SEMechanicalVentilatorVolumeControl()
    {
      mode = eMechanicalVentilator_VolumeControlMode.AssistedControl;
      flow = null;
      fractionInspiredOxygen = null;
      inspiratoryPeriod = null;
      positiveEndExpiredPressure = null;
      respirationRate = null;
      tidalVolume = null;
    }

    public SEMechanicalVentilatorVolumeControl(SEMechanicalVentilatorVolumeControl other)
    {
      Copy(other);
    }

    public void copy(SEMechanicalVentilatorVolumeControl other)
    {
      base.Copy(other);
      if (other.HasFlow())
        GetFlow().Set(other.GetFlow());
      if (other.HasFractionInspiredOxygen())
        GetFractionInspiredOxygen().Set(other.GetFractionInspiredOxygen());
      if (other.HasInspiratoryPeriod())
        GetInspiratoryPeriod().Set(other.GetInspiratoryPeriod());
      if (other.HasPositiveEndExpiredPressure())
        GetPositiveEndExpiredPressure().Set(other.GetPositiveEndExpiredPressure());
      if (other.HasRespirationRate())
        GetRespirationRate().Set(other.GetRespirationRate());
      if (other.HasTidalVolume())
        GetTidalVolume().Set(other.GetTidalVolume());
    }

    public override void Clear()
    {
      base.Clear();
      mode = eMechanicalVentilator_VolumeControlMode.AssistedControl;
      if (flow != null)
        flow.Invalidate();
      if (fractionInspiredOxygen != null)
        fractionInspiredOxygen.Invalidate();
      if (inspiratoryPeriod != null)
        inspiratoryPeriod.Invalidate();
      if (positiveEndExpiredPressure != null)
        positiveEndExpiredPressure.Invalidate();
      if (respirationRate != null)
        respirationRate.Invalidate();
      if (tidalVolume != null)
        tidalVolume.Invalidate();
    }

    public override bool IsValid()
    {
      return HasFlow() &&
        HasFractionInspiredOxygen() &&
        HasInspiratoryPeriod() &&
        HasPositiveEndExpiredPressure() &&
        HasRespirationRate() &&
        HasTidalVolume();
    }

    public eMechanicalVentilator_VolumeControlMode GetMode()
    {
      return mode;
    }
    public void SetMode(eMechanicalVentilator_VolumeControlMode m)
    {
      mode = m;
    }

    public bool HasFlow()
    {
      return flow == null ? false : flow.IsValid();
    }
    public SEScalarVolumePerTime GetFlow()
    {
      if (flow == null)
        flow = new SEScalarVolumePerTime();
      return flow;
    }

    public bool HasFractionInspiredOxygen()
    {
      return fractionInspiredOxygen == null ? false : fractionInspiredOxygen.IsValid();
    }
    public SEScalar0To1 GetFractionInspiredOxygen()
    {
      if (fractionInspiredOxygen == null)
        fractionInspiredOxygen = new SEScalar0To1();
      return fractionInspiredOxygen;
    }
    public bool HasInspiratoryPeriod()
    {
      return inspiratoryPeriod == null ? false : inspiratoryPeriod.IsValid();
    }
    public SEScalarTime GetInspiratoryPeriod()
    {
      if (inspiratoryPeriod == null)
        inspiratoryPeriod = new SEScalarTime();
      return inspiratoryPeriod;
    }
    public bool HasPositiveEndExpiredPressure()
    {
      return positiveEndExpiredPressure == null ? false : positiveEndExpiredPressure.IsValid();
    }
    public SEScalarPressure GetPositiveEndExpiredPressure()
    {
      if (positiveEndExpiredPressure == null)
        positiveEndExpiredPressure = new SEScalarPressure();
      return positiveEndExpiredPressure;
    }
    public bool HasRespirationRate()
    {
      return respirationRate == null ? false : respirationRate.IsValid();
    }
    public SEScalarFrequency GetRespirationRate()
    {
      if (respirationRate == null)
        respirationRate = new SEScalarFrequency();
      return respirationRate;
    }
    public bool HasTidalVolume()
    {
      return tidalVolume == null ? false : tidalVolume.IsValid();
    }
    public SEScalarVolume GetTidalVolume()
    {
      if (tidalVolume == null)
        tidalVolume = new SEScalarVolume();
      return tidalVolume;
    }

    public override string ToString()
    {
      string str = "Mechanical Ventilator Volume Control";
      str += "\n\tMode: " + this.mode;
      str += "\n\tFlow: " + (HasFlow() ? GetFlow().ToString() : "Not Provided");
      str += "\n\tFractionInspiredOxygen: " + (HasFractionInspiredOxygen() ? GetFractionInspiredOxygen().ToString() : "Not Provided");
      str += "\n\tInspiratoryPeriod: " + (HasInspiratoryPeriod() ? GetInspiratoryPeriod().ToString() : "Not Provided");
      str += "\n\tPositiveEndExpiredPressure: " + (HasPositiveEndExpiredPressure() ? GetPositiveEndExpiredPressure().ToString() : "Not Provided");
      str += "\n\tRespirationRate: " + (HasRespirationRate() ? GetRespirationRate().ToString() : "Not Provided");
      str += "\n\tTidalVolume: " + (HasTidalVolume() ? GetTidalVolume().ToString() : "Not Provided");


      return str;
    }
  }
}

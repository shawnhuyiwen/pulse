/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public enum eMechanicalVentilator_PressureControlMode : int
  {
    AssistedControl = 0,
    ContinuousMandatoryVentilation
  }
  public class SEMechanicalVentilatorPressureControl : SEMechanicalVentilatorMode
  {
    protected eMechanicalVentilator_PressureControlMode mode;
    protected SEScalar0To1 fractionInspiredOxygen;
    protected SEScalarTime inspiratoryPeriod;
    protected SEScalarPressure inspiratoryPressure;
    protected SEScalarPressure positiveEndExpiredPressure;
    protected SEScalarFrequency respirationRate;
    protected SEScalarTime slope;

    public SEMechanicalVentilatorPressureControl()
    {
      mode = eMechanicalVentilator_PressureControlMode.AssistedControl;
      fractionInspiredOxygen = null;
      inspiratoryPeriod = null;
      inspiratoryPressure = null;
      positiveEndExpiredPressure = null;
      respirationRate = null;
      slope = null;
    }

    public SEMechanicalVentilatorPressureControl(SEMechanicalVentilatorPressureControl other)
    {
      Copy(other);
    }

    public void copy(SEMechanicalVentilatorPressureControl other)
    {
      base.Copy(other);
      if (other.HasFractionInspiredOxygen())
        GetFractionInspiredOxygen().Set(other.GetFractionInspiredOxygen());
      if (other.HasInspiratoryPeriod())
        GetInspiratoryPeriod().Set(other.GetInspiratoryPeriod());
      if (other.HasInspiratoryPressure())
        GetInspiratoryPressure().Set(other.GetInspiratoryPressure());
      if (other.HasPositiveEndExpiredPressure())
        GetPositiveEndExpiredPressure().Set(other.GetPositiveEndExpiredPressure());
      if (other.HasRespirationRate())
        GetRespirationRate().Set(other.GetRespirationRate());
      if (other.HasSlope())
        GetSlope().Set(other.GetSlope());
    }

    public override void Clear()
    {
      base.Clear();
      mode = eMechanicalVentilator_PressureControlMode.AssistedControl;
      if (fractionInspiredOxygen != null)
        fractionInspiredOxygen.Invalidate();
      if (inspiratoryPeriod != null)
        inspiratoryPeriod.Invalidate();
      if (inspiratoryPressure != null)
        inspiratoryPressure.Invalidate();
      if (positiveEndExpiredPressure != null)
        positiveEndExpiredPressure.Invalidate();
      if (respirationRate != null)
        respirationRate.Invalidate();
      if (slope != null)
        slope.Invalidate();
    }

    public override bool IsValid()
    {
      return HasFractionInspiredOxygen() &&
        HasInspiratoryPeriod() &&
        HasInspiratoryPressure() &&
        HasPositiveEndExpiredPressure() &&
        HasRespirationRate() &&
        HasSlope();
    }

    public eMechanicalVentilator_PressureControlMode GetMode()
    {
      return mode;
    }
    public void SetMode(eMechanicalVentilator_PressureControlMode m)
    {
      mode = m;
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

    public bool HasInspiratoryPressure()
    {
      return inspiratoryPressure == null ? false : inspiratoryPressure.IsValid();
    }
    public SEScalarPressure GetInspiratoryPressure()
    {
      if (inspiratoryPressure == null)
        inspiratoryPressure = new SEScalarPressure();
      return inspiratoryPressure;
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
    public bool HasSlope()
    {
      return slope == null ? false : slope.IsValid();
    }
    public SEScalarTime GetSlope()
    {
      if (slope == null)
        slope = new SEScalarTime();
      return slope;
    }

    public override string ToString()
    {
      string str = "Mechanical Ventilator Pressure Control";
      str += "\n\tMode: " + this.mode;
      str += "\n\tFractionInspiredOxygen: " + (HasFractionInspiredOxygen() ? GetFractionInspiredOxygen().ToString() : "Not Provided");
      str += "\n\tInspiratoryPeriod: " + (HasInspiratoryPeriod() ? GetInspiratoryPeriod().ToString() : "Not Provided");
      str += "\n\tInspiratoryPressure: " + (HasInspiratoryPressure() ? GetInspiratoryPressure().ToString() : "Not Provided");
      str += "\n\tPositiveEndExpiredPressure: " + (HasPositiveEndExpiredPressure() ? GetPositiveEndExpiredPressure().ToString() : "Not Provided");
      str += "\n\tRespirationRate: " + (HasRespirationRate() ? GetRespirationRate().ToString() : "Not Provided");
      str += "\n\tSlope: " + (HasSlope() ? GetSlope().ToString() : "Not Provided");


      return str;
    }
  }
}

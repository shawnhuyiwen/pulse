/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEMechanicalVentilatorContinuousPositiveAirwayPressure : SEMechanicalVentilatorAction
  {
    protected SEScalarPressure deltaPressureSupport;
    protected SEScalar0To1     fractionInspiredOxygen;
    protected SEScalarPressure positiveEndExpiredPressure;
    protected SEScalarTime     slope;

    public SEMechanicalVentilatorContinuousPositiveAirwayPressure()
    {
      deltaPressureSupport = null;
      fractionInspiredOxygen = null;
      positiveEndExpiredPressure = null;
      slope = null;
    }

    public SEMechanicalVentilatorContinuousPositiveAirwayPressure(SEMechanicalVentilatorContinuousPositiveAirwayPressure other)
    {
      Copy(other);
    }

    public void copy(SEMechanicalVentilatorContinuousPositiveAirwayPressure other)
    {
      base.Copy(other);
      if (other.HasDeltaPressureSupport())
        GetDeltaPressureSupport().Set(other.GetDeltaPressureSupport());
      if (other.HasFractionInspiredOxygen())
        GetFractionInspiredOxygen().Set(other.GetFractionInspiredOxygen());
      if (other.HasPositiveEndExpiredPressure())
        GetPositiveEndExpiredPressure().Set(other.GetPositiveEndExpiredPressure());
      if (other.HasSlope())
        GetSlope().Set(other.GetSlope());
    }

    public override void Clear()
    {
      base.Clear();
      if (deltaPressureSupport != null)
        deltaPressureSupport.Invalidate();
      if (fractionInspiredOxygen != null)
        fractionInspiredOxygen.Invalidate();
      if (positiveEndExpiredPressure != null)
        positiveEndExpiredPressure.Invalidate();
      if (slope != null)
        slope.Invalidate();
    }

    public override bool IsValid()
    {
      return HasDeltaPressureSupport() && 
        HasFractionInspiredOxygen() &&
        HasPositiveEndExpiredPressure() &&
        HasSlope();
    }

    public bool HasDeltaPressureSupport()
    {
      return deltaPressureSupport == null ? false : deltaPressureSupport.IsValid();
    }
    public SEScalarPressure GetDeltaPressureSupport()
    {
      if (deltaPressureSupport == null)
        deltaPressureSupport = new SEScalarPressure();
      return deltaPressureSupport;
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
      string str = "Mechanical Ventilator Volume Control";
      str += "\n\tDeltaPressureSupport: " + (HasDeltaPressureSupport() ? GetDeltaPressureSupport().ToString() : "Not Provided");
      str += "\n\tFractionInspiredOxygen: " + (HasFractionInspiredOxygen() ? GetFractionInspiredOxygen().ToString() : "Not Provided");
      str += "\n\tPositiveEndExpiredPressure: " + (HasPositiveEndExpiredPressure() ? GetPositiveEndExpiredPressure().ToString() : "Not Provided");
      str += "\n\tSlope: " + (HasSlope() ? GetSlope().ToString() : "Not Provided");


      return str;
    }
  }
}

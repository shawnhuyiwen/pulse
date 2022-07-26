/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SERespiratoryMechanics
  {
    protected eSwitch                       active;
    protected eDefaultType                  default_type;
    protected SECurve                       left_compliance_curve;
    protected SECurve                       right_compliance_curve;
    protected SEScalarPressureTimePerVolume left_expiratory_resistance;
    protected SEScalarPressureTimePerVolume left_inspiratory_resistance;
    protected SEScalarPressureTimePerVolume right_expiratory_resistance;
    protected SEScalarPressureTimePerVolume right_inspiratory_resistance;
    protected SEScalarPressureTimePerVolume upper_expiratory_resistance;
    protected SEScalarPressureTimePerVolume upper_inspiratory_resistance;
    protected SEScalarPressure              inspiratory_peak_pressure;
    protected SEScalarPressure              expiratory_peak_pressure;
    protected SEScalarTime                  inspiratory_rise_time;
    protected SEScalarTime                  inspiratory_hold_time;
    protected SEScalarTime                  inspiratory_release_time;
    protected SEScalarTime                  inspiratory_to_expiratory_pause_time;
    protected SEScalarTime                  expiratory_rise_time;
    protected SEScalarTime                  expiratory_hold_time;
    protected SEScalarTime                  expiratory_release_time;
    protected SEScalarTime                  residue_time;

    public SERespiratoryMechanics()
    {
      active = eSwitch.NullSwitch;
      default_type = eDefaultType.Model;
      left_compliance_curve = null;
      right_compliance_curve = null;
      left_expiratory_resistance = null;
      left_inspiratory_resistance = null;
      right_expiratory_resistance = null;
      right_inspiratory_resistance = null;
      upper_expiratory_resistance = null;
      upper_inspiratory_resistance = null;
      inspiratory_peak_pressure = null;
      expiratory_peak_pressure = null;
      inspiratory_rise_time = null;
      inspiratory_hold_time = null;
      inspiratory_release_time = null;
      inspiratory_to_expiratory_pause_time = null;
      expiratory_rise_time = null;
      expiratory_hold_time = null;
      expiratory_release_time = null;
      residue_time = null;
    }

    public void Clear()
    {
      active = eSwitch.NullSwitch;
      default_type = eDefaultType.Model;
      if (left_compliance_curve != null)
        left_compliance_curve.Invalidate();
      if (right_compliance_curve != null)
        right_compliance_curve.Invalidate();
      if (left_expiratory_resistance != null)
        left_expiratory_resistance.Invalidate();
      if (left_inspiratory_resistance != null)
        left_inspiratory_resistance.Invalidate();
      if (right_expiratory_resistance != null)
        right_expiratory_resistance.Invalidate();
      if (right_inspiratory_resistance != null)
        right_inspiratory_resistance.Invalidate();
      if (upper_expiratory_resistance != null)
        upper_expiratory_resistance.Invalidate();
      if (upper_inspiratory_resistance != null)
        upper_inspiratory_resistance.Invalidate();
      if (inspiratory_peak_pressure != null)
        inspiratory_peak_pressure.Invalidate();
      if (expiratory_peak_pressure != null)
        expiratory_peak_pressure.Invalidate();
      if (inspiratory_rise_time != null)
        inspiratory_rise_time.Invalidate();
      if (inspiratory_hold_time != null)
        inspiratory_hold_time.Invalidate();
      if (inspiratory_release_time != null)
        inspiratory_release_time.Invalidate();
      if (inspiratory_to_expiratory_pause_time != null)
        inspiratory_to_expiratory_pause_time.Invalidate();
      if (expiratory_rise_time != null)
        expiratory_rise_time.Invalidate();
      if (expiratory_hold_time != null)
        expiratory_hold_time.Invalidate();
      if (expiratory_release_time != null)
        expiratory_release_time.Invalidate();
      if (residue_time != null)
        residue_time.Invalidate();
    }

    public void Copy(SERespiratoryMechanics from)
    {
      Clear();
      if(HasActive())
        SetActive(from.GetActive());
      default_type = from.default_type;
      if (from.HasLeftComplianceCurve())
        GetLeftComplianceCurve().Copy(from.GetLeftComplianceCurve());
      if (from.HasRightComplianceCurve())
        GetRightComplianceCurve().Copy(from.GetRightComplianceCurve());
      if (from.HasLeftExpiratoryResistance())
        GetLeftExpiratoryResistance().Copy(from.GetLeftExpiratoryResistance());
      if (from.HasLeftInspiratoryResistance())
        GetLeftInspiratoryResistance().Copy(from.GetLeftInspiratoryResistance());
      if (from.HasRightExpiratoryResistance())
        GetRightExpiratoryResistance().Copy(from.GetRightExpiratoryResistance());
      if (from.HasRightInspiratoryResistance())
        GetRightInspiratoryResistance().Copy(from.GetRightInspiratoryResistance());
      if (from.HasUpperExpiratoryResistance())
        GetUpperExpiratoryResistance().Copy(from.GetUpperExpiratoryResistance());
      if (from.HasUpperInspiratoryResistance())
        GetUpperInspiratoryResistance().Copy(from.GetUpperInspiratoryResistance());
      if (from.HasInspiratoryPeakPressure())
        GetInspiratoryPeakPressure().Copy(from.GetInspiratoryPeakPressure());
      if (from.HasExpiratoryPeakPressure())
        GetExpiratoryPeakPressure().Copy(from.GetExpiratoryPeakPressure());
      if (from.HasInspiratoryRiseTime())
        GetInspiratoryRiseTime().Copy(from.GetInspiratoryRiseTime());
      if (from.HasInspiratoryHoldTime())
        GetInspiratoryHoldTime().Copy(from.GetInspiratoryHoldTime());
      if (from.HasInspiratoryReleaseTime())
        GetInspiratoryReleaseTime().Copy(from.GetInspiratoryReleaseTime());
      if (from.HasInspiratoryToExpiratoryPauseTime())
        GetInspiratoryToExpiratoryPauseTime().Copy(from.GetInspiratoryToExpiratoryPauseTime());
      if (from.HasExpiratoryRiseTime())
        GetExpiratoryRiseTime().Copy(from.GetExpiratoryRiseTime());
      if (from.HasExpiratoryHoldTime())
        GetExpiratoryHoldTime().Copy(from.GetExpiratoryHoldTime());
      if (from.HasExpiratoryReleaseTime())
        GetExpiratoryReleaseTime().Copy(from.GetExpiratoryReleaseTime());
      if (from.HasResidueTime())
        GetResidueTime().Copy(from.GetResidueTime());
    }

    public bool HasActive()
    {
      if (active == eSwitch.NullSwitch)
        return false;
      return true;
    }
    public eSwitch GetActive()
    {
      return active;
    }
    public void SetActive(eSwitch s)
    {
      active = s;
    }

    public eDefaultType GetDefaultType()
    {
      return default_type;
    }
    public void SetDefaultType(eDefaultType t)
    {
      default_type = t;
    }

    public bool HasLeftComplianceCurve()
    {
      if (left_compliance_curve != null)
        return left_compliance_curve.IsValid();
      return false;
    }
    public SECurve GetLeftComplianceCurve()
    {
      if (left_compliance_curve == null)
        left_compliance_curve = new SECurve();
      return left_compliance_curve;
    }

    public bool HasRightComplianceCurve()
    {
      if (right_compliance_curve != null)
        return right_compliance_curve.IsValid();
      return false;
    }
    public SECurve GetRightComplianceCurve()
    {
      if (right_compliance_curve == null)
        right_compliance_curve = new SECurve();
      return right_compliance_curve;
    }

    public bool HasLeftExpiratoryResistance()
    {
      if (left_expiratory_resistance != null)
        return left_expiratory_resistance.IsValid();
      return false;
    }
    public SEScalarPressureTimePerVolume GetLeftExpiratoryResistance()
    {
      if (left_expiratory_resistance == null)
        left_expiratory_resistance = new SEScalarPressureTimePerVolume();
      return left_expiratory_resistance;
    }

    public bool HasLeftInspiratoryResistance()
    {
      if (left_inspiratory_resistance != null)
        return left_inspiratory_resistance.IsValid();
      return false;
    }
    public SEScalarPressureTimePerVolume GetLeftInspiratoryResistance()
    {
      if (left_inspiratory_resistance == null)
        left_inspiratory_resistance = new SEScalarPressureTimePerVolume();
      return left_inspiratory_resistance;
    }

    public bool HasRightExpiratoryResistance()
    {
      if (right_expiratory_resistance != null)
        return right_expiratory_resistance.IsValid();
      return false;
    }
    public SEScalarPressureTimePerVolume GetRightExpiratoryResistance()
    {
      if (right_expiratory_resistance == null)
        right_expiratory_resistance = new SEScalarPressureTimePerVolume();
      return right_expiratory_resistance;
    }

    public bool HasRightInspiratoryResistance()
    {
      if (right_inspiratory_resistance != null)
        return right_inspiratory_resistance.IsValid();
      return false;
    }
    public SEScalarPressureTimePerVolume GetRightInspiratoryResistance()
    {
      if (right_inspiratory_resistance == null)
        right_inspiratory_resistance = new SEScalarPressureTimePerVolume();
      return right_inspiratory_resistance;
    }

    public bool HasUpperExpiratoryResistance()
    {
      if (upper_expiratory_resistance != null)
        return upper_expiratory_resistance.IsValid();
      return false;
    }
    public SEScalarPressureTimePerVolume GetUpperExpiratoryResistance()
    {
      if (upper_expiratory_resistance == null)
        upper_expiratory_resistance = new SEScalarPressureTimePerVolume();
      return upper_expiratory_resistance;
    }

    public bool HasUpperInspiratoryResistance()
    {
      if (upper_inspiratory_resistance != null)
        return upper_inspiratory_resistance.IsValid();
      return false;
    }
    public SEScalarPressureTimePerVolume GetUpperInspiratoryResistance()
    {
      if (upper_inspiratory_resistance == null)
        upper_inspiratory_resistance = new SEScalarPressureTimePerVolume();
      return upper_inspiratory_resistance;
    }

    public bool HasInspiratoryPeakPressure()
    {
      if (inspiratory_peak_pressure != null)
        return inspiratory_peak_pressure.IsValid();
      return false;
    }
    public SEScalarPressure GetInspiratoryPeakPressure()
    {
      if (inspiratory_peak_pressure == null)
        inspiratory_peak_pressure = new SEScalarPressure();
      return inspiratory_peak_pressure;
    }

    public bool HasExpiratoryPeakPressure()
    {
      if (expiratory_peak_pressure != null)
        return expiratory_peak_pressure.IsValid();
      return false;
    }
    public SEScalarPressure GetExpiratoryPeakPressure()
    {
      if (expiratory_peak_pressure == null)
        expiratory_peak_pressure = new SEScalarPressure();
      return expiratory_peak_pressure;
    }

    public bool HasInspiratoryRiseTime()
    {
      if (inspiratory_rise_time != null)
        return inspiratory_rise_time.IsValid();
      return false;
    }
    public SEScalarTime GetInspiratoryRiseTime()
    {
      if (inspiratory_rise_time == null)
        inspiratory_rise_time = new SEScalarTime();
      return inspiratory_rise_time;
    }

    public bool HasInspiratoryHoldTime()
    {
      if (inspiratory_hold_time != null)
        return inspiratory_hold_time.IsValid();
      return false;
    }
    public SEScalarTime GetInspiratoryHoldTime()
    {
      if (inspiratory_hold_time == null)
        inspiratory_hold_time = new SEScalarTime();
      return inspiratory_hold_time;
    }

    public bool HasInspiratoryReleaseTime()
    {
      if (inspiratory_release_time != null)
        return inspiratory_release_time.IsValid();
      return false;
    }
    public SEScalarTime GetInspiratoryReleaseTime()
    {
      if (inspiratory_release_time == null)
        inspiratory_release_time = new SEScalarTime();
      return inspiratory_release_time;
    }

    public bool HasInspiratoryToExpiratoryPauseTime()
    {
      if (inspiratory_to_expiratory_pause_time != null)
        return inspiratory_to_expiratory_pause_time.IsValid();
      return false;
    }
    public SEScalarTime GetInspiratoryToExpiratoryPauseTime()
    {
      if (inspiratory_to_expiratory_pause_time == null)
        inspiratory_to_expiratory_pause_time = new SEScalarTime();
      return inspiratory_to_expiratory_pause_time;
    }

    public bool HasExpiratoryRiseTime()
    {
      if (expiratory_rise_time != null)
        return expiratory_rise_time.IsValid();
      return false;
    }
    public SEScalarTime GetExpiratoryRiseTime()
    {
      if (expiratory_rise_time == null)
        expiratory_rise_time = new SEScalarTime();
      return expiratory_rise_time;
    }

    public bool HasExpiratoryHoldTime()
    {
      if (expiratory_hold_time != null)
        return expiratory_hold_time.IsValid();
      return false;
    }
    public SEScalarTime GetExpiratoryHoldTime()
    {
      if (expiratory_hold_time == null)
        expiratory_hold_time = new SEScalarTime();
      return expiratory_hold_time;
    }

    public bool HasExpiratoryReleaseTime()
    {
      if (expiratory_release_time != null)
        return expiratory_release_time.IsValid();
      return false;
    }
    public SEScalarTime GetExpiratoryReleaseTime()
    {
      if (expiratory_release_time == null)
        expiratory_release_time = new SEScalarTime();
      return expiratory_release_time;
    }

    public bool HasResidueTime()
    {
      if (residue_time != null)
        return residue_time.IsValid();
      return false;
    }
    public SEScalarTime GetResidueTime()
    {
      if (residue_time == null)
        residue_time = new SEScalarTime();
      return residue_time;
    }

    public override string ToString()
    {
      string str = "Respiratory Mechanics";
      str += "\n\tActive: "; if (HasActive()) str += active; else str += "Not Set";
      str += "\n\tDefaultType: " + eEnum.Name(GetDefaultType());
      str += "\n\tLeftComplianceCurve: "; if (HasLeftComplianceCurve()) str += left_compliance_curve.ToString(); else str += "Not Set";
      str += "\n\tRightComplianceCurve: "; if (HasRightComplianceCurve()) str += right_compliance_curve.ToString(); else str += "Not Set";
      str += "\n\tLeftExpiratoryResistance: "; if (HasLeftExpiratoryResistance()) str += left_expiratory_resistance.ToString(); else str += "Not Set";
      str += "\n\tLeftInspiratoryResistance: "; if (HasLeftInspiratoryResistance()) str += left_inspiratory_resistance.ToString(); else str += "Not Set";
      str += "\n\tRightExpiratoryResistance: "; if (HasRightExpiratoryResistance()) str += right_expiratory_resistance.ToString(); else str += "Not Set";
      str += "\n\tRightInspiratoryResistance: "; if (HasRightInspiratoryResistance()) str += right_inspiratory_resistance.ToString(); else str += "Not Set";
      str += "\n\tUpperExpiratoryResistance: "; if (HasUpperExpiratoryResistance()) str += upper_expiratory_resistance.ToString(); else str += "Not Set";
      str += "\n\tUpperInspiratoryResistance: "; if (HasUpperInspiratoryResistance()) str += upper_inspiratory_resistance.ToString(); else str += "Not Set";
      str += "\n\tInspiratoryPeakPressure: "; if (HasInspiratoryPeakPressure()) str += inspiratory_peak_pressure.ToString(); else str += "Not Set";
      str += "\n\tExpiratoryPeakPressure: "; if (HasExpiratoryPeakPressure()) str += expiratory_peak_pressure.ToString(); else str += "Not Set";
      str += "\n\tInspiratoryRiseTime: "; if (HasInspiratoryRiseTime()) str += inspiratory_rise_time.ToString(); else str += "Not Set";
      str += "\n\tInspiratoryHoldTime: "; if (HasInspiratoryHoldTime()) str += inspiratory_hold_time.ToString(); else str += "Not Set";
      str += "\n\tInspiratoryReleaseTime: "; if (HasInspiratoryReleaseTime()) str += inspiratory_release_time.ToString(); else str += "Not Set";
      str += "\n\tInspiratoryToExpiratoryPauseTime: "; if (HasInspiratoryToExpiratoryPauseTime()) str += inspiratory_to_expiratory_pause_time.ToString(); else str += "Not Set";
      str += "\n\tExpiratoryRiseTime: "; if (HasExpiratoryRiseTime()) str += expiratory_rise_time.ToString(); else str += "Not Set";
      str += "\n\tExpiratoryHoldTime: "; if (HasExpiratoryHoldTime()) str += expiratory_hold_time.ToString(); else str += "Not Set";
      str += "\n\tExpiratoryReleaseTime: "; if (HasExpiratoryReleaseTime()) str += expiratory_release_time.ToString(); else str += "Not Set";
      str += "\n\tResidueTime: "; if (HasResidueTime()) str += residue_time.ToString(); else str += "Not Set";
      return str;
    }

  }
}

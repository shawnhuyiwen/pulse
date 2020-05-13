/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class PBMechanicalVentilator
  {
    public static string Connection_Name(SEMechanicalVentilator.Connection c)
    {
      return ((pulse.cdm.bind.MechanicalVentilatorData.Types.eConnection)(int)c).ToString();
    }
    public static string Control_Name(SEMechanicalVentilator.Control c)
    {
      return ((pulse.cdm.bind.MechanicalVentilatorData.Types.eControl)(int)c).ToString();
    }
    public static string DriverWaveform_Name(SEMechanicalVentilator.DriverWaveform w)
    {
      return ((pulse.cdm.bind.MechanicalVentilatorData.Types.eDriverWaveform)(int)w).ToString();
    }

    #region SEMechanicalVentilator
    public static void Load(pulse.cdm.bind.MechanicalVentilatorData src, SEMechanicalVentilator dst/*, SESubstanceManager subMgr*/)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.MechanicalVentilatorData src, SEMechanicalVentilator dst/*, SESubstanceManager subMgr*/)
    {
      dst.Clear();
      if(src.BreathProfile != null)
      {
        if (src.BreathProfile.Period != null)
          PBProperty.Load(src.BreathProfile.Period, dst.GetBreathPeriod());
        else if (src.BreathProfile.Rate != null)
          PBProperty.Load(src.BreathProfile.Rate, dst.GetRespiratoryRate());
        if (src.BreathProfile.InspiratoryExpiratoryRatio != null)
          PBProperty.Load(src.BreathProfile.InspiratoryExpiratoryRatio, dst.GetInspiratoryExpiratoryRatio());
      }
      else if(src.InspiratoryExpiratoryPeriod != null)
      {
        if (src.InspiratoryExpiratoryPeriod.ExpiratoryPeriod != null)
          PBProperty.Load(src.InspiratoryExpiratoryPeriod.ExpiratoryPeriod, dst.GetExpiratoryPeriod());
        if (src.InspiratoryExpiratoryPeriod.InspiratoryPeriod != null)
          PBProperty.Load(src.InspiratoryExpiratoryPeriod.InspiratoryPeriod, dst.GetInspiratoryPeriod());
      }
      dst.SetConnection((SEMechanicalVentilator.Connection)src.Connection);
      dst.SetControl((SEMechanicalVentilator.Control)src.Control);
      dst.SetDriverWaveform((SEMechanicalVentilator.DriverWaveform)src.DriverWaveform);

      if (src.PeakInspiratoryPressure != null)
        PBProperty.Load(src.PeakInspiratoryPressure, dst.GetPeakInspiratoryPressure());
      if (src.PositiveEndExpiredPressure != null)
        PBProperty.Load(src.PositiveEndExpiredPressure, dst.GetPositiveEndExpiredPressure());
    }

    public static pulse.cdm.bind.MechanicalVentilatorData Unload(SEMechanicalVentilator src)
    {
      pulse.cdm.bind.MechanicalVentilatorData dst = new pulse.cdm.bind.MechanicalVentilatorData();
      Unload(src, dst);
      return dst;
    }
    protected static void Unload(SEMechanicalVentilator src, pulse.cdm.bind.MechanicalVentilatorData dst)
    {
      if( (src.HasBreathPeriod() || src.HasRespiratoryRate()) && src.HasInspiratoryExpiratoryRatio())
      {
        dst.BreathProfile = new pulse.cdm.bind.MechanicalVentilatorData.Types.BreathProfileData();
        if (src.HasBreathPeriod())
          dst.BreathProfile.Period = PBProperty.Unload(src.GetBreathPeriod());
        else if (src.HasRespiratoryRate())
          dst.BreathProfile.Rate = PBProperty.Unload(src.GetRespiratoryRate());
        dst.BreathProfile.InspiratoryExpiratoryRatio = PBProperty.Unload(src.GetInspiratoryExpiratoryRatio());
      }
      else if(src.HasExpiratoryPeriod() && src.HasInspiratoryPeriod())
      {
        dst.InspiratoryExpiratoryPeriod = new pulse.cdm.bind.MechanicalVentilatorData.Types.InspiratoryExpiratoryPeriodData();
        dst.InspiratoryExpiratoryPeriod.ExpiratoryPeriod = PBProperty.Unload(src.GetExpiratoryPeriod());
        dst.InspiratoryExpiratoryPeriod.InspiratoryPeriod = PBProperty.Unload(src.GetInspiratoryPeriod());
      }

      dst.Connection = (pulse.cdm.bind.MechanicalVentilatorData.Types.eConnection)(int)src.GetConnection();
      dst.Control = (pulse.cdm.bind.MechanicalVentilatorData.Types.eControl)(int)src.GetControl();
      dst.DriverWaveform = (pulse.cdm.bind.MechanicalVentilatorData.Types.eDriverWaveform)(int)src.GetDriverWaveform();

      if (src.HasPeakInspiratoryPressure())
        dst.PeakInspiratoryPressure = PBProperty.Unload(src.GetPeakInspiratoryPressure());
      if (src.HasPositiveEndExpiredPressure())
        dst.PositiveEndExpiredPressure = PBProperty.Unload(src.GetPositiveEndExpiredPressure());
    }
    #endregion
  }
}

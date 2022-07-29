/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class PBPhysiology
  {
    #region SERespiratoryMechanics
    public static void Load(pulse.cdm.bind.RespiratoryMechanicsData src, SERespiratoryMechanics dst/*, SESubstanceManager subMgr*/)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.RespiratoryMechanicsData src, SERespiratoryMechanics dst/*, SESubstanceManager subMgr*/)
    {
      dst.Clear();
      dst.SetActive((eSwitch)src.Active);
      dst.SetDefaultType((eDefaultType)src.DefaultType);

      if (src.LeftComplianceCurve != null)
        PBProperty.Load(src.LeftComplianceCurve, dst.GetLeftComplianceCurve());
      if (src.RightComplianceCurve != null)
        PBProperty.Load(src.RightComplianceCurve, dst.GetRightComplianceCurve());

      if (src.LeftExpiratoryResistance != null)
        PBProperty.Load(src.LeftExpiratoryResistance, dst.GetLeftExpiratoryResistance());
      if (src.LeftInspiratoryResistance != null)
        PBProperty.Load(src.LeftInspiratoryResistance, dst.GetLeftInspiratoryResistance());
      if (src.RightExpiratoryResistance != null)
        PBProperty.Load(src.RightExpiratoryResistance, dst.GetRightExpiratoryResistance());
      if (src.RightInspiratoryResistance != null)
        PBProperty.Load(src.RightInspiratoryResistance, dst.GetRightInspiratoryResistance());
      if (src.UpperExpiratoryResistance != null)
        PBProperty.Load(src.UpperExpiratoryResistance, dst.GetUpperExpiratoryResistance());
      if (src.UpperInspiratoryResistance != null)
        PBProperty.Load(src.UpperInspiratoryResistance, dst.GetUpperInspiratoryResistance());
      if (src.InspiratoryPeakPressure != null)
        PBProperty.Load(src.InspiratoryPeakPressure, dst.GetInspiratoryPeakPressure());
      if (src.ExpiratoryPeakPressure != null)
        PBProperty.Load(src.ExpiratoryPeakPressure, dst.GetExpiratoryPeakPressure());
      if (src.InspiratoryRiseTime != null)
        PBProperty.Load(src.InspiratoryRiseTime, dst.GetInspiratoryRiseTime());
      if (src.InspiratoryHoldTime != null)
        PBProperty.Load(src.InspiratoryHoldTime, dst.GetInspiratoryHoldTime());
      if (src.InspiratoryReleaseTime != null)
        PBProperty.Load(src.InspiratoryReleaseTime, dst.GetInspiratoryReleaseTime());
      if (src.InspiratoryToExpiratoryPauseTime != null)
        PBProperty.Load(src.InspiratoryToExpiratoryPauseTime, dst.GetInspiratoryToExpiratoryPauseTime());
      if (src.ExpiratoryRiseTime != null)
        PBProperty.Load(src.ExpiratoryRiseTime, dst.GetExpiratoryRiseTime());
      if (src.ExpiratoryHoldTime != null)
        PBProperty.Load(src.ExpiratoryHoldTime, dst.GetExpiratoryHoldTime());
      if (src.ExpiratoryReleaseTime != null)
        PBProperty.Load(src.ExpiratoryReleaseTime, dst.GetExpiratoryReleaseTime());
      if (src.ResidueTime != null)
        PBProperty.Load(src.ResidueTime, dst.GetResidueTime());
    }

    public static pulse.cdm.bind.RespiratoryMechanicsData Unload(SERespiratoryMechanics src)
    {
      pulse.cdm.bind.RespiratoryMechanicsData dst = new pulse.cdm.bind.RespiratoryMechanicsData();
      Unload(src, dst);
      return dst;
    }
    protected static void Unload(SERespiratoryMechanics src, pulse.cdm.bind.RespiratoryMechanicsData dst)
    {
      dst.Active = (pulse.cdm.bind.eSwitch)(int)src.GetActive();
      dst.DefaultType = (pulse.cdm.bind.eDefaultType)(int)src.GetDefaultType();

      if (src.HasLeftComplianceCurve())
        dst.LeftComplianceCurve = PBProperty.Unload(src.GetLeftComplianceCurve());
      if (src.HasRightComplianceCurve())
        dst.RightComplianceCurve = PBProperty.Unload(src.GetRightComplianceCurve());

      if (src.HasLeftExpiratoryResistance())
        dst.LeftExpiratoryResistance = PBProperty.Unload(src.GetLeftExpiratoryResistance());
      if (src.HasLeftInspiratoryResistance())
        dst.LeftInspiratoryResistance = PBProperty.Unload(src.GetLeftInspiratoryResistance());
      if (src.HasRightExpiratoryResistance())
        dst.RightExpiratoryResistance = PBProperty.Unload(src.GetRightExpiratoryResistance());
      if (src.HasRightInspiratoryResistance())
        dst.RightInspiratoryResistance = PBProperty.Unload(src.GetRightInspiratoryResistance());
      if (src.HasUpperExpiratoryResistance())
        dst.UpperExpiratoryResistance = PBProperty.Unload(src.GetUpperExpiratoryResistance());
      if (src.HasUpperInspiratoryResistance())
        dst.UpperInspiratoryResistance = PBProperty.Unload(src.GetUpperInspiratoryResistance());
      if (src.HasInspiratoryPeakPressure())
        dst.InspiratoryPeakPressure = PBProperty.Unload(src.GetInspiratoryPeakPressure());
      if (src.HasExpiratoryPeakPressure())
        dst.ExpiratoryPeakPressure = PBProperty.Unload(src.GetExpiratoryPeakPressure());
      if (src.HasInspiratoryRiseTime())
        dst.InspiratoryRiseTime = PBProperty.Unload(src.GetInspiratoryRiseTime());
      if (src.HasInspiratoryHoldTime())
        dst.InspiratoryHoldTime = PBProperty.Unload(src.GetInspiratoryHoldTime());
      if (src.HasInspiratoryReleaseTime())
        dst.InspiratoryReleaseTime = PBProperty.Unload(src.GetInspiratoryReleaseTime());
      if (src.HasInspiratoryToExpiratoryPauseTime())
        dst.InspiratoryToExpiratoryPauseTime = PBProperty.Unload(src.GetInspiratoryToExpiratoryPauseTime());
      if (src.HasExpiratoryRiseTime())
        dst.ExpiratoryRiseTime = PBProperty.Unload(src.GetExpiratoryRiseTime());
      if (src.HasExpiratoryHoldTime())
        dst.ExpiratoryHoldTime = PBProperty.Unload(src.GetExpiratoryHoldTime());
      if (src.HasExpiratoryReleaseTime())
        dst.ExpiratoryReleaseTime = PBProperty.Unload(src.GetExpiratoryReleaseTime());
      if (src.HasResidueTime())
        dst.ResidueTime = PBProperty.Unload(src.GetResidueTime());
    }
    #endregion
  }
}

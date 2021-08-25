/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;
using Google.Protobuf;

namespace Pulse.CDM
{
  public class PBPatient
  {
    public static void Load(pulse.cdm.bind.PatientData src, SEPatient dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.PatientData src, SEPatient dst)
    {
      dst.Clear();
      dst.SetName(src.Name);
      dst.SetSex((ePatient_Sex)(int)src.Sex);
      if (src.Age != null)
        PBProperty.Load(src.Age, dst.GetAge());
      if (src.Weight != null)
        PBProperty.Load(src.Weight, dst.GetWeight());
      if (src.Height != null)
        PBProperty.Load(src.Height, dst.GetHeight());
      if (src.BodyDensity != null)
        PBProperty.Load(src.BodyDensity, dst.GetBodyDensity());
      if (src.BodyFatFraction != null)
        PBProperty.Load(src.BodyFatFraction, dst.GetBodyFatFraction());
      if (src.IdealBodyWeight != null)
        PBProperty.Load(src.IdealBodyWeight, dst.GetIdealBodyWeight());
      if (src.LeanBodyMass != null)
        PBProperty.Load(src.LeanBodyMass, dst.GetLeanBodyMass());

      if (src.AlveoliSurfaceArea != null)
        PBProperty.Load(src.AlveoliSurfaceArea, dst.GetAlveoliSurfaceArea());
      if (src.RightLungRatio != null)
        PBProperty.Load(src.RightLungRatio, dst.GetRightLungRatio());
      if (src.SkinSurfaceArea != null)
        PBProperty.Load(src.SkinSurfaceArea, dst.GetSkinSurfaceArea());

      if (src.BasalMetabolicRate != null)
        PBProperty.Load(src.BasalMetabolicRate, dst.GetBasalMetabolicRate());
      if (src.BloodVolumeBaseline != null)
        PBProperty.Load(src.BloodVolumeBaseline, dst.GetBloodVolumeBaseline());
      if (src.DiastolicArterialPressureBaseline != null)
        PBProperty.Load(src.DiastolicArterialPressureBaseline, dst.GetDiastolicArterialPressureBaseline());
      if (src.HeartRateBaseline != null)
        PBProperty.Load(src.HeartRateBaseline, dst.GetHeartRateBaseline());
      if (src.MeanArterialPressureBaseline != null)
        PBProperty.Load(src.MeanArterialPressureBaseline, dst.GetMeanArterialPressureBaseline());
      if (src.RespirationRateBaseline != null)
        PBProperty.Load(src.RespirationRateBaseline, dst.GetRespirationRateBaseline());
      if (src.SystolicArterialPressureBaseline != null)
        PBProperty.Load(src.SystolicArterialPressureBaseline, dst.GetSystolicArterialPressureBaseline());
      if (src.TidalVolumeBaseline != null)
        PBProperty.Load(src.TidalVolumeBaseline, dst.GetTidalVolumeBaseline());

      if (src.HeartRateMaximum != null)
        PBProperty.Load(src.HeartRateMaximum, dst.GetHeartRateMaximum());
      if (src.HeartRateMinimum != null)
        PBProperty.Load(src.HeartRateMinimum, dst.GetHeartRateMinimum());
      if (src.ExpiratoryReserveVolume != null)
        PBProperty.Load(src.ExpiratoryReserveVolume, dst.GetExpiratoryReserveVolume());
      if (src.FunctionalResidualCapacity != null)
        PBProperty.Load(src.FunctionalResidualCapacity, dst.GetFunctionalResidualCapacity());
      if (src.InspiratoryCapacity != null)
        PBProperty.Load(src.InspiratoryCapacity, dst.GetInspiratoryCapacity());
      if (src.InspiratoryReserveVolume != null)
        PBProperty.Load(src.InspiratoryReserveVolume, dst.GetInspiratoryReserveVolume());
      if (src.ResidualVolume != null)
        PBProperty.Load(src.ResidualVolume, dst.GetResidualVolume());
      if (src.TotalLungCapacity != null)
        PBProperty.Load(src.TotalLungCapacity, dst.GetTotalLungCapacity());
      if (src.VitalCapacity != null)
        PBProperty.Load(src.VitalCapacity, dst.GetVitalCapacity());
    }
    public static bool SerializeFromString(string src, SEPatient dst)
    {
      try
      {
        pulse.cdm.bind.PatientData data = JsonParser.Default.Parse<pulse.cdm.bind.PatientData>(src);
        PBPatient.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static pulse.cdm.bind.PatientData Unload(SEPatient src)
    {
      pulse.cdm.bind.PatientData dst = new pulse.cdm.bind.PatientData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEPatient src, pulse.cdm.bind.PatientData dst)
    {
      if (src.HasName())
        dst.Name = src.GetName();
      dst.Sex = (pulse.cdm.bind.PatientData.Types.eSex)(int)src.GetSex();
      if (src.HasAge())
        dst.Age = PBProperty.Unload(src.GetAge());
      if (src.HasWeight())
        dst.Weight = PBProperty.Unload(src.GetWeight());
      if (src.HasHeight())
        dst.Height = PBProperty.Unload(src.GetHeight());
      if (src.HasBodyDensity())
        dst.BodyDensity = PBProperty.Unload(src.GetBodyDensity());
      if (src.HasBodyFatFraction())
        dst.BodyFatFraction = PBProperty.Unload(src.GetBodyFatFraction());
      if (src.HasIdealBodyWeight())
        dst.IdealBodyWeight = PBProperty.Unload(src.GetIdealBodyWeight());
      if (src.HasLeanBodyMass())
        dst.LeanBodyMass = PBProperty.Unload(src.GetLeanBodyMass());

      if (src.HasAlveoliSurfaceArea())
        dst.AlveoliSurfaceArea = PBProperty.Unload(src.GetAlveoliSurfaceArea());
      if (src.HasRightLungRatio())
        dst.RightLungRatio = PBProperty.Unload(src.GetRightLungRatio());
      if (src.HasSkinSurfaceArea())
        dst.SkinSurfaceArea = PBProperty.Unload(src.GetSkinSurfaceArea());

      if (src.HasBasalMetabolicRate())
        dst.BasalMetabolicRate = PBProperty.Unload(src.GetBasalMetabolicRate());
      if (src.HasBloodVolumeBaseline())
        dst.BloodVolumeBaseline = PBProperty.Unload(src.GetBloodVolumeBaseline());
      if (src.HasDiastolicArterialPressureBaseline())
        dst.DiastolicArterialPressureBaseline = PBProperty.Unload(src.GetDiastolicArterialPressureBaseline());
      if (src.HasHeartRateBaseline())
        dst.HeartRateBaseline = PBProperty.Unload(src.GetHeartRateBaseline());
      if (src.HasMeanArterialPressureBaseline())
        dst.MeanArterialPressureBaseline = PBProperty.Unload(src.GetMeanArterialPressureBaseline());
      if (src.HasRespirationRateBaseline())
        dst.RespirationRateBaseline = PBProperty.Unload(src.GetRespirationRateBaseline());
      if (src.HasSystolicArterialPressureBaseline())
        dst.SystolicArterialPressureBaseline = PBProperty.Unload(src.GetSystolicArterialPressureBaseline());
      if (src.HasTidalVolumeBaseline())
        dst.TidalVolumeBaseline = PBProperty.Unload(src.GetTidalVolumeBaseline());

      if (src.HasHeartRateMaximum())
        dst.HeartRateMaximum = PBProperty.Unload(src.GetHeartRateMaximum());
      if (src.HasHeartRateMinimum())
        dst.HeartRateMinimum = PBProperty.Unload(src.GetHeartRateMinimum());
      if (src.HasExpiratoryReserveVolume())
        dst.ExpiratoryReserveVolume = PBProperty.Unload(src.GetExpiratoryReserveVolume());
      if (src.HasFunctionalResidualCapacity())
        dst.FunctionalResidualCapacity = PBProperty.Unload(src.GetFunctionalResidualCapacity());
      if (src.HasInspiratoryCapacity())
        dst.InspiratoryCapacity = PBProperty.Unload(src.GetInspiratoryCapacity());
      if (src.HasInspiratoryReserveVolume())
        dst.InspiratoryReserveVolume = PBProperty.Unload(src.GetInspiratoryReserveVolume());
      if (src.HasResidualVolume())
        dst.ResidualVolume = PBProperty.Unload(src.GetResidualVolume());
      if (src.HasTotalLungCapacity())
        dst.TotalLungCapacity = PBProperty.Unload(src.GetTotalLungCapacity());
      if (src.HasVitalCapacity())
        dst.VitalCapacity = PBProperty.Unload(src.GetVitalCapacity());

    }
    public static string SerializeToString(SEPatient src)
    {
      var pb = PBPatient.Unload(src);
      return pb.ToString();
    }
  }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  static class eECMO
  {
    public static string CannulationLocation_Name(this eECMO_CannulationLocation w)
    {
      return ((pulse.cdm.bind.eECMOCannulationLocation)(int)w).ToString();
    }
  }
  public class PBECMO
  {
    #region SEECMO
    public static void Load(pulse.cdm.bind.ECMOSettingsData src, SEECMOSettings dst/*, SESubstanceManager subMgr*/)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ECMOSettingsData src, SEECMOSettings dst/*, SESubstanceManager subMgr*/)
    {
      dst.Clear();
      dst.SetInflowLocation((eECMO_CannulationLocation)src.InflowLocation);
      dst.SetOutflowLocation((eECMO_CannulationLocation)src.OutflowLocation);
      if (src.OxygenatorVolume != null)
        PBProperty.Load(src.OxygenatorVolume, dst.GetOxygenatorVolume());
      if (src.TransfusionFlow != null)
        PBProperty.Load(src.TransfusionFlow, dst.GetTransfusionFlow());
      dst.SetSubstanceCompound(src.SubstanceCompound);

      foreach (pulse.cdm.bind.SubstanceConcentrationData sc in src.SubstanceConcentrations)
      {
        PBProperty.Load(sc.Concentration, dst.GetSubstanceConcentration(sc.Name).GetConcentration());
      }
    }

    public static pulse.cdm.bind.ECMOSettingsData Unload(SEECMOSettings src)
    {
      pulse.cdm.bind.ECMOSettingsData dst = new pulse.cdm.bind.ECMOSettingsData();
      Unload(src, dst);
      return dst;
    }
    protected static void Unload(SEECMOSettings src, pulse.cdm.bind.ECMOSettingsData dst)
    {
      dst.InflowLocation = (pulse.cdm.bind.eECMOCannulationLocation)(int)src.GetInflowLocation();
      dst.OutflowLocation = (pulse.cdm.bind.eECMOCannulationLocation)(int)src.GetOutflowLocation();
      if (src.HasOxygenatorVolume())
        dst.OxygenatorVolume = PBProperty.Unload(src.GetOxygenatorVolume());
      if (src.HasTransfusionFlow())
        dst.TransfusionFlow = PBProperty.Unload(src.GetTransfusionFlow());
      if (src.HasSubstanceCompound())
        dst.SubstanceCompound = src.GetSubstanceCompound();

      foreach (SESubstanceConcentration c in src.GetSubstanceConcentrations())
      {
        pulse.cdm.bind.SubstanceConcentrationData sc = new pulse.cdm.bind.SubstanceConcentrationData();
        sc.Name = c.GetSubstance();
        sc.Concentration = PBProperty.Unload(c.GetConcentration());
        dst.SubstanceConcentrations.Add(sc);
      }
    }
    #endregion
  }
}

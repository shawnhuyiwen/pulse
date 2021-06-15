/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class PBBagValveMask
  {
    public static string Connection_Name(SEBagValveMask.Connection c)
    {
      return ((pulse.cdm.bind.BagValveMaskData.Types.eConnection)(int)c).ToString();
    }

    #region SEBagValveMask
    public static void Load(pulse.cdm.bind.BagValveMaskData src, SEBagValveMask dst/*, SESubstanceManager subMgr*/)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.BagValveMaskData src, SEBagValveMask dst/*, SESubstanceManager subMgr*/)
    {
      dst.Clear();
      dst.SetConnection((SEBagValveMask.Connection)src.Connection);
      if (src.BagResistance != null)
        PBProperty.Load(src.BagResistance, dst.GetBagResistance());
      if (src.FilterResistance != null)
        PBProperty.Load(src.FilterResistance, dst.GetFilterResistance());
      else if (src.FilterVolume != null)
        PBProperty.Load(src.FilterVolume, dst.GetFilterVolume());
      else if (src.ValvePositiveEndExpiredPressure != null)
        PBProperty.Load(src.ValvePositiveEndExpiredPressure, dst.GetValvePositiveEndExpiredPressure());
      else if (src.ValveResistance != null)
        PBProperty.Load(src.ValveResistance, dst.GetValveResistance());

      foreach (pulse.cdm.bind.SubstanceFractionData sf in src.FractionInspiredGas)
      {
        PBProperty.Load(sf.Amount, dst.GetFractionInspiredGas(sf.Name).GetFractionAmount());
      }

      foreach (pulse.cdm.bind.SubstanceConcentrationData sc in src.ConcentrationInspiredAerosol)
      {
        PBProperty.Load(sc.Concentration, dst.GetConcentrationInspiredAerosol(sc.Name).GetConcentration());
      }
    }

    public static pulse.cdm.bind.BagValveMaskData Unload(SEBagValveMask src)
    {
      pulse.cdm.bind.BagValveMaskData dst = new pulse.cdm.bind.BagValveMaskData();
      Unload(src, dst);
      return dst;
    }
    protected static void Unload(SEBagValveMask src, pulse.cdm.bind.BagValveMaskData dst)
    {
      dst.Connection = (pulse.cdm.bind.BagValveMaskData.Types.eConnection)(int)src.GetConnection();
      if (src.HasBagResistance())
        dst.BagResistance = PBProperty.Unload(src.GetBagResistance());
      if (src.HasFilterResistance())
        dst.FilterResistance = PBProperty.Unload(src.GetFilterResistance());
      else if (src.HasFilterVolume())
        dst.FilterVolume = PBProperty.Unload(src.GetFilterVolume());
      else if (src.HasValvePositiveEndExpiredPressure())
        dst.ValvePositiveEndExpiredPressure = PBProperty.Unload(src.GetValvePositiveEndExpiredPressure());
      else if (src.HasValveResistance())
        dst.ValveResistance = PBProperty.Unload(src.GetValveResistance());

      foreach (SESubstanceFraction aGas in src.GetFractionInspiredGases())
      {
        pulse.cdm.bind.SubstanceFractionData sf = new pulse.cdm.bind.SubstanceFractionData();
        sf.Name = aGas.GetSubstance();
        sf.Amount = PBProperty.Unload(aGas.GetFractionAmount());
        dst.FractionInspiredGas.Add(sf);
      }

      foreach (SESubstanceConcentration aAerosol in src.GetConcentrationInspiredAerosols())
      {
        pulse.cdm.bind.SubstanceConcentrationData sc = new pulse.cdm.bind.SubstanceConcentrationData();
        sc.Name = aAerosol.GetSubstance();
        sc.Concentration = PBProperty.Unload(aAerosol.GetConcentration());
        dst.ConcentrationInspiredAerosol.Add(sc);
      }
    }
    #endregion
  }
}

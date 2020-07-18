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
      dst.SetConnection((SEMechanicalVentilator.Connection)src.Connection);
      if (src.EndotrachealTubeResistance != null)
        PBProperty.Load(src.EndotrachealTubeResistance, dst.GetEndotrachealTubeResistance());
      if (src.PositiveEndExpiredPressure != null)
        PBProperty.Load(src.PositiveEndExpiredPressure, dst.GetPositiveEndExpiredPressure());
      if (src.FunctionalResidualCapacity != null)
        PBProperty.Load(src.FunctionalResidualCapacity, dst.GetFunctionalResidualCapacity());
      if (src.ExpirationCycleFlow != null)
        PBProperty.Load(src.ExpirationCycleFlow, dst.GetExpirationCycleFlow());
      if (src.ExpirationCyclePressure != null)
        PBProperty.Load(src.ExpirationCyclePressure, dst.GetExpirationCyclePressure());
      if (src.ExpirationCycleTime != null)
        PBProperty.Load(src.ExpirationCycleTime, dst.GetExpirationCycleTime());
      if (src.ExpirationTubeResistance != null)
        PBProperty.Load(src.ExpirationTubeResistance, dst.GetExpirationTubeResistance());
      if (src.ExpirationValveResistance != null)
        PBProperty.Load(src.ExpirationValveResistance, dst.GetExpirationValveResistance());
      dst.SetExpirationWaveform((SEMechanicalVentilator.DriverWaveform)src.ExpirationWaveform);


      if (src.InspirationLimitFlow != null)
        PBProperty.Load(src.InspirationLimitFlow, dst.GetInspirationLimitFlow());
      if (src.InspirationLimitPressure != null)
        PBProperty.Load(src.InspirationLimitPressure, dst.GetInspirationLimitPressure());
      if (src.InspirationLimitPressure != null)
        PBProperty.Load(src.InspirationLimitPressure, dst.GetInspirationLimitPressure());
      if (src.InspirationLimitVolume != null)
        PBProperty.Load(src.InspirationLimitVolume, dst.GetInspirationLimitVolume());
      if (src.InspirationLimitVolume != null)
        PBProperty.Load(src.InspirationLimitVolume, dst.GetInspirationLimitVolume());
      if (src.InspirationPauseTime != null)
        PBProperty.Load(src.InspirationPauseTime, dst.GetInspirationPauseTime());
      if (src.PeakInspiratoryPressure != null)
        PBProperty.Load(src.PeakInspiratoryPressure, dst.GetPeakInspiratoryPressure());
      if (src.PeakInspiratoryPressure != null)
        PBProperty.Load(src.PeakInspiratoryPressure, dst.GetPeakInspiratoryPressure());
      if (src.PeakInspiratoryPressure != null)
        PBProperty.Load(src.PeakInspiratoryPressure, dst.GetPeakInspiratoryPressure());
      if (src.InspirationTriggerPressure != null)
        PBProperty.Load(src.InspirationTriggerPressure, dst.GetInspirationTriggerPressure());
      if (src.InspirationTriggerTime != null)
        PBProperty.Load(src.InspirationTriggerTime, dst.GetInspirationTriggerTime());
      if (src.InspirationTubeResistance != null)
        PBProperty.Load(src.InspirationTubeResistance, dst.GetInspirationTubeResistance());
      if (src.InspirationValveResistance != null)
        PBProperty.Load(src.InspirationValveResistance, dst.GetInspirationValveResistance());
      dst.SetInspirationWaveform((SEMechanicalVentilator.DriverWaveform)src.InspirationWaveform);

      foreach (pulse.cdm.bind.SubstanceFractionData sf in src.FractionInspiredGas)
      {
        PBProperty.Load(sf.Amount, dst.GetFractionInspiredGas(sf.Name).GetFractionAmount());
      }

      foreach (pulse.cdm.bind.SubstanceConcentrationData sc in src.ConcentrationInspiredAerosol)
      {
        PBProperty.Load(sc.Concentration, dst.GetConcentrationInspiredAerosol(sc.Name).GetConcentration());
      }
    }

    public static pulse.cdm.bind.MechanicalVentilatorData Unload(SEMechanicalVentilator src)
    {
      pulse.cdm.bind.MechanicalVentilatorData dst = new pulse.cdm.bind.MechanicalVentilatorData();
      Unload(src, dst);
      return dst;
    }
    protected static void Unload(SEMechanicalVentilator src, pulse.cdm.bind.MechanicalVentilatorData dst)
    {
      dst.Connection = (pulse.cdm.bind.MechanicalVentilatorData.Types.eConnection)(int)src.GetConnection();
      if (src.HasEndotrachealTubeResistance())
        dst.EndotrachealTubeResistance = PBProperty.Unload(src.GetEndotrachealTubeResistance());
      if (src.HasPositiveEndExpiredPressure())
        dst.PositiveEndExpiredPressure = PBProperty.Unload(src.GetPositiveEndExpiredPressure());
      if (src.HasFunctionalResidualCapacity())
        dst.FunctionalResidualCapacity = PBProperty.Unload(src.GetFunctionalResidualCapacity());
      if (src.HasExpirationCycleFlow())
        dst.ExpirationCycleFlow = PBProperty.Unload(src.GetExpirationCycleFlow());
      if (src.HasExpirationCyclePressure())
        dst.ExpirationCyclePressure = PBProperty.Unload(src.GetExpirationCyclePressure());
      if (src.HasExpirationCycleTime())
        dst.ExpirationCycleTime = PBProperty.Unload(src.GetExpirationCycleTime());
      if (src.HasExpirationTubeResistance())
        dst.ExpirationTubeResistance = PBProperty.Unload(src.GetExpirationTubeResistance());
      if (src.HasExpirationValveResistance())
        dst.ExpirationValveResistance = PBProperty.Unload(src.GetExpirationValveResistance());
      dst.ExpirationWaveform = (pulse.cdm.bind.MechanicalVentilatorData.Types.eDriverWaveform)(int)src.GetExpirationWaveform();

      if (src.HasInspirationLimitFlow())
        dst.InspirationLimitFlow = PBProperty.Unload(src.GetInspirationLimitFlow());
      if (src.HasInspirationLimitPressure())
        dst.InspirationLimitPressure = PBProperty.Unload(src.GetInspirationLimitPressure());
      if (src.HasInspirationLimitVolume())
        dst.InspirationLimitVolume = PBProperty.Unload(src.GetInspirationLimitVolume());
      if (src.HasInspirationPauseTime())
        dst.InspirationPauseTime = PBProperty.Unload(src.GetInspirationPauseTime());
      if (src.HasPeakInspiratoryPressure())
        dst.PeakInspiratoryPressure = PBProperty.Unload(src.GetPeakInspiratoryPressure());
      if (src.HasEndTidalCarbonDioxidePressure())
        dst.EndTidalCarbonDioxidePressure = PBProperty.Unload(src.GetEndTidalCarbonDioxidePressure());
      if (src.HasInspirationTriggerFlow())
        dst.InspirationTriggerFlow = PBProperty.Unload(src.GetInspirationTriggerFlow());
      if (src.HasInspirationTriggerPressure())
        dst.InspirationTriggerPressure = PBProperty.Unload(src.GetInspirationTriggerPressure());
      if (src.HasInspirationTriggerTime())
        dst.InspirationTriggerTime = PBProperty.Unload(src.GetInspirationTriggerTime());
      if (src.HasInspirationTubeResistance())
        dst.InspirationTubeResistance = PBProperty.Unload(src.GetInspirationTubeResistance());
      if (src.HasInspirationValveResistance())
        dst.InspirationValveResistance = PBProperty.Unload(src.GetInspirationValveResistance());
      dst.InspirationWaveform = (pulse.cdm.bind.MechanicalVentilatorData.Types.eDriverWaveform)(int)src.GetInspirationWaveform();

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

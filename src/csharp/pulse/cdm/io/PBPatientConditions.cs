/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class PBPatientCondition
  {
    #region Any Patient Condition
    /** Create a new Condition based on the binding object, load that data into the new Condition, and return said Condition */
    public static SEPatientCondition Load(pulse.cdm.bind.AnyPatientConditionData any/*, SESubstanceManager subMgr*/)
    {
      if (any.AcuteRespiratoryDistressSyndrome != null)
      {
        SEAcuteRespiratoryDistressSyndrome ards = new SEAcuteRespiratoryDistressSyndrome();
        Serialize(any.AcuteRespiratoryDistressSyndrome, ards);
        return ards;
      }
      if (any.ChronicAnemia != null)
      {
        SEChronicAnemia a = new SEChronicAnemia();
        Serialize(any.ChronicAnemia, a);
        return a;
      }
      if (any.ChronicObstructivePulmonaryDisease != null)
      {
        SEChronicObstructivePulmonaryDisease copd = new SEChronicObstructivePulmonaryDisease();
        Serialize(any.ChronicObstructivePulmonaryDisease, copd);
        return copd;
      }
      if (any.ChronicPericardialEffusion != null)
      {
        SEChronicPericardialEffusion pe = new SEChronicPericardialEffusion();
        Serialize(any.ChronicPericardialEffusion, pe);
        return pe;
      }
      if (any.ChronicRenalStenosis != null)
      {
        SEChronicRenalStenosis rs = new SEChronicRenalStenosis();
        Serialize(any.ChronicRenalStenosis, rs);
        return rs;
      }
      if (any.ChronicVentricularSystolicDysfunction != null)
      {
        SEChronicVentricularSystolicDysfunction cvsd = new SEChronicVentricularSystolicDysfunction();
        Serialize(any.ChronicVentricularSystolicDysfunction, cvsd);
        return cvsd;
      }
      if (any.ImpairedAlveolarExchange != null)
      {
        SEImpairedAlveolarExchange iae = new SEImpairedAlveolarExchange();
        Serialize(any.ImpairedAlveolarExchange, iae);
        return iae;
      }
      if (any.LobarPneumonia != null)
      {
        SELobarPneumonia lp = new SELobarPneumonia();
        Serialize(any.LobarPneumonia, lp);
        return lp;
      }
      if (any.PulmonaryFibrosis != null)
      {
        SEPulmonaryFibrosis pf = new SEPulmonaryFibrosis();
        Serialize(any.PulmonaryFibrosis, pf);
        return pf;
      }
      if (any.PulmonaryShunt != null)
      {
        SEPulmonaryShunt ps = new SEPulmonaryShunt();
        Serialize(any.PulmonaryShunt, ps);
        return ps;
      }
      if (any.Sepsis != null)
      {
        SESepsis s = new SESepsis();
        Serialize(any.Sepsis, s);
        return s;
      }
      return null;
    }
    /** Create a new bind object, unload the Condition, put that in the bind object, and return said bind object */
    public static pulse.cdm.bind.AnyPatientConditionData Unload(SEPatientCondition Condition)
    {
      pulse.cdm.bind.AnyPatientConditionData any = new pulse.cdm.bind.AnyPatientConditionData();

      if (Condition.GetType().IsAssignableFrom(typeof(SEAcuteRespiratoryDistressSyndrome)))
      {
        any.AcuteRespiratoryDistressSyndrome = Unload((SEAcuteRespiratoryDistressSyndrome)Condition);
        return any;
      }
      if (Condition.GetType().IsAssignableFrom(typeof(SEChronicAnemia)))
      {
        any.ChronicAnemia = Unload((SEChronicAnemia)Condition);
        return any;
      }
      if (Condition.GetType().IsAssignableFrom(typeof(SEChronicObstructivePulmonaryDisease)))
      {
        any.ChronicObstructivePulmonaryDisease = Unload((SEChronicObstructivePulmonaryDisease)Condition);
        return any;
      }
      if (Condition.GetType().IsAssignableFrom(typeof(SEChronicPericardialEffusion)))
      {
        any.ChronicPericardialEffusion = Unload((SEChronicPericardialEffusion)Condition);
        return any;
      }
      if (Condition.GetType().IsAssignableFrom(typeof(SEChronicRenalStenosis)))
      {
        any.ChronicRenalStenosis = Unload((SEChronicRenalStenosis)Condition);
        return any;
      }
      if (Condition.GetType().IsAssignableFrom(typeof(SEChronicVentricularSystolicDysfunction)))
      {
        any.ChronicVentricularSystolicDysfunction = Unload((SEChronicVentricularSystolicDysfunction)Condition);
        return any;
      }
      if (Condition.GetType().IsAssignableFrom(typeof(SEImpairedAlveolarExchange)))
      {
        any.ImpairedAlveolarExchange = Unload((SEImpairedAlveolarExchange)Condition);
        return any;
      }
      if (Condition.GetType().IsAssignableFrom(typeof(SELobarPneumonia)))
      {
        any.LobarPneumonia = Unload((SELobarPneumonia)Condition);
        return any;
      }
      if (Condition.GetType().IsAssignableFrom(typeof(SEPulmonaryFibrosis)))
      {
        any.PulmonaryFibrosis = Unload((SEPulmonaryFibrosis)Condition);
        return any;
      }
      if (Condition.GetType().IsAssignableFrom(typeof(SEPulmonaryShunt)))
      {
        any.PulmonaryShunt = Unload((SEPulmonaryShunt)Condition);
        return any;
      }
      if (Condition.GetType().IsAssignableFrom(typeof(SESepsis)))
      {
        any.Sepsis = Unload((SESepsis)Condition);
        return any;
      }

      return any;
    }
    #endregion

    #region SEPatientCondition
    public static void Serialize(pulse.cdm.bind.PatientConditionData src, SEPatientCondition dst)
    {
      PBCondition.Serialize(src.Condition, dst);
    }
    static void Serialize(SEPatientCondition src, pulse.cdm.bind.PatientConditionData dst)
    {
      dst.Condition = new pulse.cdm.bind.ConditionData();
      PBCondition.Serialize(src, dst.Condition);
    }
    #endregion

    #region SEAcuteRespiratoryDistressSyndrome
    public static void Load(pulse.cdm.bind.AcuteRespiratoryDistressSyndromeData src, SEAcuteRespiratoryDistressSyndrome dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.AcuteRespiratoryDistressSyndromeData src, SEAcuteRespiratoryDistressSyndrome dst)
    {
      Serialize(src.PatientCondition, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
      if (src.LeftLungAffected != null)
        PBProperty.Load(src.LeftLungAffected, dst.GetLeftLungAffected());
      if (src.RightLungAffected != null)
        PBProperty.Load(src.RightLungAffected, dst.GetRightLungAffected());
    }
    public static pulse.cdm.bind.AcuteRespiratoryDistressSyndromeData Unload(SEAcuteRespiratoryDistressSyndrome src)
    {
      pulse.cdm.bind.AcuteRespiratoryDistressSyndromeData dst = new pulse.cdm.bind.AcuteRespiratoryDistressSyndromeData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEAcuteRespiratoryDistressSyndrome src, pulse.cdm.bind.AcuteRespiratoryDistressSyndromeData dst)
    {
      dst.PatientCondition = new pulse.cdm.bind.PatientConditionData();
      Serialize(src, dst.PatientCondition);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
      if (src.HasLeftLungAffected())
        dst.LeftLungAffected = PBProperty.Unload(src.GetLeftLungAffected());
      if (src.HasRightLungAffected())
        dst.RightLungAffected = PBProperty.Unload(src.GetRightLungAffected());
    }
    #endregion

    #region SEChronicAnemia
    public static void Load(pulse.cdm.bind.ChronicAnemiaData src, SEChronicAnemia dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ChronicAnemiaData src, SEChronicAnemia dst)
    {
      Serialize(src.PatientCondition, dst);
      if (src.ReductionFactor != null)
        PBProperty.Load(src.ReductionFactor, dst.GetReductionFactor());
    }
    public static pulse.cdm.bind.ChronicAnemiaData Unload(SEChronicAnemia src)
    {
      pulse.cdm.bind.ChronicAnemiaData dst = new pulse.cdm.bind.ChronicAnemiaData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEChronicAnemia src, pulse.cdm.bind.ChronicAnemiaData dst)
    {
      dst.PatientCondition = new pulse.cdm.bind.PatientConditionData();
      Serialize(src, dst.PatientCondition);
      if (src.HasReductionFactor())
        dst.ReductionFactor = PBProperty.Unload(src.GetReductionFactor());
    }
    #endregion

    #region SEChronicObstructivePulmonaryDisease
    public static void Load(pulse.cdm.bind.ChronicObstructivePulmonaryDiseaseData src, SEChronicObstructivePulmonaryDisease dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ChronicObstructivePulmonaryDiseaseData src, SEChronicObstructivePulmonaryDisease dst)
    {
      Serialize(src.PatientCondition, dst);
      if (src.BronchitisSeverity != null)
        PBProperty.Load(src.BronchitisSeverity, dst.GetBronchitisSeverity());
      if (src.EmphysemaSeverity != null)
        PBProperty.Load(src.EmphysemaSeverity, dst.GetEmphysemaSeverity());
    }
    public static pulse.cdm.bind.ChronicObstructivePulmonaryDiseaseData Unload(SEChronicObstructivePulmonaryDisease src)
    {
      pulse.cdm.bind.ChronicObstructivePulmonaryDiseaseData dst = new pulse.cdm.bind.ChronicObstructivePulmonaryDiseaseData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEChronicObstructivePulmonaryDisease src, pulse.cdm.bind.ChronicObstructivePulmonaryDiseaseData dst)
    {
      dst.PatientCondition = new pulse.cdm.bind.PatientConditionData();
      Serialize(src, dst.PatientCondition);
      if (src.HasBronchitisSeverity())
        dst.BronchitisSeverity = PBProperty.Unload(src.GetBronchitisSeverity());
      if (src.HasEmphysemaSeverity())
        dst.EmphysemaSeverity = PBProperty.Unload(src.GetEmphysemaSeverity());
    }
    #endregion

    #region SEChronicPericardialEffusion
    public static void Load(pulse.cdm.bind.ChronicPericardialEffusionData src, SEChronicPericardialEffusion dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ChronicPericardialEffusionData src, SEChronicPericardialEffusion dst)
    {
      Serialize(src.PatientCondition, dst);
      if (src.AccumulatedVolume != null)
        PBProperty.Load(src.AccumulatedVolume, dst.GetAccumulatedVolume());
    }
    public static pulse.cdm.bind.ChronicPericardialEffusionData Unload(SEChronicPericardialEffusion src)
    {
      pulse.cdm.bind.ChronicPericardialEffusionData dst = new pulse.cdm.bind.ChronicPericardialEffusionData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEChronicPericardialEffusion src, pulse.cdm.bind.ChronicPericardialEffusionData dst)
    {
      dst.PatientCondition = new pulse.cdm.bind.PatientConditionData();
      Serialize(src, dst.PatientCondition);
      if (src.HasAccumulatedVolume())
        dst.AccumulatedVolume = PBProperty.Unload(src.GetAccumulatedVolume());
    }
    #endregion

    #region SEChronicRenalStenosis
    public static void Load(pulse.cdm.bind.ChronicRenalStenosisData src, SEChronicRenalStenosis dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ChronicRenalStenosisData src, SEChronicRenalStenosis dst)
    {
      Serialize(src.PatientCondition, dst);
      if (src.LeftKidneySeverity != null)
        PBProperty.Load(src.LeftKidneySeverity, dst.GetLeftKidneySeverity());
      if (src.RightKidneySeverity != null)
        PBProperty.Load(src.RightKidneySeverity, dst.GetRightKidneySeverity());
    }
    public static pulse.cdm.bind.ChronicRenalStenosisData Unload(SEChronicRenalStenosis src)
    {
      pulse.cdm.bind.ChronicRenalStenosisData dst = new pulse.cdm.bind.ChronicRenalStenosisData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEChronicRenalStenosis src, pulse.cdm.bind.ChronicRenalStenosisData dst)
    {
      dst.PatientCondition = new pulse.cdm.bind.PatientConditionData();
      Serialize(src, dst.PatientCondition);
      if (src.HasLeftKidneySeverity())
        dst.LeftKidneySeverity = PBProperty.Unload(src.GetLeftKidneySeverity());
      if (src.HasRightKidneySeverity())
        dst.RightKidneySeverity = PBProperty.Unload(src.GetRightKidneySeverity());
    }
    #endregion

    #region SEChronicVentricularSystolicDysfunction
    public static void Load(pulse.cdm.bind.ChronicVentricularSystolicDysfunctionData src, SEChronicVentricularSystolicDysfunction dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ChronicVentricularSystolicDysfunctionData src, SEChronicVentricularSystolicDysfunction dst)
    {
      Serialize(src.PatientCondition, dst);
    }
    public static pulse.cdm.bind.ChronicVentricularSystolicDysfunctionData Unload(SEChronicVentricularSystolicDysfunction src)
    {
      pulse.cdm.bind.ChronicVentricularSystolicDysfunctionData dst = new pulse.cdm.bind.ChronicVentricularSystolicDysfunctionData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEChronicVentricularSystolicDysfunction src, pulse.cdm.bind.ChronicVentricularSystolicDysfunctionData dst)
    {
      dst.PatientCondition = new pulse.cdm.bind.PatientConditionData();
      Serialize(src, dst.PatientCondition);
    }
    #endregion

    #region SEImpairedAlveolarExchange
    public static void Load(pulse.cdm.bind.ImpairedAlveolarExchangeData src, SEImpairedAlveolarExchange dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ImpairedAlveolarExchangeData src, SEImpairedAlveolarExchange dst)
    {
      Serialize(src.PatientCondition, dst);
      if (src.ImpairedSurfaceArea != null)
        PBProperty.Load(src.ImpairedSurfaceArea, dst.GetImpairedSurfaceArea());
      if (src.ImpairedFraction != null)
        PBProperty.Load(src.ImpairedFraction, dst.GetImpairedFraction());
    }
    public static pulse.cdm.bind.ImpairedAlveolarExchangeData Unload(SEImpairedAlveolarExchange src)
    {
      pulse.cdm.bind.ImpairedAlveolarExchangeData dst = new pulse.cdm.bind.ImpairedAlveolarExchangeData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEImpairedAlveolarExchange src, pulse.cdm.bind.ImpairedAlveolarExchangeData dst)
    {
      dst.PatientCondition = new pulse.cdm.bind.PatientConditionData();
      Serialize(src, dst.PatientCondition);
      if (src.HasImpairedSurfaceArea())
        dst.ImpairedSurfaceArea = PBProperty.Unload(src.GetImpairedSurfaceArea());
      if (src.HasImpairedFraction())
        dst.ImpairedFraction = PBProperty.Unload(src.GetImpairedFraction());
    }
    #endregion

    #region SELobarPneumonia
    public static void Load(pulse.cdm.bind.LobarPneumoniaData src, SELobarPneumonia dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.LobarPneumoniaData src, SELobarPneumonia dst)
    {
      Serialize(src.PatientCondition, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
      if (src.LeftLungAffected != null)
        PBProperty.Load(src.LeftLungAffected, dst.GetLeftLungAffected());
      if (src.RightLungAffected != null)
        PBProperty.Load(src.RightLungAffected, dst.GetRightLungAffected());
    }
    public static pulse.cdm.bind.LobarPneumoniaData Unload(SELobarPneumonia src)
    {
      pulse.cdm.bind.LobarPneumoniaData dst = new pulse.cdm.bind.LobarPneumoniaData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SELobarPneumonia src, pulse.cdm.bind.LobarPneumoniaData dst)
    {
      dst.PatientCondition = new pulse.cdm.bind.PatientConditionData();
      Serialize(src, dst.PatientCondition);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
      if (src.HasLeftLungAffected())
        dst.LeftLungAffected = PBProperty.Unload(src.GetLeftLungAffected());
      if (src.HasRightLungAffected())
        dst.RightLungAffected = PBProperty.Unload(src.GetRightLungAffected());
    }
    #endregion

    #region SEPulmonaryFibrosis
    public static void Load(pulse.cdm.bind.PulmonaryFibrosisData src, SEPulmonaryFibrosis dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.PulmonaryFibrosisData src, SEPulmonaryFibrosis dst)
    {
      Serialize(src.PatientCondition, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }
    public static pulse.cdm.bind.PulmonaryFibrosisData Unload(SEPulmonaryFibrosis src)
    {
      pulse.cdm.bind.PulmonaryFibrosisData dst = new pulse.cdm.bind.PulmonaryFibrosisData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEPulmonaryFibrosis src, pulse.cdm.bind.PulmonaryFibrosisData dst)
    {
      dst.PatientCondition = new pulse.cdm.bind.PatientConditionData();
      Serialize(src, dst.PatientCondition);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEPulmonaryShunt
    public static void Load(pulse.cdm.bind.PulmonaryShuntData src, SEPulmonaryShunt dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.PulmonaryShuntData src, SEPulmonaryShunt dst)
    {
      Serialize(src.PatientCondition, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }
    public static pulse.cdm.bind.PulmonaryShuntData Unload(SEPulmonaryShunt src)
    {
      pulse.cdm.bind.PulmonaryShuntData dst = new pulse.cdm.bind.PulmonaryShuntData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEPulmonaryShunt src, pulse.cdm.bind.PulmonaryShuntData dst)
    {
      dst.PatientCondition = new pulse.cdm.bind.PatientConditionData();
      Serialize(src, dst.PatientCondition);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SESepsis
    public static void Load(pulse.cdm.bind.SepsisData src, SESepsis dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.SepsisData src, SESepsis dst)
    {
      Serialize(src.PatientCondition, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }
    public static pulse.cdm.bind.SepsisData Unload(SESepsis src)
    {
      pulse.cdm.bind.SepsisData dst = new pulse.cdm.bind.SepsisData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SESepsis src, pulse.cdm.bind.SepsisData dst)
    {
      dst.PatientCondition = new pulse.cdm.bind.PatientConditionData();
      Serialize(src, dst.PatientCondition);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

  }
}

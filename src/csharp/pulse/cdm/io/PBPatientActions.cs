/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class PBPatientAction
  {
    #region Any Patient Action
    /** Create a new action based on the binding object, load that data into the new action, and return said action */
    public static SEPatientAction Load(pulse.cdm.bind.AnyPatientActionData any/*, SESubstanceManager subMgr*/)
    {
      if (any.AcuteRespiratoryDistressSyndromeExacerbation != null)
      {
        SEAcuteRespiratoryDistressSyndromeExacerbation ards = new SEAcuteRespiratoryDistressSyndromeExacerbation();
        Serialize(any.AcuteRespiratoryDistressSyndromeExacerbation, ards);
        return ards;
      }
      if (any.AcuteStress != null)
      {
        SEAcuteStress a = new SEAcuteStress();
        Serialize(any.AcuteStress, a);
        return a;
      }
      if (any.AirwayObstruction != null)
      {
        SEAirwayObstruction ao = new SEAirwayObstruction();
        Serialize(any.AirwayObstruction, ao);
        return ao;
      }
      if(any.Arrhythmia != null)
      {
        SEArrhythmia a = new SEArrhythmia();
        Serialize(any.Arrhythmia, a);
        return a;
      }
      if (any.AsthmaAttack != null)
      {
        SEAsthmaAttack aa = new SEAsthmaAttack();
        Serialize(any.AsthmaAttack, aa);
        return aa;
      }
      if (any.BrainInjury != null)
      {
        SEBrainInjury bi = new SEBrainInjury();
        Serialize(any.BrainInjury, bi);
        return bi;
      }
      if (any.Bronchoconstriction != null)
      {
        SEBronchoconstriction b = new SEBronchoconstriction();
        Serialize(any.Bronchoconstriction, b);
        return b;
      }
      if (any.ChestCompressionForce != null)
      {
        SEChestCompressionForce cprF = new SEChestCompressionForce();
        Serialize(any.ChestCompressionForce, cprF);
        return cprF;
      }
      if (any.ChestCompressionForceScale != null)
      {
        SEChestCompressionForceScale cprFS = new SEChestCompressionForceScale();
        Serialize(any.ChestCompressionForceScale, cprFS);
        return cprFS;
      }
      if (any.ChestOcclusiveDressing != null)
      {
        SEChestOcclusiveDressing cod = new SEChestOcclusiveDressing();
        Serialize(any.ChestOcclusiveDressing, cod);
        return cod;
      }
      if (any.ChronicObstructivePulmonaryDiseaseExacerbation != null)
      {
        SEChronicObstructivePulmonaryDiseaseExacerbation copd = new SEChronicObstructivePulmonaryDiseaseExacerbation();
        Serialize(any.ChronicObstructivePulmonaryDiseaseExacerbation, copd);
        return copd;
      }
      if (any.ConsciousRespiration != null)
      {
        SEConsciousRespiration cr = new SEConsciousRespiration();
        Serialize(any.ConsciousRespiration, cr);
        return cr;
      }
      if (any.ConsumeNutrients != null)
      {
        SEConsumeNutrients cn = new SEConsumeNutrients();
        Serialize(any.ConsumeNutrients, cn);
        return cn;
      }
      if (any.Dyspnea != null)
      {
        SEDyspnea d = new SEDyspnea();
        Serialize(any.Dyspnea, d);
        return d;
      }
      if (any.Exercise != null)
      {
        SEExercise ex = new SEExercise();
        Serialize(any.Exercise, ex);
        return ex;
      }
      if (any.Hemorrhage != null)
      {
        SEHemorrhage h = new SEHemorrhage();
        Serialize(any.Hemorrhage, h);
        return h;
      }
      if (any.ImpairedAlveolarExchangeExacerbation != null)
      {
        SEImpairedAlveolarExchangeExacerbation iae = new SEImpairedAlveolarExchangeExacerbation();
        Serialize(any.ImpairedAlveolarExchangeExacerbation, iae);
        return iae;
      }
      if (any.Intubation != null)
      {
        SEIntubation i = new SEIntubation();
        Serialize(any.Intubation, i);
        return i;
      }
      if (any.LobarPneumoniaExacerbation != null)
      {
        SELobarPneumoniaExacerbation lp = new SELobarPneumoniaExacerbation();
        Serialize(any.LobarPneumoniaExacerbation, lp);
        return lp;
      }
      if (any.MechanicalVentilation != null)
      {
        SEMechanicalVentilation mv = new SEMechanicalVentilation();
        Serialize(any.MechanicalVentilation, mv);
        return mv;
      }
      if (any.NeedleDecompression != null)
      {
        SENeedleDecompression n = new SENeedleDecompression();
        Serialize(any.NeedleDecompression, n);
        return n;
      }
      if (any.PericardialEffusion != null)
      {
        SEPericardialEffusion pe = new SEPericardialEffusion();
        Serialize(any.PericardialEffusion, pe);
        return pe;
      }
      if (any.PulmonaryShuntExacerbation != null)
      {
        SEPulmonaryShuntExacerbation pse = new SEPulmonaryShuntExacerbation();
        Serialize(any.PulmonaryShuntExacerbation, pse);
        return pse;
      }
      if (any.RespiratoryFatigue != null)
      {
        SERespiratoryFatigue rf = new SERespiratoryFatigue();
        Serialize(any.RespiratoryFatigue, rf);
        return rf;
      }
      if (any.RespiratoryMechanicsConfiguration != null)
      {
        SERespiratoryMechanicsConfiguration rmc = new SERespiratoryMechanicsConfiguration();
        Serialize(any.RespiratoryMechanicsConfiguration, rmc);
        return rmc;
      }
      if (any.SubstanceBolus != null)
      {
        SESubstanceBolus sb = new SESubstanceBolus();
        Serialize(any.SubstanceBolus, sb);
        return sb;
      }
      if (any.SubstanceCompoundInfusion != null)
      {
        SESubstanceCompoundInfusion sci = new SESubstanceCompoundInfusion();
        Serialize(any.SubstanceCompoundInfusion, sci);
        return sci;
      }
      if (any.SubstanceInfusion != null)
      {
        SESubstanceInfusion si = new SESubstanceInfusion();
        Serialize(any.SubstanceInfusion, si);
        return si;
      }
      if (any.SupplementalOxygen != null)
      {
        SESupplementalOxygen so2 = new SESupplementalOxygen();
        Serialize(any.SupplementalOxygen, so2);
        return so2;
      }
      if (any.TensionPneumothorax != null)
      {
        SETensionPneumothorax tp = new SETensionPneumothorax();
        Serialize(any.TensionPneumothorax, tp);
        return tp;
      }
      if (any.Urinate != null)
      {
        SEUrinate u = new SEUrinate();
        Serialize(any.Urinate, u);
        return u;
      }
      return null;
    }
    /** Create a new bind object, unload the action, put that in the bind object, and return said bind object */
    public static pulse.cdm.bind.AnyPatientActionData Unload(SEPatientAction action)
    {
      pulse.cdm.bind.AnyPatientActionData any = new pulse.cdm.bind.AnyPatientActionData();

      if (action.GetType().IsAssignableFrom(typeof(SEAcuteRespiratoryDistressSyndromeExacerbation)))
      {
        any.AcuteRespiratoryDistressSyndromeExacerbation = Unload((SEAcuteRespiratoryDistressSyndromeExacerbation)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAcuteStress)))
      {
        any.AcuteStress = Unload((SEAcuteStress)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAirwayObstruction)))
      {
        any.AirwayObstruction = Unload((SEAirwayObstruction)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEArrhythmia)))
      {
        any.Arrhythmia = Unload((SEArrhythmia)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEAsthmaAttack)))
      {
        any.AsthmaAttack = Unload((SEAsthmaAttack)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEBrainInjury)))
      {
        any.BrainInjury = Unload((SEBrainInjury)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEBronchoconstriction)))
      {
        any.Bronchoconstriction = Unload((SEBronchoconstriction)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEChestCompressionForce)))
      {
        any.ChestCompressionForce = Unload((SEChestCompressionForce)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEChestCompressionForceScale)))
      {
        any.ChestCompressionForceScale = Unload((SEChestCompressionForceScale)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEChestOcclusiveDressing)))
      {
        any.ChestOcclusiveDressing = Unload((SEChestOcclusiveDressing)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEChronicObstructivePulmonaryDiseaseExacerbation)))
      {
        any.ChronicObstructivePulmonaryDiseaseExacerbation = Unload((SEChronicObstructivePulmonaryDiseaseExacerbation)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEConsciousRespiration)))
      {
        any.ConsciousRespiration = Unload((SEConsciousRespiration)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEConsumeNutrients)))
      {
        any.ConsumeNutrients = Unload((SEConsumeNutrients)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEDyspnea)))
      {
        any.Dyspnea = Unload((SEDyspnea)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEExercise)))
      {
        any.Exercise = Unload((SEExercise)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEHemorrhage)))
      {
        any.Hemorrhage = Unload((SEHemorrhage)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEImpairedAlveolarExchangeExacerbation)))
      {
        any.ImpairedAlveolarExchangeExacerbation = Unload((SEImpairedAlveolarExchangeExacerbation)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEIntubation)))
      {
        any.Intubation = Unload((SEIntubation)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SELobarPneumoniaExacerbation)))
      {
        any.LobarPneumoniaExacerbation = Unload((SELobarPneumoniaExacerbation)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEMechanicalVentilation)))
      {
        any.MechanicalVentilation = Unload((SEMechanicalVentilation)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SENeedleDecompression)))
      {
        any.NeedleDecompression = Unload((SENeedleDecompression)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEPericardialEffusion)))
      {
        any.PericardialEffusion = Unload((SEPericardialEffusion)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEPulmonaryShuntExacerbation)))
      {
        any.PulmonaryShuntExacerbation = Unload((SEPulmonaryShuntExacerbation)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SERespiratoryFatigue)))
      {
        any.RespiratoryFatigue = Unload((SERespiratoryFatigue)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SERespiratoryMechanicsConfiguration)))
      {
        any.RespiratoryMechanicsConfiguration = Unload((SERespiratoryMechanicsConfiguration)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SESubstanceBolus)))
      {
        any.SubstanceBolus = Unload((SESubstanceBolus)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SESubstanceCompoundInfusion)))
      {
        any.SubstanceCompoundInfusion = Unload((SESubstanceCompoundInfusion)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SESubstanceInfusion)))
      {
        any.SubstanceInfusion = Unload((SESubstanceInfusion)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SESupplementalOxygen)))
      {
        any.SupplementalOxygen = Unload((SESupplementalOxygen)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SETensionPneumothorax)))
      {
        any.TensionPneumothorax = Unload((SETensionPneumothorax)action);
        return any;
      }
      if (action.GetType().IsAssignableFrom(typeof(SEUrinate)))
      {
        any.Urinate = Unload((SEUrinate)action);
        return any;
      }

      return any;
    }
    #endregion

    #region SEPatientAction
    public static void Serialize(pulse.cdm.bind.PatientActionData src, SEPatientAction dst)
    {
      if (src.Action != null)
        PBAction.Serialize(src.Action, dst);
    }
    static void Serialize(SEPatientAction src, pulse.cdm.bind.PatientActionData dst)
    {
      dst.Action = new pulse.cdm.bind.ActionData();
      PBAction.Serialize(src, dst.Action);
    }
    #endregion

    #region SEAcuteRespiratoryDistressSyndromeExacerbation
    public static void Load(pulse.cdm.bind.AcuteRespiratoryDistressSyndromeExacerbationData src, SEAcuteRespiratoryDistressSyndromeExacerbation dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.AcuteRespiratoryDistressSyndromeExacerbationData src, SEAcuteRespiratoryDistressSyndromeExacerbation dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
      if (src.LeftLungAffected != null)
        PBProperty.Load(src.LeftLungAffected, dst.GetLeftLungAffected());
      if (src.RightLungAffected != null)
        PBProperty.Load(src.RightLungAffected, dst.GetRightLungAffected());
    }
    public static pulse.cdm.bind.AcuteRespiratoryDistressSyndromeExacerbationData Unload(SEAcuteRespiratoryDistressSyndromeExacerbation src)
    {
      pulse.cdm.bind.AcuteRespiratoryDistressSyndromeExacerbationData dst = new pulse.cdm.bind.AcuteRespiratoryDistressSyndromeExacerbationData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEAcuteRespiratoryDistressSyndromeExacerbation src, pulse.cdm.bind.AcuteRespiratoryDistressSyndromeExacerbationData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
      if (src.HasLeftLungAffected())
        dst.LeftLungAffected = PBProperty.Unload(src.GetLeftLungAffected());
      if (src.HasRightLungAffected())
        dst.RightLungAffected = PBProperty.Unload(src.GetRightLungAffected());
    }
    #endregion

    #region SEAcuteStress
    public static void Load(pulse.cdm.bind.AcuteStressData src, SEAcuteStress dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.AcuteStressData src, SEAcuteStress dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }
    public static pulse.cdm.bind.AcuteStressData Unload(SEAcuteStress src)
    {
      pulse.cdm.bind.AcuteStressData dst = new pulse.cdm.bind.AcuteStressData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEAcuteStress src, pulse.cdm.bind.AcuteStressData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEAirwayObstruction
    public static void Load(pulse.cdm.bind.AirwayObstructionData src, SEAirwayObstruction dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.AirwayObstructionData src, SEAirwayObstruction dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }
    public static pulse.cdm.bind.AirwayObstructionData Unload(SEAirwayObstruction src)
    {
      pulse.cdm.bind.AirwayObstructionData dst = new pulse.cdm.bind.AirwayObstructionData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEAirwayObstruction src, pulse.cdm.bind.AirwayObstructionData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEArrhythmia
    public static void Load(pulse.cdm.bind.ArrhythmiaData src, SEArrhythmia dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ArrhythmiaData src, SEArrhythmia dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      dst.SetRhythm((eHeartRhythm)(int)src.Rhythm);
    }
    public static pulse.cdm.bind.ArrhythmiaData Unload(SEArrhythmia src)
    {
      pulse.cdm.bind.ArrhythmiaData dst = new pulse.cdm.bind.ArrhythmiaData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEArrhythmia src, pulse.cdm.bind.ArrhythmiaData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      dst.Rhythm = (pulse.cdm.bind.eHeartRhythm)(int)src.GetRhythm();
    }
    #endregion

    #region SEAsthmaAttack
    public static void Load(pulse.cdm.bind.AsthmaAttackData src, SEAsthmaAttack dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.AsthmaAttackData src, SEAsthmaAttack dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }
    public static pulse.cdm.bind.AsthmaAttackData Unload(SEAsthmaAttack src)
    {
      pulse.cdm.bind.AsthmaAttackData dst = new pulse.cdm.bind.AsthmaAttackData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEAsthmaAttack src, pulse.cdm.bind.AsthmaAttackData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEBrainInjury
    public static void Load(pulse.cdm.bind.BrainInjuryData src, SEBrainInjury dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.BrainInjuryData src, SEBrainInjury dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
      dst.SetInjuryType((eBrainInjury_Type)(int)src.Type);
    }
    public static pulse.cdm.bind.BrainInjuryData Unload(SEBrainInjury src)
    {
      pulse.cdm.bind.BrainInjuryData dst = new pulse.cdm.bind.BrainInjuryData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEBrainInjury src, pulse.cdm.bind.BrainInjuryData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
      dst.Type = (pulse.cdm.bind.BrainInjuryData.Types.eType)(int)src.GetInjuryType();
    }
    #endregion

    #region SEBronchoconstriction
    public static void Load(pulse.cdm.bind.BronchoconstrictionData src, SEBronchoconstriction dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.BronchoconstrictionData src, SEBronchoconstriction dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }
    public static pulse.cdm.bind.BronchoconstrictionData Unload(SEBronchoconstriction src)
    {
      pulse.cdm.bind.BronchoconstrictionData dst = new pulse.cdm.bind.BronchoconstrictionData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEBronchoconstriction src, pulse.cdm.bind.BronchoconstrictionData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEChestCompressionForce
    public static void Load(pulse.cdm.bind.ChestCompressionForceData src, SEChestCompressionForce dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ChestCompressionForceData src, SEChestCompressionForce dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.Force != null)
        PBProperty.Load(src.Force, dst.GetForce());
    }
    public static pulse.cdm.bind.ChestCompressionForceData Unload(SEChestCompressionForce src)
    {
      pulse.cdm.bind.ChestCompressionForceData dst = new pulse.cdm.bind.ChestCompressionForceData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEChestCompressionForce src, pulse.cdm.bind.ChestCompressionForceData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasForce())
        dst.Force = PBProperty.Unload(src.GetForce());
    }
    #endregion

    #region SEChestCompressionForceScale
    public static void Load(pulse.cdm.bind.ChestCompressionForceScaleData src, SEChestCompressionForceScale dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ChestCompressionForceScaleData src, SEChestCompressionForceScale dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.ForcePeriod != null)
        PBProperty.Load(src.ForcePeriod, dst.GetForcePeriod());
      if (src.ForceScale != null)
        PBProperty.Load(src.ForceScale, dst.GetForceScale());
    }
    public static pulse.cdm.bind.ChestCompressionForceScaleData Unload(SEChestCompressionForceScale src)
    {
      pulse.cdm.bind.ChestCompressionForceScaleData dst = new pulse.cdm.bind.ChestCompressionForceScaleData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEChestCompressionForceScale src, pulse.cdm.bind.ChestCompressionForceScaleData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasForcePeriod())
        dst.ForcePeriod = PBProperty.Unload(src.GetForcePeriod());
      if (src.HasForceScale())
        dst.ForceScale = PBProperty.Unload(src.GetForceScale());
    }
    #endregion

    #region SEChestOcclusiveDressing
    public static void Load(pulse.cdm.bind.ChestOcclusiveDressingData src, SEChestOcclusiveDressing dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ChestOcclusiveDressingData src, SEChestOcclusiveDressing dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      dst.SetSide((eSide)(int)src.Side);
      dst.SetState((eSwitch)(int)src.State);
    }
    public static pulse.cdm.bind.ChestOcclusiveDressingData Unload(SEChestOcclusiveDressing src)
    {
      pulse.cdm.bind.ChestOcclusiveDressingData dst = new pulse.cdm.bind.ChestOcclusiveDressingData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEChestOcclusiveDressing src, pulse.cdm.bind.ChestOcclusiveDressingData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      dst.Side = (pulse.cdm.bind.eSide)(int)src.GetSide();
      dst.State = (pulse.cdm.bind.eSwitch)(int)src.GetState();
    }
    #endregion

    #region SEChronicObstructivePulmonaryDiseaseExacerbation
    public static void Load(pulse.cdm.bind.ChronicObstructivePulmonaryDiseaseExacerbationData src, SEChronicObstructivePulmonaryDiseaseExacerbation dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ChronicObstructivePulmonaryDiseaseExacerbationData src, SEChronicObstructivePulmonaryDiseaseExacerbation dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.BronchitisSeverity != null)
        PBProperty.Load(src.BronchitisSeverity, dst.GetBronchitisSeverity());
      if (src.EmphysemaSeverity != null)
        PBProperty.Load(src.EmphysemaSeverity, dst.GetEmphysemaSeverity());
    }
    public static pulse.cdm.bind.ChronicObstructivePulmonaryDiseaseExacerbationData Unload(SEChronicObstructivePulmonaryDiseaseExacerbation src)
    {
      pulse.cdm.bind.ChronicObstructivePulmonaryDiseaseExacerbationData dst = new pulse.cdm.bind.ChronicObstructivePulmonaryDiseaseExacerbationData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEChronicObstructivePulmonaryDiseaseExacerbation src, pulse.cdm.bind.ChronicObstructivePulmonaryDiseaseExacerbationData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasBronchitisSeverity())
        dst.BronchitisSeverity = PBProperty.Unload(src.GetBronchitisSeverity());
      if (src.HasEmphysemaSeverity())
        dst.EmphysemaSeverity = PBProperty.Unload(src.GetEmphysemaSeverity());
    }
    #endregion

    #region SEConsciousRespiration
    public static void Load(pulse.cdm.bind.ConsciousRespirationData src, SEConsciousRespiration dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ConsciousRespirationData src, SEConsciousRespiration dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      dst.SetStartImmediately(src.StartImmediately);
      foreach (pulse.cdm.bind.AnyConsciousRespirationCommandData any in src.Command)
      {
        if (any.ForcedExhale != null)
        {
          SEForcedExhale c = new SEForcedExhale();
          Load(any.ForcedExhale, c);
          continue;
        }
        if (any.ForcedInhale != null)
        {
          SEForcedInhale c = new SEForcedInhale();
          Load(any.ForcedInhale, c);
          continue;
        }
        if (any.ForcedPause != null)
        {
          SEForcedPause c = new SEForcedPause();
          Load(any.ForcedPause, c);
          continue;
        }
        if (any.UseInhaler != null)
        {
          SEUseInhaler c = new SEUseInhaler();
          Load(any.UseInhaler, c);
          continue;
        }
      }
    }
    public static pulse.cdm.bind.ConsciousRespirationData Unload(SEConsciousRespiration src)
    {
      pulse.cdm.bind.ConsciousRespirationData dst = new pulse.cdm.bind.ConsciousRespirationData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEConsciousRespiration src, pulse.cdm.bind.ConsciousRespirationData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      dst.StartImmediately = src.StartImmediately();
      foreach (SEConsciousRespirationCommand command in src.GetCommands())
      {
        pulse.cdm.bind.AnyConsciousRespirationCommandData any = new pulse.cdm.bind.AnyConsciousRespirationCommandData();
        if (command.GetType().IsAssignableFrom(typeof(SEForcedExhale)))
        {
          any.ForcedExhale = Unload((SEForcedExhale)command);
          continue;
        }
        if (command.GetType().IsAssignableFrom(typeof(SEForcedInhale)))
        {
          any.ForcedInhale = Unload((SEForcedInhale)command);
          continue;
        }
        if (command.GetType().IsAssignableFrom(typeof(SEForcedPause)))
        {
          any.ForcedPause = Unload((SEForcedPause)command);
          continue;
        }
        if (command.GetType().IsAssignableFrom(typeof(SEUseInhaler)))
        {
          any.UseInhaler = Unload((SEUseInhaler)command);
          continue;
        }
        dst.Command.Add(any);
      }
    }

    public static void Load(pulse.cdm.bind.ForcedExhaleData src, SEForcedExhale dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ForcedExhaleData src, SEForcedExhale dst)
    {
      if (src.ExpiratoryReserveVolumeFraction != null)
        PBProperty.Load(src.ExpiratoryReserveVolumeFraction, dst.GetExpiratoryReserveVolumeFraction());
      if (src.ExhalePeriod != null)
        PBProperty.Load(src.ExhalePeriod, dst.GetExhalePeriod());
      if (src.HoldPeriod != null)
        PBProperty.Load(src.HoldPeriod, dst.GetHoldPeriod());
      if (src.ReleasePeriod != null)
        PBProperty.Load(src.ReleasePeriod, dst.GetReleasePeriod());
    }
    public static pulse.cdm.bind.ForcedExhaleData Unload(SEForcedExhale src)
    {
      pulse.cdm.bind.ForcedExhaleData dst = new pulse.cdm.bind.ForcedExhaleData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEForcedExhale src, pulse.cdm.bind.ForcedExhaleData dst)
    {
      if (src.HasExpiratoryReserveVolumeFraction())
        dst.ExpiratoryReserveVolumeFraction = PBProperty.Unload(src.GetExpiratoryReserveVolumeFraction());
      if (src.HasExhalePeriod())
        dst.ExhalePeriod = PBProperty.Unload(src.GetExhalePeriod());
      if (src.HasHoldPeriod())
        dst.HoldPeriod = PBProperty.Unload(src.GetHoldPeriod());
      if (src.HasReleasePeriod())
        dst.ReleasePeriod = PBProperty.Unload(src.GetReleasePeriod());
    }

    public static void Load(pulse.cdm.bind.ForcedInhaleData src, SEForcedInhale dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ForcedInhaleData src, SEForcedInhale dst)
    {
      if (src.InspiratoryCapacityFraction != null)
        PBProperty.Load(src.InspiratoryCapacityFraction, dst.GetInspiratoryCapacityFraction());
      if (src.InhalePeriod != null)
        PBProperty.Load(src.InhalePeriod, dst.GetInhalePeriod());
      if (src.HoldPeriod != null)
        PBProperty.Load(src.HoldPeriod, dst.GetHoldPeriod());
      if (src.ReleasePeriod != null)
        PBProperty.Load(src.ReleasePeriod, dst.GetReleasePeriod());
    }
    public static pulse.cdm.bind.ForcedInhaleData Unload(SEForcedInhale src)
    {
      pulse.cdm.bind.ForcedInhaleData dst = new pulse.cdm.bind.ForcedInhaleData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEForcedInhale src, pulse.cdm.bind.ForcedInhaleData dst)
    {
      if (src.HasInspiratoryCapacityFraction())
        dst.InspiratoryCapacityFraction = PBProperty.Unload(src.GetInspiratoryCapacityFraction());
      if (src.HasInhalePeriod())
        dst.InhalePeriod = PBProperty.Unload(src.GetInhalePeriod());
      if (src.HasHoldPeriod())
        dst.HoldPeriod = PBProperty.Unload(src.GetHoldPeriod());
      if (src.HasReleasePeriod())
        dst.ReleasePeriod = PBProperty.Unload(src.GetReleasePeriod());
    }

    public static void Load(pulse.cdm.bind.ForcedPauseData src, SEForcedPause dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ForcedPauseData src, SEForcedPause dst)
    {
      if (src.Period != null)
        PBProperty.Load(src.Period, dst.GetPeriod());
    }
    public static pulse.cdm.bind.ForcedPauseData Unload(SEForcedPause src)
    {
      pulse.cdm.bind.ForcedPauseData dst = new pulse.cdm.bind.ForcedPauseData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEForcedPause src, pulse.cdm.bind.ForcedPauseData dst)
    {
      if (src.HasPeriod())
        dst.Period = PBProperty.Unload(src.GetPeriod());
    }

    public static void Load(pulse.cdm.bind.UseInhalerData src, SEUseInhaler dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.UseInhalerData src, SEUseInhaler dst)
    {
     
    }
    public static pulse.cdm.bind.UseInhalerData Unload(SEUseInhaler src)
    {
      pulse.cdm.bind.UseInhalerData dst = new pulse.cdm.bind.UseInhalerData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEUseInhaler src, pulse.cdm.bind.UseInhalerData dst)
    {
      
    }
    #endregion

    #region SEConsumeNutrients
    public static void Load(pulse.cdm.bind.ConsumeNutrientsData src, SEConsumeNutrients dst)
    {
      dst.Clear();
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ConsumeNutrientsData src, SEConsumeNutrients dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.NutritionFile != null)
        dst.SetNutritionFile(src.NutritionFile);
      if (src.Nutrition != null)
        PBNutrition.Load(src.Nutrition, dst.GetNutrition());
    }

    public static pulse.cdm.bind.ConsumeNutrientsData Unload(SEConsumeNutrients src)
    {
      pulse.cdm.bind.ConsumeNutrientsData dst = new pulse.cdm.bind.ConsumeNutrientsData();
      Serialize(src, dst);
      return dst;
    }
    static void Serialize(SEConsumeNutrients src, pulse.cdm.bind.ConsumeNutrientsData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasNutritionFile())
        dst.NutritionFile = src.GetNutritionFile();
      else if (src.HasNutrition())
        dst.Nutrition = PBNutrition.Unload(src.GetNutrition());
    }
    #endregion

    #region SEDyspnea
    public static void Load(pulse.cdm.bind.DyspneaData src, SEDyspnea dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.DyspneaData src, SEDyspnea dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }
    public static pulse.cdm.bind.DyspneaData Unload(SEDyspnea src)
    {
      pulse.cdm.bind.DyspneaData dst = new pulse.cdm.bind.DyspneaData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEDyspnea src, pulse.cdm.bind.DyspneaData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEExercise
    public static void Load(pulse.cdm.bind.ExerciseData src, SEExercise dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ExerciseData src, SEExercise dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.Intensity != null)
        PBProperty.Load(src.Intensity, dst.GetIntensity());
    }
    public static pulse.cdm.bind.ExerciseData Unload(SEExercise src)
    {
      pulse.cdm.bind.ExerciseData dst = new pulse.cdm.bind.ExerciseData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEExercise src, pulse.cdm.bind.ExerciseData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasIntensity())
        dst.Intensity = PBProperty.Unload(src.GetIntensity());
    }
    #endregion

    #region SEHemorrhage
    public static void Load(pulse.cdm.bind.HemorrhageData src, SEHemorrhage dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.HemorrhageData src, SEHemorrhage dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      dst.SetType((eHemorrhage_Type)(int)src.Type);
      if (src.Compartment != null)
        dst.SetCompartment(src.Compartment);
      if (src.FlowRate != null)
        PBProperty.Load(src.FlowRate, dst.GetFlowRate());
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }
    public static pulse.cdm.bind.HemorrhageData Unload(SEHemorrhage src)
    {
      pulse.cdm.bind.HemorrhageData dst = new pulse.cdm.bind.HemorrhageData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEHemorrhage src, pulse.cdm.bind.HemorrhageData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      dst.Type = (pulse.cdm.bind.HemorrhageData.Types.eType)(int)src.GetType();
      if (src.HasCompartment())
        dst.Compartment = src.GetCompartment();
      if (src.HasFlowRate())
        dst.FlowRate = PBProperty.Unload(src.GetFlowRate());
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEImpairedAlveolarExchangeExacerbation
    public static void Load(pulse.cdm.bind.ImpairedAlveolarExchangeExacerbationData src, SEImpairedAlveolarExchangeExacerbation dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ImpairedAlveolarExchangeExacerbationData src, SEImpairedAlveolarExchangeExacerbation dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.ImpairedSurfaceArea != null)
        PBProperty.Load(src.ImpairedSurfaceArea, dst.GetImpairedSurfaceArea());
      else if (src.ImpairedFraction != null)
        PBProperty.Load(src.ImpairedFraction, dst.GetImpairedFraction());
      else if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }
    public static pulse.cdm.bind.ImpairedAlveolarExchangeExacerbationData Unload(SEImpairedAlveolarExchangeExacerbation src)
    {
      pulse.cdm.bind.ImpairedAlveolarExchangeExacerbationData dst = new pulse.cdm.bind.ImpairedAlveolarExchangeExacerbationData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEImpairedAlveolarExchangeExacerbation src, pulse.cdm.bind.ImpairedAlveolarExchangeExacerbationData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasImpairedSurfaceArea())
        dst.ImpairedSurfaceArea = PBProperty.Unload(src.GetImpairedSurfaceArea());
      else if (src.HasImpairedFraction())
        dst.ImpairedFraction = PBProperty.Unload(src.GetImpairedFraction());
      else if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEIntubation
    public static void Load(pulse.cdm.bind.IntubationData src, SEIntubation dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.IntubationData src, SEIntubation dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      dst.SetType((eIntubation_Type)(int)src.Type);
    }
    public static pulse.cdm.bind.IntubationData Unload(SEIntubation src)
    {
      pulse.cdm.bind.IntubationData dst = new pulse.cdm.bind.IntubationData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEIntubation src, pulse.cdm.bind.IntubationData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      dst.Type = (pulse.cdm.bind.IntubationData.Types.eType)(int)src.GetType();
    }
    #endregion

    #region SELobarPneumoniaExacerbation
    public static void Load(pulse.cdm.bind.LobarPneumoniaExacerbationData src, SELobarPneumoniaExacerbation dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.LobarPneumoniaExacerbationData src, SELobarPneumoniaExacerbation dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
      if (src.LeftLungAffected != null)
        PBProperty.Load(src.LeftLungAffected, dst.GetLeftLungAffected());
      if (src.RightLungAffected != null)
        PBProperty.Load(src.RightLungAffected, dst.GetRightLungAffected());
    }
    public static pulse.cdm.bind.LobarPneumoniaExacerbationData Unload(SELobarPneumoniaExacerbation src)
    {
      pulse.cdm.bind.LobarPneumoniaExacerbationData dst = new pulse.cdm.bind.LobarPneumoniaExacerbationData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SELobarPneumoniaExacerbation src, pulse.cdm.bind.LobarPneumoniaExacerbationData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
      if (src.HasLeftLungAffected())
        dst.LeftLungAffected = PBProperty.Unload(src.GetLeftLungAffected());
      if (src.HasRightLungAffected())
        dst.RightLungAffected = PBProperty.Unload(src.GetRightLungAffected());
    }
    #endregion

    #region SEMechanicalVentilation
    public static void Load(pulse.cdm.bind.MechanicalVentilationData src, SEMechanicalVentilation dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.MechanicalVentilationData src, SEMechanicalVentilation dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      dst.SetState((eSwitch)(int)src.State);
      if (src.Flow != null)
        PBProperty.Load(src.Flow, dst.GetFlow());
      if (src.Pressure != null)
        PBProperty.Load(src.Pressure, dst.GetPressure());
      foreach (pulse.cdm.bind.SubstanceFractionData sf in src.GasFraction)
      {
        PBProperty.Load(sf.Amount, dst.GetGasFraction(sf.Name).GetFractionAmount());
      }
      foreach (pulse.cdm.bind.SubstanceConcentrationData sc in src.Aerosol)
      {
        PBProperty.Load(sc.Concentration, dst.GetAerosol(sc.Name).GetConcentration());
      }
    }
    public static pulse.cdm.bind.MechanicalVentilationData Unload(SEMechanicalVentilation src)
    {
      pulse.cdm.bind.MechanicalVentilationData dst = new pulse.cdm.bind.MechanicalVentilationData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEMechanicalVentilation src, pulse.cdm.bind.MechanicalVentilationData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      dst.State = (pulse.cdm.bind.eSwitch)(int)src.GetState();
      if (src.HasFlow())
        dst.Flow = PBProperty.Unload(src.GetFlow());
      if (src.HasPressure())
        dst.Pressure = PBProperty.Unload(src.GetPressure());
      foreach (SESubstanceFraction aGas in src.GetGasFractions())
      {
        pulse.cdm.bind.SubstanceFractionData sf = new pulse.cdm.bind.SubstanceFractionData();
        sf.Name = aGas.GetSubstance();
        sf.Amount = PBProperty.Unload(aGas.GetFractionAmount());
        dst.GasFraction.Add(sf);
      }
      foreach (SESubstanceConcentration aAerosol in src.GetAerosols())
      {
        pulse.cdm.bind.SubstanceConcentrationData sc = new pulse.cdm.bind.SubstanceConcentrationData();
        sc.Name = aAerosol.GetSubstance();
        sc.Concentration = PBProperty.Unload(aAerosol.GetConcentration());
        dst.Aerosol.Add(sc);
      }
    }
    #endregion

    #region SENeedleDecompression
    public static void Load(pulse.cdm.bind.NeedleDecompressionData src, SENeedleDecompression dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.NeedleDecompressionData src, SENeedleDecompression dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      dst.SetSide((eSide)(int)src.Side);
      dst.SetState((eSwitch)(int)src.State);
    }
    public static pulse.cdm.bind.NeedleDecompressionData Unload(SENeedleDecompression src)
    {
      pulse.cdm.bind.NeedleDecompressionData dst = new pulse.cdm.bind.NeedleDecompressionData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SENeedleDecompression src, pulse.cdm.bind.NeedleDecompressionData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasSide())
        dst.Side = (pulse.cdm.bind.eSide)(int)src.GetSide();
      dst.State = (pulse.cdm.bind.eSwitch)(int)src.GetState();
    }
    #endregion

    #region SEPericardialEffusion
    public static void Load(pulse.cdm.bind.PericardialEffusionData src, SEPericardialEffusion dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.PericardialEffusionData src, SEPericardialEffusion dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.EffusionRate != null)
        PBProperty.Load(src.EffusionRate, dst.GetEffusionRate());
    }
    public static pulse.cdm.bind.PericardialEffusionData Unload(SEPericardialEffusion src)
    {
      pulse.cdm.bind.PericardialEffusionData dst = new pulse.cdm.bind.PericardialEffusionData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEPericardialEffusion src, pulse.cdm.bind.PericardialEffusionData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasEffusionRate())
        dst.EffusionRate = PBProperty.Unload(src.GetEffusionRate());
    }
    #endregion

    #region SEPulmonaryShuntExacerbation
    public static void Load(pulse.cdm.bind.PulmonaryShuntExacerbationData src, SEPulmonaryShuntExacerbation dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.PulmonaryShuntExacerbationData src, SEPulmonaryShuntExacerbation dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }
    public static pulse.cdm.bind.PulmonaryShuntExacerbationData Unload(SEPulmonaryShuntExacerbation src)
    {
      pulse.cdm.bind.PulmonaryShuntExacerbationData dst = new pulse.cdm.bind.PulmonaryShuntExacerbationData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEPulmonaryShuntExacerbation src, pulse.cdm.bind.PulmonaryShuntExacerbationData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SERespiratoryFatigue
    public static void Load(pulse.cdm.bind.RespiratoryFatigueData src, SERespiratoryFatigue dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.RespiratoryFatigueData src, SERespiratoryFatigue dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }
    public static pulse.cdm.bind.RespiratoryFatigueData Unload(SERespiratoryFatigue src)
    {
      pulse.cdm.bind.RespiratoryFatigueData dst = new pulse.cdm.bind.RespiratoryFatigueData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SERespiratoryFatigue src, pulse.cdm.bind.RespiratoryFatigueData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SERespiratoryMechanicsConfiguration
    public static void Load(pulse.cdm.bind.RespiratoryMechanicsConfigurationData src, SERespiratoryMechanicsConfiguration dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.RespiratoryMechanicsConfigurationData src, SERespiratoryMechanicsConfiguration dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.SettingsFile != null)
        dst.SetConfigurationFile(src.SettingsFile);
      else if (src.Settings != null)
        PBPhysiology.Load(src.Settings, dst.GetSettings());
      dst.SetMergeType((eMergeType)src.MergeType);
    }
    public static pulse.cdm.bind.RespiratoryMechanicsConfigurationData Unload(SERespiratoryMechanicsConfiguration src)
    {
      pulse.cdm.bind.RespiratoryMechanicsConfigurationData dst = new pulse.cdm.bind.RespiratoryMechanicsConfigurationData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SERespiratoryMechanicsConfiguration src, pulse.cdm.bind.RespiratoryMechanicsConfigurationData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasSettingsFile())
        dst.SettingsFile = src.GetSettingsFile();
      else if (src.HasSettings())
        dst.Settings = PBPhysiology.Unload(src.GetSettings());
      dst.MergeType = (pulse.cdm.bind.eMergeType)(int)src.GetMergeType();
    }
    #endregion

    #region SESubstanceBolus
    public static void Load(pulse.cdm.bind.SubstanceBolusData src, SESubstanceBolus dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.SubstanceBolusData src, SESubstanceBolus dst)
    {
      if(src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.Substance != null)
        dst.SetSubstance(src.Substance);
      dst.SetAdminRoute((eSubstanceAdministration_Route)(int)src.AdministrationRoute);

      if (src.Concentration != null)
        PBProperty.Load(src.Concentration, dst.GetConcentration());
      if (src.Dose != null)
        PBProperty.Load(src.Dose, dst.GetDose());

    }
    public static pulse.cdm.bind.SubstanceBolusData Unload(SESubstanceBolus src)
    {
      pulse.cdm.bind.SubstanceBolusData dst = new pulse.cdm.bind.SubstanceBolusData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SESubstanceBolus src, pulse.cdm.bind.SubstanceBolusData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasSubstance())
        dst.Substance = src.GetSubstance();
      dst.AdministrationRoute = (pulse.cdm.bind.SubstanceBolusData.Types.eRoute)(int)src.GetAdminRoute();
      if (src.HasConcentration())
        dst.Concentration = PBProperty.Unload(src.GetConcentration());
      if (src.HasDose())
        dst.Dose = PBProperty.Unload(src.GetDose());
    }
    #endregion

    #region SESubstanceCompoundInfusion
    public static void Load(pulse.cdm.bind.SubstanceCompoundInfusionData src, SESubstanceCompoundInfusion dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.SubstanceCompoundInfusionData src, SESubstanceCompoundInfusion dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.SubstanceCompound != null)
        dst.SetSubstanceCompound(src.SubstanceCompound);
      if (src.Rate != null)
        PBProperty.Load(src.Rate, dst.GetRate());
      if (src.BagVolume != null)
        PBProperty.Load(src.BagVolume, dst.GetBagVolume());
    }
    public static pulse.cdm.bind.SubstanceCompoundInfusionData Unload(SESubstanceCompoundInfusion src)
    {
      pulse.cdm.bind.SubstanceCompoundInfusionData dst = new pulse.cdm.bind.SubstanceCompoundInfusionData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SESubstanceCompoundInfusion src, pulse.cdm.bind.SubstanceCompoundInfusionData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      if (src.HasSubstanceCompound())
        dst.SubstanceCompound = src.GetSubstanceCompound();
      if (src.HasRate())
        dst.Rate = PBProperty.Unload(src.GetRate());
      if (src.HasBagVolume())
        dst.BagVolume = PBProperty.Unload(src.GetBagVolume());
    }
    #endregion

    #region SESubstanceInfusion
    public static void Load(pulse.cdm.bind.SubstanceInfusionData src, SESubstanceInfusion dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.SubstanceInfusionData src, SESubstanceInfusion dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      if (src.Substance != null)
        dst.SetSubstance(src.Substance);
      if (src.Concentration != null)
        PBProperty.Load(src.Concentration, dst.GetConcentration());
      if (src.Rate != null)
        PBProperty.Load(src.Rate, dst.GetRate());
      if (src.Volume != null)
        PBProperty.Load(src.Volume, dst.GetVolume());
    }
    public static pulse.cdm.bind.SubstanceInfusionData Unload(SESubstanceInfusion src)
    {
      pulse.cdm.bind.SubstanceInfusionData dst = new pulse.cdm.bind.SubstanceInfusionData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SESubstanceInfusion src, pulse.cdm.bind.SubstanceInfusionData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      dst.Substance = src.GetSubstance();
      if (src.HasConcentration())
        dst.Concentration = PBProperty.Unload(src.GetConcentration());
      if (src.HasRate())
        dst.Rate = PBProperty.Unload(src.GetRate());
      if (src.HasVolume())
        dst.Volume = PBProperty.Unload(src.GetVolume());
    }
    #endregion

    #region SESupplementalOxygen
    public static void Load(pulse.cdm.bind.SupplementalOxygenData src, SESupplementalOxygen dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.SupplementalOxygenData src, SESupplementalOxygen dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      dst.SetDevice((eSupplementalOxygen_Device)(int)src.Device);
      if (src.Flow != null)
        PBProperty.Load(src.Flow, dst.GetFlow());
      if (src.Volume != null)
        PBProperty.Load(src.Volume, dst.GetVolume());
    }
    public static pulse.cdm.bind.SupplementalOxygenData Unload(SESupplementalOxygen src)
    {
      pulse.cdm.bind.SupplementalOxygenData dst = new pulse.cdm.bind.SupplementalOxygenData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SESupplementalOxygen src, pulse.cdm.bind.SupplementalOxygenData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      dst.Device = (pulse.cdm.bind.SupplementalOxygenData.Types.eDevice)(int)src.GetDevice();
      if (src.HasFlow())
        dst.Flow = PBProperty.Unload(src.GetFlow());
      if (src.HasVolume())
        dst.Volume = PBProperty.Unload(src.GetVolume());
    }
    #endregion

    #region SETensionPneumothorax
    public static void Load(pulse.cdm.bind.TensionPneumothoraxData src, SETensionPneumothorax dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.TensionPneumothoraxData src, SETensionPneumothorax dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
      dst.SetType((eGate)(int)src.Type);
      dst.SetSide((eSide)(int)src.Side);
      if (src.Severity != null)
        PBProperty.Load(src.Severity, dst.GetSeverity());
    }
    public static pulse.cdm.bind.TensionPneumothoraxData Unload(SETensionPneumothorax src)
    {
      pulse.cdm.bind.TensionPneumothoraxData dst = new pulse.cdm.bind.TensionPneumothoraxData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SETensionPneumothorax src, pulse.cdm.bind.TensionPneumothoraxData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
      dst.Type = (pulse.cdm.bind.eGate)(int)src.GetType();
      dst.Side = (pulse.cdm.bind.eSide)(int)src.GetSide();
      if (src.HasSeverity())
        dst.Severity = PBProperty.Unload(src.GetSeverity());
    }
    #endregion

    #region SEUrinate
    public static void Load(pulse.cdm.bind.UrinateData src, SEUrinate dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.UrinateData src, SEUrinate dst)
    {
      if (src.PatientAction != null)
        Serialize(src.PatientAction, dst);
    }
    public static pulse.cdm.bind.UrinateData Unload(SEUrinate src)
    {
      pulse.cdm.bind.UrinateData dst = new pulse.cdm.bind.UrinateData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEUrinate src, pulse.cdm.bind.UrinateData dst)
    {
      dst.PatientAction = new pulse.cdm.bind.PatientActionData();
      Serialize(src, dst.PatientAction);
    }
    #endregion

  }
}

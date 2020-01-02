/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class PBPatientAction
{
  #region Any Patient Action
  /** Create a new action based on the binding object, load that data into the new action, and return said action */
  public static SEPatientAction Load(Cdm.AnyPatientActionData any/*, SESubstanceManager subMgr*/)
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
    if (any.CardiacArrest != null)
    {
      SECardiacArrest ca = new SECardiacArrest();
      Serialize(any.CardiacArrest, ca);
      return ca;
    }
    if (any.ChronicObstructivePulmonaryDiseaseExacerbation != null)
    {
      SEChronicObstructivePulmonaryDiseaseExacerbation copd = new SEChronicObstructivePulmonaryDiseaseExacerbation();
      Serialize(any.ChronicObstructivePulmonaryDiseaseExacerbation, copd);
      return copd;
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
    if (any.RespiratoryFatigue != null)
    {
      SERespiratoryFatigue rf = new SERespiratoryFatigue();
      Serialize(any.RespiratoryFatigue, rf);
      return rf;
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
    return null;
  }
  /** Create a new bind object, unload the action, put that in the bind object, and return said bind object */
  public static Cdm.AnyPatientActionData Unload(SEPatientAction action)
  {
    Cdm.AnyPatientActionData any = new Cdm.AnyPatientActionData();

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
    if (action.GetType().IsAssignableFrom(typeof(SECardiacArrest)))
    {
      any.CardiacArrest = Unload((SECardiacArrest)action);
      return any;
    }
    if (action.GetType().IsAssignableFrom(typeof(SEChronicObstructivePulmonaryDiseaseExacerbation)))
    {
      any.ChronicObstructivePulmonaryDiseaseExacerbation = Unload((SEChronicObstructivePulmonaryDiseaseExacerbation)action);
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
    if (action.GetType().IsAssignableFrom(typeof(SERespiratoryFatigue)))
    {
      any.RespiratoryFatigue = Unload((SERespiratoryFatigue)action);
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

    return any;
  }
  #endregion

  #region SEPatientAction
  public static void Serialize(Cdm.PatientActionData src, SEPatientAction dst)
  {
    PBAction.Serialize(src.Action, dst);
  }
  static void Serialize(SEPatientAction src, Cdm.PatientActionData dst)
  {
    dst.Action = new Cdm.ActionData();
    PBAction.Serialize(src, dst.Action);
  }
  #endregion

  #region SEAcuteRespiratoryDistressSyndromeExacerbation
  public static void Load(Cdm.AcuteRespiratoryDistressSyndromeExacerbationData src, SEAcuteRespiratoryDistressSyndromeExacerbation dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.AcuteRespiratoryDistressSyndromeExacerbationData src, SEAcuteRespiratoryDistressSyndromeExacerbation dst)
  {
    Serialize(src.PatientAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
    if (src.LeftLungAffected != null)
      PBProperty.Load(src.LeftLungAffected, dst.GetLeftLungAffected());
    if (src.RightLungAffected != null)
      PBProperty.Load(src.RightLungAffected, dst.GetRightLungAffected());
  }
  public static Cdm.AcuteRespiratoryDistressSyndromeExacerbationData Unload(SEAcuteRespiratoryDistressSyndromeExacerbation src)
  {
    Cdm.AcuteRespiratoryDistressSyndromeExacerbationData dst = new Cdm.AcuteRespiratoryDistressSyndromeExacerbationData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEAcuteRespiratoryDistressSyndromeExacerbation src, Cdm.AcuteRespiratoryDistressSyndromeExacerbationData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
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
  public static void Load(Cdm.AcuteStressData src, SEAcuteStress dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.AcuteStressData src, SEAcuteStress dst)
  {
    Serialize(src.PatientAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
  }
  public static Cdm.AcuteStressData Unload(SEAcuteStress src)
  {
    Cdm.AcuteStressData dst = new Cdm.AcuteStressData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEAcuteStress src, Cdm.AcuteStressData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
  }
  #endregion

  #region SEAirwayObstruction
  public static void Load(Cdm.AirwayObstructionData src, SEAirwayObstruction dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.AirwayObstructionData src, SEAirwayObstruction dst)
  {
    Serialize(src.PatientAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
  }
  public static Cdm.AirwayObstructionData Unload(SEAirwayObstruction src)
  {
    Cdm.AirwayObstructionData dst = new Cdm.AirwayObstructionData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEAirwayObstruction src, Cdm.AirwayObstructionData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
  }
  #endregion

  #region SEAsthmaAttack
  public static void Load(Cdm.AsthmaAttackData src, SEAsthmaAttack dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.AsthmaAttackData src, SEAsthmaAttack dst)
  {
    Serialize(src.PatientAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
  }
  public static Cdm.AsthmaAttackData Unload(SEAsthmaAttack src)
  {
    Cdm.AsthmaAttackData dst = new Cdm.AsthmaAttackData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEAsthmaAttack src, Cdm.AsthmaAttackData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
  }
  #endregion

  #region SEBrainInjury
  public static void Load(Cdm.BrainInjuryData src, SEBrainInjury dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.BrainInjuryData src, SEBrainInjury dst)
  {
    Serialize(src.PatientAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
    dst.SetInjuryType((SEBrainInjury.eType)(int)src.Type);
  }
  public static Cdm.BrainInjuryData Unload(SEBrainInjury src)
  {
    Cdm.BrainInjuryData dst = new Cdm.BrainInjuryData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEBrainInjury src, Cdm.BrainInjuryData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
    dst.Type = (Cdm.BrainInjuryData.Types.eType)(int)src.GetInjuryType();
  }
  #endregion

  #region SEBronchoconstriction
  public static void Load(Cdm.BronchoconstrictionData src, SEBronchoconstriction dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.BronchoconstrictionData src, SEBronchoconstriction dst)
  {
    Serialize(src.PatientAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
  }
  public static Cdm.BronchoconstrictionData Unload(SEBronchoconstriction src)
  {
    Cdm.BronchoconstrictionData dst = new Cdm.BronchoconstrictionData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEBronchoconstriction src, Cdm.BronchoconstrictionData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
  }
  #endregion

  #region SECardiacArrest
  public static void Load(Cdm.CardiacArrestData src, SECardiacArrest dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.CardiacArrestData src, SECardiacArrest dst)
  {
    Serialize(src.PatientAction, dst);
    dst.SetState((eSwitch)(int)src.State);
  }
  public static Cdm.CardiacArrestData Unload(SECardiacArrest src)
  {
    Cdm.CardiacArrestData dst = new Cdm.CardiacArrestData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SECardiacArrest src, Cdm.CardiacArrestData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    dst.State = (Cdm.eSwitch)(int)src.GetState();
  }
  #endregion

  #region SEChronicObstructivePulmonaryDiseaseExacerbation
  public static void Load(Cdm.ChronicObstructivePulmonaryDiseaseExacerbationData src, SEChronicObstructivePulmonaryDiseaseExacerbation dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ChronicObstructivePulmonaryDiseaseExacerbationData src, SEChronicObstructivePulmonaryDiseaseExacerbation dst)
  {
    Serialize(src.PatientAction, dst);
    if (src.BronchitisSeverity != null)
      PBProperty.Load(src.BronchitisSeverity, dst.GetBronchitisSeverity());
    if (src.EmphysemaSeverity != null)
      PBProperty.Load(src.EmphysemaSeverity, dst.GetEmphysemaSeverity());
  }
  public static Cdm.ChronicObstructivePulmonaryDiseaseExacerbationData Unload(SEChronicObstructivePulmonaryDiseaseExacerbation src)
  {
    Cdm.ChronicObstructivePulmonaryDiseaseExacerbationData dst = new Cdm.ChronicObstructivePulmonaryDiseaseExacerbationData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEChronicObstructivePulmonaryDiseaseExacerbation src, Cdm.ChronicObstructivePulmonaryDiseaseExacerbationData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    if (src.HasBronchitisSeverity())
      dst.BronchitisSeverity = PBProperty.Unload(src.GetBronchitisSeverity());
    if (src.HasEmphysemaSeverity())
      dst.EmphysemaSeverity = PBProperty.Unload(src.GetEmphysemaSeverity());
  }
  #endregion

  #region SEDyspnea
  public static void Load(Cdm.DyspneaData src, SEDyspnea dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.DyspneaData src, SEDyspnea dst)
  {
    Serialize(src.PatientAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
  }
  public static Cdm.DyspneaData Unload(SEDyspnea src)
  {
    Cdm.DyspneaData dst = new Cdm.DyspneaData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEDyspnea src, Cdm.DyspneaData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
  }
  #endregion

  #region SEExercise
  public static void Load(Cdm.ExerciseData src, SEExercise dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ExerciseData src, SEExercise dst)
  {
    Serialize(src.PatientAction, dst);
    if (src.Intensity != null)
      PBProperty.Load(src.Intensity, dst.GetIntensity());
  }
  public static Cdm.ExerciseData Unload(SEExercise src)
  {
    Cdm.ExerciseData dst = new Cdm.ExerciseData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEExercise src, Cdm.ExerciseData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    if (src.HasIntensity())
      dst.Intensity = PBProperty.Unload(src.GetIntensity());
  }
  #endregion

  #region SEHemorrhage
  public static void Load(Cdm.HemorrhageData src, SEHemorrhage dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.HemorrhageData src, SEHemorrhage dst)
  {
    Serialize(src.PatientAction, dst);
    if (src.Compartment != null)
      dst.SetCompartment(src.Compartment);
    if (src.Rate != null)
      PBProperty.Load(src.Rate, dst.GetRate());
    dst.SetType((SEHemorrhage.eType)(int)src.Type);
  }
  public static Cdm.HemorrhageData Unload(SEHemorrhage src)
  {
    Cdm.HemorrhageData dst = new Cdm.HemorrhageData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEHemorrhage src, Cdm.HemorrhageData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    if (src.HasCompartment())
      dst.Compartment = src.GetCompartment();
    if (src.HasRate())
      dst.Rate = PBProperty.Unload(src.GetRate());
    dst.Type = (Cdm.HemorrhageData.Types.eType)(int)src.GetType();
  }
  #endregion

  #region SEIntubation
  public static void Load(Cdm.IntubationData src, SEIntubation dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.IntubationData src, SEIntubation dst)
  {
    Serialize(src.PatientAction, dst);
    dst.SetType((SEIntubation.eType)(int)src.Type);
  }
  public static Cdm.IntubationData Unload(SEIntubation src)
  {
    Cdm.IntubationData dst = new Cdm.IntubationData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEIntubation src, Cdm.IntubationData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    dst.Type = (Cdm.IntubationData.Types.eType)(int)src.GetType();
  }
  #endregion

  #region SELobarPneumoniaExacerbation
  public static void Load(Cdm.LobarPneumoniaExacerbationData src, SELobarPneumoniaExacerbation dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.LobarPneumoniaExacerbationData src, SELobarPneumoniaExacerbation dst)
  {
    Serialize(src.PatientAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
    if (src.LeftLungAffected != null)
      PBProperty.Load(src.LeftLungAffected, dst.GetLeftLungAffected());
    if (src.RightLungAffected != null)
      PBProperty.Load(src.RightLungAffected, dst.GetRightLungAffected());
  }
  public static Cdm.LobarPneumoniaExacerbationData Unload(SELobarPneumoniaExacerbation src)
  {
    Cdm.LobarPneumoniaExacerbationData dst = new Cdm.LobarPneumoniaExacerbationData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SELobarPneumoniaExacerbation src, Cdm.LobarPneumoniaExacerbationData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
    if (src.HasLeftLungAffected())
      dst.LeftLungAffected = PBProperty.Unload(src.GetLeftLungAffected());
    if (src.HasRightLungAffected())
      dst.RightLungAffected = PBProperty.Unload(src.GetRightLungAffected());
  }
  #endregion

  #region SENeedleDecompression
  public static void Load(Cdm.NeedleDecompressionData src, SENeedleDecompression dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.NeedleDecompressionData src, SENeedleDecompression dst)
  {
    Serialize(src.PatientAction, dst);
    dst.SetSide((eSide)(int)src.Side);
    dst.SetState((eSwitch)(int)src.State);
  }
  public static Cdm.NeedleDecompressionData Unload(SENeedleDecompression src)
  {
    Cdm.NeedleDecompressionData dst = new Cdm.NeedleDecompressionData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SENeedleDecompression src, Cdm.NeedleDecompressionData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    if (src.HasSide())
      dst.Side = (Cdm.eSide)(int)src.GetSide();
    dst.State = (Cdm.eSwitch)(int)src.GetState();
  }
  #endregion

  #region SEPericardialEffusion
  public static void Load(Cdm.PericardialEffusionData src, SEPericardialEffusion dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.PericardialEffusionData src, SEPericardialEffusion dst)
  {
    Serialize(src.PatientAction, dst);
    if (src.EffusionRate != null)
      PBProperty.Load(src.EffusionRate, dst.GetEffusionRate());
  }
  public static Cdm.PericardialEffusionData Unload(SEPericardialEffusion src)
  {
    Cdm.PericardialEffusionData dst = new Cdm.PericardialEffusionData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEPericardialEffusion src, Cdm.PericardialEffusionData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    if (src.HasEffusionRate())
      dst.EffusionRate = PBProperty.Unload(src.GetEffusionRate());
  }
  #endregion

  #region SERespiratoryFatigue
  public static void Load(Cdm.RespiratoryFatigueData src, SERespiratoryFatigue dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.RespiratoryFatigueData src, SERespiratoryFatigue dst)
  {
    Serialize(src.PatientAction, dst);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
  }
  public static Cdm.RespiratoryFatigueData Unload(SERespiratoryFatigue src)
  {
    Cdm.RespiratoryFatigueData dst = new Cdm.RespiratoryFatigueData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SERespiratoryFatigue src, Cdm.RespiratoryFatigueData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
  }
  #endregion

  #region SESubstanceBolus
  public static void Load(Cdm.SubstanceBolusData src, SESubstanceBolus dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.SubstanceBolusData src, SESubstanceBolus dst)
  {
    Serialize(src.PatientAction, dst);
    if (src.Substance != null)
      dst.SetSubstance(src.Substance);
    dst.SetAdminRoute((SESubstanceBolus.eAdministration)(int)src.AdministrationRoute);

    if (src.Concentration != null)
      PBProperty.Load(src.Concentration, dst.GetConcentration());
    if (src.Dose != null)
      PBProperty.Load(src.Dose, dst.GetDose());

  }
  public static Cdm.SubstanceBolusData Unload(SESubstanceBolus src)
  {
    Cdm.SubstanceBolusData dst = new Cdm.SubstanceBolusData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SESubstanceBolus src, Cdm.SubstanceBolusData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    if (src.HasSubstance())
      dst.Substance = src.GetSubstance();
    dst.AdministrationRoute = (Cdm.SubstanceBolusData.Types.eRoute)(int)src.GetAdminRoute();
    if (src.HasConcentration())
      dst.Concentration = PBProperty.Unload(src.GetConcentration());
    if (src.HasDose())
      dst.Dose = PBProperty.Unload(src.GetDose());
  }
  #endregion

  #region SESubstanceCompoundInfusion
  public static void Load(Cdm.SubstanceCompoundInfusionData src, SESubstanceCompoundInfusion dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.SubstanceCompoundInfusionData src, SESubstanceCompoundInfusion dst)
  {
    Serialize(src.PatientAction, dst);
    if (src.SubstanceCompound != null)
      dst.SetSubstanceCompound(src.SubstanceCompound);
    if (src.Rate != null)
      PBProperty.Load(src.Rate, dst.GetRate());
    if (src.BagVolume != null)
      PBProperty.Load(src.BagVolume, dst.GetBagVolume());
  }
  public static Cdm.SubstanceCompoundInfusionData Unload(SESubstanceCompoundInfusion src)
  {
    Cdm.SubstanceCompoundInfusionData dst = new Cdm.SubstanceCompoundInfusionData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SESubstanceCompoundInfusion src, Cdm.SubstanceCompoundInfusionData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
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
  public static void Load(Cdm.SubstanceInfusionData src, SESubstanceInfusion dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.SubstanceInfusionData src, SESubstanceInfusion dst)
  {
    Serialize(src.PatientAction, dst);
    if (src.Substance != null)
      dst.SetSubstance(src.Substance);
    if (src.Concentration != null)
      PBProperty.Load(src.Concentration, dst.GetConcentration());
    if (src.Rate != null)
      PBProperty.Load(src.Rate, dst.GetRate());
  }
  public static Cdm.SubstanceInfusionData Unload(SESubstanceInfusion src)
  {
    Cdm.SubstanceInfusionData dst = new Cdm.SubstanceInfusionData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SESubstanceInfusion src, Cdm.SubstanceInfusionData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    dst.Substance = src.GetSubstance();
    if (src.HasConcentration())
      dst.Concentration = PBProperty.Unload(src.GetConcentration());
    if (src.HasRate())
      dst.Rate = PBProperty.Unload(src.GetRate());
  }
  #endregion

  #region SESupplementalOxygen
  public static void Load(Cdm.SupplementalOxygenData src, SESupplementalOxygen dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.SupplementalOxygenData src, SESupplementalOxygen dst)
  {
    Serialize(src.PatientAction, dst);
    dst.SetDevice((SESupplementalOxygen.eDevice)(int)src.Device);
    if (src.Flow != null)
      PBProperty.Load(src.Flow, dst.GetFlow());
    if (src.Volume != null)
      PBProperty.Load(src.Volume, dst.GetVolume());
  }
  public static Cdm.SupplementalOxygenData Unload(SESupplementalOxygen src)
  {
    Cdm.SupplementalOxygenData dst = new Cdm.SupplementalOxygenData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SESupplementalOxygen src, Cdm.SupplementalOxygenData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    dst.Device = (Cdm.SupplementalOxygenData.Types.eDevice)(int)src.GetDevice();
    if (src.HasFlow())
      dst.Flow = PBProperty.Unload(src.GetFlow());
    if (src.HasVolume())
      dst.Volume = PBProperty.Unload(src.GetVolume());
  }
  #endregion

  #region SETensionPneumothorax
  public static void Load(Cdm.TensionPneumothoraxData src, SETensionPneumothorax dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.TensionPneumothoraxData src, SETensionPneumothorax dst)
  {
    Serialize(src.PatientAction, dst);
    dst.SetType((eGate)(int)src.Type);
    dst.SetSide((eSide)(int)src.Side);
    if (src.Severity != null)
      PBProperty.Load(src.Severity, dst.GetSeverity());
  }
  public static Cdm.TensionPneumothoraxData Unload(SETensionPneumothorax src)
  {
    Cdm.TensionPneumothoraxData dst = new Cdm.TensionPneumothoraxData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SETensionPneumothorax src, Cdm.TensionPneumothoraxData dst)
  {
    dst.PatientAction = new Cdm.PatientActionData();
    Serialize(src, dst.PatientAction);
    dst.Type = (Cdm.eGate)(int)src.GetType();
    dst.Side = (Cdm.eSide)(int)src.GetSide();
    if (src.HasSeverity())
      dst.Severity = PBProperty.Unload(src.GetSeverity());
  }
  #endregion

}
/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class PBPatientAction
{
    #region Any Patient Action
    /** Create a new action based on the binding object, load that data into the new action, and return said action */
    public static SEPatientAction Load(Cdm.AnyPatientActionData any)
    {
        if (any.AirwayObstruction != null)
        {
            SEAirwayObstruction ao = new SEAirwayObstruction();
            Serialize(any.AirwayObstruction,ao);
            return ao;
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
        if (any.NeedleDecompression != null)
        {
            SENeedleDecompression n = new SENeedleDecompression();
            Serialize(any.NeedleDecompression, n);
            return n;
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

        if (action.GetType().IsAssignableFrom(typeof(SEAirwayObstruction)))
        {
            any.AirwayObstruction = Unload((SEAirwayObstruction)action);
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
        if (action.GetType().IsAssignableFrom(typeof(SENeedleDecompression)))
        {
            any.NeedleDecompression = Unload((SENeedleDecompression)action);
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

    #region SEAirwayObstruction
    public static void Load(Cdm.AirwayObstructionData src, SEAirwayObstruction dst)
    {
        Serialize(src, dst);
    }
    public static void Serialize(Cdm.AirwayObstructionData src, SEAirwayObstruction dst)
    {
        Serialize(src.PatientAction, dst);
        if(src.Severity != null)
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
        dst.SetType((IntubationType)(int)src.Type);
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
        dst.Type = (Cdm.eIntubation.Types.Type)(int)src.GetType();
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
        dst.SetAdminRoute((eSubstanceAdministration)(int)src.AdministrationRoute);

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
        dst.AdministrationRoute = (Cdm.eSubstanceAdministration.Types.Route)(int)src.GetAdminRoute();
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
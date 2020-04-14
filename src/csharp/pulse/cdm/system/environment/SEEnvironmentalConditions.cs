/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;

namespace Pulse.CDM
{
    public class SEEnvironmentalConditions
    {
        public enum eSurroundingType : int
        {
            NullSurrounding = 0,
            Air,
            Water
        }
        protected eSurroundingType surroundingType;
        protected SEScalarMassPerVolume airDensity;
        protected SEScalarLengthPerTime airVelocity;
        protected SEScalarTemperature ambientTemperature;
        protected SEScalarPressure atmosphericPressure;
        protected SEScalarHeatResistanceArea clothingResistance;
        protected SEScalar0To1 emissivity;
        protected SEScalarTemperature meanRadiantTemperature;
        protected SEScalar0To1 relativeHumidity;
        protected SEScalarTemperature respirationAmbientTemperature;

        protected List<SESubstanceFraction> ambientGases;
        protected List<SESubstanceConcentration> ambientAerosols;


        public SEEnvironmentalConditions()
        {
            this.surroundingType = 0;

            this.airDensity = null;
            this.airVelocity = null;
            this.ambientTemperature = null;
            this.atmosphericPressure = null;
            this.clothingResistance = null;
            this.emissivity = null;
            this.meanRadiantTemperature = null;
            this.relativeHumidity = null;
            this.respirationAmbientTemperature = null;

            this.ambientGases = new List<SESubstanceFraction>();
            this.ambientAerosols = new List<SESubstanceConcentration>();

        }

        public void Reset()
        {
            surroundingType = 0;
            if (airDensity != null)
                airDensity.Invalidate();
            if (airVelocity != null)
                airVelocity.Invalidate();
            if (ambientTemperature != null)
                ambientTemperature.Invalidate();
            if (atmosphericPressure != null)
                atmosphericPressure.Invalidate();
            if (clothingResistance != null)
                clothingResistance.Invalidate();
            if (emissivity != null)
                emissivity.Invalidate();
            if (meanRadiantTemperature != null)
                meanRadiantTemperature.Invalidate();
            if (relativeHumidity != null)
                relativeHumidity.Invalidate();
            if (respirationAmbientTemperature != null)
                respirationAmbientTemperature.Invalidate();

            ambientGases.Clear();
            ambientAerosols.Clear();
        }

        public void Copy(SEEnvironmentalConditions from)
        {
            Reset();
            if (from.surroundingType != eSurroundingType.NullSurrounding)
                this.SetSurroundingType(from.surroundingType);
            if (from.HasAirDensity())
                this.GetAirDensity().Set(from.GetAirDensity());
            if (from.HasAirVelocity())
                this.GetAirVelocity().Set(from.GetAirVelocity());
            if (from.HasAmbientTemperature())
                this.GetAmbientTemperature().Set(from.GetAmbientTemperature());
            if (from.HasAtmosphericPressure())
                this.GetAtmosphericPressure().Set(from.GetAtmosphericPressure());
            if (from.HasClothingResistance())
                this.GetClothingResistance().Set(from.GetClothingResistance());
            if (from.HasEmissivity())
                this.GetEmissivity().Set(from.GetEmissivity());
            if (from.HasMeanRadiantTemperature())
                this.GetMeanRadiantTemperature().Set(from.GetMeanRadiantTemperature());
            if (from.HasRelativeHumidity())
                this.GetRelativeHumidity().Set(from.GetRelativeHumidity());
            if (from.HasRespirationAmbientTemperature())
                this.GetRespirationAmbientTemperature().Set(from.GetRespirationAmbientTemperature());

            if (from.ambientGases != null)
            {
                SESubstanceFraction mine;
                foreach (SESubstanceFraction sf in from.ambientGases)
                {
                    mine = this.CreateAmbientGas(sf.GetSubstance());
                    if (sf.HasFractionAmount())
                        mine.GetFractionAmount().Set(sf.GetFractionAmount());
                }
            }

            if (from.ambientAerosols != null)
            {
                SESubstanceConcentration mine;
                foreach (SESubstanceConcentration sc in from.ambientAerosols)
                {
                    mine = this.CreateAmbientAerosol(sc.GetSubstance());
                    if (sc.HasConcentration())
                        mine.GetConcentration().Set(sc.GetConcentration());
                }
            }
        }

  //      public void readFile(String fileName, SESubstanceManager mgr) throws InvalidProtocolBufferException
  //      {
  //          EnvironmentalConditionsData.Builder builder = EnvironmentalConditionsData.newBuilder();
  //  JsonFormat.parser().merge(FileUtils.readFile(fileName), builder);
  //      SEEnvironmentalConditions.load(builder.build(), this, mgr);
  //}
  //  public void writeFile(String fileName) throws InvalidProtocolBufferException
  //  {
  //      FileUtils.writeFile(fileName, JsonFormat.printer().print(SEEnvironmentalConditions.unload(this)));
    //}

    //public static void load(EnvironmentalConditionsData src, SEEnvironmentalConditions dst, SESubstanceManager substances)
    //{
    //    dst.reset();
    //    if (src.getSurroundingType() != eSurroundingType.UNRECOGNIZED)
    //        dst.setSurroundingType(src.getSurroundingType());
    //    if (src.hasAirDensity())
    //        SEScalarMassPerVolume.load(src.getAirDensity(), dst.getAirDensity());
    //    if (src.hasAirVelocity())
    //        SEScalarLengthPerTime.load(src.getAirVelocity(), dst.getAirVelocity());
    //    if (src.hasAmbientTemperature())
    //        SEScalarTemperature.load(src.getAmbientTemperature(), dst.getAmbientTemperature());
    //    if (src.hasAtmosphericPressure())
    //        SEScalarPressure.load(src.getAtmosphericPressure(), dst.getAtmosphericPressure());
    //    if (src.hasClothingResistance())
    //        SEScalarHeatResistanceArea.load(src.getClothingResistance(), dst.getClothingResistance());
    //    if (src.hasEmissivity())
    //        SEScalar0To1.load(src.getEmissivity(), dst.getEmissivity());
    //    if (src.hasMeanRadiantTemperature())
    //        SEScalarTemperature.load(src.getMeanRadiantTemperature(), dst.getMeanRadiantTemperature());
    //    if (src.hasRelativeHumidity())
    //        SEScalar0To1.load(src.getRelativeHumidity(), dst.getRelativeHumidity());
    //    if (src.hasRespirationAmbientTemperature())
    //        SEScalarTemperature.load(src.getRespirationAmbientTemperature(), dst.getRespirationAmbientTemperature());

    //    string sub;
    //    if (src.getAmbientGasList() != null)
    //    {
    //        for (SubstanceFractionData subData : src.getAmbientGasList())
    //        {
    //            sub = substances.getSubstance(subData.getName());
    //            if (sub == null)
    //            {
    //                Log.error("Substance does not exist for ambient gas : " + subData.getName());
    //                continue;
    //            }
    //            if (sub.getState() != eState.Gas)
    //            {
    //                Log.error("Environment Ambient Gas must be a gas, " + subData.getName() + " is not a gas...");
    //                continue;
    //            }
    //            SEScalar0To1.load(subData.getAmount(), dst.createAmbientGas(sub).getAmount());
    //        }
    //    }

    //    if (src.getAmbientAerosolList() != null)
    //    {
    //        for (SubstanceConcentrationData subData : src.getAmbientAerosolList())
    //        {
    //            sub = substances.getSubstance(subData.getName());
    //            if (sub == null)
    //            {
    //                Log.error("Substance does not exist for ambient aerosol : " + subData.getName());
    //                continue;
    //            }
    //            if (sub.getState() != eState.Solid && sub.getState() != eState.Liquid)
    //            {
    //                Log.error("Environment Ambient Aerosol must be a liquid or a gas, " + subData.getName() + " is neither...");
    //                continue;
    //            }
    //            SEScalarMassPerVolume.load(subData.getConcentration(), dst.createAmbientAerosol(sub).getConcentration());
    //        }
    //    }
    //}
    //public static EnvironmentalConditionsData unload(SEEnvironmentalConditions src)
    //{
    //    EnvironmentalConditionsData.Builder dst = EnvironmentalConditionsData.newBuilder();
    //    unload(src, dst);
    //    return dst.build();
    //}
    //protected static void unload(SEEnvironmentalConditions src, EnvironmentalConditionsData.Builder dst)
    //{
    //    if (src.hasSurroundingType())
    //        dst.setSurroundingType(src.surroundingType);
    //    if (src.hasAirDensity())
    //        dst.setAirDensity(SEScalarMassPerVolume.unload(src.airDensity));
    //    if (src.hasAirVelocity())
    //        dst.setAirVelocity(SEScalarLengthPerTime.unload(src.airVelocity));
    //    if (src.hasAmbientTemperature())
    //        dst.setAmbientTemperature(SEScalarTemperature.unload(src.ambientTemperature));
    //    if (src.hasAtmosphericPressure())
    //        dst.setAtmosphericPressure(SEScalarPressure.unload(src.atmosphericPressure));
    //    if (src.hasClothingResistance())
    //        dst.setClothingResistance(SEScalarHeatResistanceArea.unload(src.clothingResistance));
    //    if (src.hasEmissivity())
    //        dst.setEmissivity(SEScalar0To1.unload(src.emissivity));
    //    if (src.hasMeanRadiantTemperature())
    //        dst.setMeanRadiantTemperature(SEScalarTemperature.unload(src.meanRadiantTemperature));
    //    if (src.hasRelativeHumidity())
    //        dst.setRelativeHumidity(SEScalar0To1.unload(src.relativeHumidity));
    //    if (src.hasRespirationAmbientTemperature())
    //        dst.setRespirationAmbientTemperature(SEScalarTemperature.unload(src.respirationAmbientTemperature));

    //    for (SESubstanceFraction ambSub : src.ambientGases)
    //        dst.addAmbientGas(SESubstanceFraction.unload(ambSub));

    //    for (SESubstanceConcentration ambSub : src.ambientAerosols)
    //        dst.addAmbientAerosol(SESubstanceConcentration.unload(ambSub));
    //}

    public bool HasSurroundingType()
    {
        return surroundingType != 0;
    }
    public eSurroundingType GetSurroundingType()
    {
        return surroundingType;
    }
    public void SetSurroundingType(eSurroundingType st)
    {
        surroundingType = st;
    }
    public SEScalarMassPerVolume GetAirDensity()
    {
        if (airDensity == null)
            airDensity = new SEScalarMassPerVolume();
        return airDensity;
    }
    public bool HasAirDensity()
    {
        return airDensity == null ? false : airDensity.IsValid();
    }

    public SEScalarLengthPerTime GetAirVelocity()
    {
        if (airVelocity == null)
            airVelocity = new SEScalarLengthPerTime();
        return airVelocity;
    }
    public bool HasAirVelocity()
    {
        return airVelocity == null ? false : airVelocity.IsValid();
    }

    public SEScalarTemperature GetAmbientTemperature()
    {
        if (ambientTemperature == null)
            ambientTemperature = new SEScalarTemperature();
        return ambientTemperature;
    }
    public bool HasAmbientTemperature()
    {
        return ambientTemperature == null ? false : ambientTemperature.IsValid();
    }

    public SEScalarPressure GetAtmosphericPressure()
    {
        if (atmosphericPressure == null)
            atmosphericPressure = new SEScalarPressure();
        return atmosphericPressure;
    }
    public bool HasAtmosphericPressure()
    {
        return atmosphericPressure == null ? false : atmosphericPressure.IsValid();
    }

    public SEScalarHeatResistanceArea GetClothingResistance()
    {
        if (clothingResistance == null)
            clothingResistance = new SEScalarHeatResistanceArea();
        return clothingResistance;
    }
    public bool HasClothingResistance()
    {
        return clothingResistance == null ? false : clothingResistance.IsValid();
    }

    public SEScalar0To1 GetEmissivity()
    {
        if (emissivity == null)
            emissivity = new SEScalar0To1();
        return emissivity;
    }
    public bool HasEmissivity()
    {
        return emissivity == null ? false : emissivity.IsValid();
    }

    public SEScalarTemperature GetMeanRadiantTemperature()
    {
        if (meanRadiantTemperature == null)
            meanRadiantTemperature = new SEScalarTemperature();
        return meanRadiantTemperature;
    }
    public bool HasMeanRadiantTemperature()
    {
        return meanRadiantTemperature == null ? false : meanRadiantTemperature.IsValid();
    }

    public SEScalar0To1 GetRelativeHumidity()
    {
        if (relativeHumidity == null)
            relativeHumidity = new SEScalar0To1();
        return relativeHumidity;
    }
    public bool HasRelativeHumidity()
    {
        return relativeHumidity == null ? false : relativeHumidity.IsValid();
    }

    public SEScalarTemperature GetRespirationAmbientTemperature()
    {
        if (respirationAmbientTemperature == null)
            respirationAmbientTemperature = new SEScalarTemperature();
        return respirationAmbientTemperature;
    }
    public bool HasRespirationAmbientTemperature()
    {
        return respirationAmbientTemperature == null ? false : respirationAmbientTemperature.IsValid();
    }

    public SESubstanceFraction CreateAmbientGas(string substance)
    {
        return GetAmbientGas(substance);
    }
    public SESubstanceFraction GetAmbientGas(string substance)
    {
        foreach (SESubstanceFraction sf in ambientGases)
        {
            if (sf.GetSubstance() == substance)
            {
                return sf;
            }
        }
        SESubstanceFraction nsf = new SESubstanceFraction(substance);
        ambientGases.Add(nsf);
        return nsf;
    }
    public bool HasAmbientGas()
    {
        return ambientGases.Count != 0;
    }
    public bool HasAmbientGas(string substance)
    {
        foreach (SESubstanceFraction sf in ambientGases)
        {
            if (sf.GetSubstance() == substance)
            {
                return true;
            }
        }
        return false;
    }
    public List<SESubstanceFraction> GetAmbientGas()
    {
        return ambientGases;
    }
    public void RemoveAmbientGas(string substance)
    {
        foreach (SESubstanceFraction sf in ambientGases)
        {
            if (sf.GetSubstance() == substance)
            {
                ambientGases.Remove(sf);
                return;
            }
        }
    }


    public SESubstanceConcentration CreateAmbientAerosol(string substance)
    {
        return GetAmbientAerosol(substance);
    }
    public SESubstanceConcentration GetAmbientAerosol(string substance)
    {
        foreach (SESubstanceConcentration sc in ambientAerosols)
        {
            if (sc.GetSubstance()== substance)
            {
                return sc;
            }
        }
        SESubstanceConcentration nsc = new SESubstanceConcentration(substance);
        ambientAerosols.Add(nsc);
        return nsc;
    }
    public bool HasAmbientAerosol()
    {
        return ambientAerosols.Count != 0;
        }
    public bool HasAmbientAerosol(string substance)
    {
        foreach (SESubstanceConcentration sc in ambientAerosols)
        {
            if (sc.GetSubstance() == substance)
            {
                return true;
            }
        }
        return false;
    }
    public List<SESubstanceConcentration> GetAmbientAerosol()
    {
        return ambientAerosols;
    }
    public void RemoveAmbientAerosol(string substance)
    {
        foreach (SESubstanceConcentration sc in ambientAerosols)
        {
            if (sc.GetSubstance() == substance)
            {
                ambientAerosols.Remove(sc);
                return;
            }
        }
    }


    //public void trim()
    //{
    //    SESubstanceConcentration sc;
    //    Iterator<SESubstanceConcentration> icon = this.ambientAerosols.iterator();
    //    while (icon.hasNext())
    //    {
    //        sc = icon.next();
    //        if (sc.getConcentration().getValue() == 0)
    //            icon.remove();
    //    }

    //    SESubstanceFraction sf;
    //    Iterator<SESubstanceFraction> ifrac = this.ambientGases.iterator();
    //    while (ifrac.hasNext())
    //    {
    //        sf = ifrac.next();
    //        if (sf.getAmount().getValue() == 0)
    //            ifrac.remove();
    //    }
    //}

    public new string ToString()
    {
        string str = "Envriomental Conditions:"
            + "\n\tSurroundingType: " + GetSurroundingType()
            + "\n\tAirDensity: " + (HasAirDensity() ? GetAirDensity().ToString() : "None")
            + "\n\tAirVelocity: " + (HasAirVelocity() ? GetAirVelocity().ToString() : "None")
            + "\n\tAmbientTemperature: " + (HasAmbientTemperature() ? GetAmbientTemperature().ToString() : "None")
            + "\n\tAtmosphericPressure: " + (HasAtmosphericPressure() ? GetAtmosphericPressure().ToString() : "None")
            + "\n\tClothingResistance: " + (HasClothingResistance() ? GetClothingResistance().ToString() : "None")
            + "\n\tEmissivity: " + (HasEmissivity() ? GetEmissivity().ToString() : "None")
            + "\n\tMeanRadiantTemperature: " + (HasMeanRadiantTemperature() ? GetMeanRadiantTemperature().ToString() : "None")
            + "\n\tRelativeHumidity: " + (HasRelativeHumidity() ? GetRelativeHumidity().ToString() : "None")
            + "\n\tRespirationAmbientTemperature: " + (HasRespirationAmbientTemperature() ? GetRespirationAmbientTemperature().ToString() : "None");
        foreach (SESubstanceFraction sf in this.ambientGases)
            str += "\n\t" + sf.GetSubstance();
        foreach (SESubstanceConcentration sc in this.ambientAerosols)
            str += "\n\t" + sc.GetSubstance();

        return str;
    }


}
}



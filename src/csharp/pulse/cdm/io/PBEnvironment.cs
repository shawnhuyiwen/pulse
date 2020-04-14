/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;
using Google.Protobuf;

namespace Pulse.CDM
{
    public class PBEnvironment
    {
        // Java has Builder, new Builder. Not sure what that is.

        // One Serialize Exception
        #region SEActiveConditioning
        public static void Load(pulse.cdm.bind.ActiveConditioningData src, SEActiveConditioning dst)
        {
            Serialize(src, dst);
        }
        public static pulse.cdm.bind.ActiveConditioningData Unload(SEActiveConditioning src)
        {
            pulse.cdm.bind.ActiveConditioningData dst = new pulse.cdm.bind.ActiveConditioningData();
            Serialize(src, dst);
            return dst;
        }

        public static void Serialize(SEActiveConditioning src, pulse.cdm.bind.ActiveConditioningData dst)
        {
            if (src.HasPower())
                dst.Power = PBProperty.Unload(src.GetPower());
            if (src.HasSurfaceArea())
                dst.SurfaceArea = PBProperty.Unload(src.GetSurfaceArea());
            if (src.HasSurfaceAreaFraction())
                dst.SurfaceAreaFraction = PBProperty.Unload(src.GetSurfaceAreaFraction());
        }

        public static void Serialize(pulse.cdm.bind.ActiveConditioningData src, SEActiveConditioning dst)
        {
            throw new KeyNotFoundException("serialize_active_conditioning_from_bind not implemented");
        }
        #endregion


        // One Serialize Exception

        #region SEAppliedTemperature
        public static void Load(pulse.cdm.bind.AppliedTemperatureData src, SEAppliedTemperature dst)
        {
            Serialize(src, dst);
        }
        public static pulse.cdm.bind.AppliedTemperatureData Unload(SEAppliedTemperature src)
        {
            pulse.cdm.bind.AppliedTemperatureData dst = new pulse.cdm.bind.AppliedTemperatureData();
            Serialize(src, dst);
            return dst;
        }
        
        public static void Serialize(SEAppliedTemperature src, pulse.cdm.bind.AppliedTemperatureData dst)
        {
            if (src.HasTemperature())
                dst.Temperature = PBProperty.Unload(src.GetTemperature());
            if (src.HasSurfaceArea())
                dst.SurfaceArea = PBProperty.Unload(src.GetSurfaceArea());
            if (src.HasSurfaceAreaFraction())
                dst.SurfaceAreaFraction = PBProperty.Unload(src.GetSurfaceAreaFraction());
        }

        public static void Serialize(pulse.cdm.bind.AppliedTemperatureData src, SEAppliedTemperature dst)
        {
            throw new KeyNotFoundException("serialize_active_conditioning_from_bind not implemented");
        }

        #endregion



        // Nothing about it in Python API. Add it based on C++ API and Java API. One Serialize Exception
        #region SEEnvironment
        public static void Load(pulse.cdm.bind.EnvironmentData src, SEEnvironment dst)
        {
            Serialize(src, dst);
        }
        public static pulse.cdm.bind.EnvironmentData Unload(SEEnvironment src)
        {
            pulse.cdm.bind.EnvironmentData dst = new pulse.cdm.bind.EnvironmentData();
            Serialize(src, dst);
            return dst;
        }

        public static void Serialize(SEEnvironment src, pulse.cdm.bind.EnvironmentData dst)
        {
            if (src.HasConvectiveHeatLoss())
                dst.ConvectiveHeatLoss = PBProperty.Unload(src.GetConvectiveHeatLoss());
            if (src.HasConvectiveHeatTranferCoefficient())
                dst.ConvectiveHeatTranferCoefficient = PBProperty.Unload(src.GetConvectiveHeatTranferCoefficient());           
            if (src.HasEvaporativeHeatLoss())
                dst.EvaporativeHeatLoss = PBProperty.Unload(src.GetEvaporativeHeatLoss());
            if (src.HasEvaporativeHeatTranferCoefficient())
                dst.EvaporativeHeatTranferCoefficient = PBProperty.Unload(src.GetEvaporativeHeatTranferCoefficient());
            if (src.HasRadiativeHeatLoss())
                dst.RadiativeHeatLoss = PBProperty.Unload(src.GetRadiativeHeatLoss());
            if (src.HasRadiativeHeatTranferCoefficient())
                dst.RadiativeHeatTranferCoefficient = PBProperty.Unload(src.GetRadiativeHeatTranferCoefficient());
            if (src.HasRespirationHeatLoss())
                dst.RespirationHeatLoss = PBProperty.Unload(src.GetRespirationHeatLoss());
            if (src.HasSkinHeatLoss())
                dst.SkinHeatLoss = PBProperty.Unload(src.GetSkinHeatLoss());

            if (src.HasEnvironmentalConditions())
                dst.Conditions = PBEnvironment.Unload(src.GetEnvironmentalConditions());
            if (src.HasActiveHeating())
                dst.ActiveHeating = PBEnvironment.Unload(src.GetActiveHeating());
            if (src.HasActiveCooling())
                dst.ActiveCooling = PBEnvironment.Unload(src.GetActiveCooling());
            if (src.HasAppliedTemperature())
                dst.AppliedTemperature = PBEnvironment.Unload(src.GetAppliedTemperature());
        }

        public static void Serialize(pulse.cdm.bind.EnvironmentData src, SEEnvironment dst)
        {
            throw new KeyNotFoundException("serialize_active_conditioning_from_bind not implemented");
        }

        #endregion


        // No SerilizeFromFile and SerilizeToFile

        #region SEEnvironmentalConditions
        public static void Load(pulse.cdm.bind.EnvironmentalConditionsData src, SEEnvironmentalConditions dst)
        {
            Serialize(src, dst);
        }
        public static pulse.cdm.bind.EnvironmentalConditionsData Unload(SEEnvironmentalConditions src)
        {
            pulse.cdm.bind.EnvironmentalConditionsData dst = new pulse.cdm.bind.EnvironmentalConditionsData();
            Serialize(src, dst);
            return dst;
        }


        public static void Serialize(pulse.cdm.bind.EnvironmentalConditionsData src, SEEnvironmentalConditions dst)
        {
            dst.SetSurroundingType((SEEnvironmentalConditions.eSurroundingType)(int)src.SurroundingType);
            if (src.AirDensity != null)
                PBProperty.Load(src.AirDensity, dst.GetAirDensity());
            if (src.AirVelocity != null)
                PBProperty.Load(src.AirVelocity, dst.GetAirVelocity());
            if (src.AmbientTemperature != null)
                PBProperty.Load(src.AmbientTemperature, dst.GetAmbientTemperature());
            if (src.AtmosphericPressure != null)
                PBProperty.Load(src.AtmosphericPressure, dst.GetAtmosphericPressure());
            if (src.ClothingResistance != null)
                PBProperty.Load(src.ClothingResistance, dst.GetClothingResistance());
            if (src.Emissivity != null)
                PBProperty.Load(src.Emissivity, dst.GetEmissivity());
            if (src.MeanRadiantTemperature != null)
                PBProperty.Load(src.MeanRadiantTemperature, dst.GetMeanRadiantTemperature());
            if (src.RelativeHumidity != null)
                PBProperty.Load(src.RelativeHumidity, dst.GetRelativeHumidity());
            if (src.RespirationAmbientTemperature != null)
                PBProperty.Load(src.RespirationAmbientTemperature, dst.GetRespirationAmbientTemperature());

            foreach (pulse.cdm.bind.SubstanceFractionData sf in src.AmbientGas)
            {
                PBProperty.Load(sf.Amount, dst.GetAmbientGas(sf.Name).GetFractionAmount());
            }
            
            foreach (pulse.cdm.bind.SubstanceConcentrationData sc in src.AmbientAerosol)
            {
                PBProperty.Load(sc.Concentration, dst.GetAmbientAerosol(sc.Name).GetConcentration());
            }
        }

        public static void Serialize(SEEnvironmentalConditions src, pulse.cdm.bind.EnvironmentalConditionsData dst)
        {
            dst.SurroundingType = (pulse.cdm.bind.EnvironmentalConditionsData.Types.eSurroundingType)(int)src.GetSurroundingType();
            if (src.HasAirDensity())
                dst.AirDensity = PBProperty.Unload(src.GetAirDensity());
            if (src.HasAirVelocity())
                dst.AirVelocity = PBProperty.Unload(src.GetAirVelocity());
            if (src.HasAmbientTemperature())
                dst.AmbientTemperature = PBProperty.Unload(src.GetAmbientTemperature());
            if (src.HasAtmosphericPressure())
                dst.AtmosphericPressure = PBProperty.Unload(src.GetAtmosphericPressure());
            if (src.HasClothingResistance())
                dst.ClothingResistance = PBProperty.Unload(src.GetClothingResistance());
            if (src.HasEmissivity())
                dst.Emissivity = PBProperty.Unload(src.GetEmissivity());
            if (src.HasMeanRadiantTemperature())
                dst.MeanRadiantTemperature = PBProperty.Unload(src.GetMeanRadiantTemperature());
            if (src.HasRelativeHumidity())
                dst.RelativeHumidity = PBProperty.Unload(src.GetRelativeHumidity());
            if (src.HasRespirationAmbientTemperature())
                dst.RespirationAmbientTemperature = PBProperty.Unload(src.GetRespirationAmbientTemperature());


            foreach (SESubstanceFraction aGas in src.GetAmbientGas())
            {
                pulse.cdm.bind.SubstanceFractionData sf = new pulse.cdm.bind.SubstanceFractionData();
                sf.Name = aGas.GetSubstance();
                sf.Amount = PBProperty.Unload(aGas.GetFractionAmount());
                //PBProperty.Load(aGas.GetFractionAmount(), sf.Amount);
                dst.AmbientGas.Add(sf);
            }

            foreach (SESubstanceConcentration aAerosol in src.GetAmbientAerosol())
            {
                pulse.cdm.bind.SubstanceConcentrationData sc = new pulse.cdm.bind.SubstanceConcentrationData();
                sc.Name = aAerosol.GetSubstance();
                sc.Concentration = PBProperty.Unload(aAerosol.GetConcentration());
                //PBProperty.Load(aAerosol.GetConcentration(), sc.Amount);
                dst.AmbientAerosol.Add(sc);
            }

        }

        public static bool SerilizeFromString(string src, SEEnvironmentalConditions dst)
        {
            try
            {
                pulse.cdm.bind.EnvironmentalConditionsData data = JsonParser.Default.Parse<pulse.cdm.bind.EnvironmentalConditionsData>(src);
                PBEnvironment.Load(data, dst);
            }
            catch (Google.Protobuf.InvalidJsonException)
            {
                return false;
            }
            return true;
        }

        public static string SerilizeToString(SEEnvironmentalConditions src, string output)
        {
            var pb = PBEnvironment.Unload(src);
            return pb.ToString();
        }

        //public static void SerilizeFromFile(string filename, SEEnvironmentalConditions dst)
        //{

        //}

        //public static void SerilizeToFile(SEEnvironmentalConditions src, string filename)
        //{

        //}

        #endregion
                     


    }
                
}

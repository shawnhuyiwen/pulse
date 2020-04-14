/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
    public class SEEnvironment
    {

        protected SEScalarPower convectiveHeatLoss;
        protected SEScalarHeatConductancePerArea convectiveHeatTranferCoefficient;
        protected SEScalarPower evaporativeHeatLoss;
        protected SEScalarHeatConductancePerArea evaporativeHeatTranferCoefficient;
        protected SEScalarPower radiativeHeatLoss;
        protected SEScalarHeatConductancePerArea radiativeHeatTranferCoefficient;
        protected SEScalarPower respirationHeatLoss;
        protected SEScalarPower skinHeatLoss;

        protected SEEnvironmentalConditions environmentalConditions;
        protected SEActiveConditioning activeHeating;
        protected SEActiveConditioning activeCooling;
        protected SEAppliedTemperature appliedTemperature;

        public SEEnvironment()
        {
            this.convectiveHeatLoss = null;
            this.convectiveHeatTranferCoefficient = null;
            this.evaporativeHeatLoss = null;
            this.evaporativeHeatTranferCoefficient = null;
            this.radiativeHeatLoss = null;
            this.radiativeHeatTranferCoefficient = null;
            this.respirationHeatLoss = null;
            this.skinHeatLoss = null;

            this.environmentalConditions = null;
            activeHeating = null;
            activeCooling = null;
            appliedTemperature = null;
        }

        public void Reset()
        {
            if (convectiveHeatLoss != null)
                convectiveHeatLoss.Invalidate();
            if (convectiveHeatTranferCoefficient != null)
                convectiveHeatTranferCoefficient.Invalidate();
            if (evaporativeHeatLoss != null)
                evaporativeHeatLoss.Invalidate();
            if (evaporativeHeatTranferCoefficient != null)
                evaporativeHeatTranferCoefficient.Invalidate();
            if (radiativeHeatLoss != null)
                radiativeHeatLoss.Invalidate();
            if (radiativeHeatTranferCoefficient != null)
                radiativeHeatTranferCoefficient.Invalidate();
            if (respirationHeatLoss != null)
                respirationHeatLoss.Invalidate();
            if (skinHeatLoss != null)
                skinHeatLoss.Invalidate();
            if (environmentalConditions != null)
                environmentalConditions.Reset();
            if (activeHeating != null)
                activeHeating.Reset();
            if (activeCooling != null)
                activeCooling.Reset();
            if (appliedTemperature != null)
                appliedTemperature.Reset();
        }

        //public static void Load(EnvironmentData src, SEEnvironment dst, SESubstanceManager subMgr)
        //{
        //    dst.Reset();
        //    if (src.hasConvectiveHeatLoss())
        //        SEScalarPower.Load(src.getConvectiveHeatLoss(), dst.GetConvectiveHeatLoss());
        //    if (src.hasConvectiveHeatTranferCoefficient())
        //        SEScalarHeatConductancePerArea.Load(src.getConvectiveHeatTranferCoefficient(), dst.GetConvectiveHeatTranferCoefficient());
        //    if (src.hasEvaporativeHeatLoss())
        //        SEScalarPower.Load(src.getEvaporativeHeatLoss(), dst.getEvaporativeHeatLoss());
        //    if (src.hasEvaporativeHeatTranferCoefficient())
        //        SEScalarHeatConductancePerArea.Load(src.getEvaporativeHeatTranferCoefficient(), dst.getEvaporativeHeatTranferCoefficient());
        //    if (src.hasRadiativeHeatLoss())
        //        SEScalarPower.Load(src.getRadiativeHeatLoss(), dst.getRadiativeHeatLoss());
        //    if (src.hasRadiativeHeatTranferCoefficient())
        //        SEScalarHeatConductancePerArea.Load(src.getRadiativeHeatTranferCoefficient(), dst.getRadiativeHeatTranferCoefficient());
        //    if (src.hasRespirationHeatLoss())
        //        SEScalarPower.Load(src.getRespirationHeatLoss(), dst.getRespirationHeatLoss());
        //    if (src.hasSkinHeatLoss())
        //        SEScalarPower.Load(src.getSkinHeatLoss(), dst.getSkinHeatLoss());

        //    if (src.hasEnvironmentalConditions())
        //        SEEnvironmentalConditions.Load(src.getEnvironmentalConditions(), dst.getEnvironmentalConditions(), subMgr);
        //    if (src.hasActiveHeating())
        //        SEActiveConditioning.Load(src.getActiveHeating(), dst.getActiveHeating());
        //    if (src.hasActiveCooling())
        //        SEActiveConditioning.Load(src.getActiveCooling(), dst.getActiveCooling());
        //    if (src.hasAppliedTemperature())
        //        SEAppliedTemperature.Load(src.getAppliedTemperature(), dst.getAppliedTemperature());
        //}
        //public static EnvironmentData Unload(SEEnvironment src)
        //{
        //    EnvironmentData.Builder dst = EnvironmentData.newBuilder();
        //    Unload(src, dst);
        //    return dst.build();
        //}
        //protected static void Unload(SEEnvironment src, EnvironmentData.Builder dst)
        //{
        //    if (src.HasConvectiveHeatLoss())
        //        dst.SetConvectiveHeatLoss(SEScalarPower.Unload(src.convectiveHeatLoss));
        //    if (src.HasConvectiveHeatTranferCoefficient())
        //        dst.SetConvectiveHeatTranferCoefficient(SEScalarHeatConductancePerArea.Unload(src.convectiveHeatTranferCoefficient));
        //    if (src.HasEvaporativeHeatLoss())
        //        dst.SetEvaporativeHeatLoss(SEScalarPower.Unload(src.evaporativeHeatLoss));
        //    if (src.HasEvaporativeHeatTranferCoefficient())
        //        dst.SetEvaporativeHeatTranferCoefficient(SEScalarHeatConductancePerArea.Unload(src.evaporativeHeatTranferCoefficient));
        //    if (src.HasRadiativeHeatLoss())
        //        dst.SetRadiativeHeatLoss(SEScalarPower.Unload(src.radiativeHeatLoss));
        //    if (src.HasRadiativeHeatTranferCoefficient())
        //        dst.SetRadiativeHeatTranferCoefficient(SEScalarHeatConductancePerArea.Unload(src.radiativeHeatTranferCoefficient));
        //    if (src.HasRespirationHeatLoss())
        //        dst.SetRespirationHeatLoss(SEScalarPower.Unload(src.respirationHeatLoss));
        //    if (src.HasSkinHeatLoss())
        //        dst.SetSkinHeatLoss(SEScalarPower.Unload(src.skinHeatLoss));

        //    if (src.HasEnvironmentalConditions())
        //        dst.SetEnvironmentalConditions(SEEnvironmentalConditions.Unload(src.environmentalConditions));
        //    if (src.HasActiveHeating())
        //        dst.SetActiveHeating(SEActiveConditioning.Unload(src.activeHeating));
        //    if (src.HasActiveCooling())
        //        dst.SetActiveCooling(SEActiveConditioning.Unload(src.activeCooling));
        //    if (src.HasAppliedTemperature())
        //        dst.SetAppliedTemperature(SEAppliedTemperature.Unload(src.appliedTemperature));
        //}

        public SEScalarPower GetConvectiveHeatLoss()
        {
            if (convectiveHeatLoss == null)
                convectiveHeatLoss = new SEScalarPower();
            return convectiveHeatLoss;
        }
        public bool HasConvectiveHeatLoss()
        {
            return convectiveHeatLoss == null ? false : convectiveHeatLoss.IsValid();
        }

        public SEScalarHeatConductancePerArea GetConvectiveHeatTranferCoefficient()
        {
            if (convectiveHeatTranferCoefficient == null)
                convectiveHeatTranferCoefficient = new SEScalarHeatConductancePerArea();
            return convectiveHeatTranferCoefficient;
        }
        public bool HasConvectiveHeatTranferCoefficient()
        {
            return convectiveHeatTranferCoefficient == null ? false : convectiveHeatTranferCoefficient.IsValid();
        }

        public SEScalarPower GetEvaporativeHeatLoss()
        {
            if (evaporativeHeatLoss == null)
                evaporativeHeatLoss = new SEScalarPower();
            return evaporativeHeatLoss;
        }
        public bool HasEvaporativeHeatLoss()
        {
            return evaporativeHeatLoss == null ? false : evaporativeHeatLoss.IsValid();
        }

        public SEScalarHeatConductancePerArea GetEvaporativeHeatTranferCoefficient()
        {
            if (evaporativeHeatTranferCoefficient == null)
                evaporativeHeatTranferCoefficient = new SEScalarHeatConductancePerArea();
            return evaporativeHeatTranferCoefficient;
        }
        public bool HasEvaporativeHeatTranferCoefficient()
        {
            return evaporativeHeatTranferCoefficient == null ? false : evaporativeHeatTranferCoefficient.IsValid();
        }

        public SEScalarPower GetRadiativeHeatLoss()
        {
            if (radiativeHeatLoss == null)
                radiativeHeatLoss = new SEScalarPower();
            return radiativeHeatLoss;
        }
        public bool HasRadiativeHeatLoss()
        {
            return radiativeHeatLoss == null ? false : radiativeHeatLoss.IsValid();
        }

        public SEScalarHeatConductancePerArea GetRadiativeHeatTranferCoefficient()
        {
            if (radiativeHeatTranferCoefficient == null)
                radiativeHeatTranferCoefficient = new SEScalarHeatConductancePerArea();
            return radiativeHeatTranferCoefficient;
        }
        public bool HasRadiativeHeatTranferCoefficient()
        {
            return radiativeHeatTranferCoefficient == null ? false : radiativeHeatTranferCoefficient.IsValid();
        }

        public SEScalarPower GetRespirationHeatLoss()
        {
            if (respirationHeatLoss == null)
                respirationHeatLoss = new SEScalarPower();
            return respirationHeatLoss;
        }
        public bool HasRespirationHeatLoss()
        {
            return respirationHeatLoss == null ? false : respirationHeatLoss.IsValid();
        }

        public SEScalarPower GetSkinHeatLoss()
        {
            if (skinHeatLoss == null)
                skinHeatLoss = new SEScalarPower();
            return skinHeatLoss;
        }
        public bool HasSkinHeatLoss()
        {
            return skinHeatLoss == null ? false : skinHeatLoss.IsValid();
        }

        public SEEnvironmentalConditions GetEnvironmentalConditions()
        {
            if (environmentalConditions == null)
                environmentalConditions = new SEEnvironmentalConditions();
            return environmentalConditions;
        }
        public bool HasEnvironmentalConditions()
        {
            return environmentalConditions != null;
        }

        public bool HasActiveHeating()
        {
            return activeHeating != null;
        }
        public SEActiveConditioning GetActiveHeating()
        {
            if (activeHeating == null)
                activeHeating = new SEActiveConditioning();
            return activeHeating;
        }

        public bool HasActiveCooling()
        {
            return activeCooling != null;
        }
        public SEActiveConditioning GetActiveCooling()
        {
            if (activeCooling == null)
                activeCooling = new SEActiveConditioning();
            return activeCooling;
        }

        public bool HasAppliedTemperature()
        {
            return appliedTemperature != null;
        }
        public SEAppliedTemperature GetAppliedTemperature()
        {
            if (appliedTemperature == null)
                appliedTemperature = new SEAppliedTemperature();
            return appliedTemperature;
        }
    }
}


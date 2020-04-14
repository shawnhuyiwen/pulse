/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
    public class SEThermalApplication : SEEnvironmentAction
    {
        protected SEActiveConditioning activeHeating;
        protected SEActiveConditioning activeCooling;
        protected SEAppliedTemperature appliedTemperature;
        protected bool clearContents;

        public SEThermalApplication()
        {
            clearContents = true;
            activeHeating = null;
            activeCooling = null;
            appliedTemperature = null;
        }

        public override void Clear()
        {
            base.Clear();
            if (clearContents)
            {
                activeHeating = null;
                activeCooling = null;
                appliedTemperature = null;
            }
        }


        public void Copy(SEThermalApplication other)
        {
            if (this == other)
                return;

            clearContents = other.clearContents;
            if (other.activeHeating != null)
                GetActiveHeating().Copy(other.GetActiveHeating());
            else if (activeHeating != null)
                activeHeating.Reset();
            if (other.activeCooling != null)
                GetActiveCooling().Copy(other.GetActiveCooling());
            else if (activeCooling != null)
                activeCooling.Reset();
            if (other.appliedTemperature != null)
                GetAppliedTemperature().Copy(other.GetAppliedTemperature());
            else if (appliedTemperature != null)
                appliedTemperature.Reset();
        }

        public void Reset()
        {

            clearContents = true;
            if (activeHeating != null)
                activeHeating.Reset();
            if (activeCooling != null)
                activeCooling.Reset();
            if (appliedTemperature != null)
                appliedTemperature.Reset();
        }

        public override bool IsValid()
        {
            return HasActiveCooling() || HasActiveHeating() || HasAppliedTemperature();
        }

        //public static void load(ThermalApplicationData src, SEThermalApplication dst)
        //{
        //    SEEnvironmentAction.load(src.getEnvironmentAction(), dst);
        //    if (src.getAppendToPrevious())
        //        dst.clearContents = src.getAppendToPrevious();
        //    if (src.hasActiveHeating())
        //        SEActiveConditioning.load(src.getActiveHeating(), dst.getActiveHeating());
        //    if (src.hasActiveCooling())
        //        SEActiveConditioning.load(src.getActiveCooling(), dst.getActiveCooling());
        //    if (src.hasAppliedTemperature())
        //        SEAppliedTemperature.load(src.getAppliedTemperature(), dst.getAppliedTemperature());
        //}
        //public static ThermalApplicationData unload(SEThermalApplication src)
        //{
        //    ThermalApplicationData.Builder dst = ThermalApplicationData.newBuilder();
        //    unload(src, dst);
        //    return dst.build();
        //}
        //protected static void unload(SEThermalApplication src, ThermalApplicationData.Builder dst)
        //{
        //    SEEnvironmentAction.unload(src, dst.getEnvironmentActionBuilder());
        //    if (src.clearContents != null)
        //        dst.setAppendToPrevious(src.clearContents);
        //    if (src.hasActiveHeating())
        //        dst.setActiveHeating(SEActiveConditioning.unload(src.activeHeating));
        //    if (src.hasActiveCooling())
        //        dst.setActiveCooling(SEActiveConditioning.unload(src.activeCooling));
        //    if (src.hasAppliedTemperature())
        //        dst.setAppliedTemperature(SEAppliedTemperature.unload(src.appliedTemperature));
        //}

        public bool SetState(bool t)
        {
            clearContents = t;
            return clearContents;
                    }
        public bool GetClearContents() { return clearContents; }

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

        public new string ToString()
        {
            return "Thermal Application:"
                + (HasActiveHeating() ? GetActiveHeating().ToString() : "No Heating")
                + (HasActiveCooling() ? GetActiveCooling().ToString() : "No Cooling")
                + (HasAppliedTemperature() ? GetAppliedTemperature().ToString() : "No Applied Temperature");
        }
    }
}



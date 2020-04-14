/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
namespace Pulse.CDM
{
    public class SEAppliedTemperature
    {
        protected SEScalarTemperature temperature;
        protected SEScalarArea surfaceArea;
        protected SEScalar0To1 surfaceAreaFraction;
        protected eSwitch state;

        public SEAppliedTemperature()
        {
            temperature = null;
            surfaceArea = null;
            surfaceAreaFraction = null;
            state = eSwitch.Off;
        }

        public void Copy(SEAppliedTemperature other)
        {
            if (this == other)
                return;
            state = other.state;
            if (other.temperature != null)
                GetTemperature().Set(other.GetTemperature());
            else if (temperature != null)
                temperature.Invalidate();
            if (other.surfaceArea != null)
                GetSurfaceArea().Set(other.GetSurfaceArea());
            else if (surfaceArea != null)
                surfaceArea.Invalidate();
            if (other.surfaceAreaFraction != null)
                GetSurfaceAreaFraction().Set(other.GetSurfaceAreaFraction());
            else if (surfaceAreaFraction != null)
                surfaceAreaFraction.Invalidate();
        }

        public void Reset()
        {
            state = eSwitch.Off;
            if (temperature != null)
                temperature.Invalidate();
            if (surfaceArea != null)
                surfaceArea.Invalidate();
            if (surfaceAreaFraction != null)
                surfaceAreaFraction.Invalidate();
        }


        //public static void load(AppliedTemperatureData src, SEAppliedTemperature dst)
        //{
        //    dst.reset();
        //    if (src.getState() != eSwitch.UNRECOGNIZED && src.getState() != eSwitch.NullSwitch)
        //        dst.state = src.getState();
        //    if (src.hasTemperature())
        //        SEScalarTemperature.load(src.getTemperature(), dst.getTemperature());
        //    if (src.hasSurfaceArea())
        //        SEScalarArea.load(src.getSurfaceArea(), dst.getSurfaceArea());
        //    if (src.hasSurfaceAreaFraction())
        //        SEScalar0To1.load(src.getSurfaceAreaFraction(), dst.getSurfaceAreaFraction());
        //}

        //public static AppliedTemperatureData unload(SEAppliedTemperature src)
        //{
        //    AppliedTemperatureData.Builder dst = AppliedTemperatureData.newBuilder();
        //    unload(src, dst);
        //    return dst.build();
        //}

        //protected static void unload(SEAppliedTemperature src, AppliedTemperatureData.Builder dst)
        //{
        //    dst.setState(src.state);
        //    if (src.HasTemperature())
        //        dst.setTemperature(SEScalarTemperature.unload(src.temperature));
        //    if (src.hasSurfaceArea())
        //        dst.setSurfaceArea(SEScalarArea.unload(src.surfaceArea));
        //    if (src.hasSurfaceAreaFraction())
        //        dst.setSurfaceAreaFraction(SEScalar0To1.unload(src.surfaceAreaFraction));
        //}


        public eSwitch GetState()
        {
            return state;
        }
        public void SetState(eSwitch s)
        {
            this.state = (s == eSwitch.NullSwitch) ? eSwitch.Off : s;
        }

        public bool HasTemperature()
        {
            return temperature == null ? false : temperature.IsValid();
        }
        public SEScalarTemperature GetTemperature()
        {
            if (temperature == null)
                temperature = new SEScalarTemperature();
            return temperature;
        }

        public bool HasSurfaceArea()
        {
            return surfaceArea == null ? false : surfaceArea.IsValid();
        }
        public SEScalarArea GetSurfaceArea()
        {
            if (surfaceArea == null)
                surfaceArea = new SEScalarArea();
            return surfaceArea;
        }

        public bool HasSurfaceAreaFraction()
        {
            return surfaceAreaFraction == null ? false : surfaceAreaFraction.IsValid();
        }
        public SEScalar0To1 GetSurfaceAreaFraction()
        {
            if (surfaceAreaFraction == null)
                surfaceAreaFraction = new SEScalar0To1();
            return surfaceAreaFraction;
        }


        public new string ToString()
        {
            return "Thermal Application:"
                + "\n\tState: " + GetState()
                + "\n\tAppliedTemperature: " + (this.HasTemperature() ? this.GetTemperature().ToString() : "None")
                + "\n\tAppliedSurfaceArea: " + (this.HasSurfaceArea() ? this.GetSurfaceArea().ToString() : "None")
                + "\n\tAppliedSurfaceAreaFraction: " + (this.HasSurfaceAreaFraction() ? this.GetSurfaceAreaFraction().ToString() : "None");
        }
    }

}



/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/


using System;

namespace Pulse.CDM
{
    public class SEActiveConditioning
    {
        protected SEScalarPower power;
        protected SEScalarArea surfaceArea;
        protected SEScalar0To1 surfaceAreaFraction;
        
        public SEActiveConditioning()
        {
            power = null;
            surfaceArea = null;
            surfaceAreaFraction = null;
        }

        public void Copy(SEActiveConditioning other)
        {
            if (this == other)
                return;
            if (other.power != null)
                GetPower().Set(other.GetPower());
            else if (power != null)
                power.Invalidate();
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
            if (power != null)
                power.Invalidate();
            if (surfaceArea != null)
                surfaceArea.Invalidate();
            if (surfaceAreaFraction != null)
                surfaceAreaFraction.Invalidate();
        }


        //public static void Load(ActiveConditioningData src, SEActiveConditioning dst)
        //{
        //    dst.Reset();
        //    if (src.hasPower())
        //        SEScalarPower.Load(src.getPower(), dst.GetPower());
        //    if (src.hasSurfaceArea())
        //        SEScalarArea.Load(src.getSurfaceArea(), dst.GetSurfaceArea());
        //    if (src.hasSurfaceAreaFraction())
        //        SEScalar0To1.Load(src.getSurfaceAreaFraction(), dst.GetSurfaceAreaFraction());
        //}

        //public static ActiveConditioningData Unload(SEActiveConditioning src)
        //{
        //    ActiveConditioningData.Builder dst = ActiveConditioningData.newBuilder();
        //    SEActiveConditioning.Unload(src, dst);
        //    return dst.build();
        //}

        //protected static void Unload(SEActiveConditioning src, ActiveConditioningData.Builder dst)
        //{
        //    if (src.HasPower())
        //        dst.setPower(SEScalarPower.Unload(src.power));
        //    if (src.HasSurfaceArea())
        //        dst.setSurfaceArea(SEScalarArea.Unload(src.surfaceArea));
        //    if (src.HasSurfaceAreaFraction())
        //        dst.setSurfaceAreaFraction(SEScalar0To1.Unload(src.surfaceAreaFraction));
        //}

        public Boolean HasPower()
        {
            return power == null ? false : power.IsValid();
        }
        public SEScalarPower GetPower()
        {
            if (power == null)
                power = new SEScalarPower();
            return power;
        }
        public Boolean HasSurfaceArea()
        {
            return surfaceArea == null ? false : surfaceArea.IsValid();
        }
        public SEScalarArea GetSurfaceArea()
        {
            if (surfaceArea == null)
                surfaceArea = new SEScalarArea();
            return surfaceArea;
        }
        public Boolean HasSurfaceAreaFraction()
        {
            return surfaceAreaFraction == null ? false : surfaceAreaFraction.IsValid();
        }
        public SEScalar0To1 GetSurfaceAreaFraction()
        {
            if (surfaceAreaFraction == null)
                surfaceAreaFraction = new SEScalar0To1();
            return surfaceAreaFraction;
        }

        public override string ToString()
        {
            return "Active Conditioning:"
                + "\n\tPower: " + (this.HasPower() ? this.GetPower().ToString() : "None")
                + "\n\tSurfaceArea: " + (this.HasSurfaceArea() ? this.GetSurfaceArea().ToString() : "None")
                + "\n\tSurfaceAreaFraction: " + (this.HasSurfaceAreaFraction() ? this.GetSurfaceAreaFraction().ToString() : "None");
        }
    }
}



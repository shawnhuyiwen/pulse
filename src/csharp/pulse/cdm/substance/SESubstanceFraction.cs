/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
    public class SESubstanceFraction
    {
        protected SEScalar0To1 amount;
        protected string substance;

        public SESubstanceFraction(string s)
        {
            substance = s ?? throw new System.ArgumentException("Must provide a valid substance");
        }

        public void Reset()
        {
            if (amount != null)
                amount.Invalidate();
        }

        public void Copy(SESubstanceFraction from)
        {
            substance = from.substance;
            if (from.HasFractionAmount ())
                GetFractionAmount ().Set(from.GetFractionAmount ());
        }

        //public static void load(SubstanceFractionData src, SESubstanceFraction dst)
        //{
        //  dst.reset();
        //  if(src.hasAmount())
        //    SEScalar0To1.load(src.getAmount(), dst.getAmount());
        //}
        //public static SubstanceFractionData unload(SESubstanceFraction src)
        //{
        //  SubstanceFractionData.Builder dst = SubstanceFractionData.newBuilder();
        //  SESubstanceFraction.unload(src,dst);
        //  return dst.build();
        //}
        //protected static void unload(SESubstanceFraction src, SubstanceFractionData.Builder dst)
        //{
        //  dst.setName(src.substance.getName());
        //  if(src.HasFractionAmount ())
        //    dst.setAmount(SEScalar0To1.unload(src.amount)); 
        //}

        public SEScalar0To1 GetFractionAmount ()
        {
            if (amount == null)
                amount = new SEScalar0To1();
            return amount;
        }
        public bool HasFractionAmount () { return amount == null ? false : amount.IsValid(); }

        public bool HasSubstance() { return substance != null; }
        public string GetSubstance()
        {
            return substance;
        }

        public new string ToString()
        {
            if (!HasSubstance())
                return "";
            string str = "Amount of " + substance + ": " + (HasFractionAmount () ? GetFractionAmount ().ToString() : "None");
            return str;
        }
    }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
    public class SEInitialEnvironmentalConditions : SEEnvironmentCondition
    {
        protected SEEnvironmentalConditions environmentalConditions;
        protected string environmentalConditionsFile;

        public SEInitialEnvironmentalConditions()
        {
            environmentalConditions = new SEEnvironmentalConditions();
        }

        public SEInitialEnvironmentalConditions(SEInitialEnvironmentalConditions other)
        {
            Copy(other);
        }

        public void Copy(SEInitialEnvironmentalConditions other)
        {
            if (this == other)
                return;

            environmentalConditions.Copy(other.environmentalConditions);
            environmentalConditionsFile = other.environmentalConditionsFile;
        }

        public void Reset()
        {

            environmentalConditions.Reset();
            environmentalConditionsFile = "";
        }

        public string GetName()
        {
            return "InitialEnvironmentalConditions";
        }


        public override bool IsValid()
        {
            return HasEnvironmentalConditions() || HasEnvironmentalConditionsFile();
        }

        public bool IsActive()
        {
            return IsValid();
        }

        //public static void load(InitialEnvironmentalConditionsData src, SEInitialEnvironmentalConditions dst, SESubstanceManager subMgr)
        //{
        //    SEEnvironmentCondition.load(src.getEnvironmentCondition(), dst);
        //    switch (src.getOptionCase())
        //    {
        //        case ENVIRONMENTALCONDITIONSFILE:
        //            dst.environmentalConditionsFile = src.getEnvironmentalConditionsFile();
        //            break;
        //        case ENVIRONMENTALCONDITIONS:
        //            SEEnvironmentalConditions.load(src.getEnvironmentalConditions(), dst.getEnvironmentalConditions(), subMgr);
        //            break;
        //    }
        //}
        //public static InitialEnvironmentalConditionsData unload(SEInitialEnvironmentalConditions src)
        //{
        //    InitialEnvironmentalConditionsData.Builder dst = InitialEnvironmentalConditionsData.newBuilder();
        //    unload(src, dst);
        //    return dst.build();
        //}
        //protected static void unload(SEInitialEnvironmentalConditions src, InitialEnvironmentalConditionsData.Builder dst)
        //{
        //    SEEnvironmentCondition.unload(src, dst.getEnvironmentConditionBuilder());
        //    if (src.hasEnvironmentalConditions())
        //        dst.setEnvironmentalConditions(SEEnvironmentalConditions.unload(src.environmentalConditions));
        //    else if (src.hasEnvironmentalConditionsFile())
        //        dst.setEnvironmentalConditionsFile(src.environmentalConditionsFile);
        //}

        public bool HasEnvironmentalConditions()
        {
            return environmentalConditions != null;
        }
        public SEEnvironmentalConditions GetEnvironmentalConditions()
        {
            return environmentalConditions;
        }

        public bool HasEnvironmentalConditionsFile()
        {
            //return environmentalConditionsFile != null && environmentalConditionsFile.isEmpty();
            return environmentalConditionsFile != null;
        }

        public void InvalidateEnvironmentalConditionsFile()
        {
            environmentalConditionsFile = null;
        }



        public string GetEnvironmentalConditionsFile()
        {
            return environmentalConditionsFile;
        }
        public void SetEnvironmentalConditionsFile(string s)
        {
            environmentalConditionsFile = s;
        }

        public new string ToString()
        {
            if (environmentalConditions != null)
                return "Initial Environmental Conditions : " + environmentalConditions.ToString();
            else if (HasEnvironmentalConditionsFile())
                return "Initial Envriomental Conditions:"
                    + "\n\tEnvironmentalConditionsFile: " + environmentalConditionsFile;
            else
                return "Condition not specified properly";
        }
    }


}


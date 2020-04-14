/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
    public class SEChangeEnvironmentalConditions : SEEnvironmentAction
    {
  protected SEEnvironmentalConditions environmentalConditions;
    protected string environmentalConditionsFile;

    public SEChangeEnvironmentalConditions()
    {
        environmentalConditions = new SEEnvironmentalConditions();
    }

        public override void Clear()
        {
            base.Clear();
            environmentalConditions = null;
            environmentalConditionsFile = null;

        }
        public SEChangeEnvironmentalConditions(SEChangeEnvironmentalConditions other)
    {
        Copy(other);
    }

        public void Copy(SEChangeEnvironmentalConditions other)
        {
            if (this == other)
                return;
            //base.Copy(other);
            environmentalConditions.Copy(other.environmentalConditions);
            environmentalConditionsFile = other.environmentalConditionsFile;
        }

        public void Reset()
        {
            
            environmentalConditions.Reset();
            environmentalConditionsFile = "";
        }


        public bool IsActive()
        {
            return true;
        }


        public override bool IsValid()
    {
        return HasEnvironmentalConditions() || HasEnvironmentalConditionsFile();
    }

    //public static void load(ChangeEnvironmentalConditionsData src, SEChangeEnvironmentalConditions dst, SESubstanceManager subMgr)
    //{
    //    SEEnvironmentAction.load(src.getEnvironmentAction(), dst);
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
    //public static ChangeEnvironmentalConditionsData unload(SEChangeEnvironmentalConditions src)
    //{
    //    ChangeEnvironmentalConditionsData.Builder dst = ChangeEnvironmentalConditionsData.newBuilder();
    //    unload(src, dst);
    //    return dst.build();
    //}
    //protected static void unload(SEChangeEnvironmentalConditions src, ChangeEnvironmentalConditionsData.Builder dst)
    //{
    //    SEEnvironmentAction.unload(src, dst.getEnvironmentActionBuilder());
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
        return environmentalConditionsFile != null/* && !environmentalConditionsFile.isEmpty()*/;
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
            return "Environment Configuration : " + environmentalConditions.ToString();
        else if (HasEnvironmentalConditionsFile())
            return "Envrioment Configuration:"
                + "\n\tEnvironmentalConditionsFile: " + environmentalConditionsFile;
        else
            return "Action not specified properly";
    }
}
}

    
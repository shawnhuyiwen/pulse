/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{

    public abstract class SEEnvironmentCondition : SECondition
    {
        public SEEnvironmentCondition()
        {

        }


        public override void Clear()
        {
            base.Clear();
        }

        //    public void copy(SEEnvironmentCondition other)
        //    {
        //        super.copy(other);
        //    }

        //    public void reset()
        //    {
        //        super.reset();
        //    }

        //    public static void load(EnvironmentConditionData src, SEEnvironmentCondition dst)
        //    {
        //        SECondition.load(src.getCondition(), dst);
        //    }
        //    protected static void unload(SEEnvironmentCondition src, EnvironmentConditionData.Builder dst)
        //    {
        //        SECondition.unload(src, dst.getConditionBuilder());
        //    }

        //    public abstract String toString();

        //    public static SEEnvironmentCondition ANY2CDM(AnyEnvironmentConditionData c, SESubstanceManager subMgr)
        //    {
        //        switch (c.getConditionCase())
        //        {
        //            case INITIALENVIRONMENTALCONDITIONS:
        //                SEInitialEnvironmentalConditions newC = new SEInitialEnvironmentalConditions();
        //                SEInitialEnvironmentalConditions.load(c.getInitialEnvironmentalConditions(), newC, subMgr);
        //                return newC;
        //            case CONDITION_NOT_SET:
        //                Log.warn("AnyEnvironmentConditionData was empty...was that intended?");
        //                return null;
        //        }
        //        Log.error("Unsupported Environment condition type " + c.getConditionCase());
        //        return null;
        //    }
        //    public static AnyEnvironmentConditionData CDM2ANY(SEEnvironmentCondition c)
        //    {
        //        AnyEnvironmentConditionData.Builder dst = AnyEnvironmentConditionData.newBuilder();
        //        if (c instanceof SEInitialEnvironmentalConditions)
        //{
        //            dst.setInitialEnvironmentalConditions(SEInitialEnvironmentalConditions.unload((SEInitialEnvironmentalConditions)c));
        //            return dst.build();
        //        }
        //        Log.error("Unsupported Environment condition type " + c);
        //        return dst.build();
        //    }
    }

}



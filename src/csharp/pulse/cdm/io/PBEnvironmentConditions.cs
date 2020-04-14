/* Distributed under the Apache License, Version 2.0.SEInitialEnvironmentalConditions
   See accompanying NOTICE file for details.*/
using System.Collections.Generic;
namespace Pulse.CDM
{
    public class PBEnvironmentConditions
    {


        #region SEEnvironmentCondition

        public static void Load(pulse.cdm.bind.EnvironmentConditionData src, SEEnvironmentCondition dst)
        {
            Serialize(src, dst);
        }

        public static pulse.cdm.bind.EnvironmentConditionData Unload(SEEnvironmentCondition src)
        {
            pulse.cdm.bind.EnvironmentConditionData dst = new pulse.cdm.bind.EnvironmentConditionData();
            Serialize(src, dst);
            return dst;
        }

        public static void Serialize(SEEnvironmentCondition src, pulse.cdm.bind.EnvironmentConditionData dst)
        {
            PBCondition.Serialize(src, dst.Condition);
        }

        public static void Serialize(pulse.cdm.bind.EnvironmentConditionData src, SEEnvironmentCondition dst)
        {
            PBCondition.Serialize(src.Condition, dst);
        }

        #endregion

        #region SEInitialEnvironmentalConditions
        public static void Load(pulse.cdm.bind.InitialEnvironmentConditionsData src, SEInitialEnvironmentalConditions dst)
        {
            Serialize(src, dst);
        }

        public static pulse.cdm.bind.InitialEnvironmentConditionsData Unload(SEInitialEnvironmentalConditions src)
        {
            pulse.cdm.bind.InitialEnvironmentConditionsData dst = new pulse.cdm.bind.InitialEnvironmentConditionsData();
            Serialize(src, dst);
            return dst;
        }





        static void Serialize(SEInitialEnvironmentalConditions src, pulse.cdm.bind.InitialEnvironmentConditionsData dst)
        {

            Serialize(src, dst.EnvironmentCondition);
            if (src.HasEnvironmentalConditionsFile())
                dst.ConditionsFile = src.GetEnvironmentalConditionsFile();
            else if (src.HasEnvironmentalConditions())
                dst.Conditions = PBEnvironment.Unload(src.GetEnvironmentalConditions());

        }









        public static void Serialize(pulse.cdm.bind.InitialEnvironmentConditionsData src, SEInitialEnvironmentalConditions dst)
        {
            PBEnvironmentConditions.Serialize(src.EnvironmentCondition, dst);
            throw new KeyNotFoundException("serialize_change_environmental_conditions_from_bind not implemented");
        }

        #endregion



    }
}

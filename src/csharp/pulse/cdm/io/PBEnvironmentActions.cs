/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
using System.Collections.Generic;
namespace Pulse.CDM
{
    public class PBEnvironmentAction
    {

        #region Any Environment Action

        public static SEEnvironmentAction Load(pulse.cdm.bind.AnyEnvironmentActionData any)
        {
            if (any.Conditions != null)
            {

                SEChangeEnvironmentalConditions dst = new SEChangeEnvironmentalConditions();
                PBEnvironmentAction.Load(any.Conditions, dst);
                return dst;
            }
            if (any.ThermalApplication != null)
            {
                SEThermalApplication dst = new SEThermalApplication();
                PBEnvironmentAction.Load(any.ThermalApplication, dst);
                return dst;
            }
            return null;
        }

        public static pulse.cdm.bind.AnyEnvironmentActionData Unload(SEEnvironmentAction action)
        {
            pulse.cdm.bind.AnyEnvironmentActionData any = new pulse.cdm.bind.AnyEnvironmentActionData();
            if (action.GetType().IsAssignableFrom(typeof(SEEnvironmentalConditions)))
            {
                any.Conditions = Unload((SEChangeEnvironmentalConditions)action);
                return any;
            }

            if (action.GetType().IsAssignableFrom(typeof(SEThermalApplication)))
            {
                any.ThermalApplication = Unload((SEThermalApplication)action);
                return any;
            }
            return null;
        }


        #endregion


        #region SEEnvironmentAction
        //    public static void Load(pulse.cdm.bind.EnvironmentActionData src, SEEnvironmentAction dst)
        //{
        //    Serialize(src, dst);
        //}

        //public static pulse.cdm.bind.EnvironmentActionData Unload(SEEnvironmentAction src)
        //{
        //    pulse.cdm.bind.EnvironmentActionData dst = new pulse.cdm.bind.EnvironmentActionData();
        //    Serialize(src, dst);
        //    return dst;
        //}

        public static void Serialize(pulse.cdm.bind.EnvironmentActionData src, SEEnvironmentAction dst)
        {
            PBAction.Serialize(src.Action, dst);

        }

        public static void Serialize(SEEnvironmentAction src, pulse.cdm.bind.EnvironmentActionData dst)
        {
            PBAction.Serialize(src, dst.Action);

        }
        #endregion




        #region SEChangeEnvironmentalConditions


        public static void Load(pulse.cdm.bind.ChangeEnvironmentConditionsData src, SEChangeEnvironmentalConditions dst)
        {
            Serialize(src, dst);
        }

        public static pulse.cdm.bind.ChangeEnvironmentConditionsData Unload(SEChangeEnvironmentalConditions src)
        {
            pulse.cdm.bind.ChangeEnvironmentConditionsData dst = new pulse.cdm.bind.ChangeEnvironmentConditionsData();
            Serialize(src, dst);
            return dst;
        }





        static void Serialize(SEChangeEnvironmentalConditions src, pulse.cdm.bind.ChangeEnvironmentConditionsData dst)
        {

            Serialize(src, dst.EnvironmentAction);
            if (src.HasEnvironmentalConditionsFile())
                dst.ConditionsFile = src.GetEnvironmentalConditionsFile();
            else if (src.HasEnvironmentalConditions())
                dst.Conditions = PBEnvironment.Unload(src.GetEnvironmentalConditions());
        }





        public static void Serialize(pulse.cdm.bind.ChangeEnvironmentConditionsData src, SEChangeEnvironmentalConditions dst)
        {
            Serialize(src.EnvironmentAction, dst);
            throw new KeyNotFoundException("serialize_change_environmental_conditions_from_bind not implemented");
        }

        #endregion





        #region SEThermalApplication
        public static void Load(pulse.cdm.bind.ThermalApplicationData src, SEThermalApplication dst)
        {
            Serialize(src, dst);
        }

        public static pulse.cdm.bind.ThermalApplicationData Unload(SEThermalApplication src)
        {
            pulse.cdm.bind.ThermalApplicationData dst = new pulse.cdm.bind.ThermalApplicationData();
            Serialize(src, dst);
            return dst;
        }


        public static void Serialize(SEThermalApplication src, pulse.cdm.bind.ThermalApplicationData dst)
        {
            PBEnvironmentAction.Serialize(src, dst.EnvironmentAction);
            dst.AppendToPrevious = !src.GetClearContents();

            if (src.HasActiveHeating())
                dst.ActiveHeating = PBEnvironment.Unload(src.GetActiveHeating());
            if (src.HasActiveCooling())
                dst.ActiveCooling = PBEnvironment.Unload(src.GetActiveCooling());
            if (src.HasAppliedTemperature())
                dst.AppliedTemperature = PBEnvironment.Unload(src.GetAppliedTemperature());
        }


        public static void Serialize(pulse.cdm.bind.ThermalApplicationData src, SEThermalApplication dst)
        {
            PBEnvironmentAction.Serialize(src.EnvironmentAction, dst);
            throw new KeyNotFoundException("serialize_change_environmental_conditions_from_bind not implemented");
        }


        #endregion



    }
}

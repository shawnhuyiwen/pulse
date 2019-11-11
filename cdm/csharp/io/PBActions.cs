/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;
using Google.Protobuf;

public class PBAction
{
    public static void Load(Cdm.ActionListData src, List<SEAction> dst)
    {
        Serialize(src, dst);
    }
    public static void Serialize(Cdm.ActionListData src, List<SEAction> dst)
    {
        foreach(var action in src.AnyAction)
            dst.Add(PBAction.Load(action));
    }
    public static bool SerializeFromString(string src, List<SEAction> dst)
    {
        try
        {
            Cdm.ActionListData data = JsonParser.Default.Parse<Cdm.ActionListData>(src);
            PBAction.Load(data, dst);
        }
        catch (Google.Protobuf.InvalidJsonException)
        {
            return false;
        }
        return true;
    }

    public static Cdm.ActionListData Unload(List<SEAction> src)
    {
        Cdm.ActionListData dst = new Cdm.ActionListData();
        Serialize(src, dst);
        return dst;
    }
    public static void Serialize(List<SEAction> src, Cdm.ActionListData dst)
    {
        foreach(var action in src)
            dst.AnyAction.Add(PBAction.Unload(action));
    }
    public static string SerializeToString(List<SEAction> src)
    {
        var pb = PBAction.Unload(src);
        return pb.ToString();
    }

    #region AnyAction
    /** Create a new action based on the binding object,
     *  load that data into the new action, and return said action */
    public static SEAction Load(Cdm.AnyActionData action)
    {
        switch(action.ActionCase)
        {
            case Cdm.AnyActionData.ActionOneofCase.PatientAction:
                return PBPatientAction.Load(action.PatientAction);
            case Cdm.AnyActionData.ActionOneofCase.AnesthesiaMachineAction:
                return PBAnesthesiaMachineAction.Load(action.AnesthesiaMachineAction);
        }
        return null;
    }
    /** Create a new bind object, unload the action,
     *  put that in the bind object, and return said bind object */
    public static Cdm.AnyActionData Unload(SEAction action)
    {
        Cdm.AnyActionData any = new Cdm.AnyActionData();
        if( action.GetType().IsSubclassOf(typeof(SEPatientAction)) )
            any.PatientAction = PBPatientAction.Unload((SEPatientAction)action);
        if (action.GetType().IsSubclassOf(typeof(SEAnesthesiaMachineAction)))
            any.AnesthesiaMachineAction = PBAnesthesiaMachineAction.Unload((SEAnesthesiaMachineAction)action);
        return any;
    }
    #endregion

    #region SEAction
    public static void Serialize(Cdm.ActionData src, SEAction dst)
    {
        dst.Clear();
        if(src.Comment != null)
            dst.SetComment(src.Comment);
    }
    public static void Serialize(SEAction src, Cdm.ActionData dst)
    {
        if(src.HasComment())
            dst.Comment = src.GetComment();
    }
    #endregion
}
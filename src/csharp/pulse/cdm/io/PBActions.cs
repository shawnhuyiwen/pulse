/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;
using Google.Protobuf;

namespace Pulse.CDM
{
  public class PBAction
  {
    public static void Load(pulse.cdm.bind.ActionListData src, List<SEAction> dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ActionListData src, List<SEAction> dst)
    {
      foreach (var action in src.AnyAction)
        dst.Add(PBAction.Load(action));
    }
    public static bool SerializeFromString(string src, List<SEAction> dst)
    {
      try
      {
        pulse.cdm.bind.ActionListData data = JsonParser.Default.Parse<pulse.cdm.bind.ActionListData>(src);
        PBAction.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static pulse.cdm.bind.ActionListData Unload(List<SEAction> src)
    {
      pulse.cdm.bind.ActionListData dst = new pulse.cdm.bind.ActionListData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(List<SEAction> src, pulse.cdm.bind.ActionListData dst)
    {
      foreach (var action in src)
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
    public static SEAction Load(pulse.cdm.bind.AnyActionData action)
    {
      switch (action.ActionCase)
      {
        case pulse.cdm.bind.AnyActionData.ActionOneofCase.PatientAction:
          return PBPatientAction.Load(action.PatientAction);
        case pulse.cdm.bind.AnyActionData.ActionOneofCase.AnesthesiaMachineAction:
          return PBAnesthesiaMachineAction.Load(action.AnesthesiaMachineAction);
      }
      return null;
    }
    /** Create a new bind object, unload the action,
     *  put that in the bind object, and return said bind object */
    public static pulse.cdm.bind.AnyActionData Unload(SEAction action)
    {
      pulse.cdm.bind.AnyActionData any = new pulse.cdm.bind.AnyActionData();
      if (action.GetType().IsSubclassOf(typeof(SEPatientAction)))
        any.PatientAction = PBPatientAction.Unload((SEPatientAction)action);
      if (action.GetType().IsSubclassOf(typeof(SEAnesthesiaMachineAction)))
        any.AnesthesiaMachineAction = PBAnesthesiaMachineAction.Unload((SEAnesthesiaMachineAction)action);
      return any;
    }
    #endregion

    #region SEAction
    public static void Serialize(pulse.cdm.bind.ActionData src, SEAction dst)
    {
      dst.Clear();
      if (src.Comment != null)
        dst.SetComment(src.Comment);
    }
    public static void Serialize(SEAction src, pulse.cdm.bind.ActionData dst)
    {
      if (src.HasComment())
        dst.Comment = src.GetComment();
    }
    #endregion
  }
}
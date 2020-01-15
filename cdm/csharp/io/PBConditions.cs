/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;
using Google.Protobuf;

namespace Pulse.CDM
{
  public class PBCondition
  {
    public static void Load(Cdm.ConditionListData src, List<SECondition> dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(Cdm.ConditionListData src, List<SECondition> dst)
    {
      foreach (var Condition in src.AnyCondition)
        dst.Add(PBCondition.Load(Condition));
    }
    public static bool SerializeFromString(string src, List<SECondition> dst)
    {
      try
      {
        Cdm.ConditionListData data = JsonParser.Default.Parse<Cdm.ConditionListData>(src);
        PBCondition.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static Cdm.ConditionListData Unload(List<SECondition> src)
    {
      Cdm.ConditionListData dst = new Cdm.ConditionListData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(List<SECondition> src, Cdm.ConditionListData dst)
    {
      foreach (var Condition in src)
        dst.AnyCondition.Add(PBCondition.Unload(Condition));
    }
    public static string SerializeToString(List<SECondition> src)
    {
      var pb = PBCondition.Unload(src);
      return pb.ToString();
    }

    #region AnyCondition
    /** Create a new Condition based on the binding object,
     *  load that data into the new Condition, and return said Condition */
    public static SECondition Load(Cdm.AnyConditionData Condition)
    {
      switch (Condition.ConditionCase)
      {
        case Cdm.AnyConditionData.ConditionOneofCase.PatientCondition:
          return PBPatientCondition.Load(Condition.PatientCondition);
          //case Cdm.AnyConditionData.ConditionOneofCase.EnvironmentCondition:
          //  return PBEnvironmentCondition.Load(Condition.EnvironmentCondition);
      }
      return null;
    }
    /** Create a new bind object, unload the Condition,
     *  put that in the bind object, and return said bind object */
    public static Cdm.AnyConditionData Unload(SECondition Condition)
    {
      Cdm.AnyConditionData any = new Cdm.AnyConditionData();
      if (Condition.GetType().IsSubclassOf(typeof(SEPatientCondition)))
        any.PatientCondition = PBPatientCondition.Unload((SEPatientCondition)Condition);
      //if (Condition.GetType().IsSubclassOf(typeof(SEEnvironmentCondition)))
      //  any.EnvironmentCondition = PBEnvironmentCondition.Unload((SEEnvironmentCondition)Condition);
      return any;
    }
    #endregion

    #region SECondition
    public static void Serialize(Cdm.ConditionData src, SECondition dst)
    {
      dst.Clear();
      if (src.Comment != null)
        dst.SetComment(src.Comment);
    }
    public static void Serialize(SECondition src, Cdm.ConditionData dst)
    {
      if (src.HasComment())
        dst.Comment = src.GetComment();
    }
    #endregion
  }
}

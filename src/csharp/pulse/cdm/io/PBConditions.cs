/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;
using Google.Protobuf;

namespace Pulse.CDM
{
  public class PBCondition
  {
    public static void Load(pulse.cdm.bind.ConditionListData src, List<SECondition> dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ConditionListData src, List<SECondition> dst)
    {
      foreach (var Condition in src.AnyCondition)
        dst.Add(PBCondition.Load(Condition));
    }
    public static bool SerializeFromString(string src, List<SECondition> dst)
    {
      try
      {
        pulse.cdm.bind.ConditionListData data = JsonParser.Default.Parse<pulse.cdm.bind.ConditionListData>(src);
        PBCondition.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static pulse.cdm.bind.ConditionListData Unload(List<SECondition> src)
    {
      pulse.cdm.bind.ConditionListData dst = new pulse.cdm.bind.ConditionListData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(List<SECondition> src, pulse.cdm.bind.ConditionListData dst)
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
    public static SECondition Load(pulse.cdm.bind.AnyConditionData Condition)
    {
      switch (Condition.ConditionCase)
      {
        case pulse.cdm.bind.AnyConditionData.ConditionOneofCase.PatientCondition:
          return PBPatientCondition.Load(Condition.PatientCondition);
        case pulse.cdm.bind.AnyConditionData.ConditionOneofCase.EnvironmentCondition:
          return PBEnvironmentCondition.Load(Condition.EnvironmentCondition);
      }
      return null;
    }
    /** Create a new bind object, unload the Condition,
     *  put that in the bind object, and return said bind object */
    public static pulse.cdm.bind.AnyConditionData Unload(SECondition Condition)
    {
      pulse.cdm.bind.AnyConditionData any = new pulse.cdm.bind.AnyConditionData();
      if (Condition.GetType().IsSubclassOf(typeof(SEPatientCondition)))
        any.PatientCondition = PBPatientCondition.Unload((SEPatientCondition)Condition);
      if (Condition.GetType().IsSubclassOf(typeof(SEEnvironmentCondition)))
        any.EnvironmentCondition = PBEnvironmentCondition.Unload((SEEnvironmentCondition)Condition);
      return any;
    }
    #endregion

    #region SECondition
    public static void Serialize(pulse.cdm.bind.ConditionData src, SECondition dst)
    {
      dst.Clear();
      if (src.Comment != null)
        dst.SetComment(src.Comment);
    }
    public static void Serialize(SECondition src, pulse.cdm.bind.ConditionData dst)
    {
      if (src.HasComment())
        dst.Comment = src.GetComment();
    }
    #endregion
  }
}

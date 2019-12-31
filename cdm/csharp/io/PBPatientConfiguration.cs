/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;
using Google.Protobuf;

public class PBPatientConfiguration
{
  #region SEAction
  public static void Load(Cdm.PatientConfigurationData src, SEPatientConfiguration dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.PatientConfigurationData src, SEPatientConfiguration dst)
  {
    if (src.PatientFile != null)
      dst.SetPatientFile(src.PatientFile);
    else if(src.Patient != null)
      PBPatient.Serialize(src.Patient, dst.GetPatient());
    if (src.Conditions != null)
      Serialize(src.Conditions, dst.GetConditions());
  }
  public static bool SerializeFromString(string src, SEPatientConfiguration dst)
  {
    try
    {
      Cdm.PatientConfigurationData data = JsonParser.Default.Parse<Cdm.PatientConfigurationData>(src);
      PBPatientConfiguration.Load(data, dst);
    }
    catch (Google.Protobuf.InvalidJsonException)
    {
      return false;
    }
    return true;
  }

  public static Cdm.PatientConfigurationData Unload(SEPatientConfiguration src)
  {
    Cdm.PatientConfigurationData dst = new Cdm.PatientConfigurationData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEPatientConfiguration src, Cdm.PatientConfigurationData dst)
  {
    if (src.HasPatientFile())
      dst.PatientFile = src.GetPatientFile();
    else if (src.HasPatient())
      dst.Patient = PBPatient.Unload(src.GetPatient());
    if (src.HasConditions())
      dst.Conditions = PBPatientConfiguration.Unload(src.GetConditions());
  }
  public static string SerializeToString(SEPatientConfiguration src)
  {
    var pb = PBPatientConfiguration.Unload(src);
    return pb.ToString();
  }
  #endregion

  #region SEConditionManager
  public static void Load(Cdm.ConditionListData src, SEConditionManager dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ConditionListData src, SEConditionManager dst)
  {
    foreach (var condition in src.AnyCondition)
    {
      if (!dst.Add(PBCondition.Load(condition)))
        System.Console.WriteLine("Failed to serialize an unsupported condition " + condition.GetType());
    }
  }
  public static Cdm.ConditionListData Unload(SEConditionManager src)
  {
    Cdm.ConditionListData dst = new Cdm.ConditionListData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEConditionManager src, Cdm.ConditionListData dst)
  {
    if (src.HasAcuteRespiratoryDistressSyndrome())
      dst.AnyCondition.Add(PBCondition.Unload(src.GetAcuteRespiratoryDistressSyndrome()));
    if (src.HasChronicAnemia())
      dst.AnyCondition.Add(PBCondition.Unload(src.GetChronicAnemia()));
    if (src.HasChronicObstructivePulmonaryDisease())
      dst.AnyCondition.Add(PBCondition.Unload(src.GetChronicObstructivePulmonaryDisease()));
    if (src.HasChronicPericardialEffusion())
      dst.AnyCondition.Add(PBCondition.Unload(src.GetChronicPericardialEffusion()));
    if (src.HasChronicRenalStenosis())
      dst.AnyCondition.Add(PBCondition.Unload(src.GetChronicRenalStenosis()));
    if (src.HasChronicVentricularSystolicDysfunction())
      dst.AnyCondition.Add(PBCondition.Unload(src.GetChronicVentricularSystolicDysfunction()));
    if (src.HasImpairedAlveolarExchange())
      dst.AnyCondition.Add(PBCondition.Unload(src.GetImpairedAlveolarExchange()));
    if (src.HasLobarPneumonia())
      dst.AnyCondition.Add(PBCondition.Unload(src.GetLobarPneumonia()));
    if (src.HasSepsis())
      dst.AnyCondition.Add(PBCondition.Unload(src.GetSepsis()));
  }
  #endregion
}

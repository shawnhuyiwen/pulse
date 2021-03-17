/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using Google.Protobuf;

namespace Pulse.CDM
{
  public class PBScenario
  {
    public static void Load(pulse.cdm.bind.ScenarioExecData src, SEScenarioExec dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScenarioExecData src, SEScenarioExec dst)
    {
      dst.Clear();
      dst.SetLogToConsole((eSwitch)(int)src.LogToConsole);
      if (src.LogFilename.Length > 0)
        dst.SetLogFilename(src.LogFilename);
      if (src.DataRequestCSVFilename.Length > 0)
        dst.SetDataRequestCSVFilename(src.DataRequestCSVFilename);
      if (src.DataRootDirectory.Length > 0)
        dst.SetDataRootDirectory(src.DataRootDirectory);

      if (src.SerializationDirectory.Length > 0)
        dst.SetSerializationDirectory(src.SerializationDirectory);
      if (src.AutoSerializeFilename.Length > 0)
        dst.SetAutoSerializeFilename(src.AutoSerializeFilename);
      dst.SetAutoSerializeAfterActions((eSwitch)(int)src.AutoSerializeAfterActions);
      dst.SetAutoSerializePeriod_s(src.AutoSerializePeriodS);
      dst.SetTimeStampSerializedStates((eSwitch)(int)src.TimeStampSerializedStates);
      dst.SetReloadSerializedState((eSwitch)(int)src.ReloadSerializedState);

      if (src.EngineConfigurationContent.Length > 0)
        dst.SetEngineConfigurationContent(src.EngineConfigurationContent);
      else if (src.EngineConfigurationFilename.Length > 0)
        dst.SetEngineConfigurationFilename(src.EngineConfigurationFilename);

      if (src.ScenarioContent.Length > 0)
        dst.SetScenarioContent(src.ScenarioContent);
      else if (src.ScenarioFilename.Length > 0)
        dst.SetScenarioFilename(src.ScenarioFilename);

      dst.SetContentFormat((eSerializationFormat)(int)src.ContentFormat);
    }
    public static bool SerializeFromString(string src, SEScenarioExec dst)
    {
      try
      {
        pulse.cdm.bind.ScenarioExecData data = JsonParser.Default.Parse<pulse.cdm.bind.ScenarioExecData>(src);
        PBScenario.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static pulse.cdm.bind.ScenarioExecData Unload(SEScenarioExec src)
    {
      pulse.cdm.bind.ScenarioExecData dst = new pulse.cdm.bind.ScenarioExecData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEScenarioExec src, pulse.cdm.bind.ScenarioExecData dst)
    {
      dst.LogToConsole = (pulse.cdm.bind.eSwitch)(int)src.GetLogToConsole();
      dst.LogFilename = src.GetLogFilename();
      dst.DataRequestCSVFilename = src.GetDataRequestCSVFilename();
      dst.DataRootDirectory = src.GetDataRootDirectory();

      dst.SerializationDirectory = src.GetSerializationDirectory();
      dst.AutoSerializeFilename = src.GetAutoSerializeFilename();
      dst.AutoSerializeAfterActions = (pulse.cdm.bind.eSwitch)(int)src.GetAutoSerializeAfterActions();
      dst.AutoSerializePeriodS = src.GetAutoSerializePeriod_s();
      dst.TimeStampSerializedStates = (pulse.cdm.bind.eSwitch)(int)src.GetTimeStampSerializedStates();
      dst.ReloadSerializedState = (pulse.cdm.bind.eSwitch)(int)src.GetReloadSerializedState();

      if(src.GetEngineConfigurationContent().Length>0)
        dst.EngineConfigurationContent = src.GetEngineConfigurationContent();
      else if (src.GetEngineConfigurationFilename().Length > 0)
        dst.EngineConfigurationFilename = src.GetEngineConfigurationFilename();

      if (src.GetScenarioContent().Length > 0)
        dst.ScenarioContent = src.GetScenarioContent();
      else if (src.GetScenarioFilename().Length > 0)
        dst.ScenarioFilename = src.GetScenarioFilename();

      dst.ContentFormat = (pulse.cdm.bind.eSerializationFormat)(int)src.GetContentFormat();

    }
    public static string SerializeToString(SEScenarioExec src)
    {
      var pb = PBScenario.Unload(src);
      return pb.ToString();
    }
  }
}


/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using Google.Protobuf;

namespace Pulse
{
  public class PBScenario
  {
    #region PulseScenarioExec
    public static void Load(pulse.engine.bind.ScenarioExecData src, PulseScenarioExec dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.engine.bind.ScenarioExecData src, PulseScenarioExec dst)
    {
      Pulse.CDM.PBScenario.Serialize(src.ScenarioExec, dst);
      dst.SetModelType((eModelType)(int)src.ModelType);
    }
    public static bool SerializeFromString(string src, PulseScenarioExec dst)
    {
      try
      {
        pulse.engine.bind.ScenarioExecData data = JsonParser.Default.Parse<pulse.engine.bind.ScenarioExecData>(src);
        PBScenario.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static pulse.engine.bind.ScenarioExecData Unload(PulseScenarioExec src)
    {
      pulse.engine.bind.ScenarioExecData dst = new pulse.engine.bind.ScenarioExecData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(PulseScenarioExec src, pulse.engine.bind.ScenarioExecData dst)
    {
      dst.ScenarioExec = new pulse.cdm.bind.ScenarioExecData();
      Pulse.CDM.PBScenario.Serialize(src, dst.ScenarioExec);
      dst.ModelType = (pulse.engine.bind.eModelType)(int)src.GetModelType();
    }
    public static string SerializeToString(PulseScenarioExec src)
    {
      var pb = PBScenario.Unload(src);
      return pb.ToString();
    }
    #endregion
  }
}


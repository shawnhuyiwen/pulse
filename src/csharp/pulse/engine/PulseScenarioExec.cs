/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System;
using System.Runtime.InteropServices;
using Pulse.CDM;

namespace Pulse
{

  public abstract class PulseScenarioExecBase : SEScenarioExec
  {
    protected eModelType model_type;

    public PulseScenarioExecBase() : base()
    {

    }

    public override void Clear()
    {
      base.Clear();
      model_type = eModelType.HumanAdultWholeBody;
    }

    public eModelType GetModelType() { return model_type; }
    public void SetModelType(eModelType t) { model_type = t; }

    protected abstract bool BaseExecuteScenario(string exeOpts, int format);
  }
  public class PulseScenarioExec : PulseScenarioExecBase
  {
    // You can change the name of our Pulse library (PulseC or PulseCd)
    private const string Attribute = "PulseC";

    public bool Execute()
    {
      string opts_str = PBScenario.SerializeToString(this);
      return BaseExecuteScenario(opts_str, (int)eSerializationFormat.JSON);
    }

    [DllImport(Attribute)]
    private static extern bool ExecuteScenario(string exeOpts, int format);
    protected override bool BaseExecuteScenario(string exeOpts, int format)
    {
      return ExecuteScenario(exeOpts, format);
    }
  }
}

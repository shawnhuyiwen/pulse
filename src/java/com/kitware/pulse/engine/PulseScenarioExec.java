/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.engine;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;
import com.kitware.pulse.cdm.bind.Scenario.eSerializationFormat;
import com.kitware.pulse.cdm.scenario.SEScenarioExec;
import com.kitware.pulse.engine.bind.Enums.eModelType;
import com.kitware.pulse.engine.bind.Scenario.ScenarioExecData;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.LogListener;

/**
 * This is a class that will run a scenario through Pulse
 * and create it's results file. You could easily write your
 * own exec class that does what you want in between actions
 * but this may work for most of your needs.
 * @author abray
 */
public class PulseScenarioExec extends SEScenarioExec
{
  protected static class MyListener extends LogListener
  {
    public String origin = "";
    protected MyListener()
    {
      super();
      listen(false);
    }    
    @Override public void handleDebug(String msg) { Log.debug(msg, origin); }
    @Override public void handleInfo(String msg)  { Log.info(msg, origin); }
    @Override public void handleWarn(String msg)  { Log.warn(msg, origin); }
    @Override public void handleError(String msg) { Log.error(msg, origin); }
    @Override public void handleFatal(String msg) { Log.fatal(msg, origin); }
  }
  
  protected MyListener logListener = new MyListener();
  protected eModelType modelType;

  public PulseScenarioExec()
  {
    clear();
  }

  public void clear()
  {
    super.clear();
    modelType = eModelType.HumanAdultWholeBody;
  }
  
  public void copy(PulseScenarioExec src)
  {
    PulseScenarioExec.load(PulseScenarioExec.unload(src), this);
  }
  
  public static void load(ScenarioExecData src, PulseScenarioExec dst)
  {
    SEScenarioExec.load(src.getScenarioExec(), dst);
    dst.modelType = src.getModelType();
  }
  public static ScenarioExecData unload(PulseScenarioExec src)
  {
    ScenarioExecData.Builder dst = ScenarioExecData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(PulseScenarioExec src, ScenarioExecData.Builder dst)
  {
    SEScenarioExec.unload(src, dst.getScenarioExecBuilder());
    dst.setModelType(src.modelType);
  }
  
  public String toJSON() 
  {
    String json;
    try
    {
      json = JsonFormat.printer().print(PulseScenarioExec.unload(this));
    }
    catch (InvalidProtocolBufferException e)
    {
      Log.error("Unable to generate json from PulseScenarioExec");
      json = "";
    }
    return json;
  }
  
  public eModelType getModelType() { return modelType; }
  public void setModelType(eModelType mt) { modelType=mt; }
  
  public void setLogPrepend(String lp) { logListener.origin = lp; }
  
  public boolean execute()
  {
    return nativeExecuteScenario(toJSON(), eSerializationFormat.JSON_VALUE);
  }
  protected native boolean nativeExecuteScenario(String sceOpts, int format);
  
  ///////////////////////
  // LISTENER/HANDLERS //
  ///////////////////////
  
  protected void handleDebug(String msg, String origin)
  {
    if(this.logListener!=null)
    this.logListener.debug(msg, origin);
  }
  protected void handleInfo(String msg, String origin)
  {
    if(this.logListener!=null)
    this.logListener.info(msg, origin);
  }
  protected void handleWarning(String msg, String origin)
  {
    if(this.logListener!=null)
    this.logListener.warn(msg, origin);
  }
  protected void handleError(String msg, String origin)
  {
    if(this.logListener!=null)
    this.logListener.error(msg, origin);
  }
  protected void handleFatal(String msg, String origin)
  {
    if(this.logListener!=null)
    this.logListener.fatal(msg, origin);
  }
}

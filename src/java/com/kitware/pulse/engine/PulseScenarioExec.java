/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.engine;

import com.google.protobuf.util.JsonFormat;
import com.kitware.pulse.SerializationType;

import com.kitware.pulse.cdm.scenario.SEScenario;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.LogListener;
import com.kitware.pulse.utilities.JNIBridge;

/**
 * This is a class that will run a scenario through Pulse
 * and create it's results file. You could easily write your
 * own exec class that does what you want in between actions
 * but this may work for most of your needs.
 * @author abray
 */
public class PulseScenarioExec
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
  protected MyListener listener = new MyListener();
  public PulseScenarioExec()
  {
    JNIBridge.initialize();
    engine=new PulseEngine();
    engine.setLogListener(listener);
  }
  
  public void finalize()
  {
    engine.cleanUp();
    engine = null;
  }
  
  public boolean runScenario(SEScenario scenario, String resultsFile, String logFile)
  {
    return runScenario(scenario, resultsFile, logFile, "./");
  }
  public boolean runScenario(SEScenario scenario, String resultsFile, String logFile, String dataDir)
  {
    try
    {
      String sString =  JsonFormat.printer().print(SEScenario.unload(scenario));
      return runScenario(sString, SerializationType.JSON, resultsFile, logFile, dataDir);
    }
    catch(Exception ex)
    {
      Log.error("Unable to run scenario");
    }
    return false;
  }
  
  public boolean runScenario(String scenario, SerializationType format, String resultsFile, String logFile)
  {
    return runScenario(scenario, format, resultsFile, logFile, "./");
  }
  public boolean runScenario(String scenario, SerializationType format, String resultsFile, String logFile, String dataDir)
  {
    listener.origin = resultsFile.substring(resultsFile.lastIndexOf("/")+1,resultsFile.lastIndexOf("."));
    return engine.nativeExecuteScenario(engine.nativeObj, scenario, format.value(), resultsFile, logFile, dataDir);
  }
  
  protected PulseEngine engine;
}

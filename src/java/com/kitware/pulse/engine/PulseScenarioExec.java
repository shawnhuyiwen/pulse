/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.engine;

import com.kitware.pulse.cdm.bind.Scenario.eSerializationFormat;
import com.kitware.pulse.cdm.scenario.SEScenarioExec;
import com.kitware.pulse.engine.bind.Enums.eModelType;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.LogListener;

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
    this("./", eModelType.HumanAdultWholeBody);
  }
  public PulseScenarioExec(eModelType m)
  {
    this("./", m);
  }
  public PulseScenarioExec(String dataDir)
  {
    this(dataDir, eModelType.HumanAdultWholeBody);
  }
  public PulseScenarioExec(String dataDir, eModelType m)
  {
    engine=new PulseEngine(dataDir, m);
    engine.setLogListener(listener);
  }
  
  public void finalize()
  {
    engine.cleanUp();
    engine = null;
  }
  
  public boolean runScenario(SEScenarioExec execOpts)
  {
    String resultsFile = execOpts.getDataRequestCSVFilename();
    if(resultsFile.isEmpty())
      listener.origin = "Scenario";
    else
      listener.origin = resultsFile.substring(resultsFile.lastIndexOf("/")+1,resultsFile.lastIndexOf("."));
    
    return engine.nativeExecuteScenario(engine.nativeObj, execOpts.toJSON(), eSerializationFormat.JSON_VALUE);
  }
  
  protected PulseEngine engine;
}

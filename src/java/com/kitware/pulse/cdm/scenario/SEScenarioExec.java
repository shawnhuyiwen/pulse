/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.scenario;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;
import com.kitware.pulse.cdm.bind.Enums.eSwitch;
import com.kitware.pulse.cdm.bind.Scenario.ScenarioExecData;
import com.kitware.pulse.cdm.bind.Scenario.eSerializationFormat;
import com.kitware.pulse.utilities.Log;

public class SEScenarioExec
{
  protected eSwitch logToConsole;
  protected String  logFilename;
  protected String  dataRequestCSVFilename;
  protected String  dataRootDirectory;

  protected String  serializationDirectory;
  protected String  autoSerializeFilename;
  protected eSwitch autoSerializeAfterActions;
  protected double  autoSerializePeriod_s;
  protected eSwitch timeStampSerializedStates;
  protected eSwitch reloadSerializedState;

  protected String  engineConfigurationContent;
  protected String  engineConfigurationFilename;

  protected String  scenarioContent;
  protected String  scenarioFilename;

  protected eSerializationFormat  contentFormat;
  
  public SEScenarioExec()
  {
    clear();
  }
  
  public void clear()
  {
    logToConsole = eSwitch.Off;
    logFilename = "";
    dataRequestCSVFilename = "";
    dataRootDirectory = "./";
    
    serializationDirectory = "./";
    autoSerializeFilename = "";
    autoSerializeAfterActions = eSwitch.Off;
    autoSerializePeriod_s = 0;
    timeStampSerializedStates = eSwitch.On;
    reloadSerializedState = eSwitch.On;
    
    engineConfigurationContent = "";
    engineConfigurationFilename = "";
    
    scenarioContent = "";
    scenarioFilename = "";
    
    contentFormat = eSerializationFormat.JSON;
  }
  
  public void copy(SEScenarioExec src)
  {
    SEScenarioExec.load(SEScenarioExec.unload(src), this);
  }
  
  public static void load(ScenarioExecData src, SEScenarioExec dst)
  {
    dst.clear();
    dst.logToConsole = src.getLogToConsole();
    if(src.getLogFilename()!=null)
      dst.logFilename = src.getLogFilename();
    if(src.getDataRequestCSVFilename()!=null)
      dst.dataRequestCSVFilename = src.getDataRequestCSVFilename();
    if(src.getDataRootDirectory()!=null)
      dst.dataRootDirectory = src.getDataRootDirectory();
    
    if(src.getSerializationDirectory()!=null)
      dst.serializationDirectory = src.getSerializationDirectory();
    if(src.getAutoSerializeFilename()!=null)
      dst.autoSerializeFilename = src.getAutoSerializeFilename();
    dst.autoSerializeAfterActions = src.getAutoSerializeAfterActions();
    dst.autoSerializePeriod_s = src.getAutoSerializePeriodS();
    dst.timeStampSerializedStates = src.getTimeStampSerializedStates();
    dst.reloadSerializedState = src.getReloadSerializedState();
    
    if(src.getEngineConfigurationContent()!=null)
      dst.engineConfigurationContent = src.getEngineConfigurationContent();
    else if(src.getEngineConfigurationFilename()!=null)
      dst.engineConfigurationFilename = src.getEngineConfigurationFilename();
    
    if(src.getScenarioContent()!=null)
      dst.scenarioContent = src.getScenarioContent();
    else if(src.getEngineConfigurationFilename()!=null)
      dst.engineConfigurationFilename = src.getEngineConfigurationFilename();
    
    dst.contentFormat = src.getContentFormat();
  }
  public static ScenarioExecData unload(SEScenarioExec src)
  {
    ScenarioExecData.Builder dst = ScenarioExecData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEScenarioExec src, ScenarioExecData.Builder dst)
  {
    dst.setLogToConsole(src.logToConsole);
    if (!src.logFilename.isEmpty())
      dst.setLogFilename(src.logFilename);
    if (!src.dataRequestCSVFilename.isEmpty())
      dst.setDataRequestCSVFilename(src.dataRequestCSVFilename);
    if (!src.dataRootDirectory.isEmpty())
      dst.setDataRootDirectory(src.dataRootDirectory);

    if (!src.serializationDirectory.isEmpty())
      dst.setSerializationDirectory(src.serializationDirectory);
    if (!src.autoSerializeFilename.isEmpty())
      dst.setAutoSerializeFilename(src.autoSerializeFilename);
    dst.setAutoSerializeAfterActions(src.autoSerializeAfterActions);
    dst.setAutoSerializePeriodS(src.autoSerializePeriod_s);
    dst.setTimeStampSerializedStates(src.timeStampSerializedStates);
    dst.setReloadSerializedState(src.reloadSerializedState);
    
    if (!src.engineConfigurationContent.isEmpty())
      dst.setEngineConfigurationContent(src.engineConfigurationContent);
    else if (!src.engineConfigurationFilename.isEmpty())
      dst.setEngineConfigurationFilename(src.engineConfigurationFilename);
    
    if (!src.scenarioContent.isEmpty())
      dst.setScenarioContent(src.scenarioContent);
    else if (!src.scenarioFilename.isEmpty())
      dst.setScenarioFilename(src.scenarioFilename);
    
    dst.setContentFormat(src.contentFormat);
  }
  
  public String toJSON() 
  {
    String json;
    try
    {
      json = JsonFormat.printer().print(SEScenarioExec.unload(this));
    }
    catch (InvalidProtocolBufferException e)
    {
      Log.error("Unable to generate json from SEScenarioExec");
      json = "";
    }
    return json;
  }
  
  public eSwitch getLogToConsole() { return logToConsole; }
  public void setLogToConsole(eSwitch s) { logToConsole=s; }

  public String getLogFilename() { return logFilename; }
  public void setLogFilename(String s) { logFilename=s; }

  public String getDataRequestCSVFilename() { return dataRequestCSVFilename; }
  public void setDataRequestCSVFilename(String s) { dataRequestCSVFilename=s; }

  public String getDataRootDirectory() { return dataRootDirectory; }
  public void setDataRootDirectory(String s) { dataRootDirectory=s; }

  public String getSerializationDirectory() { return serializationDirectory; }
  public void setSerializationDirectory(String s) { serializationDirectory=s; }

  public String getAutoSerializeFilename() { return autoSerializeFilename; }
  public void setAutoSerializeFilename(String s) { autoSerializeFilename=s; }

  public eSwitch getAutoSerializeAfterActions() { return autoSerializeAfterActions; }
  public void setAutoSerializeAfterActions(eSwitch s) { autoSerializeAfterActions=s; }

  public double getAutoSerializePeriod_s() { return autoSerializePeriod_s; }
  public void setAutoSerializePeriod_s(double d) { autoSerializePeriod_s=d; }

  public eSwitch getTimeStampSerializedStates() { return timeStampSerializedStates; }
  public void setTimeStampSerializedStates(eSwitch s) { timeStampSerializedStates=s; }

  public eSwitch getReloadSerializedState() { return reloadSerializedState; }
  public void setReloadSerializedState(eSwitch s) { reloadSerializedState=s; }

  public String getEngineConfigurationContent() { return engineConfigurationContent; }
  public void setEngineConfigurationContent(String s) { engineConfigurationContent=s; }

  public String getEngineConfigurationFilename() { return engineConfigurationFilename; }
  public void setEngineConfigurationFilename(String s) { engineConfigurationFilename=s; }

  public String getScenarioContent() { return scenarioContent; }
  public void setScenarioContent(String s) { scenarioContent=s; }

  public String getScenarioFilename() { return scenarioFilename; }
  public void setScenarioFilename(String s) { scenarioFilename=s; }

  public eSerializationFormat getContentFormat() { return contentFormat; }
  public void setContentFormat(eSerializationFormat s) { contentFormat=s; }
  
}

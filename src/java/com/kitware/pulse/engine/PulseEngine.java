/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.engine;

import java.util.ArrayList;
import java.util.List;

import com.google.common.primitives.Doubles;
import com.google.protobuf.util.JsonFormat;
import com.kitware.pulse.SerializationType;

import com.kitware.pulse.cdm.actions.SEAction;
import com.kitware.pulse.cdm.bind.Engine.ActionListData;
import com.kitware.pulse.cdm.bind.Engine.AnyActionData;
import com.kitware.pulse.cdm.bind.Engine.AnyConditionData;
import com.kitware.pulse.cdm.bind.Engine.ConditionListData;
import com.kitware.pulse.cdm.bind.Events.ActiveEventData;
import com.kitware.pulse.cdm.bind.Events.ActiveEventListData;
import com.kitware.pulse.cdm.bind.Events.EventChangeData;
import com.kitware.pulse.cdm.bind.Events.EventChangeListData;
import com.kitware.pulse.cdm.bind.Patient.PatientData;
import com.kitware.pulse.cdm.bind.PatientAssessments.CompleteBloodCountData;
import com.kitware.pulse.cdm.bind.PatientAssessments.ComprehensiveMetabolicPanelData;
import com.kitware.pulse.cdm.bind.PatientAssessments.PulmonaryFunctionTestData;
import com.kitware.pulse.cdm.bind.PatientAssessments.UrinalysisData;
import com.kitware.pulse.cdm.bind.PatientAssessments.ePatientAssessmentType;
import com.kitware.pulse.cdm.conditions.SECondition;
import com.kitware.pulse.cdm.datarequests.SEDataRequestManager;
import com.kitware.pulse.cdm.engine.SEActiveEvent;
import com.kitware.pulse.cdm.engine.SEEventHandler;
import com.kitware.pulse.cdm.engine.SEPatientConfiguration;
import com.kitware.pulse.cdm.patient.assessments.SECompleteBloodCount;
import com.kitware.pulse.cdm.patient.assessments.SEComprehensiveMetabolicPanel;
import com.kitware.pulse.cdm.patient.assessments.SEPatientAssessment;
import com.kitware.pulse.cdm.patient.assessments.SEPulmonaryFunctionTest;
import com.kitware.pulse.cdm.patient.assessments.SEUrinalysis;
import com.kitware.pulse.cdm.properties.SEScalarTime;
import com.kitware.pulse.cdm.properties.CommonUnits.TimeUnit;
import com.kitware.pulse.cdm.patient.SEPatient;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.LogListener;
import com.kitware.pulse.utilities.JNIBridge;

public class PulseEngine
{

  protected boolean        alive  = false;
  protected double         timeStep_s = 0.02;
  protected double         timeRemainder = 0;
  protected LogListener    logListener = null;
  protected SEEventHandler eventHandler = null;

  public PulseEngine()
  {
    this("./");
  }
  public PulseEngine(String dataDir)
  {
    JNIBridge.initialize();
    nativeObj=nativeAllocate(dataDir);
    timeStep_s = nativeGetTimeStep(nativeObj,"s");
  }

  public void finalize()
  {
    this.cleanUp();
  }

  public void cleanUp()
  {
    if(nativeObj!=0)
    {
      nativeDelete(nativeObj);
      nativeObj=0;
    }
    this.alive = false;
    this.logListener = null;
    this.eventHandler = null;
  }
  
  ////////////////////
  // STATE FILE I/O //
  ////////////////////
  
  public boolean serializeFromFile(String stateFile, SEDataRequestManager dataRequests)
  {
    try
    {
      String drString = JsonFormat.printer().print(SEDataRequestManager.unload(dataRequests));
      alive = nativeSerializeFromFile(nativeObj, stateFile, drString, thunkType.value());
    }
    catch(Exception ex)
    {
      if(logListener!=null)
        logListener.error("Unable to serialize state file", ex);
      alive = false;
    }
    return alive;
  }
  public boolean serializeToFile(String stateFile)
  {
    if(!alive)
    {
      Log.error("Engine has not been initialized");
      return alive;
    }
    return nativeSerializeToFile(nativeObj, stateFile);
  }

  ////////////////////////
  // STRING/STREAM I/O //
  ///////////////////////
  
  public boolean serializeFromString(String state, SEDataRequestManager dataRequests)
  {
    try
    {
      String drString = JsonFormat.printer().print(SEDataRequestManager.unload(dataRequests));
      alive = nativeSerializeFromString(nativeObj, state, drString, thunkType.value());
    }
    catch(Exception ex)
    {
      if(logListener!=null)
        logListener.error("Unable to serialize state file", ex);
      alive = false;
    }
    return alive;
  }
  public String serializeToString(String stateFile, SerializationType format)
  {
    if(!alive)
    {
      Log.error("Engine has not been initialized");
      return "";
    }
    return nativeSerializeToString(nativeObj, stateFile, format.value());
  }

  //////////////////////
  // PATIENT CREATION //
  //////////////////////
  
  public boolean initializeEngine(SEPatientConfiguration patientConfiguration, SEDataRequestManager dataRequests)
  {
    try
    {
      String pcString = JsonFormat.printer().print(SEPatientConfiguration.unload(patientConfiguration));
      String drString = JsonFormat.printer().print(SEDataRequestManager.unload(dataRequests));
      alive = nativeInitializeEngine(nativeObj, pcString, drString, thunkType.value());
    }
    catch(Exception ex)
    {
      if(logListener!=null)
        logListener.error("Unable to initialize engine", ex);
      alive = false;
    }
    return alive;
  }

  public boolean getInitialPatient(SEPatient p)
  {
    if(!alive)
    {
      Log.error("Engine has not been initialized");
      return alive;
    }
    try
    {
      PatientData.Builder b = PatientData.newBuilder();
      String str = nativeGetInitialPatient(nativeObj, thunkType.value());
      JsonFormat.parser().merge(str, b);
      SEPatient.load(b.build(),p);
    }
    catch(Exception ex)
    {
      if(logListener!=null)
        logListener.error("Unable to get initial patient", ex);
      return false;
    }
    return true;
  }
  
  public boolean getConditions(List<SECondition> conditions)
  {
    if(!alive)
    {
      Log.error("Engine has not been initialized");
      return alive;
    }
    try
    {
      conditions.clear();
      ConditionListData.Builder b = ConditionListData.newBuilder();
      String str = nativeGetConditions(nativeObj, thunkType.value());
      JsonFormat.parser().merge(str, b);
      for(AnyConditionData any : b.getAnyConditionList())
        conditions.add(SECondition.ANY2CDM(any));
    }
    catch(Exception ex)
    {
      if(logListener!=null)
        logListener.error("Unable to get conditions", ex);
      return false;
    }
    return true;
  }
  
  public boolean getPatientAssessment(SEPatientAssessment assessment)
  {
    if(!alive)
    {
      Log.error("Engine has not been initialized");
      return alive;
    }
    try
    {
      if(assessment instanceof SEPulmonaryFunctionTest)
      {
        PulmonaryFunctionTestData.Builder b = PulmonaryFunctionTestData.newBuilder();
        String str = nativeGetAssessment(nativeObj, ePatientAssessmentType.PulmonaryFunctionTest.ordinal(), thunkType.value());
        JsonFormat.parser().merge(str, b);
        SEPulmonaryFunctionTest.load(b.build(),((SEPulmonaryFunctionTest)assessment));
        return true;
      }
      
      if(assessment instanceof SECompleteBloodCount)
      {
        CompleteBloodCountData.Builder b = CompleteBloodCountData.newBuilder();
        String str = nativeGetAssessment(nativeObj, ePatientAssessmentType.CompleteBloodCount.ordinal(), thunkType.value());
        JsonFormat.parser().merge(str, b);
        SECompleteBloodCount.load(b.build(),((SECompleteBloodCount)assessment));
        return true;
      }
      
      if(assessment instanceof SEComprehensiveMetabolicPanel)
      {
        ComprehensiveMetabolicPanelData.Builder b = ComprehensiveMetabolicPanelData.newBuilder();
        String str = nativeGetAssessment(nativeObj, ePatientAssessmentType.ComprehensiveMetabolicPanel.ordinal(), thunkType.value());
        JsonFormat.parser().merge(str, b);
        SEComprehensiveMetabolicPanel.load(b.build(),((SEComprehensiveMetabolicPanel)assessment));
        return true;
      }
      
      if(assessment instanceof SEUrinalysis)
      {
        UrinalysisData.Builder b = UrinalysisData.newBuilder();
        String str = nativeGetAssessment(nativeObj, ePatientAssessmentType.Urinalysis.ordinal(), thunkType.value());
        JsonFormat.parser().merge(str, b);
        SEUrinalysis.load(b.build(),((SEUrinalysis)assessment));
        return true;
      }
    }
    catch(Exception ex)
    {
      if(logListener!=null)
        logListener.error("Unable to get patient assessment", ex);
    }
    return false;
  }
  
  /////////////////////
  // ADVANCE SUPPORT //
  /////////////////////
  
  public synchronized boolean advanceTime()
  {
    if(!alive)
    {
      Log.error("Engine has not been initialized");
      return alive;
    }
    if(!nativeAdvanceTimeStep(nativeObj))
    {
      Log.error("Engine could not advance time");
      alive=false;
    }
    // Grab any event changes and pass them to handler
    if (eventHandler != null)
    {
      String evts = nativePullEvents(nativeObj, thunkType.value());
      if(evts!=null && !evts.isEmpty())
      {
        try
        {
          SEScalarTime time = new SEScalarTime();
          EventChangeListData.Builder b = EventChangeListData.newBuilder();
          JsonFormat.parser().merge(evts, b);
          for(EventChangeData change : b.getChangeList())
          {
            time.invalidate();
            if(change.hasSimTime())
              SEScalarTime.load(change.getSimTime(), time);
            eventHandler.handleEvent(change.getEvent(), change.getActive(), time);
          }
        }
        catch(Exception ex)
        {
          Log.error("Unable to process log messages", ex);
        }
      }
    }
    return alive;
  }
  
  public synchronized boolean advanceTime(SEScalarTime time)
  {
    if(!alive)
    {
      Log.error("Engine has not been initialized");
      return alive;
    }
    timeRemainder += time.getValue(TimeUnit.s);
    int cnt = (int)(timeRemainder/timeStep_s);
    timeRemainder = timeRemainder - (timeStep_s*cnt);
    for(int i=0; i<cnt; i++)
    {
      if(!advanceTime())
        return alive;
    }
    return true;
  }
  
  public List<Double> pullData()
  {
    if(!alive)
    {
      Log.error("Engine has not been initialized");
      return null;
    }
    return Doubles.asList(nativePullData(nativeObj));
  }
  
  ////////////////////
  // ACTION SUPPORT //
  ////////////////////
  
  public synchronized boolean processAction(SEAction action)
  {
    List <SEAction> actions = new ArrayList<>();
    actions.add(action);
    return processActions(actions);
  }
  public synchronized boolean processActions(List<SEAction> actions)
  {
    if(!alive)
    {
      Log.error("Engine has not been initialized");
      return alive;
    }
    if(actions !=null && !actions.isEmpty())
    {
      ActionListData.Builder aData = ActionListData.newBuilder();
      for(SEAction a : actions)
        aData.addAnyAction(SEAction.CDM2ANY(a));
      try
      {
        String actionsStr = JsonFormat.printer().print(aData);
        if(!nativeProcessActions(nativeObj,actionsStr, thunkType.value()))
        {
          Log.error("Engine could not process actions");
          alive=false;
        }
      }
      catch(Exception ex)
      {
        Log.error("Unable to convert action to json",ex);
        alive = false;
      }
    }
    return alive;
  }
  
  public boolean getActiveActions(List<SEAction> actions)
  {
    if(!alive)
    {
      Log.error("Engine has not been initialized");
      return alive;
    }
    try
    {
      actions.clear();
      ActionListData.Builder b = ActionListData.newBuilder();
      String str = nativePullActiveActions(nativeObj, thunkType.value());
      JsonFormat.parser().merge(str, b);
      for(AnyActionData any : b.getAnyActionList())
        actions.add(SEAction.ANY2CDM(any));
    }
    catch(Exception ex)
    {
      if(logListener!=null)
        logListener.error("Unable to get active actions", ex);
      return false;
    }
    return true;
  }
  
  ///////////////////////
  // LISTENER/HANDLERS //
  ///////////////////////
  
  public void setLogListener(LogListener listener)
  {
    logListener = listener;
    nativeForwardLogMessages(nativeObj, logListener!=null);
  }
  public void setLogFilename(String logFilename)
  {
    nativeSetLogFilename(nativeObj, logFilename);
  }
  
  protected void LogDebug(String msg, String origin)
  {
    if(this.logListener!=null)
      this.logListener.debug(msg, origin);
  }
  protected void LogInfo(String msg, String origin)
  {
    if(this.logListener!=null)
      this.logListener.info(msg, origin);
  }
  protected void LogWarning(String msg, String origin)
  {
    if(this.logListener!=null)
      this.logListener.warn(msg, origin);
  }
  protected void LogError(String msg, String origin)
  {
    if(this.logListener!=null)
      this.logListener.error(msg, origin);
  }
  protected void LogFatal(String msg, String origin)
  {
    if(this.logListener!=null)
      this.logListener.fatal(msg, origin);
  }

  
  public void setEventHandler(SEEventHandler eh)
  {
    this.eventHandler = eh;
    nativeKeepEventChanges(nativeObj, eventHandler!=null);
  }
  
  public void getActiveEvents(List<SEActiveEvent> activeEvents)
  {
    try
    {
      activeEvents.clear();
      String aEvents = nativePullActiveEvents(nativeObj, thunkType.value());
      if(aEvents == null || aEvents.isEmpty())
        return;
      ActiveEventListData.Builder b = ActiveEventListData.newBuilder();
      JsonFormat.parser().merge(aEvents, b);
      for(ActiveEventData ae : b.getActiveEventList())
      {
        SEActiveEvent active = new SEActiveEvent();
        active.type = ae.getEvent();
        SEScalarTime.load(ae.getDuration(), active.duration);
        activeEvents.add(active);
      }
    }
    catch(Exception ex)
    {
      Log.error("Unable to pull active events",ex);
    }
    
  }
  
  ////////////
  // NATIVE //
  ////////////
  
  protected long nativeObj;
  protected SerializationType thunkType = SerializationType.JSON;
  protected native long nativeAllocate(String dataDir);
  protected native void nativeDelete(long nativeObj);

  protected native double nativeGetTimeStep(long nativeObj, String unit);
  
  protected native boolean nativeSerializeFromFile(long nativeObj, String stateFile, String dataRequests, int dataRequestsFormat);
  protected native boolean nativeSerializeToFile(long nativeObj, String stateFile);
  
  protected native boolean nativeSerializeFromString(long nativeObj, String state, String dataRequests, int format);
  protected native String  nativeSerializeToString(long nativeObj, String stateFile, int format);
  
  protected native boolean nativeInitializeEngine(long nativeObj, String patient_configuration, String dataRequests, int thunk_format);
  
  protected native void nativeLogToConsole(long nativeObj, boolean b);
  protected native void nativeSetLogFilename(long nativeObj, String filename);
  protected native void nativeForwardLogMessages(long nativeObj, boolean b);
  
  protected native void nativeKeepEventChanges(long nativeObj, boolean b);
  protected native String nativePullEvents(long nativeObj, int format);
  protected native String nativePullActiveEvents(long nativeObj, int format);
  
  protected native boolean nativeProcessActions(long nativeObj, String actions, int format);
  protected native String  nativePullActiveActions(long nativeObj, int format);
  
  protected native boolean nativeAdvanceTimeStep(long nativeObj);
  protected native double[] nativePullData(long nativeObj);
  
  protected native String nativeGetInitialPatient(long nativeObj, int format);
  protected native String nativeGetConditions(long nativeObj, int format);
  protected native String nativeGetAssessment(long nativeObj, int type, int format);
  
  protected native boolean nativeExecuteScenario(long nativeObj, String sceOpts, int format);
}

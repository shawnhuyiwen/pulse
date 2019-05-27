/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.physiology.pulse.engine;

import java.util.ArrayList;
import java.util.List;

import com.google.protobuf.*;
import com.google.protobuf.util.*;
import com.kitware.physiology.cdm.PatientAssessments.CompleteBloodCountData;
import com.kitware.physiology.cdm.PatientAssessments.ComprehensiveMetabolicPanelData;
import com.kitware.physiology.cdm.PatientAssessments.PulmonaryFunctionTestData;
import com.kitware.physiology.cdm.PatientAssessments.UrinalysisData;
import com.kitware.physiology.cdm.PatientAssessments.ePatientAssessmentType;
import com.kitware.physiology.cdm.Engine.ActionListData;
import com.kitware.physiology.pulse.PulseState.StateData;

import com.kitware.physiology.datamodel.actions.SEAction;
import com.kitware.physiology.datamodel.datarequests.SEDataRequestManager;
import com.kitware.physiology.datamodel.engine.SEPatientConfiguration;
import com.kitware.physiology.datamodel.patient.assessments.SECompleteBloodCount;
import com.kitware.physiology.datamodel.patient.assessments.SEComprehensiveMetabolicPanel;
import com.kitware.physiology.datamodel.patient.assessments.SEPatientAssessment;
import com.kitware.physiology.datamodel.patient.assessments.SEPulmonaryFunctionTest;
import com.kitware.physiology.datamodel.patient.assessments.SEUrinalysis;
import com.kitware.physiology.datamodel.properties.CommonUnits.TimeUnit;
import com.kitware.physiology.datamodel.properties.SEScalarTime;
import com.kitware.physiology.utilities.Log;

public class PulseEngine extends Pulse
{
  protected boolean deadEngine = false;
  
  public PulseEngine()
  {
    super();
  }
  
  public synchronized void reset()
  {
    super.reset();
    if(this.nativeObj!=0)
      this.nativeReset(this.nativeObj);
    this.deadEngine = false;
  }
  
  public synchronized boolean isDead()
  {
    return deadEngine;
  }

  // TODO Set a callback for patient events
  
  public synchronized boolean serializeFromFile(String logFile, String stateFile, SEDataRequestManager dataRequests)
  {    
    return loadStateContents(logFile, stateFile, -1.0, dataRequests);
  }  
  public synchronized boolean serializeFromFile(String logFile, String stateFile, SEScalarTime simTime, SEDataRequestManager dataRequests)
  {        
    return loadStateContents(logFile, stateFile, simTime.getValue(TimeUnit.s), dataRequests);
  }  
  protected synchronized boolean loadStateContents(String logFile, String stateFile, double simTime_s, SEDataRequestManager dataRequests)
  {
    this.reset();
    String dataRequestsStr = null;
    if(dataRequests !=null && !dataRequests.getRequestedData().isEmpty())
      try {
        dataRequestsStr = JsonFormat.printer().print(SEDataRequestManager.unload(dataRequests));
       } catch (Exception ex) { Log.error("Could not convert Data Requests provided to json",ex); return false; }
    if(dataRequestsStr == null)
    {
      Log.error("Invalid/No data requests provided");
      return false;
    }
    this.requestData(dataRequests);
    this.nativeObj = nativeAllocate(logFile);
    return nativeSerializeFromFile(this.nativeObj, stateFile, simTime_s, dataRequestsStr);
  }
  
  public synchronized StateData serializeToFile(String stateFile) throws InvalidProtocolBufferException
  {
    String str = nativeSerializeToFile(this.nativeObj, stateFile);
    StateData.Builder sd = StateData.newBuilder();
    JsonFormat.parser().merge(str, sd);
    return sd.build();
  }

  public synchronized boolean initializeEngine(String logFile, SEPatientConfiguration patient_configuration, SEDataRequestManager dataRequests)
  {    
    this.reset();
    
    String patient_configurationStr;
    try { 
      patient_configurationStr =  JsonFormat.printer().print(SEPatientConfiguration.unload(patient_configuration));
    } catch (Exception ex) { Log.error("Could not convert configuration provided to json",ex); return false; }
    if(patient_configurationStr == null || patient_configurationStr.isEmpty())
    {
      Log.error("Invalid/No patient configuration provided");
      return false;
    }
    String dataRequestsStr = null;
    if(dataRequests !=null && !dataRequests.getRequestedData().isEmpty())
      try {
    	 dataRequestsStr = JsonFormat.printer().print(SEDataRequestManager.unload(dataRequests));
      } catch (Exception ex) { Log.error("Could not convert Data Requests provided to json",ex); return false; }
    if(dataRequestsStr == null)
    {
      Log.error("Invalid/No data requests provided");
      return false;
    }
    this.requestData(dataRequests);
    this.nativeObj = nativeAllocate(logFile);
    this.deadEngine = !nativeInitializeEngine(this.nativeObj, patient_configurationStr, dataRequestsStr);
    if(this.deadEngine)
      Log.error("Unable to initialize engine");
    return !this.deadEngine;
  }
  
  public synchronized boolean advanceTime()
  {
    if(this.deadEngine)
    {
      Log.error("Engine has died");
      return false;
    }
    if(!nativeAdvanceTimeStep(this.nativeObj))
      deadEngine=true;
    return !deadEngine;
  }
  
  public synchronized boolean advanceTime(SEScalarTime time)
  {
    if(this.deadEngine)
    {
      Log.error("Engine has died");
      return false;
    }
    if(!nativeAdvanceTime(this.nativeObj, time.getValue(TimeUnit.s)))
      deadEngine=true;
    return !deadEngine;
  }
  
  public synchronized boolean processAction(SEAction action)
  {
    List <SEAction> actions = new ArrayList<SEAction>(); 
    actions.add(action);
    return processActions(actions);
  }
  
  public synchronized boolean processActions(List<SEAction> actions)
  {
    if(this.deadEngine)
    {
      Log.error("Engine has died");
      return false;
    }
    if(actions !=null && !actions.isEmpty())
    {
      ActionListData.Builder aData = ActionListData.newBuilder();
      for(SEAction a : actions)
      	aData.addAnyAction(SEAction.CDM2ANY(a));
      try 
      {
        String actionsStr = JsonFormat.printer().print(aData);
        if(!nativeProcessActions(this.nativeObj,actionsStr))
          deadEngine=true;
        return !deadEngine;
      }
      catch(Exception ex)
      {
        Log.error("Unable to convert action to json",ex);
        return false;
      }
      
    }
    return true;
  }
  
  public synchronized boolean getPatientAssessment(SEPatientAssessment assessment) throws InvalidProtocolBufferException
  {
    if(this.deadEngine)
    {
      Log.error("Engine has died");
      return false;
    }
    if(assessment instanceof SEPulmonaryFunctionTest)
    {
    	PulmonaryFunctionTestData.Builder b = PulmonaryFunctionTestData.newBuilder();
      String str = nativeGetAssessment(this.nativeObj, ePatientAssessmentType.PulmonaryFunctionTest.ordinal());
      JsonFormat.parser().merge(str, b);
      SEPulmonaryFunctionTest.load(b.build(),((SEPulmonaryFunctionTest)assessment));
      return true;
    }
    
    if(assessment instanceof SECompleteBloodCount)
    {
    	CompleteBloodCountData.Builder b = CompleteBloodCountData.newBuilder();
      String str = nativeGetAssessment(this.nativeObj, ePatientAssessmentType.CompleteBloodCount.ordinal());
      JsonFormat.parser().merge(str, b);
      SECompleteBloodCount.load(b.build(),((SECompleteBloodCount)assessment));
      return true;
    }
    
    if(assessment instanceof SEComprehensiveMetabolicPanel)
    {
    	ComprehensiveMetabolicPanelData.Builder b = ComprehensiveMetabolicPanelData.newBuilder();
      String str = nativeGetAssessment(this.nativeObj, ePatientAssessmentType.ComprehensiveMetabolicPanel.ordinal());
      JsonFormat.parser().merge(str, b);
      SEComprehensiveMetabolicPanel.load(b.build(),((SEComprehensiveMetabolicPanel)assessment));
      return true;
    }
    
    if(assessment instanceof SEUrinalysis)
    {
    	UrinalysisData.Builder b = UrinalysisData.newBuilder();
      String str = nativeGetAssessment(this.nativeObj, ePatientAssessmentType.Urinalysis.ordinal());
      JsonFormat.parser().merge(str, b);
      SEUrinalysis.load(b.build(),((SEUrinalysis)assessment));
      return true;
    }
    
    return false;
  }
  
  /**
   * Used for C++ communication for calculation outside of the java language
   * @return - success flag from calculate
   */
  protected native void nativeReset(long nativeObj);
  
  protected native boolean nativeInitializeEngine(long nativeObj, String patient_configuration, String dataRequests);
  protected native boolean nativeSerializeFromFile(long nativeObj, String stateFile, double simTime_s, String dataRequests);// pass <0 as simTime to use the time in the file
  protected native String  nativeSerializeToFile(long nativeObj, String stateFile);
  protected native boolean nativeAdvanceTimeStep(long nativeObj);
  protected native boolean nativeAdvanceTime(long nativeObj, double time_s);
  protected native boolean nativeProcessActions(long nativeObj, String actions);
  protected native String nativeGetAssessment(long nativeObj, int type);
}

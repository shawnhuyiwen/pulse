/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.engine;

import java.util.ArrayList;
import java.util.List;

import com.google.protobuf.*;
import com.google.protobuf.util.*;
import pulse.cdm.bind.PatientAssessments.CompleteBloodCountData;
import pulse.cdm.bind.PatientAssessments.ComprehensiveMetabolicPanelData;
import pulse.cdm.bind.PatientAssessments.PulmonaryFunctionTestData;
import pulse.cdm.bind.PatientAssessments.UrinalysisData;
import pulse.cdm.bind.PatientAssessments.ePatientAssessmentType;
import pulse.cdm.bind.Engine.ActionListData;
import pulse.cpm.bind.PulseState.StateData;

import pulse.cdm.actions.SEAction;
import pulse.cdm.datarequests.SEDataRequestManager;
import pulse.cdm.engine.SEPatientConfiguration;
import pulse.cdm.patient.assessments.SECompleteBloodCount;
import pulse.cdm.patient.assessments.SEComprehensiveMetabolicPanel;
import pulse.cdm.patient.assessments.SEPatientAssessment;
import pulse.cdm.patient.assessments.SEPulmonaryFunctionTest;
import pulse.cdm.patient.assessments.SEUrinalysis;
import pulse.cdm.properties.CommonUnits.TimeUnit;
import pulse.cdm.properties.SEScalarTime;
import pulse.utilities.Log;

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
    return loadStateContents(logFile, stateFile, -1.0, dataRequests, "./");
  }
  public synchronized boolean serializeFromFile(String logFile, String stateFile, SEDataRequestManager dataRequests, String dataDir)
  {
    return loadStateContents(logFile, stateFile, -1.0, dataRequests, dataDir);
  }
  public synchronized boolean serializeFromFile(String logFile, String stateFile, SEScalarTime simTime, SEDataRequestManager dataRequests)
  {
    return loadStateContents(logFile, stateFile, simTime.getValue(TimeUnit.s), dataRequests, "./");
  }  
  public synchronized boolean serializeFromFile(String logFile, String stateFile, SEScalarTime simTime, SEDataRequestManager dataRequests, String dataDir)
  {
    return loadStateContents(logFile, stateFile, simTime.getValue(TimeUnit.s), dataRequests, dataDir);
  }
  protected synchronized boolean loadStateContents(String logFile, String stateFile, double simTime_s, SEDataRequestManager dataRequests, String dataDir)
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
    this.nativeObj = nativeAllocate();
    nativeSetLogFilename(this.nativeObj, logFile);
    return nativeSerializeFromFile(this.nativeObj, stateFile, dataRequestsStr, 1);
  }
  
  public synchronized boolean serializeToFile(String stateFile) throws InvalidProtocolBufferException
  {
    return nativeSerializeToFile(this.nativeObj, stateFile, 1);
  }

  public synchronized boolean initializeEngine(String logFile, SEPatientConfiguration patient_configuration, SEDataRequestManager dataRequests)
  {
    return initializeEngine(logFile, patient_configuration, dataRequests, "./");
  }
  public synchronized boolean initializeEngine(String logFile, SEPatientConfiguration patient_configuration, SEDataRequestManager dataRequests, String dataDir)
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
    this.nativeObj = nativeAllocate();
    nativeSetLogFilename(this.nativeObj, logFile);
    this.deadEngine = !nativeInitializeEngine(this.nativeObj, patient_configurationStr, dataRequestsStr, 1, dataDir);
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
  
  protected native boolean nativeSerializeFromFile(long nativeObj, String stateFile, String dataRequests, int format);
  protected native boolean nativeSerializeToFile(long nativeObj, String stateFile, int format);
  
  protected native boolean nativeSerializeFromString(long nativeObj, String state, String dataRequests, int format);
  protected native String  nativeSerializeToString(long nativeObj, String stateFile);
  
  protected native boolean nativeInitializeEngine(long nativeObj, String patient_configuration, String dataRequests, int format, String dataDir);
  
  protected native boolean nativeAdvanceTimeStep(long nativeObj);
  protected native boolean nativeAdvanceTime(long nativeObj, double time_s);
  
  protected native boolean nativeProcessActions(long nativeObj, String actions);
  
  protected native String nativeGetAssessment(long nativeObj, int type);
}

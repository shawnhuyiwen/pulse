/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.engine.testing;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;

import com.kitware.pulse.engine.bind.*;
import com.kitware.pulse.cdm.bind.Enums.eSwitch;
import com.kitware.pulse.cdm.bind.Scenario.ScenarioData;
import com.kitware.pulse.cdm.testing.SETestDriver;
import com.kitware.pulse.cdm.testing.SETestJob;
import com.kitware.pulse.utilities.FileUtils;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.JNIBridge;

public class ScenarioTestDriver implements SETestDriver.Executor
{
  @Override
  public boolean ExecuteTest(SETestJob job)
  {
    String json = FileUtils.readFile(job.scenarioDirectory+"/"+job.name);
    if(json==null)
    {
      Log.error("Could not read file : "+job.scenarioDirectory+"/"+job.name);
      return false;
    }
    ScenarioData.Builder builder;
    Scenario.ScenarioData.Builder pBuilder = Scenario.ScenarioData.newBuilder();
    try
    {
    	JsonFormat.parser().merge(json, pBuilder);
    	builder = pBuilder.getScenarioBuilder();
    }
    catch(InvalidProtocolBufferException ex)
    {
	    try
	    {
	    	builder = pBuilder.getScenarioBuilder();
	    	JsonFormat.parser().merge(json, builder);
	    }
	    catch(InvalidProtocolBufferException ex2)
	    {
	    	Log.error("Unable to read scenario"+job.scenarioDirectory+"/"+job.name,ex2);
	    	return false;
	    }
    }
    builder.setName("./test_results/scenarios/"+job.name.substring(0,job.name.length()-5));
    if(job.patientFile!=null)
    {
      if(builder.hasPatientConfiguration())
      {
      	  builder.getPatientConfigurationBuilder().clearPatient();
          builder.getPatientConfigurationBuilder().setPatientFile(job.patientFile);
      }
      else
      {
      		builder.clearEngineStateFile();
          builder.getPatientConfigurationBuilder().setPatientFile(job.patientFile);
      }      
    }
    if(job.useState && builder.hasPatientConfiguration())
    {
      	String pFile = pBuilder.getScenario().getPatientConfiguration().getPatientFile();
      	pFile =  pFile.substring(0, pFile.indexOf(".json"));
      	pFile = "./states/"+pFile+"@0s.json";
      	builder.clearPatientConfiguration();
      	builder.setEngineStateFile(pFile);
    }
    
    pBuilder.getConfigurationBuilder();
    try 
    {
      json = JsonFormat.printer().print(pBuilder);
    } 
    catch (InvalidProtocolBufferException ex) 
    {
      Log.error("Unable to refactor the scenario",ex);
      return false;
    }
    
    job.execOpts.setLogToConsole(eSwitch.Off);
    job.execOpts.setScenarioContent(json);
    job.execOpts.setLogPrepend(job.name);
    //System.out.println(json);
    job.execOpts.execute();
    Log.info("Completed running "+job.name);
    return true;
  }
  
  public static void main(String[] args)
  {
    JNIBridge.initialize();
    SETestDriver.main(args);
  }
}

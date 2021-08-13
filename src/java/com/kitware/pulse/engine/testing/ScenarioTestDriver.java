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
import com.kitware.pulse.engine.PulseScenarioExec;
import com.kitware.pulse.utilities.FileUtils;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.JNIBridge;

public class ScenarioTestDriver implements SETestDriver.Executor
{
  @Override
  public boolean ExecuteTest(SETestJob job)
  {
    
    String logFilename;
    String resultsFilename;
    String outputFile = job.computedDirectory+"/"+job.name;
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
    
    if(job.patientFile==null)
    {
      logFilename = outputFile.replaceAll("json", "log");
      resultsFilename = outputFile.replaceAll(".json", "Results.csv");
    }
    else
    {
      String patientName = job.patientFile.substring(0,job.patientFile.length()-5);
      logFilename = outputFile.replaceAll(".json", "-"+patientName+".log");
      resultsFilename = outputFile.replaceAll(".json", "-"+patientName+"Results.csv");
      
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
    job.execOpts.setLogFilename(logFilename);
    job.execOpts.setDataRequestCSVFilename(resultsFilename);
    job.execOpts.setScenarioContent(json);
    //System.out.println(json);
    PulseScenarioExec pse = new PulseScenarioExec(job.modelType);
    pse.runScenario(job.execOpts);
    Log.info("Completed running "+job.name);
    pse=null;
    return true;
  }
  
  public static void main(String[] args)
  {
    JNIBridge.initialize();
    SETestDriver.main(args);
  }
}

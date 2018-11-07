/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.physiology.pulse.testing;

import com.google.protobuf.TextFormat;
import com.kitware.physiology.cdm.Engine.EngineInitializationData.Builder;
import com.kitware.physiology.cdm.Scenario.ScenarioData;
import com.kitware.physiology.pulse.*;

import com.kitware.physiology.datamodel.engine.SEAutoSerialization;
import com.kitware.physiology.pulse.engine.PulseScenarioExec;
import com.kitware.physiology.utilities.FileUtils;
import com.kitware.physiology.utilities.jniBridge;
import com.kitware.physiology.utilities.Log;
import com.kitware.physiology.testing.SETestDriver;
import com.kitware.physiology.testing.SETestJob;

public class ScenarioTestDriver implements SETestDriver.Executor
{
  public boolean ExecuteTest(SETestJob job)
  {
    String outputFile = job.computedDirectory+"/"+job.name;
    String log;
    String results;
    String pba = FileUtils.readFile(job.baselineDirectory+"/"+job.name);
    if(pba==null)
    {
      Log.error("Could not read file : "+job.baselineDirectory+"/"+job.name);
      return false;
    }
    ScenarioData.Builder builder;
    Pulse.ScenarioData.Builder pBuilder = Pulse.ScenarioData.newBuilder();
    try
    {
    	TextFormat.getParser().merge(pba, pBuilder);
    	builder = pBuilder.getScenarioBuilder();
    }
    catch(Exception ex)
    {
	    try
	    {
	    	builder = pBuilder.getScenarioBuilder();
	    	TextFormat.getParser().merge(pba, builder);
	    }
	    catch(Exception ex2)
	    {
	    	Log.error("Unable to read scenario"+job.baselineDirectory+"/"+job.name,ex2);
	    	return false;
	    }
    }
    if(!builder.hasStartType())
    {
    	Log.error("Scenario does not have a start type");
    	return false;
    }
    
    if(job.patientFile==null)
    {
      log = outputFile.replaceAll("pba", "log");
      results = outputFile.replaceAll(".pba", "Results.csv");
    }
    else
    {
      String patientName = job.patientFile.substring(0,job.patientFile.length()-4);
      log = outputFile.replaceAll(".pba", "-"+patientName+".log");
      results = outputFile.replaceAll(".pba", "-"+patientName+"Results.csv");
      
      if(builder.getStartType().hasInitialParameters())      
      {
      	  builder.getStartTypeBuilder().getInitialParametersBuilder().clearPatient();
          builder.getStartTypeBuilder().getInitialParametersBuilder().setPatientFile(job.patientFile);
      }
      else
      {
      		builder.getStartTypeBuilder().clearEngineStateFile();
          builder.getStartTypeBuilder().getInitialParametersBuilder().setPatientFile(job.patientFile);
      }      
    }
    if(job.useState && builder.getStartType().hasInitialParameters())      
    {
      	String pFile = pBuilder.getScenario().getStartType().getInitialParameters().getPatientFile();
      	pFile =  pFile.substring(0, pFile.indexOf(".pba"));
      	pFile = "./states/"+pFile+"@0s.pba";
      	builder.getStartTypeBuilder().clearInitialParameters();
      	builder.getStartTypeBuilder().setEngineStateFile(pFile);
    }

    if(job.autoSerialization!=null)
    	pBuilder.getConfigurationBuilder().setAutoSerialization(SEAutoSerialization.unload(job.autoSerialization)); 

    pba = pBuilder.toString();
    //System.out.println(pba);
    PulseScenarioExec pse = new PulseScenarioExec();
    pse.setListener(job);      
    pse.runScenario(log, pba, results);
    Log.info("Completed running "+job.name);
    pse=null;
    return true;
  }
  
  public static void main(String[] args)
  {
    jniBridge.initialize();
    SETestDriver.main(args);
    jniBridge.deinitialize();
  }
}

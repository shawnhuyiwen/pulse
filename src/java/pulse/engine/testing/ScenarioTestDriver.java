/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.engine.testing;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;

import pulse.cdm.bind.Scenario.ScenarioData;
import pulse.cdm.engine.SEAutoSerialization;
import pulse.cdm.testing.SETestDriver;
import pulse.cdm.testing.SETestJob;
import pulse.cpm.bind.Pulse;
import pulse.engine.PulseScenarioExec;
import pulse.utilities.FileUtils;
import pulse.utilities.Log;
import pulse.utilities.jniBridge;

public class ScenarioTestDriver implements SETestDriver.Executor
{
  @Override
  public boolean ExecuteTest(SETestJob job)
  {
    String outputFile = job.computedDirectory+"/"+job.name;
    String log;
    String results;
    String json = FileUtils.readFile(job.scenarioDirectory+"/"+job.name);
    if(json==null)
    {
      Log.error("Could not read file : "+job.scenarioDirectory+"/"+job.name);
      return false;
    }
    ScenarioData.Builder builder;
    Pulse.ScenarioData.Builder pBuilder = Pulse.ScenarioData.newBuilder();
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
      log = outputFile.replaceAll("json", "log");
      results = outputFile.replaceAll(".json", "Results.csv");
    }
    else
    {
      String patientName = job.patientFile.substring(0,job.patientFile.length()-5);
      log = outputFile.replaceAll(".json", "-"+patientName+".log");
      results = outputFile.replaceAll(".json", "-"+patientName+"Results.csv");
      
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

    if(job.autoSerialization!=null)
    	pBuilder.getConfigurationBuilder().setAutoSerialization(SEAutoSerialization.unload(job.autoSerialization));

    try 
    {
      json = JsonFormat.printer().print(pBuilder);
    } 
    catch (InvalidProtocolBufferException ex) 
    {
      Log.error("Unable to refactor the scenario",ex);
      return false;
    }
    //System.out.println(json);
    PulseScenarioExec pse = new PulseScenarioExec();
    pse.setListener(job);
    pse.runScenario(log, json, results);
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

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.howto;

import com.kitware.pulse.cdm.actions.SEAdvanceTime;
import com.kitware.pulse.cdm.bind.Engine.DataRequestData.eCategory;
import com.kitware.pulse.cdm.datarequests.SEDataRequest;
import com.kitware.pulse.cdm.datarequests.SEDataRequestManager;
import com.kitware.pulse.cdm.properties.CommonUnits.FrequencyUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.TimeUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.VolumeUnit;
import com.kitware.pulse.cdm.scenario.SEScenario;
import com.kitware.pulse.cdm.scenario.SEScenarioExec;
import com.kitware.pulse.engine.PulseScenarioExec;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.LogListener;
import com.kitware.pulse.utilities.RunConfiguration;
import com.kitware.pulse.utilities.JNIBridge;

public class HowTo_RunScenario
{
  
  // Create a listener that will catch any messages logged in C++
  // This class will take the messages and add them to the log created in Java
  protected static class MyListener extends LogListener
  {
    protected MyListener()
    {
      super();
      // Since we are just pushing this data into the log
      // We set listen to false, when you create a LogListener
      // is will automatically register itself with the static Log class,
      // and when ever a Log even class is called, this will be called
      // and since this just calls the Log to log you will get into a recursive infinite loop.
      // This is just because I use the LogListener interface to listen to any log messages coming
      // from C++. Technically it is a LogListener as it is 'listening' to log events in C++.
      // At any rate the Java side will receive log events from C++, and you create your own 
      // LogListener so you can do as you see fit with those messages. BUT if you do want to 
      // push those messages to the Java Log, you will need this class to not listen so you
      // don't get into an infinite recursive loop
      listen(false);
    }    
    @Override
    public void handleDebug(String msg) { Log.debug(msg); }
    @Override
    public void handleInfo(String msg)  { Log.info(msg); }
    @Override
    public void handleWarn(String msg)  { Log.warn(msg); }
    @Override
    public void handleError(String msg) { Log.error(msg); }
    @Override
    public void handleFatal(String msg) { Log.fatal(msg); }
  }
  
  public static void main(String[] args)
  {
    // Load all Pulse native libraries
    JNIBridge.initialize();
    example();
  }
  
  public static void example()
  {
    RunConfiguration cfg = new RunConfiguration();
    SEScenarioExec execOpts = new SEScenarioExec();
    // Load and run a scenario
    PulseScenarioExec pse = new PulseScenarioExec();
    execOpts.setDataRequestCSVFilename("./test_results/howto/scenarios/patient/BasicStandardResults.csv");
    execOpts.setLogFilename("./test_results/howto/scenarios/patient/BasicStandardResults.log");
    execOpts.setScenarioFilename(cfg.getScenarioDirectory()+"/patient/BasicStandard.json");
    pse.runScenario(execOpts);
    
    // Create and run a scenario
    execOpts.reset();
    SEScenario sce = new SEScenario();
    sce.setName("HowTo_StaticEngine");
    sce.setDescription("Simple Scenario to demonstraight building a scenario by the CDM API");
    sce.getPatientConfiguration().setPatientFile("./patients/StandardMale.json");
    // When filling out a data request, units are optional
    // The units will be set to whatever units the engine uses.
    SEDataRequestManager dataRequests = sce.getDataRequestManager();
    dataRequests.createPhysiologyDataRequest("HeartRate", FrequencyUnit.Per_min);
    dataRequests.createPhysiologyDataRequest("TotalLungVolume", VolumeUnit.mL);
    dataRequests.createPhysiologyDataRequest("RespirationRate", FrequencyUnit.Per_min);
    dataRequests.createPhysiologyDataRequest("BloodVolume", VolumeUnit.mL);
    // Let's just run for 2 minutes
    SEAdvanceTime adv = new SEAdvanceTime();
    adv.getTime().setValue(2,TimeUnit.min);
    sce.getActions().add(adv);
    execOpts.setDataRequestCSVFilename("./test_results/HowToStaticEngineResults.csv");
    execOpts.setLogFilename("./test_results/HowToStaticEngineResults.log");
    execOpts.setScenarioContent(sce.toJSON());
    pse.runScenario(execOpts);
  }
}

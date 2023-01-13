/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.howto;

import java.util.List;

import com.kitware.pulse.cdm.bind.Enums.eSide;
import com.kitware.pulse.cdm.bind.Events.eEvent;
import com.kitware.pulse.cdm.datarequests.SEDataRequestManager;
import com.kitware.pulse.cdm.engine.SEEventHandler;
import com.kitware.pulse.cdm.patient.actions.SEHemothorax;
import com.kitware.pulse.cdm.patient.actions.SETubeThoracostomy;
import com.kitware.pulse.cdm.properties.CommonUnits.FrequencyUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.PressureUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.TimeUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.VolumePerTimeUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.VolumeUnit;
import com.kitware.pulse.engine.PulseEngine;
import com.kitware.pulse.cdm.properties.SEScalarTime;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.LogListener;
import com.kitware.pulse.utilities.JNIBridge;

/**
 * How to use the Java interface to the Pulse Physiology Engine
 */
public class HowTo_Hemothorax
{
  public static void main(String[] args)
  {
    example();
  }

  protected static class MyListener extends LogListener
  {
    public boolean error = false;
    public boolean fatal = false;
    protected MyListener()
    {
      super();
      listen(false);
    }
    @Override public void handleDebug(String msg) { Log.debug(msg); }
    @Override public void handleInfo(String msg)  { Log.info(msg); }
    @Override public void handleWarn(String msg)  { Log.warn(msg); }
    @Override public void handleError(String msg) { error=true; Log.error(msg); }
    @Override public void handleFatal(String msg) { fatal=true; Log.fatal(msg); }
  }

  protected static class MyEventHandler implements SEEventHandler
  {
    @Override
    public void handleEvent(eEvent e, boolean active, SEScalarTime time)
    {
      // Here is how you test against an event of interest and do something
      if(e == eEvent.MassiveHemothorax && active)
        Log.info("Patient has Massive Hemothorax "+time);
      else if(e == eEvent.MediumHemothorax && active)
        Log.info("Patient has Medium Hemothorax "+time);
      else if(e == eEvent.MinimalHemothorax && active)
        Log.info("Patient has Minimal Hemothorax "+time);
    }
  }

  public static void example()
  {
    // Create a Pulse Engine
    PulseEngine pe = new PulseEngine();
    MyListener listener = new MyListener();
    pe.setLogListener(listener);
    Log.output2Console = true;
    pe.setEventHandler(new MyEventHandler());
    pe.setLogFilename("./test_results/howto/HowTo_Hemothorax.java.log");

    // Here is how to specify the data to get back from the engine
    SEDataRequestManager dataRequests = new SEDataRequestManager();
    dataRequests.createPhysiologyDataRequest("TidalVolume", VolumeUnit.mL);
    dataRequests.createPhysiologyDataRequest("MeanArterialPressure", PressureUnit.mmHg);
    dataRequests.createPhysiologyDataRequest("SystolicArterialPressure", PressureUnit.mmHg);
    dataRequests.createPhysiologyDataRequest("DiastolicArterialPressure", PressureUnit.mmHg);
    dataRequests.createPhysiologyDataRequest("HeartRate", FrequencyUnit.Per_min);
    dataRequests.createPhysiologyDataRequest("RespirationRate", FrequencyUnit.Per_min);
    dataRequests.createPhysiologyDataRequest("OxygenSaturation");
    dataRequests.createPhysiologyDataRequest("CardiacOutput", VolumePerTimeUnit.mL_Per_min);
    dataRequests.createPhysiologyDataRequest("BloodVolume", VolumeUnit.mL);
    dataRequests.createActionDataRequest("LeftHemothorax", "FlowRate", VolumePerTimeUnit.mL_Per_min);
    dataRequests.createActionDataRequest("LeftHemothorax", "BloodVolume", VolumeUnit.mL);

    List<Double> dataValues;
    pe.serializeFromFile("./states/StandardMale@0s.json", dataRequests);

    dataValues = pe.pullData();
    dataRequests.writeData(dataValues);
    Log.info("");

    // Let's do something to the patient, you can either send actions over one at a time, or pass in a List<SEAction>
    SEHemothorax hemothorax = new SEHemothorax();
    hemothorax.setSide(eSide.Left);
    hemothorax.getSeverity().setValue(0.75);
    pe.processAction(hemothorax);

    pe.advanceTime(new SEScalarTime(120,TimeUnit.s));

    dataValues = pe.pullData();
    dataRequests.writeData(dataValues);

    // Tube thoracostomy should help the patient out
    SETubeThoracostomy tube_thoracostomy = new SETubeThoracostomy();
    tube_thoracostomy.setSide(eSide.Left);
    pe.processAction(tube_thoracostomy);

    pe.advanceTime(new SEScalarTime(400,TimeUnit.s));

    dataValues = pe.pullData();
    dataRequests.writeData(dataValues);
  }
}

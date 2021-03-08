/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.howto;

import java.util.List;
import com.kitware.pulse.cdm.bind.Engine.DataRequestData.eCategory;
import com.kitware.pulse.cdm.bind.Events.eEvent;
import com.kitware.pulse.cdm.bind.PatientActions.HemorrhageData;
import com.kitware.pulse.cdm.datarequests.SEDataRequest;
import com.kitware.pulse.cdm.datarequests.SEDataRequestManager;
import com.kitware.pulse.cdm.engine.SEEventHandler;
import com.kitware.pulse.cdm.patient.actions.SEHemorrhage;
import com.kitware.pulse.cdm.properties.CommonUnits.FrequencyUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.PressureUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.VolumeUnit;
import com.kitware.pulse.cdm.properties.SEScalarTime;
import com.kitware.pulse.engine.PulseCompartments;
import com.kitware.pulse.engine.PulseEngine;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.LogListener;
import com.kitware.pulse.utilities.JNIBridge;

/**
 * How to use the Java interface to the Pulse Physiology Engine
 */
public class HowTo_Hemorrhage
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
      if(e == eEvent.HypovolemicShock && active)
        Log.info("Patient is in shock "+time);
      else if(e == eEvent.CardiovascularCollapse && active)
        Log.info("Patient is in collapse "+time);
    }
  }

  public static void example()
  {
    JNIBridge.initialize();
    // Create a Pulse Engine
    PulseEngine pe = new PulseEngine();
    MyListener listener = new MyListener();
    pe.setLogListener(listener);
    Log.output2Console = true;
    pe.setEventHandler(new MyEventHandler());

    // Here is how to specify the data to get back from the engine
    SEDataRequestManager dataRequests = new SEDataRequestManager();
    SEDataRequest hr = new SEDataRequest();
    hr.setCategory(eCategory.Physiology);
    hr.setPropertyName("HeartRate");
    hr.setUnit(FrequencyUnit.Per_min.toString());
    dataRequests.getRequestedData().add(hr);
    SEDataRequest rr = new SEDataRequest();
    rr.setCategory(eCategory.Physiology);
    rr.setPropertyName("RespirationRate");
    rr.setUnit(FrequencyUnit.Per_min.toString());
    dataRequests.getRequestedData().add(rr);
    SEDataRequest tlv = new SEDataRequest(); 
    tlv.setCategory(eCategory.Physiology);   
    tlv.setPropertyName("TotalLungVolume");
    tlv.setUnit(VolumeUnit.mL.toString());
    dataRequests.getRequestedData().add(tlv);
    SEDataRequest bv = new SEDataRequest();
    bv.setCategory(eCategory.Physiology);
    bv.setPropertyName("BloodVolume");
    bv.setUnit(VolumeUnit.mL.toString());
    dataRequests.getRequestedData().add(bv);
    SEDataRequest map = new SEDataRequest();
    map.setCategory(eCategory.Physiology);
    map.setPropertyName("MeanArterialPressure");
    map.setUnit(PressureUnit.mmHg.toString());
    dataRequests.getRequestedData().add(map);
    
    List<Double> dataValues;
    pe.serializeFromFile("./states/StandardMale@0s.json", dataRequests);
    
    dataValues = pe.pullData();
    Log.info("Simulation Time(s) " + dataValues.get(0));
    Log.info("Heart Rate(bpm) " + dataValues.get(1));
    Log.info("Respiration Rate(bpm) " + dataValues.get(2));
    Log.info("Total Lung Volume(mL) " + dataValues.get(3));
    Log.info("Blood Volume(mL) " + dataValues.get(4));
    Log.info("Mean Arterial Pressure(mmHg) " + dataValues.get(5));
    
    // Let's do something to the patient, you can either send actions over one at a time, or pass in a List<SEAction>
    SEHemorrhage h = new SEHemorrhage();
    
    // Setting up a realistic hemorrhage can be difficult
    // Here is an example of how the engine will act if you create an unrealistic hemorrhage
    h.setType(HemorrhageData.eType.External);
    h.setCompartment(PulseCompartments.Vascular.VenaCava);
    h.getSeverity().setValue(0.8);// This is WAY too much, and will cause the engine to fail
    pe.processAction(h);
    // Hypovolemic Shock will be thrown, but this bleed takes so much blood
    // that we will jump past Cardiovascular Collapse, and straight to irreversible state
    // advance time will then return false, and the engine will no longer do anything on subsequent advance time calls and only return false
    
    for(int i=0; i<=9000; i++)
    {
      if(!pe.advanceTime())
      {
        Log.error("Engine was unable to stay within modeling parameters with requested actions");
        return;
      }
      if (i % 3000 == 0 || listener.error)
      {
        dataValues = pe.pullData();
        Log.info("Simulation Time(s) " + dataValues.get(0));
        Log.info("Heart Rate(bpm) " + dataValues.get(1));
        Log.info("Respiration Rate(bpm) " + dataValues.get(2));
        Log.info("Total Lung Volume(mL) " + dataValues.get(3));
        Log.info("Blood Volume(mL) " + dataValues.get(4));
        Log.info("Mean Arterial Pressure(mmHg) " + dataValues.get(5));
        
        if(listener.error)
        {
          Log.info("I'm getting outta here!");
          break;
        }
      }
    }
    
    pe.cleanUp();
  }
}

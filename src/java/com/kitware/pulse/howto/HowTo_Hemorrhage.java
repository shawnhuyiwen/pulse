/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.howto;

import java.util.List;

import com.kitware.pulse.cdm.bind.Events.eEvent;
import com.kitware.pulse.cdm.bind.PatientActions.HemorrhageData.eCompartment;
import com.kitware.pulse.cdm.bind.PatientActions.HemorrhageData.eType;
import com.kitware.pulse.cdm.engine.SEDataRequestManager;
import com.kitware.pulse.cdm.engine.SEEventHandler;
import com.kitware.pulse.cdm.patient.actions.SEHemorrhage;
import com.kitware.pulse.cdm.properties.CommonUnits.FrequencyUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.MassUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.PressureUnit;
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
    pe.setLogFilename("./test_results/howto/HowTo_Hemorrhage.java.log");

    // Here is how to specify the data to get back from the engine
    SEDataRequestManager dataRequests = new SEDataRequestManager();
    dataRequests.createPhysiologyDataRequest("HeartRate", FrequencyUnit.Per_min);
    dataRequests.createPhysiologyDataRequest("TotalLungVolume", VolumeUnit.mL);
    dataRequests.createPhysiologyDataRequest("RespirationRate", FrequencyUnit.Per_min);
    dataRequests.createPhysiologyDataRequest("BloodVolume", VolumeUnit.mL);

    dataRequests.createPhysiologyDataRequest("CardiacOutput", VolumePerTimeUnit.mL_Per_min);
    dataRequests.createPhysiologyDataRequest("MeanArterialPressure", PressureUnit.mmHg);
    dataRequests.createPhysiologyDataRequest("SystolicArterialPressure", PressureUnit.mmHg);
    dataRequests.createPhysiologyDataRequest("DiastolicArterialPressure", PressureUnit.mmHg);
    dataRequests.createPhysiologyDataRequest("HemoglobinContent", MassUnit.g);
    dataRequests.createPhysiologyDataRequest("TotalHemorrhageRate", VolumePerTimeUnit.mL_Per_s);
    dataRequests.createPhysiologyDataRequest("TotalHemorrhagedVolume", VolumeUnit.mL);
    dataRequests.createActionCompartmentDataRequest("Hemorrhage", "RightLeg", "FlowRate", VolumePerTimeUnit.mL_Per_s);
    dataRequests.createActionCompartmentDataRequest("Hemorrhage", "RightLeg", "TotalBloodLost", VolumeUnit.mL);
    
    List<Double> dataValues;
    pe.serializeFromFile("./states/StandardMale@0s.json", dataRequests);
    
    dataValues = pe.pullData();
    dataRequests.writeData(dataValues);
    Log.info("");
    
    // Let's do something to the patient, you can either send actions over one at a time, or pass in a List<SEAction>
    SEHemorrhage rightLeg = new SEHemorrhage();
    // Setting up a realistic hemorrhage can be difficult
    // Here is an example of how the engine will act if you create an unrealistic hemorrhage
    rightLeg.setCompartment(eCompartment.RightLeg);
    rightLeg.getSeverity().setValue(0.4);
    pe.processAction(rightLeg);
    
    // An internal hemorrhage will fill the abdominal cavity
    SEHemorrhage internalLiver = new SEHemorrhage();
    internalLiver.setType(eType.Internal);
    internalLiver.setCompartment(eCompartment.Liver);
    internalLiver.getSeverity().setValue(0.2);
    pe.processAction(internalLiver);
    // Note you can also set Type to External to have the blood exit the body
    // By default, if you do not set Type, the type will be External
    
    // If the hemorrhage is very bad, a Hypovolemic Shock event will be thrown
    // Eventually Cardiovascular Collapse will be triggered, then you need to shut the engine down.
    // This collapse should be interpreted as death for a bleeding patient.
    // If you let it bleed after that, you will eventually get an irreversible state
    // advance time will then return false, and the engine will no longer do anything on subsequent advance time calls and only return false
    // If the provided hemorrhage is very bad, you may skip cardiovascular collapse and go straight to irreversable state...
    
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
        dataRequests.writeData(dataValues);
        Log.info("");
        
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

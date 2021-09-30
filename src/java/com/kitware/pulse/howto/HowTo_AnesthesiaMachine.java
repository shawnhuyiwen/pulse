/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.howto;

import com.kitware.pulse.cdm.bind.Engine.DataRequestData.eCategory;
import com.kitware.pulse.cdm.bind.Enums;
import com.kitware.pulse.cdm.bind.Enums.eSwitch;
import com.kitware.pulse.cdm.bind.Patient.PatientData.eSex;
import com.kitware.pulse.cdm.datarequests.SEDataRequest;
import com.kitware.pulse.cdm.datarequests.SEDataRequestManager;
import com.kitware.pulse.cdm.engine.SEPatientConfiguration;
import com.kitware.pulse.cdm.patient.SEPatient;
import com.kitware.pulse.cdm.properties.CommonUnits.FrequencyUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.LengthUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.MassPerVolumeUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.MassUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.PressureUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.TimeUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.VolumeUnit;
import com.kitware.pulse.cdm.properties.SEScalarTime;
import com.kitware.pulse.cdm.system.equipment.anesthesia_machine.actions.SEAnesthesiaMachineConfiguration;
import com.kitware.pulse.engine.PulseEngine;
import com.kitware.pulse.utilities.JNIBridge;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.LogListener;
import java.util.List;

/**
 *
 * @author esteban
 */
public class HowTo_AnesthesiaMachine
{

  protected static class MyListener extends LogListener
  {

    protected MyListener()
    {
      super();
      listen(false);
    }

    @Override public void handleDebug(String msg) { Log.debug(msg); }
    @Override public void handleInfo(String msg) { Log.info(msg); }
    @Override public void handleWarn(String msg) { Log.warn(msg); }
    @Override public void handleError(String msg) { Log.error(msg); }
    @Override public void handleFatal(String msg) { Log.fatal(msg); }
  }

  public static void main(String[] args)
  {
    boolean useStateFile = true;

    JNIBridge.initialize();

    ///Engine
    PulseEngine pe = new PulseEngine();
    pe.setLogListener(new MyListener());
    pe.setLogFilename("/test_results/howto/HowTo_AnesthesiaMachineVentilator.java.log");

    SEDataRequestManager dataRequests = new SEDataRequestManager();
    dataRequests.createPhysiologyDataRequest("HeartRate", FrequencyUnit.Per_min);
    dataRequests.createPhysiologyDataRequest("TotalLungVolume", VolumeUnit.mL);
    dataRequests.createPhysiologyDataRequest("RespirationRate", FrequencyUnit.Per_min);
    dataRequests.createPhysiologyDataRequest("BloodVolume", VolumeUnit.mL);

    if (useStateFile)
    {
      pe.serializeFromFile("./states/StandardMale@0s.json", dataRequests);
    }
    else
    {
      SEPatientConfiguration sepc = new SEPatientConfiguration();

      SEPatient patient = sepc.getPatient();
      patient.setName("Johnny Lawrence");
      patient.setSex(eSex.Male);
      patient.getAge().setValue(44, TimeUnit.yr);
      patient.getWeight().setValue(170, MassUnit.lb);
      patient.getHeight().setValue(71, LengthUnit.in);
      patient.getBodyFatFraction().setValue(0.21);
      patient.getHeartRateBaseline().setValue(72, FrequencyUnit.Per_min);
      patient.getDiastolicArterialPressureBaseline().setValue(72., PressureUnit.mmHg);
      patient.getSystolicArterialPressureBaseline().setValue(114., PressureUnit.mmHg);
      patient.getRespirationRateBaseline().setValue(16, FrequencyUnit.Per_min);
      
      pe.initializeEngine(sepc, dataRequests);
    }
    
    //Test
    List<Double> data;
    SEScalarTime time = new SEScalarTime(1, TimeUnit.s);

    // Configure the Anesthesia Machine
    SEAnesthesiaMachineConfiguration anesthesiaMachineConfigAction = new SEAnesthesiaMachineConfiguration();
    anesthesiaMachineConfigAction.getConfiguration().setConnection(eSwitch.On);

    //The following line will cause a SIGSEGV no matter what eSwitch value you use.
    anesthesiaMachineConfigAction.getConfiguration().getLeftChamber().setState(Enums.eSwitch.On);

    pe.processAction(anesthesiaMachineConfigAction);

    pe.advanceTime(time);

    data = pe.pullData();
    dataRequests.writeData(data);

  }

}

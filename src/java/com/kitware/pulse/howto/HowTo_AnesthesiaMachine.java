/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.howto;

import com.kitware.pulse.cdm.bind.AnesthesiaMachine;
import com.kitware.pulse.cdm.bind.Engine.DataRequestData.eCategory;
import com.kitware.pulse.cdm.bind.Enums;
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
import com.kitware.pulse.cdm.system.equipment.mechanical_ventilator.actions.SEMechanicalVentilatorConfiguration;
import com.kitware.pulse.engine.PulseEngine;
import com.kitware.pulse.utilities.JNIBridge;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.LogListener;
import java.io.IOException;
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
    pe.setLogFilename("/test_results/HowTo_AnesthesiaMachineVentilator.java.log");

    if (useStateFile) {
      pe.serializeFromFile("./states/StandardMale@0s.json", createDataRequests());
    } else {
      SEPatient patient = new SEPatient();
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

      SEPatientConfiguration sepc = new SEPatientConfiguration();
      sepc.setPatient(patient);
      SEDataRequestManager dataRequests = createDataRequests();

      pe.initializeEngine(sepc, dataRequests);
    }

    //Test
    List<Double> data;
    SEScalarTime time = new SEScalarTime(1, TimeUnit.s);

    // Configure the Anesthesia Machine
    SEAnesthesiaMachineConfiguration anesthesiaMachineConfigAction = new SEAnesthesiaMachineConfiguration();
    anesthesiaMachineConfigAction.getConfiguration().setConnection(AnesthesiaMachine.AnesthesiaMachineData.eConnection.Mask);

    //The following line will cause a SIGSEGV no matter what eSwitch value you use.
    anesthesiaMachineConfigAction.getConfiguration().getLeftChamber().setState(Enums.eSwitch.On);

    pe.processAction(anesthesiaMachineConfigAction);

    pe.advanceTime(time);

    data = pe.pullData();
    Log.info("Data:");
    Log.info("\tSimulation Time(s) " + data.get(0));
    Log.info("\tHeight(in) " + data.get(1));
    Log.info("\tWeight(kg) " + data.get(2));
    Log.info("\tBloodPH " + data.get(3));
    Log.info("\tArterialOxygenPressure(mmHg) " + data.get(4));
    Log.info("\tArterialCarbonDioxidePressure(mmHg) " + data.get(5));
    Log.info("\tTidalVolume(L) " + data.get(6));
    Log.info("\tEndTidalOxygenFraction " + data.get(7));
    Log.info("\tCarricoIndex(mmHg) " + data.get(8));
    Log.info("\tRespiration Rate(bpm) " + data.get(9));
    Log.info("\tRocuronium PlasmaConcentration(bpm) " + data.get(10));
    Log.info("\tSuccinylcholine PlasmaConcentration(bpm) " + data.get(11));
    Log.info("\tInspirationMachineTriggerTime(s) " + data.get(12));
    Log.info("\tPositiveEndExpiredPressure(cmH2O) " + data.get(13));

  }

  private static SEDataRequestManager createDataRequests()
  {
    SEDataRequestManager dataRequests = new SEDataRequestManager();
    SEDataRequest ht = new SEDataRequest();
    ht.setCategory(eCategory.Patient);
    ht.setPropertyName("Height");
    ht.setUnit(LengthUnit.in.toString());
    dataRequests.getRequestedData().add(ht);

    SEDataRequest wt = new SEDataRequest();
    wt.setCategory(eCategory.Patient);
    wt.setPropertyName("Weight");
    wt.setUnit(MassUnit.kg.toString());
    dataRequests.getRequestedData().add(wt);

    SEDataRequest pH = new SEDataRequest();
    pH.setCategory(eCategory.Physiology);
    pH.setPropertyName("BloodPH");
    dataRequests.getRequestedData().add(pH);

    SEDataRequest aO2 = new SEDataRequest();
    aO2.setCategory(eCategory.Physiology);
    aO2.setPropertyName("ArterialOxygenPressure");
    aO2.setUnit(PressureUnit.mmHg.toString());
    dataRequests.getRequestedData().add(aO2);

    SEDataRequest aCO2 = new SEDataRequest();
    aCO2.setCategory(eCategory.Physiology);
    aCO2.setPropertyName("ArterialCarbonDioxidePressure");
    aCO2.setUnit(PressureUnit.mmHg.toString());
    dataRequests.getRequestedData().add(aCO2);

    SEDataRequest tVol = new SEDataRequest();
    tVol.setCategory(eCategory.Physiology);
    tVol.setPropertyName("TidalVolume");
    tVol.setUnit(VolumeUnit.L.toString());
    dataRequests.getRequestedData().add(tVol);

    SEDataRequest etCO2 = new SEDataRequest();
    etCO2.setCategory(eCategory.Physiology);
    etCO2.setPropertyName("EndTidalOxygenFraction");
    dataRequests.getRequestedData().add(etCO2);

    SEDataRequest ci = new SEDataRequest();
    ci.setCategory(eCategory.Physiology);
    ci.setPropertyName("CarricoIndex");
    ci.setUnit(PressureUnit.mmHg.toString());
    dataRequests.getRequestedData().add(ci);

    SEDataRequest rr = new SEDataRequest();
    rr.setCategory(eCategory.Physiology);
    rr.setPropertyName("RespirationRate");
    rr.setUnit(FrequencyUnit.Per_min.toString());
    dataRequests.getRequestedData().add(rr);

    SEDataRequest rpc = new SEDataRequest();
    rpc.setCategory(eCategory.Substance);
    rpc.setSubstanceName("Rocuronium");
    rpc.setPropertyName("PlasmaConcentration");
    rpc.setUnit(MassPerVolumeUnit.ug_Per_L.toString());
    dataRequests.getRequestedData().add(rpc);

    SEDataRequest spc = new SEDataRequest();
    spc.setCategory(eCategory.Substance);
    spc.setSubstanceName("Succinylcholine");
    spc.setPropertyName("PlasmaConcentration");
    spc.setUnit(MassPerVolumeUnit.ug_Per_L.toString());
    dataRequests.getRequestedData().add(spc);

    SEDataRequest mv_imtt = new SEDataRequest();
    mv_imtt.setCategory(eCategory.MechanicalVentilator);
    mv_imtt.setPropertyName("InspirationMachineTriggerTime");
    mv_imtt.setUnit(TimeUnit.s.toString());
    dataRequests.getRequestedData().add(mv_imtt);

    SEDataRequest mv_peep = new SEDataRequest();
    mv_peep.setCategory(eCategory.MechanicalVentilator);
    mv_peep.setPropertyName("PositiveEndExpiredPressure");
    mv_peep.setUnit(PressureUnit.cmH2O.toString());
    dataRequests.getRequestedData().add(mv_peep);

    return dataRequests;
  }
}

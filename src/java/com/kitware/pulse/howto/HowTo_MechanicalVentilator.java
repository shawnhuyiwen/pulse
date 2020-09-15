/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.howto;

import java.util.List;

import com.kitware.pulse.cdm.bind.Engine.DataRequestData.eCategory;
import com.kitware.pulse.cdm.bind.MechanicalVentilator.MechanicalVentilatorData.*;
import com.kitware.pulse.cdm.bind.PatientActions.IntubationData.eType;
import com.kitware.pulse.cdm.datarequests.SEDataRequest;
import com.kitware.pulse.cdm.datarequests.SEDataRequestManager;
import com.kitware.pulse.cdm.patient.actions.SEIntubation;
import com.kitware.pulse.cdm.properties.CommonUnits.FrequencyUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.LengthUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.MassUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.MassPerVolumeUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.PressureUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.TimeUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.VolumeUnit;
import com.kitware.pulse.cdm.properties.SEScalarTime;
import com.kitware.pulse.cdm.system.equipment.mechanical_ventilator.actions.*;
import com.kitware.pulse.engine.PulseEngine;
import com.kitware.pulse.utilities.JNIBridge;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.LogListener;

public class HowTo_MechanicalVentilator
{
  protected static class MyListener extends LogListener
  {
    protected MyListener()
    {
      super();
      listen(false);
    }    
    @Override public void handleDebug(String msg) { Log.debug(msg); }
    @Override public void handleInfo(String msg)  { Log.info(msg); }
    @Override public void handleWarn(String msg)  { Log.warn(msg); }
    @Override public void handleError(String msg) { Log.error(msg); }
    @Override public void handleFatal(String msg) { Log.fatal(msg); }
  }
  
  public static void main(String[] args)
  {
    List<Double> data;
    SEScalarTime time = new SEScalarTime(1,TimeUnit.min);
    
    JNIBridge.initialize();
    PulseEngine pe = new PulseEngine();
    pe.setLogListener(new MyListener());
    pe.setLogFilename("./test_results/HowTo_MechanicalVentilator.java.log");
    
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
    
    pe.serializeFromFile("./states/StandardMale@0s.json", dataRequests);
    
    SEIntubation intubate = new SEIntubation();
    intubate.setType(eType.Tracheal);
    pe.processAction(intubate);
    
    // Configure the Mechanical Ventilator
    SEMechanicalVentilatorConfiguration ventilatorAction = new SEMechanicalVentilatorConfiguration();
    ventilatorAction.getConfiguration().setConnection(eConnection.Tube);
    ventilatorAction.getConfiguration().setExpirationWaveform(eDriverWaveform.Square);
    ventilatorAction.getConfiguration().setInspirationWaveform(eDriverWaveform.Square);
    ventilatorAction.getConfiguration().getPeakInspiratoryPressure().setValue(21, PressureUnit.cmH2O);
    ventilatorAction.getConfiguration().getPositiveEndExpiredPressure().setValue(10, PressureUnit.cmH2O);

    double desiredRR = 20.0;
    double ieRatio = 0.5;
    double totalPeriodS = 60.0 / desiredRR;
    double inspiratoryPeriodS = ieRatio * totalPeriodS / (1.0 + ieRatio);
    double expiratoryPeriodS = totalPeriodS - inspiratoryPeriodS;
    ventilatorAction.getConfiguration().getInspirationMachineTriggerTime().setValue(expiratoryPeriodS, TimeUnit.s);
    ventilatorAction.getConfiguration().getExpirationCycleTime().setValue(inspiratoryPeriodS, TimeUnit.s);

    ventilatorAction.getConfiguration().getFractionInspiredGas().clear();
    ventilatorAction.getConfiguration().getFractionInspiredGas("Oxygen").getAmount().setValue(0.5);
    ventilatorAction.getConfiguration().getConcentrationInspiredAerosol("Albuterol").getConcentration().setValue(12, MassPerVolumeUnit.ug_Per_mL);
    
    pe.processAction(ventilatorAction);
    
    pe.advanceTime(time);
    
    data = pe.pullData();
    Log.info("Simulation Time(s) " + data.get(0));
    Log.info("Height(in) " + data.get(1));
    Log.info("Weight(kg) " + data.get(2));
    Log.info("BloodPH " + data.get(3));
    Log.info("ArterialOxygenPressure(mmHg) " + data.get(4));
    Log.info("ArterialCarbonDioxidePressure(mmHg) " + data.get(5));
    Log.info("TidalVolume(L) " + data.get(6));
    Log.info("EndTidalOxygenFraction " + data.get(7));
    Log.info("CarricoIndex(mmHg) " + data.get(8));
    Log.info("Respiration Rate(bpm) " + data.get(9));
    Log.info("Rocuronium PlasmaConcentration(bpm) " + data.get(10));
    Log.info("Succinylcholine PlasmaConcentration(bpm) " + data.get(11));
    Log.info("InspirationMachineTriggerTime(s) " + data.get(12));
    Log.info("PositiveEndExpiredPressure(cmH2O) " + data.get(13));
    
  }

}

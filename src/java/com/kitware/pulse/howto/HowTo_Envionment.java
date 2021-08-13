package com.kitware.pulse.howto;

import java.util.List;

import com.kitware.pulse.cdm.bind.Engine.DataRequestData.eCategory;
import com.kitware.pulse.cdm.datarequests.SEDataRequest;
import com.kitware.pulse.cdm.datarequests.SEDataRequestManager;
import com.kitware.pulse.cdm.properties.SEScalarTime;
import com.kitware.pulse.cdm.system.environment.actions.SEChangeEnvironmentalConditions;
import com.kitware.pulse.engine.PulseEngine;
import com.kitware.pulse.cdm.properties.CommonUnits.FrequencyUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.TemperatureUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.TimeUnit;
import com.kitware.pulse.cdm.properties.CommonUnits.VolumeUnit;
import com.kitware.pulse.utilities.JNIBridge;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.LogListener;

public class HowTo_Envionment
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
    List<Double> dataValues;
    
    SEScalarTime time = new SEScalarTime(10,TimeUnit.min);

    JNIBridge.initialize();
    PulseEngine pe = new PulseEngine();
    pe.setLogListener(new MyListener());
    pe.setLogFilename("./test_results/HowTo_Environment.java.log");

    SEDataRequestManager dataRequests = new SEDataRequestManager();
    SEDataRequest tVol = new SEDataRequest();
    tVol.setCategory(eCategory.Physiology);
    tVol.setPropertyName("TidalVolume");
    tVol.setUnit(VolumeUnit.L.toString());
    dataRequests.getRequestedData().add(tVol);

    SEDataRequest etCO2 = new SEDataRequest();
    etCO2.setCategory(eCategory.Physiology);
    etCO2.setPropertyName("EndTidalOxygenFraction");
    dataRequests.getRequestedData().add(etCO2);

    SEDataRequest rr = new SEDataRequest();
    rr.setCategory(eCategory.Physiology);
    rr.setPropertyName("RespirationRate");
    rr.setUnit(FrequencyUnit.Per_min.toString());
    dataRequests.getRequestedData().add(rr);
    
    SEDataRequest coretemp = new SEDataRequest();
    coretemp.setCategory(eCategory.Physiology);
    coretemp.setPropertyName("CoreTemperature");
    coretemp.setUnit(TemperatureUnit.F.toString());
    dataRequests.getRequestedData().add(coretemp);

    SEDataRequest skinTemp = new SEDataRequest();
    skinTemp.setCategory(eCategory.Physiology);
    skinTemp.setPropertyName("SkinTemperature");
    skinTemp.setUnit(TemperatureUnit.F.toString());
    dataRequests.getRequestedData().add(skinTemp);
    
    pe.serializeFromFile("./states/StandardMale@0s.json", dataRequests);
    
    dataValues = pe.pullData();
    Log.info("Simulation Time(s) " + dataValues.get(0));
    Log.info("TidalVolume (L) " + dataValues.get(1));
    Log.info("EndTidalOxygenFraction " + dataValues.get(2));
    Log.info("Respiration Rate (bpm) " + dataValues.get(3));
    Log.info("Core Temp (F) " + dataValues.get(4));
    Log.info("Skin Temp (F) " + dataValues.get(5));
    
    SEChangeEnvironmentalConditions env = new SEChangeEnvironmentalConditions();
    env.setEnvironmentalConditionsFile("./environments/AnchorageDecember.json");
    pe.processAction(env);
    
    pe.advanceTime(time);

    dataValues = pe.pullData();
    Log.info("Simulation Time(s) " + dataValues.get(0));
    Log.info("TidalVolume (L) " + dataValues.get(1));
    Log.info("EndTidalOxygenFraction " + dataValues.get(2));
    Log.info("Respiration Rate (bpm) " + dataValues.get(3));
    Log.info("Core Temp (F) " + dataValues.get(4));
    Log.info("Skin Temp (F) " + dataValues.get(5));
  }

}

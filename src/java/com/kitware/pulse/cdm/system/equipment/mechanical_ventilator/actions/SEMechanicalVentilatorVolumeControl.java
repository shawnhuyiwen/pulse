/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.mechanical_ventilator.actions;

import com.kitware.pulse.cdm.bind.MechanicalVentilatorActions.MechanicalVentilatorVolumeControlData;
import com.kitware.pulse.cdm.bind.MechanicalVentilatorActions.MechanicalVentilatorVolumeControlData.eMode;
import com.kitware.pulse.cdm.properties.*;

public class SEMechanicalVentilatorVolumeControl extends SEMechanicalVentilatorMode
{
  protected eMode                 mode;
  protected SEScalarVolumePerTime flow;
  protected SEScalar0To1          fractionInspiredOxygen;
  protected SEScalarTime          inspiratoryPeriod;
  protected SEScalarPressure      positiveEndExpiredPressure;
  protected SEScalarFrequency     respirationRate;
  protected SEScalarVolume        tidalVolume;

  public SEMechanicalVentilatorVolumeControl()
  {
    super();
    mode = eMode.AssistedControl;
    flow = null;
    fractionInspiredOxygen = null;
    inspiratoryPeriod = null;
    positiveEndExpiredPressure = null;
    respirationRate = null;
    tidalVolume = null;
  }

  public SEMechanicalVentilatorVolumeControl(SEMechanicalVentilatorVolumeControl other)
  {
    copy(other);
  }

  public void copy(SEMechanicalVentilatorVolumeControl other)
  {
    super.copy(other);
    if (other.hasFlow())
      getFlow().set(other.getFlow());
    if (other.hasFractionInspiredOxygen())
      getFractionInspiredOxygen().set(other.getFractionInspiredOxygen());
    if (other.hasInspiratoryPeriod())
      getInspiratoryPeriod().set(other.getInspiratoryPeriod());
    if (other.hasPositiveEndExpiredPressure())
      getPositiveEndExpiredPressure().set(other.getPositiveEndExpiredPressure());
    if (other.hasRespirationRate())
      getRespirationRate().set(other.getRespirationRate());
    if (other.hasTidalVolume())
      getTidalVolume().set(other.getTidalVolume());
  }

  public void clear()
  {
    super.clear();
    mode = eMode.AssistedControl;
    if (flow != null)
      flow.invalidate();
    if (fractionInspiredOxygen != null)
      fractionInspiredOxygen.invalidate();
    if (inspiratoryPeriod != null)
      inspiratoryPeriod.invalidate();
    if (positiveEndExpiredPressure != null)
      positiveEndExpiredPressure.invalidate();
    if (respirationRate != null)
      respirationRate.invalidate();
    if (tidalVolume != null)
      tidalVolume.invalidate();
  }
  
  public static void load(MechanicalVentilatorVolumeControlData src, SEMechanicalVentilatorVolumeControl dst)
  {
    dst.reset();
    SEMechanicalVentilatorMode.load(src.getMechanicalVentilatorMode(),dst);
    if (src.hasFlow())
      SEScalarVolumePerTime.load(src.getFlow(), dst.getFlow());
    if (src.hasFractionInspiredOxygen())
      SEScalar0To1.load(src.getFractionInspiredOxygen(), dst.getFractionInspiredOxygen());
    if (src.hasInspiratoryPeriod())
      SEScalarTime.load(src.getInspiratoryPeriod(), dst.getInspiratoryPeriod());
    if (src.hasPositiveEndExpiredPressure())
      SEScalarPressure.load(src.getPositiveEndExpiredPressure(), dst.getPositiveEndExpiredPressure());
    if (src.hasRespirationRate())
      SEScalarFrequency.load(src.getRespirationRate(), dst.getRespirationRate());
    if (src.hasTidalVolume())
      SEScalarVolume.load(src.getTidalVolume(), dst.getTidalVolume());
  }
  public static MechanicalVentilatorVolumeControlData unload(SEMechanicalVentilatorVolumeControl src)
  {
    MechanicalVentilatorVolumeControlData.Builder dst = MechanicalVentilatorVolumeControlData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEMechanicalVentilatorVolumeControl src, MechanicalVentilatorVolumeControlData.Builder dst)
  {
    SEMechanicalVentilatorMode.unload(src, dst.getMechanicalVentilatorModeBuilder());
    if(src.hasFlow())
      dst.setFlow(SEScalarVolumePerTime.unload(src.getFlow()));
    if (src.hasFractionInspiredOxygen())
      dst.setFractionInspiredOxygen(SEScalar0To1.unload(src.getFractionInspiredOxygen()));
    if (src.hasInspiratoryPeriod())
      dst.setInspiratoryPeriod(SEScalarTime.unload(src.getInspiratoryPeriod()));
    if (src.hasPositiveEndExpiredPressure())
      dst.setPositiveEndExpiredPressure(SEScalarPressure.unload(src.getPositiveEndExpiredPressure()));
    if (src.hasRespirationRate())
      dst.setRespirationRate(SEScalarFrequency.unload(src.getRespirationRate()));
    if (src.hasTidalVolume())
      dst.setTidalVolume(SEScalarVolume.unload(src.getTidalVolume()));
  }

  public boolean isValid()
  {
    return hasFlow() && hasFractionInspiredOxygen() && hasInspiratoryPeriod() && hasPositiveEndExpiredPressure()
        && hasRespirationRate() && hasTidalVolume();
  }

  public eMode getMode()
  {
    return mode;
  }

  public void setMode(eMode m)
  {
    mode = m;
  }

  public boolean hasFlow()
  {
    return flow == null ? false : flow.isValid();
  }

  public SEScalarVolumePerTime getFlow()
  {
    if (flow == null)
      flow = new SEScalarVolumePerTime();
    return flow;
  }

  public boolean hasFractionInspiredOxygen()
  {
    return fractionInspiredOxygen == null ? false : fractionInspiredOxygen.isValid();
  }

  public SEScalar0To1 getFractionInspiredOxygen()
  {
    if (fractionInspiredOxygen == null)
      fractionInspiredOxygen = new SEScalar0To1();
    return fractionInspiredOxygen;
  }

  public boolean hasInspiratoryPeriod()
  {
    return inspiratoryPeriod == null ? false : inspiratoryPeriod.isValid();
  }

  public SEScalarTime getInspiratoryPeriod()
  {
    if (inspiratoryPeriod == null)
      inspiratoryPeriod = new SEScalarTime();
    return inspiratoryPeriod;
  }

  public boolean hasPositiveEndExpiredPressure()
  {
    return positiveEndExpiredPressure == null ? false : positiveEndExpiredPressure.isValid();
  }

  public SEScalarPressure getPositiveEndExpiredPressure()
  {
    if (positiveEndExpiredPressure == null)
      positiveEndExpiredPressure = new SEScalarPressure();
    return positiveEndExpiredPressure;
  }

  public boolean hasRespirationRate()
  {
    return respirationRate == null ? false : respirationRate.isValid();
  }

  public SEScalarFrequency getRespirationRate()
  {
    if (respirationRate == null)
      respirationRate = new SEScalarFrequency();
    return respirationRate;
  }

  public boolean hasTidalVolume()
  {
    return tidalVolume == null ? false : tidalVolume.isValid();
  }

  public SEScalarVolume getTidalVolume()
  {
    if (tidalVolume == null)
      tidalVolume = new SEScalarVolume();
    return tidalVolume;
  }

  public String toString()
  {
    String str = "Mechanical Ventilator Volume Control";
    str += "\n\tMode: " + this.mode;
    str += "\n\tFlow: " + (hasFlow() ? getFlow().toString() : "Not Provided");
    str += "\n\tFractionInspiredOxygen: "
        + (hasFractionInspiredOxygen() ? getFractionInspiredOxygen().toString() : "Not Provided");
    str += "\n\tInspiratoryPeriod: " + (hasInspiratoryPeriod() ? getInspiratoryPeriod().toString() : "Not Provided");
    str += "\n\tPositiveEndExpiredPressure: "
        + (hasPositiveEndExpiredPressure() ? getPositiveEndExpiredPressure().toString() : "Not Provided");
    str += "\n\tRespirationRate: " + (hasRespirationRate() ? getRespirationRate().toString() : "Not Provided");
    str += "\n\tTidalVolume: " + (hasTidalVolume() ? getTidalVolume().toString() : "Not Provided");

    return str;
  }
}

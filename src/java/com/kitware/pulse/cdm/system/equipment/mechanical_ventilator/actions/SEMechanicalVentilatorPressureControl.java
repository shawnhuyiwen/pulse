/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.mechanical_ventilator.actions;

import com.kitware.pulse.cdm.properties.*;
import com.kitware.pulse.cdm.bind.MechanicalVentilatorActions.MechanicalVentilatorPressureControlData;
import com.kitware.pulse.cdm.bind.MechanicalVentilatorActions.MechanicalVentilatorPressureControlData.eMode;

public class SEMechanicalVentilatorPressureControl extends SEMechanicalVentilatorMode
  {
    protected eMode mode;
    protected SEScalar0To1 fractionInspiredOxygen;
    protected SEScalarTime inspiratoryPeriod;
    protected SEScalarPressure inspiratoryPressure;
    protected SEScalarPressure positiveEndExpiredPressure;
    protected SEScalarFrequency respirationRate;
    protected SEScalarTime slope;

    public SEMechanicalVentilatorPressureControl()
    {
      mode = eMode.AssistedControl;
      fractionInspiredOxygen = null;
      inspiratoryPeriod = null;
      inspiratoryPressure = null;
      positiveEndExpiredPressure = null;
      respirationRate = null;
      slope = null;
    }

    public SEMechanicalVentilatorPressureControl(SEMechanicalVentilatorPressureControl other)
    {
      copy(other);
    }

    public void copy(SEMechanicalVentilatorPressureControl other)
    {
      super.copy(other);
      if (other.hasFractionInspiredOxygen())
        getFractionInspiredOxygen().set(other.getFractionInspiredOxygen());
      if (other.hasInspiratoryPeriod())
        getInspiratoryPeriod().set(other.getInspiratoryPeriod());
      if (other.hasInspiratoryPressure())
        getInspiratoryPressure().set(other.getInspiratoryPressure());
      if (other.hasPositiveEndExpiredPressure())
        getPositiveEndExpiredPressure().set(other.getPositiveEndExpiredPressure());
      if (other.hasRespirationRate())
        getRespirationRate().set(other.getRespirationRate());
      if (other.hasSlope())
        getSlope().set(other.getSlope());
    }

    public void clear()
    {
      super.clear();
      mode = eMode.AssistedControl;
      if (fractionInspiredOxygen != null)
        fractionInspiredOxygen.invalidate();
      if (inspiratoryPeriod != null)
        inspiratoryPeriod.invalidate();
      if (inspiratoryPressure != null)
        inspiratoryPressure.invalidate();
      if (positiveEndExpiredPressure != null)
        positiveEndExpiredPressure.invalidate();
      if (respirationRate != null)
        respirationRate.invalidate();
      if (slope != null)
        slope.invalidate();
    }
    
    public static void load(MechanicalVentilatorPressureControlData src, SEMechanicalVentilatorPressureControl dst)
    {
      dst.clear();
      SEMechanicalVentilatorMode.load(src.getMechanicalVentilatorMode(),dst);
      if (src.hasFractionInspiredOxygen())
        SEScalar0To1.load(src.getFractionInspiredOxygen(), dst.getFractionInspiredOxygen());
      if (src.hasInspiratoryPeriod())
        SEScalarTime.load(src.getInspiratoryPeriod(), dst.getInspiratoryPeriod());
      if (src.hasInspiratoryPressure())
        SEScalarPressure.load(src.getInspiratoryPressure(), dst.getInspiratoryPressure());
      if (src.hasPositiveEndExpiredPressure())
        SEScalarPressure.load(src.getPositiveEndExpiredPressure(), dst.getPositiveEndExpiredPressure());
      if (src.hasRespirationRate())
        SEScalarFrequency.load(src.getRespirationRate(), dst.getRespirationRate());
      if (src.hasSlope())
        SEScalarTime.load(src.getSlope(), dst.getSlope());
    }
    public static MechanicalVentilatorPressureControlData unload(SEMechanicalVentilatorPressureControl src)
    {
      MechanicalVentilatorPressureControlData.Builder dst = MechanicalVentilatorPressureControlData.newBuilder();
      unload(src,dst);
      return dst.build();
    }
    protected static void unload(SEMechanicalVentilatorPressureControl src, MechanicalVentilatorPressureControlData.Builder dst)
    {
      SEMechanicalVentilatorMode.unload(src, dst.getMechanicalVentilatorModeBuilder());
      if(src.hasFractionInspiredOxygen())
        dst.setFractionInspiredOxygen(SEScalar0To1.unload(src.getFractionInspiredOxygen()));
      if (src.hasInspiratoryPeriod())
        dst.setInspiratoryPeriod(SEScalarTime.unload(src.getInspiratoryPeriod()));
      if (src.hasInspiratoryPressure())
        dst.setInspiratoryPressure(SEScalarPressure.unload(src.getInspiratoryPressure()));
      if (src.hasPositiveEndExpiredPressure())
        dst.setPositiveEndExpiredPressure(SEScalarPressure.unload(src.getPositiveEndExpiredPressure()));
      if (src.hasRespirationRate())
        dst.setRespirationRate(SEScalarFrequency.unload(src.getRespirationRate()));
      if (src.hasSlope())
        dst.setSlope(SEScalarTime.unload(src.getSlope()));
    }

    public boolean isValid()
    {
      return hasFractionInspiredOxygen() &&
        hasInspiratoryPeriod() &&
        hasInspiratoryPressure() &&
        hasPositiveEndExpiredPressure() &&
        hasRespirationRate() &&
        hasSlope();
    }

    public eMode getMode()
    {
      return mode;
    }
    public void setMode(eMode m)
    {
      mode = m;
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

    public boolean hasInspiratoryPressure()
    {
      return inspiratoryPressure == null ? false : inspiratoryPressure.isValid();
    }
    public SEScalarPressure getInspiratoryPressure()
    {
      if (inspiratoryPressure == null)
        inspiratoryPressure = new SEScalarPressure();
      return inspiratoryPressure;
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
    public boolean hasSlope()
    {
      return slope == null ? false : slope.isValid();
    }
    public SEScalarTime getSlope()
    {
      if (slope == null)
        slope = new SEScalarTime();
      return slope;
    }

    public String toString()
    {
      String str = "Mechanical Ventilator Pressure Control";
      str += "\n\tMode: " + mode;
      str += "\n\tFractionInspiredOxygen: " + (hasFractionInspiredOxygen() ? getFractionInspiredOxygen().toString() : "Not Provided");
      str += "\n\tInspiratoryPeriod: " + (hasInspiratoryPeriod() ? getInspiratoryPeriod().toString() : "Not Provided");
      str += "\n\tInspiratoryPressure: " + (hasInspiratoryPressure() ? getInspiratoryPressure().toString() : "Not Provided");
      str += "\n\tPositiveEndExpiredPressure: " + (hasPositiveEndExpiredPressure() ? getPositiveEndExpiredPressure().toString() : "Not Provided");
      str += "\n\tRespirationRate: " + (hasRespirationRate() ? getRespirationRate().toString() : "Not Provided");
      str += "\n\tSlope: " + (hasSlope() ? getSlope().toString() : "Not Provided");


      return str;
    }
  }

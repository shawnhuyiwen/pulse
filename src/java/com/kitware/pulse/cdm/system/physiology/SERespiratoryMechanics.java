/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.physiology;

import com.kitware.pulse.cdm.bind.Enums.eDefaultType;
import com.kitware.pulse.cdm.bind.Enums.eSwitch;
import com.kitware.pulse.cdm.bind.Physiology.RespiratoryMechanicsData;
import com.kitware.pulse.cdm.properties.*;

public class SERespiratoryMechanics
  {
    protected eSwitch                       active;
    protected eDefaultType                  defaultType;
    protected SECurve                       leftComplianceCurve;
    protected SECurve                       rightComplianceCurve;
    protected SEScalarPressureTimePerVolume leftExpiratoryResistance;
    protected SEScalarPressureTimePerVolume leftInspiratoryResistance;
    protected SEScalarPressureTimePerVolume rightExpiratoryResistance;
    protected SEScalarPressureTimePerVolume rightInspiratoryResistance;
    protected SEScalarPressureTimePerVolume upperExpiratoryResistance;
    protected SEScalarPressureTimePerVolume upperInspiratoryResistance;
    protected SEScalarPressure              inspiratoryPeakPressure;
    protected SEScalarPressure              expiratoryPeakPressure;
    protected SEScalarTime                  inspiratoryRiseTime;
    protected SEScalarTime                  inspiratoryHoldTime;
    protected SEScalarTime                  inspiratoryReleaseTime;
    protected SEScalarTime                  inspiratoryToExpiratoryPauseTime;
    protected SEScalarTime                  expiratoryRiseTime;
    protected SEScalarTime                  expiratoryHoldTime;
    protected SEScalarTime                  expiratoryReleaseTime;
    protected SEScalarTime                  residueTime;

    public SERespiratoryMechanics()
    {
      active = eSwitch.NullSwitch;
      defaultType = eDefaultType.Model;
      leftComplianceCurve = null;
      rightComplianceCurve = null;
      leftExpiratoryResistance = null;
      leftInspiratoryResistance = null;
      rightExpiratoryResistance = null;
      rightInspiratoryResistance = null;
      upperExpiratoryResistance = null;
      upperInspiratoryResistance = null;
      inspiratoryPeakPressure = null;
      expiratoryPeakPressure = null;
      inspiratoryRiseTime = null;
      inspiratoryHoldTime = null;
      inspiratoryReleaseTime = null;
      inspiratoryToExpiratoryPauseTime = null;
      expiratoryRiseTime = null;
      expiratoryHoldTime = null;
      expiratoryReleaseTime = null;
      residueTime = null;
    }

    public void clear()
    {
      active = eSwitch.NullSwitch;
      defaultType = eDefaultType.Model;
      if (leftComplianceCurve != null)
        leftComplianceCurve.invalidate();
      if (rightComplianceCurve != null)
        rightComplianceCurve.invalidate();
      if (leftExpiratoryResistance != null)
        leftExpiratoryResistance.invalidate();
      if (leftInspiratoryResistance != null)
        leftInspiratoryResistance.invalidate();
      if (rightExpiratoryResistance != null)
        rightExpiratoryResistance.invalidate();
      if (rightInspiratoryResistance != null)
        rightInspiratoryResistance.invalidate();
      if (upperExpiratoryResistance != null)
        upperExpiratoryResistance.invalidate();
      if (upperInspiratoryResistance != null)
        upperInspiratoryResistance.invalidate();
      if (inspiratoryPeakPressure != null)
        inspiratoryPeakPressure.invalidate();
      if (expiratoryPeakPressure != null)
        expiratoryPeakPressure.invalidate();
      if (inspiratoryRiseTime != null)
        inspiratoryRiseTime.invalidate();
      if (inspiratoryHoldTime != null)
        inspiratoryHoldTime.invalidate();
      if (inspiratoryReleaseTime != null)
        inspiratoryReleaseTime.invalidate();
      if (inspiratoryToExpiratoryPauseTime != null)
        inspiratoryToExpiratoryPauseTime.invalidate();
      if (expiratoryRiseTime != null)
        expiratoryRiseTime.invalidate();
      if (expiratoryHoldTime != null)
        expiratoryHoldTime.invalidate();
      if (expiratoryReleaseTime != null)
        expiratoryReleaseTime.invalidate();
      if (residueTime != null)
        residueTime.invalidate();
    }

    public void copy(SERespiratoryMechanics from)
    {
      clear();
      if(hasActive())
        setActive(from.getActive());
      setDefaultType(from.getDefaultType());
      if (from.hasLeftComplianceCurve())
        getLeftComplianceCurve().copy(from.getLeftComplianceCurve());
      if (from.hasRightComplianceCurve())
        getRightComplianceCurve().copy(from.getRightComplianceCurve());
      if (from.hasLeftExpiratoryResistance())
        getLeftExpiratoryResistance().set(from.getLeftExpiratoryResistance());
      if (from.hasLeftInspiratoryResistance())
        getLeftInspiratoryResistance().set(from.getLeftInspiratoryResistance());
      if (from.hasRightExpiratoryResistance())
        getRightExpiratoryResistance().set(from.getRightExpiratoryResistance());
      if (from.hasRightInspiratoryResistance())
        getRightInspiratoryResistance().set(from.getRightInspiratoryResistance());
      if (from.hasUpperExpiratoryResistance())
        getUpperExpiratoryResistance().set(from.getUpperExpiratoryResistance());
      if (from.hasUpperInspiratoryResistance())
        getUpperInspiratoryResistance().set(from.getUpperInspiratoryResistance());
      if (from.hasInspiratoryPeakPressure())
        getInspiratoryPeakPressure().set(from.getInspiratoryPeakPressure());
      if (from.hasExpiratoryPeakPressure())
        getExpiratoryPeakPressure().set(from.getExpiratoryPeakPressure());
      if (from.hasInspiratoryRiseTime())
        getInspiratoryRiseTime().set(from.getInspiratoryRiseTime());
      if (from.hasInspiratoryHoldTime())
        getInspiratoryHoldTime().set(from.getInspiratoryHoldTime());
      if (from.hasInspiratoryReleaseTime())
        getInspiratoryReleaseTime().set(from.getInspiratoryReleaseTime());
      if (from.hasInspiratoryToExpiratoryPauseTime())
        getInspiratoryToExpiratoryPauseTime().set(from.getInspiratoryToExpiratoryPauseTime());
      if (from.hasExpiratoryRiseTime())
        getExpiratoryRiseTime().set(from.getExpiratoryRiseTime());
      if (from.hasExpiratoryHoldTime())
        getExpiratoryHoldTime().set(from.getExpiratoryHoldTime());
      if (from.hasExpiratoryReleaseTime())
        getExpiratoryReleaseTime().set(from.getExpiratoryReleaseTime());
      if (from.hasResidueTime())
        getResidueTime().set(from.getResidueTime());
    }
    
    public static void load(RespiratoryMechanicsData src, SERespiratoryMechanics dst)
    {
      dst.setActive(src.getActive());
      dst.setDefaultType(src.getDefaultType());
      if (src.hasLeftComplianceCurve())
        SECurve.load(src.getLeftComplianceCurve(),dst.getLeftComplianceCurve());
      if (src.hasRightComplianceCurve())
        SECurve.load(src.getRightComplianceCurve(),dst.getRightComplianceCurve());
      if (src.hasLeftExpiratoryResistance())
        SEScalarPressureTimePerVolume.load(src.getLeftExpiratoryResistance(),dst.getLeftExpiratoryResistance());
      if (src.hasLeftInspiratoryResistance())
        SEScalarPressureTimePerVolume.load(src.getLeftInspiratoryResistance(),dst.getLeftInspiratoryResistance());
      if (src.hasRightExpiratoryResistance())
        SEScalarPressureTimePerVolume.load(src.getRightExpiratoryResistance(),dst.getRightExpiratoryResistance());
      if (src.hasRightInspiratoryResistance())
        SEScalarPressureTimePerVolume.load(src.getRightInspiratoryResistance(),dst.getRightInspiratoryResistance());
      if (src.hasUpperExpiratoryResistance())
        SEScalarPressureTimePerVolume.load(src.getUpperExpiratoryResistance(),dst.getUpperExpiratoryResistance());
      if (src.hasUpperInspiratoryResistance())
        SEScalarPressureTimePerVolume.load(src.getUpperInspiratoryResistance(),dst.getUpperInspiratoryResistance());
      if (src.hasInspiratoryPeakPressure())
        SEScalarPressure.load(src.getInspiratoryPeakPressure(),dst.getInspiratoryPeakPressure());
      if (src.hasExpiratoryPeakPressure())
        SEScalarPressure.load(src.getExpiratoryPeakPressure(),dst.getExpiratoryPeakPressure());
      if (src.hasInspiratoryRiseTime())
        SEScalarTime.load(src.getInspiratoryRiseTime(),dst.getInspiratoryRiseTime());
      if (src.hasInspiratoryHoldTime())
        SEScalarTime.load(src.getInspiratoryHoldTime(),dst.getInspiratoryHoldTime());
      if (src.hasInspiratoryReleaseTime())
        SEScalarTime.load(src.getInspiratoryReleaseTime(),dst.getInspiratoryReleaseTime());
      if (src.hasInspiratoryToExpiratoryPauseTime())
        SEScalarTime.load(src.getInspiratoryToExpiratoryPauseTime(),dst.getInspiratoryToExpiratoryPauseTime());
      if (src.hasExpiratoryRiseTime())
        SEScalarTime.load(src.getExpiratoryRiseTime(),dst.getExpiratoryRiseTime());
      if (src.hasExpiratoryHoldTime())
        SEScalarTime.load(src.getExpiratoryHoldTime(),dst.getExpiratoryHoldTime());
      if (src.hasExpiratoryReleaseTime())
        SEScalarTime.load(src.getExpiratoryReleaseTime(),dst.getExpiratoryReleaseTime());
      if (src.hasResidueTime())
        SEScalarTime.load(src.getResidueTime(),dst.getResidueTime());
      
    }

    public static RespiratoryMechanicsData unload(SERespiratoryMechanics src)
    {
      RespiratoryMechanicsData.Builder dst = RespiratoryMechanicsData.newBuilder();
      unload(src,dst);
      return dst.build();
    }

    protected static void unload(SERespiratoryMechanics src, RespiratoryMechanicsData.Builder dst)
    {
      dst.setActive(src.getActive());
      dst.setDefaultType(src.getDefaultType());
      if (src.hasLeftComplianceCurve())
        dst.setLeftComplianceCurve(SECurve.unload(src.getLeftComplianceCurve()));
      if (src.hasRightComplianceCurve())
        dst.setRightComplianceCurve(SECurve.unload(src.getRightComplianceCurve()));
      if (src.hasLeftExpiratoryResistance())
        dst.setLeftExpiratoryResistance(SEScalarPressureTimePerVolume.unload(src.getLeftExpiratoryResistance()));
      if (src.hasLeftInspiratoryResistance())
        dst.setLeftInspiratoryResistance(SEScalarPressureTimePerVolume.unload(src.getLeftInspiratoryResistance()));
      if (src.hasRightExpiratoryResistance())
        dst.setRightExpiratoryResistance(SEScalarPressureTimePerVolume.unload(src.getRightExpiratoryResistance()));
      if (src.hasRightInspiratoryResistance())
        dst.setRightInspiratoryResistance(SEScalarPressureTimePerVolume.unload(src.getRightInspiratoryResistance()));
      if (src.hasUpperExpiratoryResistance())
        dst.setUpperExpiratoryResistance(SEScalarPressureTimePerVolume.unload(src.getUpperExpiratoryResistance()));
      if (src.hasUpperExpiratoryResistance())
        dst.setUpperExpiratoryResistance(SEScalarPressureTimePerVolume.unload(src.getUpperExpiratoryResistance()));
      if (src.hasInspiratoryPeakPressure())
        dst.setInspiratoryPeakPressure(SEScalarPressure.unload(src.getInspiratoryPeakPressure()));
      if (src.hasExpiratoryPeakPressure())
        dst.setExpiratoryPeakPressure(SEScalarPressure.unload(src.getExpiratoryPeakPressure()));
      if (src.hasInspiratoryRiseTime())
        dst.setInspiratoryRiseTime(SEScalarTime.unload(src.getInspiratoryRiseTime()));
      if (src.hasInspiratoryHoldTime())
        dst.setInspiratoryHoldTime(SEScalarTime.unload(src.getInspiratoryHoldTime()));
      if (src.hasInspiratoryReleaseTime())
        dst.setInspiratoryReleaseTime(SEScalarTime.unload(src.getInspiratoryReleaseTime()));
      if (src.hasInspiratoryToExpiratoryPauseTime())
        dst.setInspiratoryToExpiratoryPauseTime(SEScalarTime.unload(src.getInspiratoryToExpiratoryPauseTime()));
      if (src.hasExpiratoryRiseTime())
        dst.setExpiratoryRiseTime(SEScalarTime.unload(src.getExpiratoryRiseTime()));
      if (src.hasExpiratoryHoldTime())
        dst.setExpiratoryHoldTime(SEScalarTime.unload(src.getExpiratoryHoldTime()));
      if (src.hasExpiratoryReleaseTime())
        dst.setExpiratoryReleaseTime(SEScalarTime.unload(src.getExpiratoryReleaseTime()));
      if (src.hasResidueTime())
        dst.setResidueTime(SEScalarTime.unload(src.getResidueTime()));
      
    }

    public boolean hasActive()
    {
      if (active == eSwitch.NullSwitch)
        return false;
      return true;
    }
    public eSwitch getActive()
    {
      return active;
    }
    public void setActive(eSwitch s)
    {
      active = s;
    }

    public eDefaultType getDefaultType()
    {
      return defaultType;
    }
    public void setDefaultType(eDefaultType d)
    {
      defaultType = d;
    }

    public boolean hasLeftComplianceCurve()
    {
      if (leftComplianceCurve != null)
        return leftComplianceCurve.isValid();
      return false;
    }
    public SECurve getLeftComplianceCurve()
    {
      if (leftComplianceCurve == null)
        leftComplianceCurve = new SECurve();
      return leftComplianceCurve;
    }

    public boolean hasRightComplianceCurve()
    {
      if (rightComplianceCurve != null)
        return rightComplianceCurve.isValid();
      return false;
    }
    public SECurve getRightComplianceCurve()
    {
      if (rightComplianceCurve == null)
        rightComplianceCurve = new SECurve();
      return rightComplianceCurve;
    }

    public boolean hasLeftExpiratoryResistance()
    {
      if (leftExpiratoryResistance != null)
        return leftExpiratoryResistance.isValid();
      return false;
    }
    public SEScalarPressureTimePerVolume getLeftExpiratoryResistance()
    {
      if (leftExpiratoryResistance == null)
        leftExpiratoryResistance = new SEScalarPressureTimePerVolume();
      return leftExpiratoryResistance;
    }

    public boolean hasLeftInspiratoryResistance()
    {
      if (leftInspiratoryResistance != null)
        return leftInspiratoryResistance.isValid();
      return false;
    }
    public SEScalarPressureTimePerVolume getLeftInspiratoryResistance()
    {
      if (leftInspiratoryResistance == null)
        leftInspiratoryResistance = new SEScalarPressureTimePerVolume();
      return leftInspiratoryResistance;
    }

    public boolean hasRightExpiratoryResistance()
    {
      if (rightExpiratoryResistance != null)
        return rightExpiratoryResistance.isValid();
      return false;
    }
    public SEScalarPressureTimePerVolume getRightExpiratoryResistance()
    {
      if (rightExpiratoryResistance == null)
        rightExpiratoryResistance = new SEScalarPressureTimePerVolume();
      return rightExpiratoryResistance;
    }

    public boolean hasRightInspiratoryResistance()
    {
      if (rightInspiratoryResistance != null)
        return rightInspiratoryResistance.isValid();
      return false;
    }
    public SEScalarPressureTimePerVolume getRightInspiratoryResistance()
    {
      if (rightInspiratoryResistance == null)
        rightInspiratoryResistance = new SEScalarPressureTimePerVolume();
      return rightInspiratoryResistance;
    }

    public boolean hasUpperExpiratoryResistance()
    {
      if (upperExpiratoryResistance != null)
        return upperExpiratoryResistance.isValid();
      return false;
    }
    public SEScalarPressureTimePerVolume getUpperExpiratoryResistance()
    {
      if (upperExpiratoryResistance == null)
        upperExpiratoryResistance = new SEScalarPressureTimePerVolume();
      return upperExpiratoryResistance;
    }

    public boolean hasUpperInspiratoryResistance()
    {
      if (upperInspiratoryResistance != null)
        return upperInspiratoryResistance.isValid();
      return false;
    }
    public SEScalarPressureTimePerVolume getUpperInspiratoryResistance()
    {
      if (upperInspiratoryResistance == null)
        upperInspiratoryResistance = new SEScalarPressureTimePerVolume();
      return upperInspiratoryResistance;
    }

    public boolean hasInspiratoryPeakPressure()
    {
      if (inspiratoryPeakPressure != null)
        return inspiratoryPeakPressure.isValid();
      return false;
    }
    public SEScalarPressure getInspiratoryPeakPressure()
    {
      if (inspiratoryPeakPressure == null)
        inspiratoryPeakPressure = new SEScalarPressure();
      return inspiratoryPeakPressure;
    }

    public boolean hasExpiratoryPeakPressure()
    {
      if (expiratoryPeakPressure != null)
        return expiratoryPeakPressure.isValid();
      return false;
    }
    public SEScalarPressure getExpiratoryPeakPressure()
    {
      if (expiratoryPeakPressure == null)
        expiratoryPeakPressure = new SEScalarPressure();
      return expiratoryPeakPressure;
    }

    public boolean hasInspiratoryRiseTime()
    {
      if (inspiratoryRiseTime != null)
        return inspiratoryRiseTime.isValid();
      return false;
    }
    public SEScalarTime getInspiratoryRiseTime()
    {
      if (inspiratoryRiseTime == null)
        inspiratoryRiseTime = new SEScalarTime();
      return inspiratoryRiseTime;
    }

    public boolean hasInspiratoryHoldTime()
    {
      if (inspiratoryHoldTime != null)
        return inspiratoryHoldTime.isValid();
      return false;
    }
    public SEScalarTime getInspiratoryHoldTime()
    {
      if (inspiratoryHoldTime == null)
        inspiratoryHoldTime = new SEScalarTime();
      return inspiratoryHoldTime;
    }

    public boolean hasInspiratoryReleaseTime()
    {
      if (inspiratoryReleaseTime != null)
        return inspiratoryReleaseTime.isValid();
      return false;
    }
    public SEScalarTime getInspiratoryReleaseTime()
    {
      if (inspiratoryReleaseTime == null)
        inspiratoryReleaseTime = new SEScalarTime();
      return inspiratoryReleaseTime;
    }

    public boolean hasInspiratoryToExpiratoryPauseTime()
    {
      if (inspiratoryToExpiratoryPauseTime != null)
        return inspiratoryToExpiratoryPauseTime.isValid();
      return false;
    }
    public SEScalarTime getInspiratoryToExpiratoryPauseTime()
    {
      if (inspiratoryToExpiratoryPauseTime == null)
        inspiratoryToExpiratoryPauseTime = new SEScalarTime();
      return inspiratoryToExpiratoryPauseTime;
    }

    public boolean hasExpiratoryRiseTime()
    {
      if (expiratoryRiseTime != null)
        return expiratoryRiseTime.isValid();
      return false;
    }
    public SEScalarTime getExpiratoryRiseTime()
    {
      if (expiratoryRiseTime == null)
        expiratoryRiseTime = new SEScalarTime();
      return expiratoryRiseTime;
    }

    public boolean hasExpiratoryHoldTime()
    {
      if (expiratoryHoldTime != null)
        return expiratoryHoldTime.isValid();
      return false;
    }
    public SEScalarTime getExpiratoryHoldTime()
    {
      if (expiratoryHoldTime == null)
        expiratoryHoldTime = new SEScalarTime();
      return expiratoryHoldTime;
    }

    public boolean hasExpiratoryReleaseTime()
    {
      if (expiratoryReleaseTime != null)
        return expiratoryReleaseTime.isValid();
      return false;
    }
    public SEScalarTime getExpiratoryReleaseTime()
    {
      if (expiratoryReleaseTime == null)
        expiratoryReleaseTime = new SEScalarTime();
      return expiratoryReleaseTime;
    }

    public boolean hasResidueTime()
    {
      if (residueTime != null)
        return residueTime.isValid();
      return false;
    }
    public SEScalarTime getResidueTime()
    {
      if (residueTime == null)
        residueTime = new SEScalarTime();
      return residueTime;
    }

    public String toString()
    {
      String str = "Respiratory Mechanics";
      str += "\n\tActive: "; if (hasActive()) str += active; else str += "Not Set";
      str += "\n\tDefaultType: "; str += defaultType;
      str += "\n\tLeftComplianceCurve: "; if (hasLeftComplianceCurve()) str += leftComplianceCurve.toString(); else str += "Not Set";
      str += "\n\tRightComplianceCurve: "; if (hasRightComplianceCurve()) str += rightComplianceCurve.toString(); else str += "Not Set";
      str += "\n\tLeftExpiratoryResistance: "; if (hasLeftExpiratoryResistance()) str += leftExpiratoryResistance.toString(); else str += "Not Set";
      str += "\n\tLeftInspiratoryResistance: "; if (hasLeftInspiratoryResistance()) str += leftInspiratoryResistance.toString(); else str += "Not Set";
      str += "\n\tRightExpiratoryResistance: "; if (hasRightExpiratoryResistance()) str += rightExpiratoryResistance.toString(); else str += "Not Set";
      str += "\n\tRightInspiratoryResistance: "; if (hasRightInspiratoryResistance()) str += rightInspiratoryResistance.toString(); else str += "Not Set";
      str += "\n\tUpperExpiratoryResistance: "; if (hasUpperExpiratoryResistance()) str += upperExpiratoryResistance.toString(); else str += "Not Set";
      str += "\n\tUpperInspiratoryResistance: "; if (hasUpperInspiratoryResistance()) str += upperInspiratoryResistance.toString(); else str += "Not Set";
      str += "\n\tInspiratoryPeakPressure: "; if (hasInspiratoryPeakPressure()) str += inspiratoryPeakPressure.toString(); else str += "Not Set";
      str += "\n\tExpiratoryPeakPressure: "; if (hasExpiratoryPeakPressure()) str += expiratoryPeakPressure.toString(); else str += "Not Set";
      str += "\n\tInspiratoryRiseTime: "; if (hasInspiratoryRiseTime()) str += inspiratoryRiseTime.toString(); else str += "Not Set";
      str += "\n\tInspiratoryHoldTime: "; if (hasInspiratoryHoldTime()) str += inspiratoryHoldTime.toString(); else str += "Not Set";
      str += "\n\tInspiratoryReleaseTime: "; if (hasInspiratoryReleaseTime()) str += inspiratoryReleaseTime.toString(); else str += "Not Set";
      str += "\n\tInspiratoryToExpiratoryPauseTime: "; if (hasInspiratoryToExpiratoryPauseTime()) str += inspiratoryToExpiratoryPauseTime.toString(); else str += "Not Set";
      str += "\n\tExpiratoryRiseTime: "; if (hasExpiratoryRiseTime()) str += expiratoryRiseTime.toString(); else str += "Not Set";
      str += "\n\tExpiratoryHoldTime: "; if (hasExpiratoryHoldTime()) str += expiratoryHoldTime.toString(); else str += "Not Set";
      str += "\n\tExpiratoryReleaseTime: "; if (hasExpiratoryReleaseTime()) str += expiratoryReleaseTime.toString(); else str += "Not Set";
      str += "\n\tResidueTime: "; if (hasResidueTime()) str += residueTime.toString(); else str += "Not Set";
      return str;
    }

  }

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.Enums.eSide;
import com.kitware.pulse.cdm.bind.PatientActions.HemothoraxData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;
import com.kitware.pulse.cdm.properties.SEScalarVolumePerTime;
import com.kitware.pulse.cdm.properties.SEScalarVolume;

public class SEHemothorax extends SEPatientAction
{
  protected eSide side;
  protected SEScalar0To1 severity;
  protected SEScalarVolumePerTime flowRate;
  protected SEScalarVolume targetVolume;

  public SEHemothorax()
  {
    side = eSide.NullSide;
    severity = null;
    flowRate = null;
    targetVolume = null;
  }

  public void copy(SEHemothorax other)
  {
    if(this==other)
      return;
    super.copy(other);
    side = other.side;
    if (other.severity != null)
      getSeverity().set(other.severity);
    else if (severity != null)
      severity.invalidate();
    if (other.flowRate != null)
      getFlowRate().set(other.getFlowRate());
    else if (flowRate != null)
      flowRate.invalidate();
    if (other.targetVolume != null)
      getTargetVolume().set(other.getTargetVolume());
    else if (targetVolume != null)
      targetVolume.invalidate();
  }

  @Override
  public void clear()
  {
    super.clear();
    side = eSide.NullSide;
    if (severity != null)
      severity.invalidate();
    if (flowRate != null)
      flowRate.invalidate();
    if (targetVolume != null)
      targetVolume.invalidate();
  }

  @Override
  public boolean isValid()
  {
    return hasSide() && (hasFlowRate() || hasSeverity());
  }

  public static void load(HemothoraxData src, SEHemothorax dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.getSide()!=eSide.UNRECOGNIZED)
    	dst.side = src.getSide();
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
    if(src.hasFlowRate())
      SEScalarVolumePerTime.load(src.getFlowRate(),dst.getFlowRate());
    if(src.hasTargetVolume())
      SEScalarVolume.load(src.getTargetVolume(),dst.getTargetVolume());
  }

  public static HemothoraxData unload(SEHemothorax src)
  {
    HemothoraxData.Builder dst = HemothoraxData.newBuilder();
    unload(src,dst);
    return dst.build();
  }

  protected static void unload(SEHemothorax src, HemothoraxData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if (src.hasSide())
      dst.setSide(src.side);
    if (src.hasSeverity())
      dst.setSeverity(SEScalar0To1.unload(src.severity));
    if (src.hasFlowRate())
      dst.setFlowRate(SEScalarVolumePerTime.unload(src.flowRate));
    if (src.hasTargetVolume())
      dst.setTargetVolume(SEScalarVolume.unload(src.targetVolume));
  }

  public eSide getSide()
  {
    return side;
  }
  public void setSide(eSide side)
  {
    this.side = side;
  }
  public boolean hasSide()
  {
    return side == eSide.NullSide ? false : true;
  }

  public boolean hasSeverity()
  {
    return severity == null ? false : severity.isValid();
  }
  public SEScalar0To1 getSeverity()
  {
    if (severity == null)
      severity = new SEScalar0To1();
    return severity;
  }

  public boolean hasFlowRate()
  {
    return flowRate == null ? false : flowRate.isValid();
  }
  public SEScalarVolumePerTime getFlowRate()
  {
    if (flowRate == null)
      flowRate = new SEScalarVolumePerTime();
    return flowRate;
  }

  public boolean hasTargetVolume()
  {
    return targetVolume == null ? false : targetVolume.isValid();
  }
  public SEScalarVolume getTargetVolume()
  {
    if (targetVolume == null)
      targetVolume = new SEScalarVolume();
    return targetVolume;
  }

  @Override
  public String toString()
  {
    return "Hemothorax"
        + "\n\tSide : " + getSide()
        + "\n\tSeverity: " + getSeverity()
        + "\n\tFlowRate: " + getFlowRate()
        + "\n\tTargetVolume : " + getTargetVolume();

  }

}

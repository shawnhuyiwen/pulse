/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.Enums.eSide;
import com.kitware.pulse.cdm.bind.PatientActions.TubeThoracostomyData;
import com.kitware.pulse.cdm.properties.SEScalarVolumePerTime;

public class SETubeThoracostomy extends SEPatientAction
{
  protected eSide side;
  protected SEScalarVolumePerTime flowRate;

  public SETubeThoracostomy()
  {
    side = eSide.NullSide;
    flowRate = null;
  }

  public void copy(SETubeThoracostomy other)
  {
    if(this==other)
      return;
    super.copy(other);
    side = other.side;
    if (other.flowRate != null)
      getFlowRate().set(other.getFlowRate());
    else if (flowRate != null)
      flowRate.invalidate();
  }

  @Override
  public void clear()
  {
    super.clear();
    side = eSide.NullSide;
    if (flowRate != null)
      flowRate.invalidate();
  }

  @Override
  public boolean isValid()
  {
    return hasSide();
  }

  public static void load(TubeThoracostomyData src, SETubeThoracostomy dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.getSide()!=eSide.UNRECOGNIZED)
    	dst.side = src.getSide();
    if(src.hasFlowRate())
      SEScalarVolumePerTime.load(src.getFlowRate(),dst.getFlowRate());
  }

  public static TubeThoracostomyData unload(SETubeThoracostomy src)
  {
    TubeThoracostomyData.Builder dst = TubeThoracostomyData.newBuilder();
    unload(src,dst);
    return dst.build();
  }

  protected static void unload(SETubeThoracostomy src, TubeThoracostomyData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if (src.hasSide())
      dst.setSide(src.side);
    if (src.hasFlowRate())
      dst.setFlowRate(SEScalarVolumePerTime.unload(src.flowRate));
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

  @Override
  public String toString()
  {
    return "Tube Thoracostomy"
        + "\n\tSide : " + getSide()
        + "\n\tFlowRate: " + getFlowRate();

  }

}

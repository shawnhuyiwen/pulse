/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.bag_valve_mask.actions;

import com.kitware.pulse.cdm.bind.BagValveMaskActions.BagValveMaskInstantaneousData;
import com.kitware.pulse.cdm.properties.*;

public class SEBagValveMaskInstantaneous extends SEBagValveMaskAction
{
  private static final long serialVersionUID = -1487014080271791164L;
  protected SEScalarVolumePerTime flow;
  protected SEScalarPressure      pressure;
  
  public SEBagValveMaskInstantaneous()
  {
    flow=null;
    pressure=null;
  }
  
  public SEBagValveMaskInstantaneous(SEBagValveMaskInstantaneous other)
  {
    this();
    copy(other);
  }
  
  public void copy(SEBagValveMaskInstantaneous from)
  {
    if(this==from)
      return;
    super.copy(from);
    if(from.hasFlow())
      this.getFlow().set(from.getFlow());
    if(from.hasPressure())
      this.getPressure().set(from.getPressure());
  }
  
  @Override
  public void clear()
  {
    super.clear();
    
    if (flow != null)
      flow.invalidate();
    if (pressure != null)
      pressure.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return hasFlow() || hasPressure();
  }
  
  public static void load(BagValveMaskInstantaneousData src, SEBagValveMaskInstantaneous dst)
  {
    dst.clear();
    SEBagValveMaskAction.load(src.getBagValveMaskAction(),dst);
    if (src.hasFlow())
      SEScalarVolumePerTime.load(src.getFlow(), dst.getFlow());
    if (src.hasPressure())
      SEScalarPressure.load(src.getPressure(), dst.getPressure());
  }
  public static BagValveMaskInstantaneousData unload(SEBagValveMaskInstantaneous src)
  {
    BagValveMaskInstantaneousData.Builder dst = BagValveMaskInstantaneousData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEBagValveMaskInstantaneous src, BagValveMaskInstantaneousData.Builder dst)
  {
    SEBagValveMaskAction.unload(src, dst.getBagValveMaskActionBuilder());
    if(src.hasFlow())
      dst.setFlow(SEScalarVolumePerTime.unload(src.getFlow()));
    if (src.hasPressure())
      dst.setPressure(SEScalarPressure.unload(src.getPressure()));
  }
  
  public SEScalarVolumePerTime getFlow()
  {
    if (flow == null)
      flow = new SEScalarVolumePerTime();
    return flow;
  }
  public boolean hasFlow()
  {
    return flow == null ? false : flow.isValid();
  }
  
  public SEScalarPressure getPressure()
  {
    if (pressure == null)
      pressure = new SEScalarPressure();
    return pressure;
  }
  public boolean hasPressure()
  {
    return pressure == null ? false : pressure.isValid();
  }
  
  @Override
  public String toString()
  {
    String str = "Bag Valve Mask Instantaneous"
        + "\n\tFlow: " + (hasFlow()?getFlow():"NotProvided")
        + "\n\tPressure: " + (hasPressure()?getPressure():"NotProvided");
    
    return str;
  }
}
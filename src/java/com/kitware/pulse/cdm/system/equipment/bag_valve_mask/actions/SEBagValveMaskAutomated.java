/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.bag_valve_mask.actions;

import com.kitware.pulse.cdm.bind.BagValveMaskActions.BagValveMaskAutomatedData;
import com.kitware.pulse.cdm.properties.*;

public class SEBagValveMaskAutomated extends SEBagValveMaskAction
{
  private static final long serialVersionUID = -1487014080271791164L;
  protected SEScalarFrequency breathFrequency;
  protected SEScalar          inspiratoryExpiratoryRatio;
  protected SEScalarPressure  squeezePressure;
  protected SEScalarVolume    squeezeVolume;
  
  public SEBagValveMaskAutomated()
  {
    breathFrequency=null;
    inspiratoryExpiratoryRatio=null;
    squeezePressure=null;
    squeezeVolume=null;
  }
  
  public SEBagValveMaskAutomated(SEBagValveMaskAutomated other)
  {
    this();
    copy(other);
  }
  
  public void copy(SEBagValveMaskAutomated from)
  {
    if(this==from)
      return;
    super.copy(from);
    if(from.hasSqueezePressure())
      this.getSqueezePressure().set(from.getSqueezePressure());
    if(from.hasSqueezeVolume())
      this.getSqueezeVolume().set(from.getSqueezeVolume());
    if(from.hasBreathFrequency())
      this.getBreathFrequency().set(from.getBreathFrequency());
    if(from.hasInspiratoryExpiratoryRatio())
      this.getInspiratoryExpiratoryRatio().set(from.getInspiratoryExpiratoryRatio());
  }
  
  @Override
  public void clear()
  {
    super.clear();
    
    if (breathFrequency != null)
      breathFrequency.invalidate();
    if (inspiratoryExpiratoryRatio != null)
      inspiratoryExpiratoryRatio.invalidate();
    if (squeezePressure != null)
      squeezePressure.invalidate();
    if (squeezeVolume != null)
      squeezeVolume.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return true;
  }
  
  public static void load(BagValveMaskAutomatedData src, SEBagValveMaskAutomated dst)
  {
    dst.clear();
    SEBagValveMaskAction.load(src.getBagValveMaskAction(),dst);
    if (src.hasBreathFrequency())
      SEScalarFrequency.load(src.getBreathFrequency(), dst.getBreathFrequency());
    if (src.hasInspiratoryExpiratoryRatio())
      SEScalar.load(src.getInspiratoryExpiratoryRatio(), dst.getInspiratoryExpiratoryRatio());
    if (src.hasSqueezePressure())
      SEScalarPressure.load(src.getSqueezePressure(), dst.getSqueezePressure());
    if (src.hasSqueezeVolume())
      SEScalarVolume.load(src.getSqueezeVolume(), dst.getSqueezeVolume());
  }
  public static BagValveMaskAutomatedData unload(SEBagValveMaskAutomated src)
  {
    BagValveMaskAutomatedData.Builder dst = BagValveMaskAutomatedData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEBagValveMaskAutomated src, BagValveMaskAutomatedData.Builder dst)
  {
    SEBagValveMaskAction.unload(src, dst.getBagValveMaskActionBuilder());
    if (src.hasBreathFrequency())
      dst.setBreathFrequency(SEScalarFrequency.unload(src.getBreathFrequency()));
    if (src.hasInspiratoryExpiratoryRatio())
      dst.setInspiratoryExpiratoryRatio(SEScalar.unload(src.getInspiratoryExpiratoryRatio()));
    if(src.hasSqueezePressure())
      dst.setSqueezePressure(SEScalarPressure.unload(src.getSqueezePressure()));
    if (src.hasSqueezeVolume())
      dst.setSqueezeVolume(SEScalarVolume.unload(src.getSqueezeVolume()));
  }

  public SEScalarFrequency getBreathFrequency()
  {
    if (breathFrequency == null)
      breathFrequency = new SEScalarFrequency();
    return breathFrequency;
  }
  public boolean hasBreathFrequency()
  {
    return breathFrequency == null ? false : breathFrequency.isValid();
  }
  
  public SEScalar getInspiratoryExpiratoryRatio()
  {
    if (inspiratoryExpiratoryRatio == null)
      inspiratoryExpiratoryRatio = new SEScalar();
    return inspiratoryExpiratoryRatio;
  }
  public boolean hasInspiratoryExpiratoryRatio()
  {
    return inspiratoryExpiratoryRatio == null ? false : inspiratoryExpiratoryRatio.isValid();
  }
  
  public SEScalarPressure getSqueezePressure()
  {
    if (squeezePressure == null)
      squeezePressure = new SEScalarPressure();
    return squeezePressure;
  }
  public boolean hasSqueezePressure()
  {
    return squeezePressure == null ? false : squeezePressure.isValid();
  }
  
  public SEScalarVolume getSqueezeVolume()
  {
    if (squeezeVolume == null)
      squeezeVolume = new SEScalarVolume();
    return squeezeVolume;
  }
  public boolean hasSqueezeVolume()
  {
    return squeezeVolume == null ? false : squeezeVolume.isValid();
  }
  
  @Override
  public String toString()
  {
    String str = "Bag Valve Mask Squeeze"
        + "\n\tBreathFrequency: " + (hasBreathFrequency()?getBreathFrequency():"NotProvided")
        + "\n\tInspiratoryExpiratoryRatio: " + (hasInspiratoryExpiratoryRatio()?getInspiratoryExpiratoryRatio():"NotProvided")
        + "\n\tSqueezePressure: " + (hasSqueezePressure()?getSqueezePressure():"NotProvided")
        + "\n\tSqueezeVolume: " + (hasSqueezeVolume()?getSqueezeVolume():"NotProvided");
    
    return str;
  }
}
/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.bag_valve_mask.actions;

import com.kitware.pulse.cdm.bind.BagValveMaskActions.BagValveMaskSqueezeData;
import com.kitware.pulse.cdm.properties.*;

public class SEBagValveMaskSqueeze extends SEBagValveMaskAction
{
  private static final long serialVersionUID = -1487014080271791164L;
  protected SEScalarPressure  squeezePressure;
  protected SEScalarVolume    squeezeVolume;
  protected SEScalarTime      expiratoryPeriod;
  protected SEScalarTime      inspiratoryPeriod;
  
  public SEBagValveMaskSqueeze()
  {
    squeezePressure=null;
    squeezeVolume=null;
    expiratoryPeriod=null;
    inspiratoryPeriod=null;
  }
  
  public SEBagValveMaskSqueeze(SEBagValveMaskSqueeze other)
  {
    this();
    copy(other);
  }
  
  public void copy(SEBagValveMaskSqueeze from)
  {
    if(this==from)
      return;
    super.copy(from);
    if(from.hasSqueezePressure())
      this.getSqueezePressure().set(from.getSqueezePressure());
    if(from.hasSqueezeVolume())
      this.getSqueezeVolume().set(from.getSqueezeVolume());
    if(from.hasExpiratoryPeriod())
      this.getExpiratoryPeriod().set(from.getExpiratoryPeriod());
    if(from.hasInspiratoryPeriod())
      this.getInspiratoryPeriod().set(from.getInspiratoryPeriod());
  }
  
  @Override
  public void clear()
  {
    super.clear();
    
    if (squeezePressure != null)
      squeezePressure.invalidate();
    if (squeezeVolume != null)
      squeezeVolume.invalidate();
    if (expiratoryPeriod != null)
      expiratoryPeriod.invalidate();
    if (inspiratoryPeriod != null)
      inspiratoryPeriod.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return true;
  }
  
  public static void load(BagValveMaskSqueezeData src, SEBagValveMaskSqueeze dst)
  {
    dst.clear();
    SEBagValveMaskAction.load(src.getBagValveMaskAction(),dst);
    if (src.hasSqueezePressure())
      SEScalarPressure.load(src.getSqueezePressure(), dst.getSqueezePressure());
    if (src.hasSqueezeVolume())
      SEScalarVolume.load(src.getSqueezeVolume(), dst.getSqueezeVolume());
    if (src.hasExpiratoryPeriod())
      SEScalarTime.load(src.getExpiratoryPeriod(), dst.getExpiratoryPeriod());
    if (src.hasInspiratoryPeriod())
      SEScalarTime.load(src.getInspiratoryPeriod(), dst.getInspiratoryPeriod());
  }
  public static BagValveMaskSqueezeData unload(SEBagValveMaskSqueeze src)
  {
    BagValveMaskSqueezeData.Builder dst = BagValveMaskSqueezeData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEBagValveMaskSqueeze src, BagValveMaskSqueezeData.Builder dst)
  {
    SEBagValveMaskAction.unload(src, dst.getBagValveMaskActionBuilder());
    if(src.hasSqueezePressure())
      dst.setSqueezePressure(SEScalarPressure.unload(src.getSqueezePressure()));
    if (src.hasSqueezeVolume())
      dst.setSqueezeVolume(SEScalarVolume.unload(src.getSqueezeVolume()));
    if (src.hasExpiratoryPeriod())
      dst.setExpiratoryPeriod(SEScalarTime.unload(src.getExpiratoryPeriod()));
    if (src.hasInspiratoryPeriod())
      dst.setInspiratoryPeriod(SEScalarTime.unload(src.getInspiratoryPeriod()));
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
  
  public SEScalarTime getExpiratoryPeriod()
  {
    if (expiratoryPeriod == null)
      expiratoryPeriod = new SEScalarTime();
    return expiratoryPeriod;
  }
  public boolean hasExpiratoryPeriod()
  {
    return expiratoryPeriod == null ? false : expiratoryPeriod.isValid();
  }
  
  public SEScalarTime getInspiratoryPeriod()
  {
    if (inspiratoryPeriod == null)
      inspiratoryPeriod = new SEScalarTime();
    return inspiratoryPeriod;
  }
  public boolean hasInspiratoryPeriod()
  {
    return inspiratoryPeriod == null ? false : inspiratoryPeriod.isValid();
  }
  
  @Override
  public String toString()
  {
    String str = "Bag Valve Mask Squeeze"
        + "\n\tSqueezePressure: " + (hasSqueezePressure()?getSqueezePressure():"NotProvided")
        + "\n\tSqueezeVolume: " + (hasSqueezeVolume()?getSqueezeVolume():"NotProvided")
        + "\n\tExpiratoryPeriod: " + (hasExpiratoryPeriod()?getExpiratoryPeriod():"NotProvided")
        + "\n\tInspiratoryPeriod: " + (hasInspiratoryPeriod()?getInspiratoryPeriod():"NotProvided");
    
    return str;
  }
}
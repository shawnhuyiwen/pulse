/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.ChestCompressionInstantaneousData;
import com.kitware.pulse.cdm.properties.SEScalarForce;
import com.kitware.pulse.cdm.properties.SEScalar0To1;
import com.kitware.pulse.cdm.properties.SEScalarTime;

public class SEChestCompressionInstantaneous extends SEPatientAction
{
  private static final long serialVersionUID = 4548017190134330946L;
  
  protected SEScalarForce    force;
  protected SEScalar0To1     forceScale;
  protected SEScalarTime     forcePeriod;  
  
  public SEChestCompressionInstantaneous()
  {
    force = null;
    forceScale = null;
    forcePeriod = null;
  }
  
  public SEChestCompressionInstantaneous(SEChestCompressionInstantaneous other)
  {
    this();
    copy(other);
  }
  
  public void copy(SEChestCompressionInstantaneous other)
  {
    if (other.force != null)
      getForce().set(other.force);
    else if (force != null)
      force.invalidate();
    
    if (other.forceScale != null)
      getForceScale().set(other.forceScale);
    else if (forceScale != null)
      forceScale.invalidate();
    
    if (other.forcePeriod != null)
      getForcePeriod().set(other.forcePeriod);
    else if (forcePeriod != null)
      forcePeriod.invalidate();
  }
  
  @Override
  public void clear()
  {
    super.clear();
    if (force != null)
      force.invalidate();
    if (forceScale != null)
      forceScale.invalidate();
    if (forcePeriod != null)
      forcePeriod.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return hasForce() != hasForceScale();
  }
  
  public static void load(ChestCompressionInstantaneousData src, SEChestCompressionInstantaneous dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasForce())
      SEScalarForce.load(src.getForce(),dst.getForce());
    if(src.hasForceScale())
      SEScalar0To1.load(src.getForceScale(),dst.getForceScale());
    if(src.hasForcePeriod())
      SEScalarTime.load(src.getForcePeriod(),dst.getForcePeriod());
  }
  
  public static ChestCompressionInstantaneousData unload(SEChestCompressionInstantaneous src)
  {
    ChestCompressionInstantaneousData.Builder dst = ChestCompressionInstantaneousData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEChestCompressionInstantaneous src, ChestCompressionInstantaneousData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if (src.hasForce())
      dst.setForce(SEScalarForce.unload(src.force));
    if (src.hasForceScale())
      dst.setForceScale(SEScalar0To1.unload(src.forceScale));
    if (src.hasForcePeriod())
      dst.setForcePeriod(SEScalarTime.unload(src.forcePeriod));
  }
  
  public boolean hasForce()
  {
    return force == null ? false : force.isValid();
  }
  public SEScalarForce getForce()
  {
    if (force == null)
      force = new SEScalarForce();
    return force;
  }

  public boolean hasForceScale()
  {
    return forceScale == null ? false : forceScale.isValid();
  }
  public SEScalar0To1 getForceScale()
  {
    if (forceScale == null)
      forceScale = new SEScalar0To1();
    return forceScale;
  }
  
  public boolean hasForcePeriod()
  {
    return forcePeriod == null ? false : forcePeriod.isValid();
  }
  public SEScalarTime getForcePeriod()
  {
    if (forcePeriod == null)
      forcePeriod = new SEScalarTime();
    return forcePeriod;
  }
  
  @Override
  public String toString()
  {
    if (force != null)
      return "Chest Compression" 
          + "\n\tForce: " +(hasForce() ? getForce() : "Not Provided")
          + "\n\tForceScale: " + (hasForceScale() ? getForceScale() : "Not Provided")
          + "\n\tForcePeriod: " + (hasForcePeriod() ? getForcePeriod() : "Not Provided");
    else
      return "Action not specified properly";
  }
}
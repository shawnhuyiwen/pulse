/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.ChestCompressionInstantaneousData;
import com.kitware.pulse.cdm.properties.SEScalarForce;
import com.kitware.pulse.cdm.properties.SEScalar0To1;

public class SEChestCompressionInstantaneous extends SEPatientAction
{
  private static final long serialVersionUID = 4548017190134330946L;
  
  protected SEScalarForce    force;
  protected SEScalar0To1     forceScale;
  
  public SEChestCompressionInstantaneous()
  {
    force = null;
    forceScale = null;
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
  }
  
  @Override
  public void clear()
  {
    super.clear();
    if (force != null)
      force.invalidate();
    if (forceScale != null)
      forceScale.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return (hasForce() || hasForceScale());
  }
  
  public static void load(ChestCompressionInstantaneousData src, SEChestCompressionInstantaneous dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasForce())
      SEScalarForce.load(src.getForce(),dst.getForce());
    if(src.hasForceScale())
      SEScalar0To1.load(src.getForceScale(),dst.getForceScale());
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
  
  @Override
  public String toString()
  {
    if (force != null)
      return "Chest Compression" 
          + "\n\tForce: " +(hasForce() ? getForce() : "Not Provided")
          + "\n\tForceScale: " + (hasForceScale() ? getForceScale() : "Not Provided");
    else
      return "Action not specified properly";
  }
}
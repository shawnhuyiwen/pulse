/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.properties.SEScalarForce;
import com.kitware.pulse.cdm.bind.PatientActions.ChestCompressionData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;
import com.kitware.pulse.cdm.properties.SEScalarTime;

public class SEChestCompression extends SEPatientAction
{
  private static final long serialVersionUID = 4548017190134330946L;
  
  protected SEScalarForce    force;
  protected SEScalar0To1     forceScale;
  protected SEScalarTime     compressionPeriod;
  
  public SEChestCompression()
  {
    force = null;
    forceScale = null;
    compressionPeriod = null;
  }
  
  public SEChestCompression(SEChestCompression other)
  {
    this();
    copy(other);
  }
  
  public void copy(SEChestCompression other)
  {
    if (other.force != null)
      getForce().set(other.force);
    else if (force != null)
      force.invalidate();
    
    if (other.forceScale != null)
      getForceScale().set(other.forceScale);
    else if (forceScale != null)
      forceScale.invalidate();
    
    if (other.compressionPeriod != null)
      getCompressionPeriod().set(other.compressionPeriod);
    else if (compressionPeriod != null)
      compressionPeriod.invalidate();
  }
  
  @Override
  public void clear()
  {
    super.clear();
    if (force != null)
      force.invalidate();
    if (forceScale != null)
      forceScale.invalidate();
    if (compressionPeriod != null)
      compressionPeriod.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return (hasForce() || hasForceScale()) && hasCompressionPeriod();
  }
  
  public static void load(ChestCompressionData src, SEChestCompression dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasForce())
      SEScalarForce.load(src.getForce(),dst.getForce());
    if(src.hasForceScale())
      SEScalar0To1.load(src.getForceScale(),dst.getForceScale());
    if(src.hasCompressionPeriod())
      SEScalarTime.load(src.getCompressionPeriod(),dst.getCompressionPeriod());
  }
  
  public static ChestCompressionData unload(SEChestCompression src)
  {
    ChestCompressionData.Builder dst = ChestCompressionData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEChestCompression src, ChestCompressionData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if (src.hasForce())
      dst.setForce(SEScalarForce.unload(src.force));
    if (src.hasForceScale())
      dst.setForceScale(SEScalar0To1.unload(src.forceScale));
    if (src.hasCompressionPeriod())
      dst.setCompressionPeriod(SEScalarTime.unload(src.compressionPeriod));
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
  
  public boolean hasCompressionPeriod()
  {
    return compressionPeriod == null ? false : compressionPeriod.isValid();
  }
  public SEScalarTime getCompressionPeriod()
  {
    if (compressionPeriod == null)
      compressionPeriod = new SEScalarTime();
    return compressionPeriod;
  }
  
  @Override
  public String toString()
  {
    if (force != null)
      return "Chest Compression" 
          + "\n\tForce: " +(hasForce() ? getForce() : "Not Provided")
          + "\n\tForceScale: " + (hasForceScale() ? getForceScale() : "Not Provided")
          + "\n\tCompressionPeriod: " + (hasCompressionPeriod() ? getCompressionPeriod() : "Not Provided");
    else
      return "Action not specified properly";
  }
}
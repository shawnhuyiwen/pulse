/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.properties.SEScalarForce;
import com.kitware.pulse.cdm.bind.PatientActions.ChestCompressionData;
import com.kitware.pulse.cdm.properties.SEScalarTime;
import com.kitware.pulse.cdm.properties.SEScalarLength;

public class SEChestCompression extends SEPatientAction
{
  private static final long serialVersionUID = 4548017190134330946L;
  
  protected SEScalarForce    force;
  protected SEScalarLength   depth;
  protected SEScalarTime     compressionPeriod;
  
  public SEChestCompression()
  {
    force = null;
    depth = null;
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
    
    if (other.depth != null)
      getDepth().set(other.depth);
    else if (depth != null)
      depth.invalidate();
    
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
    if (depth != null)
      depth.invalidate();
    if (compressionPeriod != null)
      compressionPeriod.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return (hasForce() || hasDepth()) && hasCompressionPeriod();
  }
  
  public static void load(ChestCompressionData src, SEChestCompression dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasForce())
      SEScalarForce.load(src.getForce(),dst.getForce());
    if(src.hasDepth())
      SEScalarLength.load(src.getDepth(),dst.getDepth());
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
    if (src.hasDepth())
      dst.setDepth(SEScalarLength.unload(src.depth));
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

  public boolean hasDepth()
  {
    return depth == null ? false : depth.isValid();
  }
  public SEScalarLength getDepth()
  {
    if (depth == null)
      depth = new SEScalarLength();
    return depth;
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
          + "\n\tDepth: " + (hasDepth() ? getDepth() : "Not Provided")
          + "\n\tCompressionPeriod: " + (hasCompressionPeriod() ? getCompressionPeriod() : "Not Provided");
    else
      return "Action not specified properly";
  }
}
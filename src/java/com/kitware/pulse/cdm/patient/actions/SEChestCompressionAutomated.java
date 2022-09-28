/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.ChestCompressionAutomatedData;
import com.kitware.pulse.cdm.properties.SEScalarForce;
import com.kitware.pulse.cdm.properties.SEScalar0To1;
import com.kitware.pulse.cdm.properties.SEScalarFrequency;
import com.kitware.pulse.cdm.properties.SEScalarLength;

public class SEChestCompressionAutomated extends SEPatientAction
{
  private static final long serialVersionUID = 4548017190134330946L;
  
  protected SEScalar0To1         appliedForceFraction;
  protected SEScalarFrequency    compressionFrequency;
  protected SEScalarForce        force;
  protected SEScalarLength       depth;

  public SEChestCompressionAutomated()
  {
    appliedForceFraction = null;
    compressionFrequency = null;
    force = null;
    depth = null;
  }
  
  public SEChestCompressionAutomated(SEChestCompressionAutomated other)
  {
    this();
    copy(other);
  }
  
  public void copy(SEChestCompressionAutomated other)
  {
    if (other.force != null)
      getForce().set(other.force);
    else if (force != null)
      force.invalidate();
    
    if (other.depth != null)
      getDepth().set(other.depth);
    else if (depth != null)
      depth.invalidate();
    
    if (other.appliedForceFraction != null)
      getAppliedForceFraction().set(other.appliedForceFraction);
    else if (appliedForceFraction != null)
      appliedForceFraction.invalidate();
    
    if (other.compressionFrequency != null)
      getCompressionFrequency().set(other.compressionFrequency);
    else if (compressionFrequency != null)
      compressionFrequency.invalidate();
  }
  
  @Override
  public void clear()
  {
    super.clear();
    if (force != null)
      force.invalidate();
    if (depth != null)
      depth.invalidate();
    if (appliedForceFraction != null)
      appliedForceFraction.invalidate();
    if (compressionFrequency != null)
      compressionFrequency.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return (hasForce() || hasDepth()) && hasCompressionFrequency();
  }
  
  public static void load(ChestCompressionAutomatedData src, SEChestCompressionAutomated dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasForce())
      SEScalarForce.load(src.getForce(),dst.getForce());
    if(src.hasDepth())
      SEScalarLength.load(src.getDepth(),dst.getDepth());
    if(src.hasAppliedForceFraction())
      SEScalar0To1.load(src.getAppliedForceFraction(),dst.getAppliedForceFraction());
    if(src.hasCompressionFrequency())
      SEScalarFrequency.load(src.getCompressionFrequency(),dst.getCompressionFrequency());
  }
  
  public static ChestCompressionAutomatedData unload(SEChestCompressionAutomated src)
  {
    ChestCompressionAutomatedData.Builder dst = ChestCompressionAutomatedData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEChestCompressionAutomated src, ChestCompressionAutomatedData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if (src.hasForce())
      dst.setForce(SEScalarForce.unload(src.force));
    if (src.hasDepth())
      dst.setDepth(SEScalarLength.unload(src.depth));
    if (src.hasAppliedForceFraction())
      dst.setAppliedForceFraction(SEScalar0To1.unload(src.appliedForceFraction));
    if (src.hasCompressionFrequency())
      dst.setCompressionFrequency(SEScalarFrequency.unload(src.compressionFrequency));
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
  
  public boolean hasAppliedForceFraction()
  {
    return appliedForceFraction == null ? false : appliedForceFraction.isValid();
  }
  public SEScalar0To1 getAppliedForceFraction()
  {
    if (appliedForceFraction == null)
      appliedForceFraction = new SEScalar0To1();
    return appliedForceFraction;
  }
  
  public boolean hasCompressionFrequency()
  {
    return compressionFrequency == null ? false : compressionFrequency.isValid();
  }
  public SEScalarFrequency getCompressionFrequency()
  {
    if (compressionFrequency == null)
      compressionFrequency = new SEScalarFrequency();
    return compressionFrequency;
  }
  
  @Override
  public String toString()
  {
    if (force != null)
      return "Chest Compression" 
          + "\n\tForce: " +(hasForce() ? getForce() : "Not Provided")
          + "\n\tDepth: " + (hasDepth() ? getDepth() : "Not Provided")
          + "\n\tAppliedForceFraction: " + (hasAppliedForceFraction() ? getAppliedForceFraction() : "Not Provided")
          + "\n\tCompressionFrequency: " + (hasCompressionFrequency() ? getCompressionFrequency() : "Not Provided");
    else
      return "Action not specified properly";
  }
}
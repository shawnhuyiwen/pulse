/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.ChestCompressionInstantaneousData;
import com.kitware.pulse.cdm.properties.SEScalarForce;
import com.kitware.pulse.cdm.properties.SEScalarLength;

public class SEChestCompressionInstantaneous extends SEPatientAction
{
  private static final long serialVersionUID = 4548017190134330946L;
  
  protected SEScalarForce    force;
  protected SEScalarLength   depth;
  
  public SEChestCompressionInstantaneous()
  {
    force = null;
    depth = null;
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
    
    if (other.depth != null)
      getDepth().set(other.depth);
    else if (depth != null)
      depth.invalidate();
  }
  
  @Override
  public void clear()
  {
    super.clear();
    if (force != null)
      force.invalidate();
    if (depth != null)
      depth.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return (hasForce() || hasDepth());
  }
  
  public static void load(ChestCompressionInstantaneousData src, SEChestCompressionInstantaneous dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasForce())
      SEScalarForce.load(src.getForce(),dst.getForce());
    if(src.hasDepth())
      SEScalarLength.load(src.getDepth(),dst.getDepth());
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
    if (src.hasDepth())
      dst.setDepth(SEScalarLength.unload(src.depth));
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
  
  @Override
  public String toString()
  {
    if (force != null)
      return "Chest Compression" 
          + "\n\tForce: " +(hasForce() ? getForce() : "Not Provided")
          + "\n\tDepth: " + (hasDepth() ? getDepth() : "Not Provided");
    else
      return "Action not specified properly";
  }
}
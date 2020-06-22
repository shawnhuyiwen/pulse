/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.ChestCompressionForceData;
import com.kitware.pulse.cdm.properties.SEScalarForce;

public class SEChestCompressionForce extends SEPatientAction
{
  private static final long serialVersionUID = 4548017190134330946L;
  
  protected SEScalarForce force;
  
  public SEChestCompressionForce()
  {
    force = null;
  }
  
  public SEChestCompressionForce(SEChestCompressionForce other)
  {
    this();
    copy(other);
  }
  
  public void copy(SEChestCompressionForce other)
  {
    if (other.force != null)
      getForce().set(other.force);
    else if (force != null)
      force.invalidate();
  }
  
  @Override
  public void reset()
  {
    super.reset();
    if (force != null)
      force.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return hasForce();
  }
  
  public static void load(ChestCompressionForceData src, SEChestCompressionForce dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasForce())
      SEScalarForce.load(src.getForce(),dst.getForce());
  }
  
  public static ChestCompressionForceData unload(SEChestCompressionForce src)
  {
    ChestCompressionForceData.Builder dst = ChestCompressionForceData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEChestCompressionForce src, ChestCompressionForceData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if (src.hasForce())
      dst.setForce(SEScalarForce.unload(src.force));
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
  
  @Override
  public String toString()
  {
    if (force != null)
      return "Chest Compression" 
          + "\n\tForce: " + getForce();
    else
      return "Action not specified properly";
  }
}
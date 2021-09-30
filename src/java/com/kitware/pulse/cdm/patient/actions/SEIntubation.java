/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.IntubationData;
import com.kitware.pulse.cdm.bind.PatientActions.IntubationData.eType;

public  class SEIntubation extends SEPatientAction
{

  private static final long serialVersionUID = -7814100167946869768L;
  protected eType type;
  
  public SEIntubation()
  {
    type = null;
  }
  
  public void copy(SEIntubation other)
  {
    if(this==other)
      return;
    super.copy(other);
    type = other.type;
  }  
  
  @Override
  public void clear()
  {
    super.clear();
    type = null;
  }
  
  @Override
  public boolean isValid()
  {
    return hasType();
  }
  
  public static void load(IntubationData src, SEIntubation dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.getType()!=eType.UNRECOGNIZED)
    	dst.type = src.getType();
  }
  
  public static IntubationData unload(SEIntubation src)
  {
    IntubationData.Builder dst = IntubationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEIntubation src, IntubationData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if (src.hasType())
      dst.setType(src.type);
  }
  
  public eType getType()
  {
    return type;
  }
  public void setType(eType t)
  {
    type = t;
  }
  public boolean hasType()
  {
    return type == null ? false : true;
  }
  
  @Override
  public String toString() 
  {
    return "Intubation"
        + "\n\tType: " + getType();
  }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.physiology.datamodel.patient.actions;

import com.kitware.physiology.cdm.PatientActions.IntubationData;
import com.kitware.physiology.cdm.PatientActionEnums.eIntubation.Type;

public  class SEIntubation extends SEPatientAction
{
  protected Type type;
  
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
  
  public void reset()
  {
    super.reset();
    type = null;
  }
  
  public boolean isValid()
  {
    return hasType();
  }
  
  public static void load(IntubationData src, SEIntubation dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.getType()!=Type.UNRECOGNIZED)
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
  
  public Type getType()
  {
    return type;
  }
  public void setType(Type t)
  {
    type = t;
  }
  public boolean hasType()
  {
    return type == null ? false : true;
  }
  
  public String toString() 
  {
    return "Intubation"
        + "\n\tType: " + getType();
  }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.conditions;

import com.kitware.pulse.cdm.bind.PatientConditions.ChronicVentricularSystolicDysfunctionData;

public class SEChronicVentricularSystolicDysfunction extends SEPatientCondition
{

  private static final long serialVersionUID = -7871986033442360506L;

  public SEChronicVentricularSystolicDysfunction()
  {
    
  }
  
  @Override
  public void clear()
  {
    super.clear();
  }
  
  public void copy(SEChronicVentricularSystolicDysfunction other)
  {
    if(this==other)
      return;
    super.copy(other);    
  }
  
  public static void load(ChronicVentricularSystolicDysfunctionData src, SEChronicVentricularSystolicDysfunction dst) 
  {
    SEPatientCondition.load(src.getPatientCondition(), dst);
  }
  
  public static ChronicVentricularSystolicDysfunctionData unload(SEChronicVentricularSystolicDysfunction src) 
  {
    ChronicVentricularSystolicDysfunctionData.Builder dst = ChronicVentricularSystolicDysfunctionData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEChronicVentricularSystolicDysfunction src, ChronicVentricularSystolicDysfunctionData.Builder dst)
  {
    SEPatientCondition.unload(src, dst.getPatientConditionBuilder());
  }
  
  @Override
  public String toString()
  {
    return "Chronic Ventricular Systolic Dysfunction";
  }

  
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.physiology.datamodel.patient.conditions;

import com.kitware.physiology.cdm.PatientConditions.AcuteRespiratoryDistressSyndromeData;

import com.kitware.physiology.datamodel.properties.SEScalar0To1;

public class SEAcuteRespiratoryDistressSyndrome extends SEPatientCondition
{
  protected SEScalar0To1 severity;
  
  public SEAcuteRespiratoryDistressSyndrome()
  {
    severity = null;
  }
  
  public void reset()
  {
    super.reset();
    if (severity != null)
      severity.invalidate();
  }
  
  public void copy(SEAcuteRespiratoryDistressSyndrome other)
  {
    if(this==other)
      return;
    super.copy(other);
    if (other.severity != null)
      getSeverity().set(other.getSeverity());
  }
  
  public static void load(AcuteRespiratoryDistressSyndromeData src, SEAcuteRespiratoryDistressSyndrome dst) 
  {
    SEPatientCondition.load(src.getPatientCondition(), dst);
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  
  public static AcuteRespiratoryDistressSyndromeData unload(SEAcuteRespiratoryDistressSyndrome src) 
  {
    AcuteRespiratoryDistressSyndromeData.Builder dst = AcuteRespiratoryDistressSyndromeData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEAcuteRespiratoryDistressSyndrome src, AcuteRespiratoryDistressSyndromeData.Builder dst)
  {
    SEPatientCondition.unload(src, dst.getPatientConditionBuilder());
    if (src.hasSeverity())
      dst.setSeverity(SEScalar0To1.unload(src.severity));
  }
  
  public boolean hasSeverity()
  {
    return severity == null ? false : severity.isValid();
  }
  public SEScalar0To1 getSeverity()
  {
    if (severity == null)
      severity = new SEScalar0To1();
    return severity;
  }
  
  public String toString()
  {
    return "ARDS" 
        + "\n\tSeverity: " + getSeverity();
  }

  
}

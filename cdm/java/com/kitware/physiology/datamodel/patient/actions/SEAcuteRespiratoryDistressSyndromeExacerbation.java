/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.physiology.datamodel.patient.actions;

import com.kitware.physiology.cdm.PatientActions.AcuteRespiratoryDistressSyndromeExacerbationData;

import com.kitware.physiology.datamodel.properties.SEScalar0To1;

public class SEAcuteRespiratoryDistressSyndromeExacerbation extends SEPatientAction
{
  protected SEScalar0To1 severity;
  
  public SEAcuteRespiratoryDistressSyndromeExacerbation()
  {
    severity = null;
  }
  
  public void reset()
  {
    super.reset();
    if (severity != null)
      severity.invalidate();
  }
  
  public void copy(SEAcuteRespiratoryDistressSyndromeExacerbation other)
  {
    if(this==other)
      return;
    super.copy(other);
    if (other.severity != null)
      getSeverity().set(other.getSeverity());
  }
  
  public boolean isValid()
  {
    return hasSeverity();
  }
  
  public static void load(AcuteRespiratoryDistressSyndromeExacerbationData src, SEAcuteRespiratoryDistressSyndromeExacerbation dst) 
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  
  public static AcuteRespiratoryDistressSyndromeExacerbationData unload(SEAcuteRespiratoryDistressSyndromeExacerbation src) 
  {
    AcuteRespiratoryDistressSyndromeExacerbationData.Builder dst = AcuteRespiratoryDistressSyndromeExacerbationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEAcuteRespiratoryDistressSyndromeExacerbation src, AcuteRespiratoryDistressSyndromeExacerbationData.Builder dst)
  {
    SEPatientAction.unload(src, dst.getPatientActionBuilder());
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
    return "ARDS Exacerbation" 
        + "\n\tSeverity: " + getSeverity();
  }

  
}

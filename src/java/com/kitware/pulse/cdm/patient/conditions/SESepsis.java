/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.conditions;

import com.kitware.pulse.cdm.bind.PatientConditions.SepsisData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;

public class SESepsis extends SEPatientCondition
{

  private static final long serialVersionUID = 8862611857864194588L;
  protected SEScalar0To1 severity;
  
  public SESepsis()
  {
    severity = null;
  }
  
  @Override
  public void clear()
  {
    super.clear();
    if (severity != null)
      severity.invalidate();
  }
  
  public void copy(SESepsis other)
  {
    if(this==other)
      return;
    super.copy(other);
    if (other.severity != null)
      getSeverity().set(other.getSeverity());
    else if (severity != null)
      severity.invalidate();
  }
  
  public static void load(SepsisData src, SESepsis dst) 
  {
    SEPatientCondition.load(src.getPatientCondition(), dst);
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  
  public static SepsisData unload(SESepsis src) 
  {
    SepsisData.Builder dst = SepsisData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SESepsis src, SepsisData.Builder dst)
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
  
  @Override
  public String toString()
  {
    return "Sepsis" 
        + "\n\tSeverity: " + getSeverity();
  }

  
}

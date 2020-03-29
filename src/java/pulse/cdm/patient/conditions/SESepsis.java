/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.patient.conditions;

import pulse.cdm.bind.PatientConditions.SepsisData;

import pulse.cdm.properties.SEScalar0To1;

public class SESepsis extends SEPatientCondition
{
  protected SEScalar0To1 severity;
  
  public SESepsis()
  {
    severity = null;
  }
  
  public void reset()
  {
    super.reset();
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
  
  public String toString()
  {
    return "Sepsis" 
        + "\n\tSeverity: " + getSeverity();
  }

  
}

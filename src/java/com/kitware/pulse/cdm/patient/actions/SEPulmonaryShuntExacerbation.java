/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.PulmonaryShuntExacerbationData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;

public class SEPulmonaryShuntExacerbation extends SEPatientAction
{

  private static final long serialVersionUID = 5044648250444265871L;
  protected SEScalar0To1 severity;
  
  public SEPulmonaryShuntExacerbation()
  {
    severity = null;
  }
  
  public void copy(SEPulmonaryShuntExacerbation other)
  {
    if(this==other)
      return;
    super.copy(other);
    if (other.severity != null)
      getSeverity().set(other.getSeverity());
    else if (severity != null)
      severity.invalidate();
  }
  
  @Override
  public void reset()
  {
    super.reset();
    if (severity != null)
      severity.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return hasSeverity();
  }
  
  public static void load(PulmonaryShuntExacerbationData src, SEPulmonaryShuntExacerbation dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  
  public static PulmonaryShuntExacerbationData unload(SEPulmonaryShuntExacerbation src)
  {
    PulmonaryShuntExacerbationData.Builder dst = PulmonaryShuntExacerbationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEPulmonaryShuntExacerbation src, PulmonaryShuntExacerbationData.Builder dst)
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
  
  @Override
  public String toString()
  {
    return "Pulmonary Shunt Exacerbation" 
        + "\n\tSeverity: " + getSeverity();
  }
}

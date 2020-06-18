/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.patient.conditions;

import pulse.cdm.bind.PatientConditions.PulmonaryFibrosisData;
import pulse.cdm.properties.SEScalar0To1;

public class SEPulmonaryFibrosis extends SEPatientCondition
{

  private static final long serialVersionUID = -2186578906373865449L;
  protected SEScalar0To1 severity;
  
  public SEPulmonaryFibrosis()
  {
    severity = null;
  }
  
  @Override
  public void reset()
  {
    super.reset();
    if (severity != null)
      severity.invalidate();
  }
  
  public void copy(SEPulmonaryFibrosis other)
  {
    if(this==other)
      return;
    super.copy(other);
    if (other.severity != null)
      getSeverity().set(other.getSeverity());
    else if (severity != null)
      severity.invalidate();
  }
  
  public static void load(PulmonaryFibrosisData src, SEPulmonaryFibrosis dst) 
  {
    SEPatientCondition.load(src.getPatientCondition(), dst);
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  
  public static PulmonaryFibrosisData unload(SEPulmonaryFibrosis src) 
  {
    PulmonaryFibrosisData.Builder dst = PulmonaryFibrosisData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEPulmonaryFibrosis src, PulmonaryFibrosisData.Builder dst)
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
    return "Pulmonary Fibrosis" 
        + "\n\tSeverity: " + getSeverity();
  }

  
}

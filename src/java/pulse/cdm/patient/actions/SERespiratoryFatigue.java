/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.patient.actions;

import pulse.cdm.bind.PatientActions.RespiratoryFatigueData;
import pulse.cdm.properties.SEScalar0To1;

public class SERespiratoryFatigue extends SEPatientAction
{

  private static final long serialVersionUID = -8967263581748846332L;
  protected SEScalar0To1 severity;
  
  public SERespiratoryFatigue()
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
  
  public void copy(SERespiratoryFatigue other)
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
  public boolean isValid()
  {
    return hasSeverity();
  }
  
  public static void load(RespiratoryFatigueData src, SERespiratoryFatigue dst) 
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  
  public static RespiratoryFatigueData unload(SERespiratoryFatigue src)
  {
    RespiratoryFatigueData.Builder dst = RespiratoryFatigueData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SERespiratoryFatigue src, RespiratoryFatigueData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
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
    if (severity != null)
      return "Respiratory Fatigue" 
          + "\n\tSeverity: " + getSeverity();
    else
      return "Action not specified properly";
  }
}

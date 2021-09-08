/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.BronchoconstrictionData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;

public class SEBronchoconstriction extends SEPatientAction
{
  private static final long serialVersionUID = -3173204376090087459L;
  
  protected SEScalar0To1 severity;
  
  public SEBronchoconstriction()
  {
    severity = null;
  }
  
  public void copy(SEBronchoconstriction other)
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
  public void clear()
  {
    super.clear();
    if (severity != null)
      severity.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return hasSeverity();
  }
  
  public static void load(BronchoconstrictionData src, SEBronchoconstriction dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  
  public static BronchoconstrictionData unload(SEBronchoconstriction src)
  {
    BronchoconstrictionData.Builder dst = BronchoconstrictionData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEBronchoconstriction src, BronchoconstrictionData.Builder dst)
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
      return "Bronchoconstriction" 
          + "\n\tSeverity: " + getSeverity();
    else
      return "Action not specified properly";
  }
}

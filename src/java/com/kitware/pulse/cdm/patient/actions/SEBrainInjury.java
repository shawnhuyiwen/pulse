/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.BrainInjuryData;
import com.kitware.pulse.cdm.bind.PatientActions.BrainInjuryData.eType;
import com.kitware.pulse.cdm.properties.SEScalar0To1;

public class SEBrainInjury extends SEPatientAction
{
  private static final long serialVersionUID = -2562160026757269439L;
  
  protected SEScalar0To1  severity;
  protected eType         type;
  
  public SEBrainInjury()
  {
    severity = null;
    type     = null;
  }
  
  @Override
  public void clear()
  {
    super.clear();
    if (severity != null)
      severity.invalidate();
    this.type = null;
  }
  
  public void copy(SEBrainInjury other)
  {
    if(this==other)
      return;
    super.copy(other);
    if (other.severity != null)
      getSeverity().set(other.getSeverity());
    else if (severity != null)
      severity.invalidate();
    this.type = other.type;
  }
  
  @Override
  public boolean isValid()
  {
    return hasSeverity() && hasType();
  }
  
  public static void load(BrainInjuryData src, SEBrainInjury dst) 
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.getType()!=eType.UNRECOGNIZED)
    	dst.setType(src.getType());
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  
  public static  BrainInjuryData unload(SEBrainInjury src)
  {
    BrainInjuryData.Builder dst = BrainInjuryData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEBrainInjury src, BrainInjuryData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if(src.hasType())
      dst.setType(src.type);
    if (src.hasSeverity())
      dst.setSeverity(SEScalar0To1.unload(src.severity));
  }
  
  public eType getType()
  {
    return type;
  }
  public void setType(eType t)
  {
    type = t;
  }
  public boolean hasType()
  {
    return type == null ? false : true;
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
      return "Brain Injury" 
          + "\n\tType: " + getType()
          + "\n\tSeverity: " + getSeverity();
    else
      return "Action not specified properly";
  }
}

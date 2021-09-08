/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.PatientAssessmentRequestData;
import com.kitware.pulse.cdm.bind.PatientAssessments.ePatientAssessmentType;

public class SEPatientAssessmentRequest extends SEPatientAction
{

  private static final long serialVersionUID = -3104761170947638289L;
  protected ePatientAssessmentType type;
  
  public SEPatientAssessmentRequest()
  {
    type = null;
  }
  
  public void copy(SEPatientAssessmentRequest other)
  {
    if(this==other)
      return;
    super.copy(other);
    type = other.type;
  }
  
  @Override
  public void clear()
  {
    super.clear();
    type = null;
  }
  
  @Override
  public boolean isValid()
  {
    return hasType();
  }
  
  public static void load(PatientAssessmentRequestData src, SEPatientAssessmentRequest dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.getType()!=ePatientAssessmentType.UNRECOGNIZED)
    	dst.type = src.getType();
  }
  
  public static PatientAssessmentRequestData unload(SEPatientAssessmentRequest src)
  {
    PatientAssessmentRequestData.Builder dst = PatientAssessmentRequestData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEPatientAssessmentRequest src, PatientAssessmentRequestData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if (src.hasType())
      dst.setType(src.type);
  }
  
  public ePatientAssessmentType getType()
  {
    return type;
  }
  public void setType(ePatientAssessmentType type)
  {
    this.type = type;
  }
  public boolean hasType()
  {
    return type == null ? false : true;
  }
  
  @Override
  public String toString() 
  {
    if (type != null)
      return "Patient Type"
          + "\n\tType: " + getType();
    else
      return "Type not specified properly";
  }
}

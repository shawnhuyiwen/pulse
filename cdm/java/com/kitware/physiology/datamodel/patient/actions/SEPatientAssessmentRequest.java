/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.physiology.datamodel.patient.actions;

import com.kitware.physiology.cdm.PatientActions.PatientAssessmentRequestData;
import com.kitware.physiology.cdm.PatientAssessmentEnums.ePatientAssessment.Type;

public class SEPatientAssessmentRequest extends SEPatientAction
{
  protected Type type;
  
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
  
  public void reset()
  {
    super.reset();
    type = null;
  }
  
  public boolean isValid()
  {
    return hasType();
  }
  
  public static void load(PatientAssessmentRequestData src, SEPatientAssessmentRequest dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.getType()!=Type.UNRECOGNIZED)
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
  
  public Type getType()
  {
    return type;
  }
  public void setType(Type type)
  {
    this.type = type;
  }
  public boolean hasType()
  {
    return type == null ? false : true;
  }
  
  public String toString() 
  {
    if (type != null)
      return "Patient Type"
          + "\n\tType: " + getType();
    else
      return "Type not specified properly";
  }
}

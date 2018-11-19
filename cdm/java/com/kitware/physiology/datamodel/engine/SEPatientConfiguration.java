/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.physiology.datamodel.engine;

import java.util.ArrayList;
import java.util.List;

import com.kitware.physiology.cdm.Engine.AnyConditionData;
import com.kitware.physiology.cdm.Scenario.ScenarioData;
import com.kitware.physiology.cdm.Engine.PatientConfigurationData;
import com.kitware.physiology.datamodel.conditions.SECondition;
import com.kitware.physiology.datamodel.patient.SEPatient;
import com.kitware.physiology.datamodel.substance.SESubstanceManager;

public class SEPatientConfiguration 
{
  protected SEPatient                     patient;
  protected String                        patientFile;
  protected List<SECondition>             conditions;
  
  public SEPatientConfiguration()
  {
    this.patient = null;
    this.patientFile = "";
    this.conditions = new ArrayList<SECondition>();
  }
  
  public void reset() 
  {
    patient=null;
    patientFile = "";
    conditions.clear();
  }
  
  public static void load(PatientConfigurationData src, SEPatientConfiguration dst, SESubstanceManager subMgr)
  {
    dst.reset();
    
    switch(src.getPatientTypeCase())
    {
    case PATIENT:
    {
      SEPatient p = new SEPatient();
      SEPatient.load(src.getPatient(),p);
      dst.setPatient(p);
      break;
    }
    case PATIENTFILE:
      dst.setPatientFile(src.getPatientFile());
    }
    
    for(AnyConditionData cData : src.getAnyConditionList())
      dst.conditions.add(SECondition.ANY2CDM(cData, subMgr));
  }
  
  public static PatientConfigurationData unload(SEPatientConfiguration src)
  {
    PatientConfigurationData.Builder dst = PatientConfigurationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEPatientConfiguration src, PatientConfigurationData.Builder dst)
  {
    if(src.hasPatient())
      dst.setPatient(SEPatient.unload(src.patient));
    if(src.hasPatientFile())
      dst.setPatientFile(src.patientFile);
    for(SECondition c : src.conditions)
      dst.addAnyCondition(SECondition.CDM2ANY(c));    
  }
  
  public boolean isValid()
  {
    if (!hasPatientFile()&&!hasPatient())
      return false;
   return true;
  }
  
  public SEPatient getPatient()
  {
    return patient;
  }
  public void setPatient(SEPatient patient)
  {
    this.patient = patient;
    this.patientFile = "";
  }
  public boolean hasPatient()
  {
    return patient==null ? false : true;
  }
  public void invalidatePatient()
  {
    patient = null;
  }

  public String getPatientFile()
  {
    return patientFile;
  }
  public void setPatientFile(String patientFile)
  {
    this.patient = null;
    this.patientFile = patientFile;
  }
  public boolean hasPatientFile()
  {
    return patientFile.isEmpty() ? false : true;
  }
  public void invalidatePatientFile()
  {
    patientFile = "";
  }
  
  public List<SECondition> getConditions() 
  {
    return conditions;
  }
}
/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.engine;

import java.util.ArrayList;
import java.util.List;

import com.kitware.pulse.cdm.bind.Engine.AnyConditionData;
import com.kitware.pulse.cdm.bind.Engine.PatientConfigurationData;
import com.kitware.pulse.cdm.conditions.SECondition;
import com.kitware.pulse.cdm.patient.SEPatient;

public class SEPatientConfiguration 
{
  protected SEPatient                     patient;
  protected String                        patientFile;
  protected List<SECondition>             conditions;
  protected String                        dataRootDir="./";
  
  public SEPatientConfiguration()
  {
    this.patient = null;
    this.patientFile = "";
    this.conditions = new ArrayList<>();
  }
  
  public void reset() 
  {
    if(patient != null)
      patient.reset();
    patientFile = "";
    conditions.clear();
  }
  
  public static void load(PatientConfigurationData src, SEPatientConfiguration dst)
  {
    dst.reset();
    
    switch(src.getPatientTypeCase())
    {
    case PATIENT:
    {
      SEPatient.load(src.getPatient(),dst.getPatient());
      break;
    }
    case PATIENTFILE:
      dst.setPatientFile(src.getPatientFile());
    default: // do nothing
    }
    
    for(AnyConditionData cData : src.getConditions().getAnyConditionList())
      dst.conditions.add(SECondition.ANY2CDM(cData));
    dst.setDataRootDir(src.getDataRoot());
  }
  
  public static PatientConfigurationData unload(SEPatientConfiguration src)
  {
    PatientConfigurationData.Builder dst = PatientConfigurationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEPatientConfiguration src, PatientConfigurationData.Builder dst)
  {
    if(src.hasPatientFile())
      dst.setPatientFile(src.patientFile);
    else if(src.hasPatient())
      dst.setPatient(SEPatient.unload(src.patient));
    if(src.conditions.size()>0)
    {
      for(SECondition c : src.conditions)
        dst.getConditionsBuilder().addAnyCondition(SECondition.CDM2ANY(c));
    }
    dst.setDataRoot(src.dataRootDir);
  }
  
  public boolean isValid()
  {
    if (!hasPatientFile()&&!hasPatient())
      return false;
   return true;
  }
  
  public void setDataRootDir(String dir)
  {
    dataRootDir = dir;
  }
  public String getDataRootDir()
  {
    return dataRootDir;
  }
  
  public SEPatient getPatient()
  {
    if(patient == null)
      this.patient = new SEPatient();
    return patient;
  }
  public boolean hasPatient()
  {
    return patient==null ? false : true;
  }
  
  public String getPatientFile()
  {
    return patientFile;
  }
  public void setPatientFile(String patientFile)
  {
    this.patientFile = patientFile;
  }
  public boolean hasPatientFile()
  {
    return patientFile.isEmpty() ? false : true;
  }
  
  public List<SECondition> getConditions() 
  {
    return conditions;
  }
}

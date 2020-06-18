/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.engine;

import java.util.ArrayList;
import java.util.List;

import pulse.cdm.bind.Engine.AnyConditionData;
import pulse.cdm.bind.Engine.PatientConfigurationData;
import pulse.cdm.conditions.SECondition;
import pulse.cdm.patient.SEPatient;
import pulse.cdm.substance.SESubstanceManager;

public class SEPatientConfiguration 
{
  protected SEPatient                     patient;
  protected String                        patientFile;
  protected List<SECondition>             conditions;
  
  public SEPatientConfiguration()
  {
    this.patient = null;
    this.patientFile = "";
    this.conditions = new ArrayList<>();
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
    default: // do nothing
    }
    
    for(AnyConditionData cData : src.getConditions().getAnyConditionList())
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
    if(src.conditions.size()>0)
    {
      for(SECondition c : src.conditions)
        dst.getConditionsBuilder().addAnyCondition(SECondition.CDM2ANY(c));    
    }
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

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.assessments;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;

import com.kitware.pulse.cdm.bind.PatientAssessments.PatientAssessmentData;
import com.kitware.pulse.utilities.FileUtils;
import com.kitware.pulse.utilities.Log;

public abstract class SEPatientAssessment
{
  public SEPatientAssessment()
  {
    
  }
  
  public void clear()
  {
    
  }
  
  public static void load(PatientAssessmentData src, SEPatientAssessment dst) 
  {
    dst.clear();
  }
  protected static void unload(SEPatientAssessment src, PatientAssessmentData.Builder dst)
  {

  }
  
  public static SEPatientAssessment readAssessment(String fileName) throws InvalidProtocolBufferException
  {
    try
    {
      SEArterialBloodGasTest abg = new SEArterialBloodGasTest();
      abg.readFile(fileName);
      return abg;
    }
    catch(InvalidProtocolBufferException ex){}
    
    try
    {
      SECompleteBloodCount cbc = new SECompleteBloodCount();
      cbc.readFile(fileName);
      return cbc;
    }
    catch(InvalidProtocolBufferException ex){}
    
    try
    {
      SEComprehensiveMetabolicPanel cmp = new SEComprehensiveMetabolicPanel();
      cmp.readFile(fileName);
      return cmp;
    }
    catch(InvalidProtocolBufferException ex){}
    
    SEUrinalysis u = new SEUrinalysis();
    u.readFile(fileName);
    return u;
  }
  public static boolean writeAssement(String fileName, SEPatientAssessment ass) throws InvalidProtocolBufferException
  {
    if(ass instanceof SEArterialBloodGasTest)
      return FileUtils.writeFile(fileName, JsonFormat.printer().print(SEArterialBloodGasTest.unload((SEArterialBloodGasTest)ass)));
    if(ass instanceof SECompleteBloodCount)
      return FileUtils.writeFile(fileName, JsonFormat.printer().print(SECompleteBloodCount.unload((SECompleteBloodCount)ass)));
    if(ass instanceof SEComprehensiveMetabolicPanel)
      return FileUtils.writeFile(fileName, JsonFormat.printer().print(SEComprehensiveMetabolicPanel.unload((SEComprehensiveMetabolicPanel)ass)));
    if(ass instanceof SEUrinalysis)
      return FileUtils.writeFile(fileName, JsonFormat.printer().print(SEUrinalysis.unload((SEUrinalysis)ass)));
    Log.error("No write support for assesment of class "+ass.getClass().getName());
    return false;
  }
}

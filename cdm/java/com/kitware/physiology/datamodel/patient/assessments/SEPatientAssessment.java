/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.physiology.datamodel.patient.assessments;

import org.jfree.util.Log;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;
import com.kitware.physiology.cdm.PatientAssessments.PatientAssessmentData;

import com.kitware.physiology.utilities.FileUtils;

public abstract class SEPatientAssessment
{
  public SEPatientAssessment()
  {
    
  }
  
  public void clear()
  {
    
  }
  
  public void reset()
  {
    
  }
  
  public static void load(PatientAssessmentData src, SEPatientAssessment dst) 
  {
    dst.reset();
  }
  protected static void unload(SEPatientAssessment src, PatientAssessmentData.Builder dst)
  {

  }
  
  public static SEPatientAssessment readAssessment(String fileName) throws InvalidProtocolBufferException
  {
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
    
    try
    {
      SEPulmonaryFunctionTest pft = new SEPulmonaryFunctionTest();
      pft.readFile(fileName);
      return pft;
    }
    catch(InvalidProtocolBufferException ex){}
    
    SEUrinalysis u = new SEUrinalysis();
    u.readFile(fileName);
    return u;
  }
  public static boolean writeAssement(String fileName, SEPatientAssessment ass) throws InvalidProtocolBufferException
  {
    if(ass instanceof SECompleteBloodCount)
      return FileUtils.writeFile(fileName, JsonFormat.printer().print(SECompleteBloodCount.unload((SECompleteBloodCount)ass)));
    if(ass instanceof SEComprehensiveMetabolicPanel)
      return FileUtils.writeFile(fileName, JsonFormat.printer().print(SEComprehensiveMetabolicPanel.unload((SEComprehensiveMetabolicPanel)ass)));
    if(ass instanceof SEPulmonaryFunctionTest)
      return FileUtils.writeFile(fileName, JsonFormat.printer().print(SEPulmonaryFunctionTest.unload((SEPulmonaryFunctionTest)ass)));
    if(ass instanceof SEUrinalysis)
      return FileUtils.writeFile(fileName, JsonFormat.printer().print(SEUrinalysis.unload((SEUrinalysis)ass)));
    Log.error("No write support for assesment of class "+ass.getClass().getName());
    return false;
  }
}

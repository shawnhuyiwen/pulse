/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.patient.assessments;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;

import com.kitware.pulse.cdm.bind.PatientAssessments.ArterialBloodGasTestData;
import com.kitware.pulse.cdm.properties.*;
import com.kitware.pulse.utilities.FileUtils;

public class SEArterialBloodGasTest extends SEPatientAssessment
{
  protected SEScalar                          bloodPH;
  protected SEScalarEquivalentWeightPerVolume bicarbonate;
  protected SEScalarPressure                  partialPressureOfOxygen;
  protected SEScalarPressure                  partialPressureOfCarbonDioxide;
  protected SEScalar                          oxygenSaturation;
  
  @Override
  public void clear()
  {
    super.clear();
    if(this.bloodPH!=null)
      this.bloodPH.invalidate();
    if(this.bicarbonate!=null)
      this.bicarbonate.invalidate();
    if(this.partialPressureOfOxygen!=null)
      this.partialPressureOfOxygen.invalidate();
    if(this.partialPressureOfCarbonDioxide!=null)
      this.partialPressureOfCarbonDioxide.invalidate();
    if(this.oxygenSaturation!=null)
      this.oxygenSaturation.invalidate();
  }
  
  public void readFile(String fileName) throws InvalidProtocolBufferException
  {
    ArterialBloodGasTestData.Builder builder = ArterialBloodGasTestData.newBuilder();
    JsonFormat.parser().merge(FileUtils.readFile(fileName), builder);
    SEArterialBloodGasTest.load(builder.build(), this);
  }
  public void writeFile(String fileName) throws InvalidProtocolBufferException
  {
    FileUtils.writeFile(fileName, JsonFormat.printer().print(SEArterialBloodGasTest.unload(this)));
  }
  
  public static void load(ArterialBloodGasTestData src, SEArterialBloodGasTest dst)
  {
    SEPatientAssessment.load(src.getPatientAssessment(), dst);
    if(src.hasBloodPH())
      SEScalar.load(src.getBloodPH(),dst.getBloodPH());
    if(src.hasBicarbonate())
      SEScalarEquivalentWeightPerVolume.load(src.getBicarbonate(),dst.getBicarbonate());
    if(src.hasPartialPressureOfOxygen())
      SEScalarPressure.load(src.getPartialPressureOfOxygen(),dst.getPartialPressureOfOxygen());
    if(src.hasPartialPressureOfCarbonDioxide())
      SEScalarPressure.load(src.getPartialPressureOfCarbonDioxide(),dst.getPartialPressureOfCarbonDioxide());
    if(src.hasOxygenSaturation())
      SEScalar.load(src.getOxygenSaturation(),dst.getOxygenSaturation());
  }
  
  public static ArterialBloodGasTestData unload(SEArterialBloodGasTest src)
  {
    ArterialBloodGasTestData.Builder dst = ArterialBloodGasTestData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEArterialBloodGasTest src, ArterialBloodGasTestData.Builder dst)
  {
    SEPatientAssessment.unload(src, dst.getPatientAssessmentBuilder());
    if (src.hasBloodPH())
      dst.setBloodPH(SEScalar.unload(src.getBloodPH()));
    if (src.hasBicarbonate())
      dst.setBicarbonate(SEScalarEquivalentWeightPerVolume.unload(src.getBicarbonate()));
    if (src.hasPartialPressureOfOxygen())
      dst.setPartialPressureOfOxygen(SEScalarPressure.unload(src.getPartialPressureOfOxygen()));
    if (src.hasPartialPressureOfCarbonDioxide())
      dst.setPartialPressureOfCarbonDioxide(SEScalarPressure.unload(src.getPartialPressureOfCarbonDioxide()));
    if (src.hasOxygenSaturation())
      dst.setOxygenSaturation(SEScalar.unload(src.getOxygenSaturation()));
  }
  
  public boolean hasBloodPH()
  {
    return bloodPH == null ? false : bloodPH.isValid();
  }
  public SEScalar getBloodPH()
  {
    if (bloodPH == null)
      bloodPH = new SEScalar();
    return bloodPH;
  }
  
  public boolean hasBicarbonate()
  {
    return bicarbonate == null ? false : bicarbonate.isValid();
  }
  public SEScalarEquivalentWeightPerVolume getBicarbonate()
  {
    if (bicarbonate == null)
      bicarbonate = new SEScalarEquivalentWeightPerVolume();
    return bicarbonate;
  }
  
  public boolean hasPartialPressureOfOxygen()
  {
    return partialPressureOfOxygen == null ? false : partialPressureOfOxygen.isValid();
  }
  public SEScalarPressure getPartialPressureOfOxygen()
  {
    if (partialPressureOfOxygen == null)
      partialPressureOfOxygen = new SEScalarPressure();
    return partialPressureOfOxygen;
  }
  
  public boolean hasPartialPressureOfCarbonDioxide()
  {
    return partialPressureOfCarbonDioxide == null ? false : partialPressureOfCarbonDioxide.isValid();
  }
  public SEScalarPressure getPartialPressureOfCarbonDioxide()
  {
    if (partialPressureOfCarbonDioxide == null)
      partialPressureOfCarbonDioxide = new SEScalarPressure();
    return partialPressureOfCarbonDioxide;
  }
  
  public boolean hasOxygenSaturation()
  {
    return oxygenSaturation == null ? false : oxygenSaturation.isValid();
  }
  public SEScalar getOxygenSaturation()
  {
    if (oxygenSaturation == null)
      oxygenSaturation = new SEScalar();
    return oxygenSaturation;
  }

}


/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.patient.assessments;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;

import com.kitware.pulse.cdm.bind.PatientAssessments.UrinalysisData;
import com.kitware.pulse.cdm.bind.PatientAssessments.UrinalysisData.eClarityIndicator;
import com.kitware.pulse.cdm.bind.PatientAssessments.UrinalysisData.ePresenceIndicator;
import com.kitware.pulse.cdm.bind.PatientAssessments.UrinalysisData.eUrineColor;
import com.kitware.pulse.cdm.properties.SEScalar;
import com.kitware.pulse.cdm.properties.SEScalarMassPerVolume;
import com.kitware.pulse.utilities.FileUtils;

public class SEUrinalysis extends SEPatientAssessment
{
  protected eUrineColor             color;
  protected eClarityIndicator       appearance;
  protected ePresenceIndicator      glucose;
  protected ePresenceIndicator      ketone;
  protected SEScalar                bilirubin;
  protected SEScalar                specificGravity;
  protected ePresenceIndicator      blood;
  protected SEScalar                pH;  
  protected ePresenceIndicator      protein;
  protected SEScalarMassPerVolume   urobilinogen;
  protected ePresenceIndicator      nitrite;
  protected ePresenceIndicator      leukocyteEsterase;
  
  protected SEUrinalysisMicroscopic microscopic;
  
  @Override
  public void clear()
  {
    super.clear();
    this.color = null;
    this.appearance = null;
    this.urobilinogen = null;
    this.ketone = null;
    this.bilirubin = null;
    this.specificGravity = null;
    this.blood = null;
    this.pH = null;
    this.protein = null;
    this.urobilinogen = null;
    this.nitrite = null;
    this.leukocyteEsterase = null;
    
    if(this.microscopic != null)
      this.microscopic.clear();
  }
  
  @Override
  public void reset()
  {
    super.reset();
    this.color = null;
    this.appearance = null;
    this.urobilinogen = null;
    this.ketone = null;
    if(this.bilirubin!=null)
      this.bilirubin.invalidate();
    if(this.specificGravity!=null)
      this.specificGravity.invalidate();
    this.blood = null;
    if(this.pH!=null)
      this.pH.invalidate();    
    this.protein = null;
    if(this.urobilinogen!=null)
      this.urobilinogen.invalidate();
    this.nitrite = null;
    this.leukocyteEsterase = null;
    
    if(this.microscopic != null)
      this.microscopic.reset();
  }
  
  public void readFile(String fileName) throws InvalidProtocolBufferException
  {
    UrinalysisData.Builder builder = UrinalysisData.newBuilder();
    JsonFormat.parser().merge(FileUtils.readFile(fileName), builder);
    SEUrinalysis.load(builder.build(), this);
  }
  public void writeFile(String fileName) throws InvalidProtocolBufferException
  {
    FileUtils.writeFile(fileName, JsonFormat.printer().print(SEUrinalysis.unload(this)));
  }
  
  public static void load(UrinalysisData src, SEUrinalysis dst)
  {
    SEPatientAssessment.load(src.getPatientAssessment(), dst);
    if(src.getColor()!=eUrineColor.UNRECOGNIZED)
      dst.setColorResult(src.getColor());
    if(src.getAppearance()!=eClarityIndicator.UNRECOGNIZED)
      dst.setAppearanceResult(src.getAppearance());
    if(src.getGlucose()!=ePresenceIndicator.UNRECOGNIZED)
      dst.setGlucoseResult(src.getGlucose());
    if(src.getKetone()!=ePresenceIndicator.UNRECOGNIZED)
      dst.setKetoneResult(src.getKetone());
    if(src.hasBilirubin())
      SEScalar.load(src.getBilirubin(),dst.getBilirubinResult());
    if(src.hasSpecificGravity())
      SEScalar.load(src.getSpecificGravity(),dst.getSpecificGravityResult());
    if(src.getBlood()!=ePresenceIndicator.UNRECOGNIZED)
      dst.setBloodResult(src.getBlood());
    if(src.hasPH())
      SEScalar.load(src.getPH(),dst.getPHResult());
    if(src.getProtein()!=ePresenceIndicator.UNRECOGNIZED)
      dst.setProteinResult(src.getProtein());
    if(src.hasUrobilinogen())
      SEScalarMassPerVolume.load(src.getUrobilinogen(),dst.getUrobilinogenResult());
    if(src.getNitrite()!=ePresenceIndicator.UNRECOGNIZED)
      dst.setNitriteResult(src.getNitrite());
    if(src.getLeukocyteEsterase()!=ePresenceIndicator.UNRECOGNIZED)
      dst.setLeukocyteEsteraseResult(src.getLeukocyteEsterase());
    
    if(src.hasMicroscopic())
      SEUrinalysisMicroscopic.load(src.getMicroscopic(),dst.getMicroscopicResult());
  }
  
  public static UrinalysisData unload(SEUrinalysis src)
  {
    UrinalysisData.Builder dst = UrinalysisData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEUrinalysis src, UrinalysisData.Builder dst)
  {
    SEPatientAssessment.unload(src, dst.getPatientAssessmentBuilder());
    if (src.hasColorResult())
      dst.setColor(src.color);
    if (src.hasAppearanceResult())
      dst.setAppearance(src.appearance);
    if (src.hasGlucoseResult())
      dst.setGlucose(src.glucose);
    if (src.hasKetoneResult())
      dst.setKetone(src.ketone);
    if (src.hasBilirubinResult())
      dst.setBilirubin(SEScalar.unload(src.bilirubin));
    if (src.hasSpecificGravityResult())
      dst.setSpecificGravity(SEScalar.unload(src.specificGravity));
    if (src.hasBloodResult())
      dst.setBlood(src.blood);
    if (src.hasPHResult())
      dst.setPH(SEScalar.unload(src.pH));
    if (src.hasProteinResult())
      dst.setProtein(src.protein);
    if (src.hasUrobilinogenResult())
      dst.setUrobilinogen(SEScalarMassPerVolume.unload(src.urobilinogen));
    if (src.hasNitriteResult())
      dst.setNitrite(src.nitrite);
    if (src.hasLeukocyteEsteraseResult())
      dst.setLeukocyteEsterase(src.leukocyteEsterase);
    
    if(src.hasMicroscopicResult())
      dst.setMicroscopic(SEUrinalysisMicroscopic.unload(src.microscopic));
  }
  
  public eUrineColor getColorResult() { return this.color;}
  public void    setColorResult(eUrineColor color){this.color=color;}
  public boolean hasColorResult(){return this.color==null?false:color!=eUrineColor.NullColor;}
  
  public eClarityIndicator  getAppearanceResult() { return this.appearance;}
  public void    setAppearanceResult(eClarityIndicator a){this.appearance=a;}
  public boolean hasAppearanceResult(){return this.appearance==null?false:appearance!=eClarityIndicator.NullClarity;}
  
  public ePresenceIndicator  getGlucoseResult() { return this.glucose;}
  public void    setGlucoseResult(ePresenceIndicator g){this.glucose=g;}
  public boolean hasGlucoseResult(){return this.glucose==null?false:glucose!=ePresenceIndicator.NullPresence;}
  
  public ePresenceIndicator  getKetoneResult() { return this.ketone;}
  public void    setKetoneResult(ePresenceIndicator k){this.ketone=k;}
  public boolean hasKetoneResult(){return this.ketone==null?false:ketone!=ePresenceIndicator.NullPresence;}
  
  public boolean hasBilirubinResult()
  {
    return bilirubin == null ? false : bilirubin.isValid();
  }
  public SEScalar getBilirubinResult()
  {
    if (bilirubin == null)
      bilirubin = new SEScalar();
    return bilirubin;
  }
  
  public boolean hasSpecificGravityResult()
  {
    return specificGravity == null ? false : specificGravity.isValid();
  }
  public SEScalar getSpecificGravityResult()
  {
    if (specificGravity == null)
      specificGravity = new SEScalar();
    return specificGravity;
  }
  
  public ePresenceIndicator  getBloodResult() { return this.blood;}
  public void    setBloodResult(ePresenceIndicator b){this.blood=b;}
  public boolean hasBloodResult(){return this.blood==null?false:blood!=ePresenceIndicator.NullPresence;}
  
  public boolean hasPHResult()
  {
    return pH == null ? false : pH.isValid();
  }
  public SEScalar getPHResult()
  {
    if (pH == null)
      pH = new SEScalar();
    return pH;
  }
  
  public ePresenceIndicator  getProteinResult() { return this.protein;}
  public void    setProteinResult(ePresenceIndicator p){this.protein=p;}
  public boolean hasProteinResult(){return this.protein==null?false:protein!=ePresenceIndicator.NullPresence;}
 
  
  public boolean hasUrobilinogenResult()
  {
    return urobilinogen == null ? false : urobilinogen.isValid();
  }
  public SEScalarMassPerVolume getUrobilinogenResult()
  {
    if (urobilinogen == null)
      urobilinogen = new SEScalarMassPerVolume();
    return urobilinogen;
  }
  
  public ePresenceIndicator  getNitriteResult() { return this.nitrite;}
  public void    setNitriteResult(ePresenceIndicator n){this.nitrite=n;}
  public boolean hasNitriteResult(){return this.nitrite==null?false:nitrite!=ePresenceIndicator.NullPresence;}
  
  public ePresenceIndicator  getLeukocyteEsteraseResult() { return this.leukocyteEsterase;}
  public void    setLeukocyteEsteraseResult(ePresenceIndicator l){this.leukocyteEsterase=l;}
  public boolean hasLeukocyteEsteraseResult(){return this.leukocyteEsterase==null?false:leukocyteEsterase!=ePresenceIndicator.NullPresence;}

  public boolean hasMicroscopicResult()
  {
    return microscopic == null ? false : true;
  }
  public SEUrinalysisMicroscopic getMicroscopicResult()
  {
    if (microscopic == null)
      microscopic = new SEUrinalysisMicroscopic();
    return microscopic;
  }

}

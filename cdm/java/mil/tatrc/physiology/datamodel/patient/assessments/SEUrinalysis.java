/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package mil.tatrc.physiology.datamodel.patient.assessments;

import com.google.protobuf.TextFormat;
import com.google.protobuf.TextFormat.ParseException;
import com.kitware.physiology.cdm.PatientAssessments.UrinalysisData;
import com.kitware.physiology.cdm.PatientAssessmentEnums.eUrinalysis.*;

import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.utilities.FileUtils;

public class SEUrinalysis extends SEPatientAssessment
{
  protected UrineColor            color;
  protected ClarityIndicator      appearance;
  protected PresenceIndicator     glucose;
  protected PresenceIndicator     ketone;
  protected SEScalar              bilirubin;
  protected SEScalar              specificGravity;
  protected PresenceIndicator     blood;
  protected SEScalar              pH;  
  protected PresenceIndicator     protein;
  protected SEScalarMassPerVolume urobilinogen;
  protected PresenceIndicator     nitrite;
  protected PresenceIndicator     leukocyteEsterase;
  
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
  }
  
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
  }
  
  public void readFile(String fileName) throws ParseException
  {
    UrinalysisData.Builder builder = UrinalysisData.newBuilder();
    TextFormat.getParser().merge(FileUtils.readFile(fileName), builder);
    SEUrinalysis.load(builder.build(), this);
  }
  public void writeFile(String fileName)
  {
    FileUtils.writeFile(fileName, SEUrinalysis.unload(this).toString());
  }
  
  public static void load(UrinalysisData src, SEUrinalysis dst)
  {
    SEPatientAssessment.load(src.getPatientAssessment(), dst);
    if(src.getColor()!=UrineColor.UNRECOGNIZED)
      dst.setColor(src.getColor());
    if(src.getAppearance()!=ClarityIndicator.UNRECOGNIZED)
      dst.setAppearance(src.getAppearance());
    if(src.getGlucose()!=PresenceIndicator.UNRECOGNIZED)
      dst.setGlucose(src.getGlucose());
    if(src.getKetone()!=PresenceIndicator.UNRECOGNIZED)
      dst.setKetone(src.getKetone());
    if(src.hasBilirubin())
      SEScalar.load(src.getBilirubin(),dst.getBilirubin());
    if(src.hasSpecificGravity())
      SEScalar.load(src.getSpecificGravity(),dst.getSpecificGravity());
    if(src.getBlood()!=PresenceIndicator.UNRECOGNIZED)
      dst.setBlood(src.getBlood());
    if(src.hasPH())
      SEScalar.load(src.getPH(),dst.getPH());
    if(src.getProtein()!=PresenceIndicator.UNRECOGNIZED)
      dst.setProtein(src.getProtein());
    if(src.hasUrobilinogen())
      SEScalarMassPerVolume.load(src.getUrobilinogen(),dst.getUrobilinogen());
    if(src.getNitrite()!=PresenceIndicator.UNRECOGNIZED)
      dst.setNitrite(src.getNitrite());
    if(src.getLeukocyteEsterase()!=PresenceIndicator.UNRECOGNIZED)
      dst.setLeukocyteEsterase(src.getLeukocyteEsterase());
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
    if (src.hasColor())
      dst.setColor(src.color);
    if (src.hasAppearance())
      dst.setAppearance(src.appearance);
    if (src.hasGlucose())
      dst.setGlucose(src.glucose);
    if (src.hasKetone())
      dst.setKetone(src.ketone);
    if (src.hasBilirubin())
      dst.setBilirubin(SEScalar.unload(src.bilirubin));
    if (src.hasSpecificGravity())
      dst.setSpecificGravity(SEScalar.unload(src.specificGravity));
    if (src.hasBlood())
      dst.setBlood(src.blood);
    if (src.hasPH())
      dst.setPH(SEScalar.unload(src.pH));
    if (src.hasProtein())
      dst.setProtein(src.protein);
    if (src.hasUrobilinogen())
      dst.setUrobilinogen(SEScalarMassPerVolume.unload(src.urobilinogen));
    if (src.hasNitrite())
      dst.setNitrite(src.nitrite);
    if (src.hasLeukocyteEsterase())
      dst.setLeukocyteEsterase(src.leukocyteEsterase);
  }
  
  public UrineColor getColor() { return this.color;}
  public void    setColor(UrineColor color){this.color=color;}
  public boolean hasColor(){return this.color==null?false:color!=UrineColor.NullColor;}
  
  public ClarityIndicator  getAppearance() { return this.appearance;}
  public void    setAppearance(ClarityIndicator a){this.appearance=a;}
  public boolean hasAppearance(){return this.appearance==null?false:appearance!=ClarityIndicator.NullClarity;}
  
  public PresenceIndicator  getGlucose() { return this.glucose;}
  public void    setGlucose(PresenceIndicator g){this.glucose=g;}
  public boolean hasGlucose(){return this.glucose==null?false:glucose!=PresenceIndicator.NullPresence;}
  
  public PresenceIndicator  getKetone() { return this.ketone;}
  public void    setKetone(PresenceIndicator k){this.ketone=k;}
  public boolean hasKetone(){return this.ketone==null?false:ketone!=PresenceIndicator.NullPresence;}
  
  public boolean hasBilirubin()
  {
    return bilirubin == null ? false : bilirubin.isValid();
  }
  public SEScalar getBilirubin()
  {
    if (bilirubin == null)
      bilirubin = new SEScalar();
    return bilirubin;
  }
  
  public boolean hasSpecificGravity()
  {
    return specificGravity == null ? false : specificGravity.isValid();
  }
  public SEScalar getSpecificGravity()
  {
    if (specificGravity == null)
      specificGravity = new SEScalar();
    return specificGravity;
  }
  
  public PresenceIndicator  getBlood() { return this.blood;}
  public void    setBlood(PresenceIndicator b){this.blood=b;}
  public boolean hasBlood(){return this.blood==null?false:blood!=PresenceIndicator.NullPresence;}
  
  public boolean hasPH()
  {
    return pH == null ? false : pH.isValid();
  }
  public SEScalar getPH()
  {
    if (pH == null)
      pH = new SEScalar();
    return pH;
  }
  
  public PresenceIndicator  getProtein() { return this.protein;}
  public void    setProtein(PresenceIndicator p){this.protein=p;}
  public boolean hasProtein(){return this.protein==null?false:protein!=PresenceIndicator.NullPresence;}
 
  
  public boolean hasUrobilinogen()
  {
    return urobilinogen == null ? false : urobilinogen.isValid();
  }
  public SEScalarMassPerVolume getUrobilinogen()
  {
    if (urobilinogen == null)
      urobilinogen = new SEScalarMassPerVolume();
    return urobilinogen;
  }
  
  public PresenceIndicator  getNitrite() { return this.nitrite;}
  public void    setNitrite(PresenceIndicator n){this.nitrite=n;}
  public boolean hasNitrite(){return this.nitrite==null?false:nitrite!=PresenceIndicator.NullPresence;}
  
  public PresenceIndicator  getLeukocyteEsterase() { return this.leukocyteEsterase;}
  public void    setLeukocyteEsterase(PresenceIndicator l){this.leukocyteEsterase=l;}
  public boolean hasLeukocyteEsterase(){return this.leukocyteEsterase==null?false:leukocyteEsterase!=PresenceIndicator.NullPresence;}
}

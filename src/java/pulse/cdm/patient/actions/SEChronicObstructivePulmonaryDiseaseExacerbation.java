/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.patient.actions;

import pulse.cdm.bind.PatientActions.ChronicObstructivePulmonaryDiseaseExacerbationData;

import pulse.cdm.properties.SEScalar0To1;

public class SEChronicObstructivePulmonaryDiseaseExacerbation extends SEPatientAction
{
  protected SEScalar0To1 bronchitisSeverity;
  protected SEScalar0To1 emphysemaSeverity;
  
  public SEChronicObstructivePulmonaryDiseaseExacerbation()
  {
    bronchitisSeverity = null;
    emphysemaSeverity = null;
  }
  
  public void reset()
  {
    super.reset();
    if (bronchitisSeverity != null)
      bronchitisSeverity.invalidate();
    if (emphysemaSeverity != null)
      emphysemaSeverity.invalidate();
  }
  
  public void copy(SEChronicObstructivePulmonaryDiseaseExacerbation other)
  {
    if(this==other)
      return;
    super.copy(other);
    if (other.bronchitisSeverity != null)
      getBronchitisSeverity().set(other.getBronchitisSeverity());
    if (other.emphysemaSeverity != null)
      getEmphysemaSeverity().set(other.getEmphysemaSeverity());
  }
  
  public boolean isValid()
  {
    return hasBronchitisSeverity() && hasEmphysemaSeverity();
  }
  
  public static void load(ChronicObstructivePulmonaryDiseaseExacerbationData src, SEChronicObstructivePulmonaryDiseaseExacerbation dst) 
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasBronchitisSeverity())
      SEScalar0To1.load(src.getBronchitisSeverity(),dst.getBronchitisSeverity());
    if(src.hasEmphysemaSeverity())
      SEScalar0To1.load(src.getEmphysemaSeverity(),dst.getEmphysemaSeverity());
  }
  
  public static ChronicObstructivePulmonaryDiseaseExacerbationData unload(SEChronicObstructivePulmonaryDiseaseExacerbation src)
  {
    ChronicObstructivePulmonaryDiseaseExacerbationData.Builder dst = ChronicObstructivePulmonaryDiseaseExacerbationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEChronicObstructivePulmonaryDiseaseExacerbation src, ChronicObstructivePulmonaryDiseaseExacerbationData.Builder dst)
  {
    SEPatientAction.unload(src, dst.getPatientActionBuilder());
    if (src.hasBronchitisSeverity())
      dst.setBronchitisSeverity(SEScalar0To1.unload(src.bronchitisSeverity));
    if (src.hasEmphysemaSeverity())
      dst.setEmphysemaSeverity(SEScalar0To1.unload(src.emphysemaSeverity));
  }
  
  public boolean hasBronchitisSeverity()
  {
    return bronchitisSeverity == null ? false : bronchitisSeverity.isValid();
  }
  public SEScalar0To1 getBronchitisSeverity()
  {
    if (bronchitisSeverity == null)
      bronchitisSeverity = new SEScalar0To1();
    return bronchitisSeverity;
  }
  
  public boolean hasEmphysemaSeverity()
  {
    return emphysemaSeverity == null ? false : emphysemaSeverity.isValid();
  }
  public SEScalar0To1 getEmphysemaSeverity()
  {
    if (emphysemaSeverity == null)
      emphysemaSeverity = new SEScalar0To1();
    return emphysemaSeverity;
  }
  
  public String toString()
  {
    return "COPD Exacerbation" 
        + "\n\tBronchitis Severity: " + getBronchitisSeverity()
        + "\n\tEmphysema Severity: " + getEmphysemaSeverity();
  }
}

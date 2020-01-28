/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.patient.actions;

import pulse.cdm.bind.PatientActions.AcuteRespiratoryDistressSyndromeExacerbationData;

import pulse.cdm.properties.SEScalar0To1;

public class SEAcuteRespiratoryDistressSyndromeExacerbation extends SEPatientAction
{
  protected SEScalar0To1 severity;
  protected SEScalar0To1 leftLungAffected;
  protected SEScalar0To1 rightLungAffected;
  
  public SEAcuteRespiratoryDistressSyndromeExacerbation()
  {
    severity = null;
    leftLungAffected = null;
    rightLungAffected = null;
  }
  
  public void reset()
  {
    super.reset();
    if (severity != null)
      severity.invalidate();
    if (leftLungAffected != null)
      leftLungAffected.invalidate();
    if (rightLungAffected != null)
      rightLungAffected.invalidate();
  }
  
  public void copy(SEAcuteRespiratoryDistressSyndromeExacerbation other)
  {
    if(this==other)
      return;
    super.copy(other);
    if (other.severity != null)
      getSeverity().set(other.getSeverity());
    if (other.leftLungAffected != null)
      getLeftLungAffected().set(other.getLeftLungAffected());
    if (other.rightLungAffected != null)
      getRightLungAffected().set(other.getRightLungAffected());
  }
  
  public boolean isValid()
  {
    return hasSeverity() && hasLeftLungAffected() && hasRightLungAffected();
  }
  
  public static void load(AcuteRespiratoryDistressSyndromeExacerbationData src, SEAcuteRespiratoryDistressSyndromeExacerbation dst) 
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
    if(src.hasLeftLungAffected())
      SEScalar0To1.load(src.getLeftLungAffected(),dst.getLeftLungAffected());
    if(src.hasRightLungAffected())
      SEScalar0To1.load(src.getRightLungAffected(),dst.getRightLungAffected());
  }
  
  public static AcuteRespiratoryDistressSyndromeExacerbationData unload(SEAcuteRespiratoryDistressSyndromeExacerbation src) 
  {
    AcuteRespiratoryDistressSyndromeExacerbationData.Builder dst = AcuteRespiratoryDistressSyndromeExacerbationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEAcuteRespiratoryDistressSyndromeExacerbation src, AcuteRespiratoryDistressSyndromeExacerbationData.Builder dst)
  {
    SEPatientAction.unload(src, dst.getPatientActionBuilder());
    if (src.hasSeverity())
      dst.setSeverity(SEScalar0To1.unload(src.severity));
    if (src.hasLeftLungAffected())
      dst.setLeftLungAffected(SEScalar0To1.unload(src.leftLungAffected));
    if (src.hasRightLungAffected())
      dst.setRightLungAffected(SEScalar0To1.unload(src.rightLungAffected));
  }
  
  public boolean hasSeverity()
  {
    return severity == null ? false : severity.isValid();
  }
  public SEScalar0To1 getSeverity()
  {
    if (severity == null)
      severity = new SEScalar0To1();
    return severity;
  }
  
  public boolean hasLeftLungAffected()
  {
    return leftLungAffected == null ? false : leftLungAffected.isValid();
  }
  public SEScalar0To1 getLeftLungAffected()
  {
    if (leftLungAffected == null)
      leftLungAffected = new SEScalar0To1();
    return leftLungAffected;
  }
  
  public boolean hasRightLungAffected()
  {
    return rightLungAffected == null ? false : rightLungAffected.isValid();
  }
  public SEScalar0To1 getRightLungAffected()
  {
    if (rightLungAffected == null)
      rightLungAffected = new SEScalar0To1();
    return rightLungAffected;
  }
  
  public String toString()
  {
    return "ARDS Exacerbation" 
        + "\n\tSeverity: " + getSeverity()
        + "\n\tLeftLungAffected: " + getLeftLungAffected()
        + "\n\tRightLungAffected: " + getRightLungAffected();
  }

  
}

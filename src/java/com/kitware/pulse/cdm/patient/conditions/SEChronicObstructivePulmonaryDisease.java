/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.conditions;

import java.util.HashMap;
import java.util.Map;

import com.kitware.pulse.cdm.bind.PatientConditions.ChronicObstructivePulmonaryDiseaseData;
import com.kitware.pulse.cdm.bind.Physiology.LungImpairmentData;
import com.kitware.pulse.cdm.bind.Physiology.eLungCompartment;
import com.kitware.pulse.cdm.properties.SEScalar0To1;

public class SEChronicObstructivePulmonaryDisease extends SEPatientCondition
{

  private static final long serialVersionUID = 143498768711182994L;
  protected SEScalar0To1 bronchitisSeverity;
  protected Map<eLungCompartment,SEScalar0To1> emphysemaSeverities;
  
  public SEChronicObstructivePulmonaryDisease()
  {
    bronchitisSeverity = null;
    emphysemaSeverities = new HashMap<eLungCompartment,SEScalar0To1>();
  }
  
  @Override
  public void clear()
  {
    super.clear();
    if (bronchitisSeverity != null)
      bronchitisSeverity.invalidate();
    for(SEScalar0To1 s : emphysemaSeverities.values())
      s.invalidate();
  }
  
  public void copy(SEChronicObstructivePulmonaryDisease other)
  {
    if(this==other)
      return;
    super.copy(other);
    if (other.bronchitisSeverity != null)
      getBronchitisSeverity().set(other.getBronchitisSeverity());
    for (Map.Entry<eLungCompartment, SEScalar0To1> entry : other.emphysemaSeverities.entrySet())
      getEmphysemaSeverity(entry.getKey()).set(entry.getValue());
  }
  
  public static void load(ChronicObstructivePulmonaryDiseaseData src, SEChronicObstructivePulmonaryDisease dst) 
  {
    SEPatientCondition.load(src.getPatientCondition(), dst);
    if(src.hasBronchitisSeverity())
      SEScalar0To1.load(src.getBronchitisSeverity(),dst.getBronchitisSeverity());
    for (LungImpairmentData d : src.getEmphysemaSeverityList())
      SEScalar0To1.load(d.getSeverity(), dst.getEmphysemaSeverity(d.getCompartment()));
  }
  
  public static ChronicObstructivePulmonaryDiseaseData unload(SEChronicObstructivePulmonaryDisease src)
  {
    ChronicObstructivePulmonaryDiseaseData.Builder dst = ChronicObstructivePulmonaryDiseaseData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEChronicObstructivePulmonaryDisease src, ChronicObstructivePulmonaryDiseaseData.Builder dst)
  {
    SEPatientCondition.unload(src, dst.getPatientConditionBuilder());
    if (src.hasBronchitisSeverity())
      dst.setBronchitisSeverity(SEScalar0To1.unload(src.bronchitisSeverity));
    for (Map.Entry<eLungCompartment, SEScalar0To1> entry : src.emphysemaSeverities.entrySet())
    {
      LungImpairmentData.Builder builder = LungImpairmentData.newBuilder();
      builder.setCompartment(entry.getKey());
      builder.setSeverity(SEScalar0To1.unload(entry.getValue()));
      dst.addEmphysemaSeverity(builder);
    }
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
    for(SEScalar0To1 s : emphysemaSeverities.values())
      if(s.isValid())
        return true;
    return false;
  }
  public boolean hasEmphysemaSeverity(eLungCompartment c)
  {
    SEScalar0To1 s = emphysemaSeverities.get(c);
    if(s!=null)
      return s.isValid();
    return false;
  }
  public SEScalar0To1 getEmphysemaSeverity(eLungCompartment c)
  {
    SEScalar0To1 s = emphysemaSeverities.get(c);
    if(s==null)
    {
      s = new SEScalar0To1();
      emphysemaSeverities.put(c, s);
    }
    return s;
  }
  
  @Override
  public String toString()
  {
    String out = "COPD";
    out += "\n\tBronchitis Severity: " + getBronchitisSeverity();
    for (Map.Entry<eLungCompartment, SEScalar0To1> entry : emphysemaSeverities.entrySet())
      out += "\n\t"+ entry.getKey().toString()+" Emphysema Severity: " + getEmphysemaSeverity(entry.getKey());
    return out;
  }
}

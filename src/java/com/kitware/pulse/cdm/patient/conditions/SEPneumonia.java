/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.conditions;

import java.util.HashMap;
import java.util.Map;

import com.kitware.pulse.cdm.bind.PatientConditions.PneumoniaData;
import com.kitware.pulse.cdm.bind.Physiology.LungImpairmentData;
import com.kitware.pulse.cdm.bind.Physiology.eLungCompartment;
import com.kitware.pulse.cdm.properties.SEScalar0To1;

public class SEPneumonia extends SEPatientCondition
{

  private static final long serialVersionUID = 1187107669133323128L;
  protected Map<eLungCompartment,SEScalar0To1> severities;
  
  public SEPneumonia()
  {
    severities = new HashMap<eLungCompartment,SEScalar0To1>();
  }
  
  @Override
  public void clear()
  {
    super.clear();
    for(SEScalar0To1 s : severities.values())
      s.invalidate();
  }
  
  public void copy(SEPneumonia other)
  {
    if(this==other)
      return;
    super.copy(other);
    for (Map.Entry<eLungCompartment, SEScalar0To1> entry : other.severities.entrySet())
      getSeverity(entry.getKey()).set(entry.getValue());
  }
  
  public static void load(PneumoniaData src, SEPneumonia dst) 
  {
    SEPatientCondition.load(src.getPatientCondition(), dst);
    for (LungImpairmentData d : src.getSeverityList())
      SEScalar0To1.load(d.getSeverity(), dst.getSeverity(d.getCompartment()));
  }
  
  public static PneumoniaData unload(SEPneumonia src) 
  {
    PneumoniaData.Builder dst = PneumoniaData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEPneumonia src, PneumoniaData.Builder dst)
  {
    SEPatientCondition.unload(src, dst.getPatientConditionBuilder());
    for (Map.Entry<eLungCompartment, SEScalar0To1> entry : src.severities.entrySet())
    {
      LungImpairmentData.Builder builder = LungImpairmentData.newBuilder();
      builder.setCompartment(entry.getKey());
      builder.setSeverity(SEScalar0To1.unload(entry.getValue()));
      dst.addSeverity(builder);
    }
  }
  

  public boolean hasSeverity()
  {
    for(SEScalar0To1 s : severities.values())
      if(s.isValid())
        return true;
    return false;
  }
  public boolean hasSeverity(eLungCompartment c)
  {
    SEScalar0To1 s = severities.get(c);
    if(s!=null)
      return s.isValid();
    return false;
  }
  public SEScalar0To1 getSeverity(eLungCompartment c)
  {
    SEScalar0To1 s = severities.get(c);
    if(s==null)
    {
      s = new SEScalar0To1();
      severities.put(c, s);
    }
    return s;
  }
  
  @Override
  public String toString()
  {
    String out = "Pneumonia";
    for (Map.Entry<eLungCompartment, SEScalar0To1> entry : severities.entrySet())
      out += "\n\t"+ entry.getKey().toString()+" Severity: " + getSeverity(entry.getKey());
    return out;
  }
  
}

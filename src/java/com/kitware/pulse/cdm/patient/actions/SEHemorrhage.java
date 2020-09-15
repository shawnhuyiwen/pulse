/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.HemorrhageData;
import com.kitware.pulse.cdm.bind.PatientActions.HemorrhageData.eType;
import com.kitware.pulse.cdm.properties.SEScalar0To1;
import com.kitware.pulse.cdm.properties.SEScalarVolumePerTime;

public class SEHemorrhage extends SEPatientAction
{

  private static final long serialVersionUID = -1654353830396880L;
  protected String compartment;
  protected eType type;
  protected SEScalarVolumePerTime flowRate;
  protected SEScalar0To1 severity;
  
  public SEHemorrhage()
  {
    type = eType.External;
    compartment = null;
    flowRate = null;
    severity = null;
  }
  
  public void copy(SEHemorrhage other)
  {
    if(this==other)
      return;
    super.copy(other);
    type = other.type;
    compartment = other.compartment;
    if (other.flowRate != null)
      getFlowRate().set(other.getFlowRate());
    else if (flowRate != null)
      flowRate.invalidate();
    if (other.severity != null)
      getSeverity().set(other.getSeverity());
    else if (severity != null)
      severity.invalidate();
  }
  
  @Override
  public void reset()
  {
    super.reset();
    type = eType.External;
    compartment = null;
    if (flowRate != null)
      flowRate.invalidate();
    if (severity != null)
      severity.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return hasCompartment() && (hasFlowRate() || hasSeverity());
  }
  
  public static void load(HemorrhageData src, SEHemorrhage dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    dst.type = src.getType();
    dst.compartment = src.getCompartment();
    if(src.hasFlowRate())
      SEScalarVolumePerTime.load(src.getFlowRate(),dst.getFlowRate());
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  
  public static HemorrhageData unload(SEHemorrhage src)
  {
    HemorrhageData.Builder dst = HemorrhageData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEHemorrhage src, HemorrhageData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if (src.hasCompartment())
    dst.setType(src.type);
      dst.setCompartment(src.compartment);
    if (src.hasFlowRate())
      dst.setFlowRate(SEScalarVolumePerTime.unload(src.flowRate));
    if (src.hasSeverity())
      dst.setSeverity(SEScalar0To1.unload(src.severity));
  }
  
  public eType getType()
  {
    return type;
  }
  public void setType(eType t)
  {
    type = t;
  }
  public boolean hasType()
  {
    return type != null;
  }
  
  public String getCompartment()
  {
    return compartment;
  }
  public void setCompartment(Enum<?> compartment)
  {
    this.compartment = compartment.name();
  }
  public void setCompartment(String compartment)
  {
    this.compartment = compartment;
  }
  public boolean hasCompartment()
  {
    return compartment == null ? false : !compartment.isEmpty();
  }
  
  public boolean hasFlowRate()
  {
    return flowRate == null ? false : flowRate.isValid();
  }
  public SEScalarVolumePerTime getFlowRate()
  {
    if (flowRate == null)
      flowRate = new SEScalarVolumePerTime();
    return flowRate;
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
  
  @Override
  public String toString()
  {
    return "Hemorrhage" 
        + "\n\tType: " + getType()
        + "\n\tCompartment: " + getCompartment()
        + "\n\tFlowRate: " + getFlowRate()
        + "\n\tSeverity: " + getSeverity();
  }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.system.equipment.electrocardiogram;

import com.kitware.pulse.cdm.bind.ElectroCardioGram.ElectroCardioGramWaveformData;
import com.kitware.pulse.cdm.bind.ElectroCardioGram.ElectroCardioGramWaveformData.eWaveformLead;
import com.kitware.pulse.cdm.bind.ElectroCardioGram.ElectroCardioGramWaveformData.eWaveformType;
import com.kitware.pulse.cdm.bind.Physiology.eHeartRhythm;
import com.kitware.pulse.cdm.properties.SEArrayElectricPotential;
import com.kitware.pulse.cdm.properties.SEFunctionElectricPotentialVsTime;
import com.kitware.pulse.cdm.properties.SEScalarTime;

public class SEElectroCardioGramWaveform
{
  protected eWaveformLead                     lead;
  protected eWaveformType                     type;
  protected SEArrayElectricPotential          originalData;
  
  public SEElectroCardioGramWaveform()
  {
    clear();
  }
  
  public void clear()
  {
    lead = null;
    type = null;
    originalData = null;
  }
  
  public static void load(ElectroCardioGramWaveformData src, SEElectroCardioGramWaveform dst)
  {
    dst.clear();
    if(src.getType()!=eWaveformType.UNRECOGNIZED)
      dst.setType(src.getType());
    if(src.getLead()!=ElectroCardioGramWaveformData.eWaveformLead.UNRECOGNIZED)
     dst.setLead(src.getLead());
    if(src.hasOriginalData())
      SEArrayElectricPotential.load(src.getOriginalData(),dst.getOriginalData());
  }
  public static ElectroCardioGramWaveformData unload(SEElectroCardioGramWaveform src)
  {
    ElectroCardioGramWaveformData.Builder dst =  ElectroCardioGramWaveformData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEElectroCardioGramWaveform src, ElectroCardioGramWaveformData.Builder dst)
  {
    if(src.hasLead())
      dst.setLead(src.lead);
    if(src.hasType())
      dst.setType(src.type);
    if(src.hasOriginalData())
      dst.setOriginalData(SEArrayElectricPotential.unload(src.originalData));
  }
  
  public ElectroCardioGramWaveformData.eWaveformLead getLead()
  {
    return lead;
  }
  public void setLead(ElectroCardioGramWaveformData.eWaveformLead l)
  {
    this.lead = l;
  }
  public boolean hasLead()
  {
    return lead==null ? false : lead != ElectroCardioGramWaveformData.eWaveformLead.NullLead;
  }
  public void removeLead()
  {
    this.lead = null;
  }
  
  public eWaveformType getType()
  {
    return type;
  }
  public void setType(eWaveformType t)
  {
    this.type = t;
  }
  public boolean hasType()
  {
    return type == null ? false : true;
  }
  
  public boolean hasOriginalData()
  {
    return originalData == null ? false : originalData.isValid();
  }
  public SEArrayElectricPotential getOriginalData()
  {
    if (originalData == null)
      originalData = new SEArrayElectricPotential();
    return originalData;
  }
}

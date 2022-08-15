/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.system.equipment.electrocardiogram;

import com.kitware.pulse.cdm.bind.ElectroCardioGram.ElectroCardioGramWaveformData;
import com.kitware.pulse.cdm.bind.ElectroCardioGram.eElectroCardioGramWaveformLead;
import com.kitware.pulse.cdm.bind.ElectroCardioGram.eElectroCardioGramWaveformType;
import com.kitware.pulse.cdm.properties.SEArrayElectricPotential;

public class SEElectroCardioGramWaveform
{
  protected eElectroCardioGramWaveformLead    lead;
  protected eElectroCardioGramWaveformType    type;
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
    if(src.getType()!=eElectroCardioGramWaveformType.UNRECOGNIZED)
      dst.setType(src.getType());
    if(src.getLead()!=eElectroCardioGramWaveformLead.UNRECOGNIZED)
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
  
  public eElectroCardioGramWaveformLead getLead()
  {
    return lead;
  }
  public void setLead(eElectroCardioGramWaveformLead l)
  {
    this.lead = l;
  }
  public boolean hasLead()
  {
    return lead==null ? false : lead != eElectroCardioGramWaveformLead.NullLead;
  }
  public void removeLead()
  {
    this.lead = null;
  }
  
  public eElectroCardioGramWaveformType getType()
  {
    return type;
  }
  public void setType(eElectroCardioGramWaveformType t)
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

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.electrocardiogram;

import java.util.*;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;
import com.kitware.pulse.cdm.bind.ElectroCardioGram.ElectroCardioGramData;
import com.kitware.pulse.cdm.bind.ElectroCardioGram.ElectroCardioGramWaveformData;
import com.kitware.pulse.cdm.bind.Substance.SubstanceFractionData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;
import com.kitware.pulse.cdm.properties.SEScalarElectricPotential;
import com.kitware.pulse.cdm.substance.SESubstanceConcentration;
import com.kitware.pulse.cdm.substance.SESubstanceFraction;
import com.kitware.pulse.cdm.system.equipment.SEEquipment;
import com.kitware.pulse.utilities.FileUtils;

public class SEElectroCardioGram implements SEEquipment
{
  protected SEScalarElectricPotential lead3ElectricPotential;
  
  protected List<SEElectroCardioGramWaveform> waveforms = new ArrayList<SEElectroCardioGramWaveform>();
  
  public SEElectroCardioGram()
  {
    lead3ElectricPotential = null;
  }
  
  @Override
  public void clear()
  {
    if (hasLead3ElectricPotential())
      lead3ElectricPotential.invalidate();

    this.waveforms.clear();
  }
  
  public void readFile(String fileName) throws InvalidProtocolBufferException
  {
    ElectroCardioGramData.Builder builder = ElectroCardioGramData.newBuilder();
    JsonFormat.parser().merge(FileUtils.readFile(fileName), builder);
    SEElectroCardioGram.load(builder.build(), this);
  }
  public void writeFile(String fileName) throws InvalidProtocolBufferException
  {
    FileUtils.writeFile(fileName, JsonFormat.printer().print(SEElectroCardioGram.unload(this)));
  }
  
  public static void load(ElectroCardioGramData src, SEElectroCardioGram dst)
  {
    dst.clear();
    
    if (src.hasLead3ElectricPotential())
      SEScalarElectricPotential.load(src.getLead3ElectricPotential(),dst.getLead3ElectricPotential());
    
    if(src.getWaveformsList()!=null)
    {
      for(ElectroCardioGramWaveformData waveformData : src.getWaveformsList())
      {
        SEElectroCardioGramWaveform waveform = new SEElectroCardioGramWaveform();
        SEElectroCardioGramWaveform.load(waveformData, waveform);
        dst.waveforms.add(waveform);
      }
    }
  }
  public static ElectroCardioGramData unload(SEElectroCardioGram src)
  {
    ElectroCardioGramData.Builder dst = ElectroCardioGramData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEElectroCardioGram src, ElectroCardioGramData.Builder dst)
  {
    if (src.hasLead3ElectricPotential())
      dst.setLead3ElectricPotential(SEScalarElectricPotential.unload(src.lead3ElectricPotential));
    
    for(SEElectroCardioGramWaveform waveform : src.waveforms)
    {
      dst.addWaveforms(SEElectroCardioGramWaveform.unload(waveform));
    }
  }
  
  public SEScalarElectricPotential getLead3ElectricPotential()
  {
    if (lead3ElectricPotential == null)
      lead3ElectricPotential = new SEScalarElectricPotential();
    return lead3ElectricPotential;
  }
  public boolean hasLead3ElectricPotential()
  {
    return lead3ElectricPotential == null ? false : lead3ElectricPotential.isValid();
  }
  
  public boolean hasWaveforms()
  {
    return !this.waveforms.isEmpty();
  }
  public List<SEElectroCardioGramWaveform> getWaveforms()
  {
    return this.waveforms;
  }
  
}

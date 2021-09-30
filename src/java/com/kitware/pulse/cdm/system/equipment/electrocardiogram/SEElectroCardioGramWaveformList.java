/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.system.equipment.electrocardiogram;

import java.util.HashMap;
import java.util.Map;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;

import com.kitware.pulse.cdm.bind.ElectroCardioGram.ElectroCardioGramWaveformData;
import com.kitware.pulse.cdm.bind.ElectroCardioGram.ElectroCardioGramWaveformData.eWaveformLead;
import com.kitware.pulse.cdm.bind.ElectroCardioGram.ElectroCardioGramWaveformListData;
import com.kitware.pulse.cdm.bind.Physiology.eHeartRhythm;
import com.kitware.pulse.utilities.FileUtils;

public class SEElectroCardioGramWaveformList
{
  Map<eWaveformLead,Map<eHeartRhythm,SEElectroCardioGramWaveform>> waveforms = new HashMap<>();
 
  public SEElectroCardioGramWaveformList()
  {
    clear();
  }
  
  public void clear()
  {
    waveforms.clear();
  }
  
  public void readFile(String fileName) throws InvalidProtocolBufferException
  {
    ElectroCardioGramWaveformListData.Builder builder = ElectroCardioGramWaveformListData.newBuilder();
    JsonFormat.parser().merge(FileUtils.readFile(fileName), builder);
    SEElectroCardioGramWaveformList.load(builder.build(), this);
  }
  public void writeFile(String fileName) throws InvalidProtocolBufferException
  {
    FileUtils.writeFile(fileName, JsonFormat.printer().print(SEElectroCardioGramWaveformList.unload(this)));
  }
  
  public static void load(ElectroCardioGramWaveformListData src, SEElectroCardioGramWaveformList dst)
  {
    dst.clear();
    for(ElectroCardioGramWaveformData wData : src.getWaveformList())
    {
      SEElectroCardioGramWaveform w = new SEElectroCardioGramWaveform();
      SEElectroCardioGramWaveform.load(wData,w);
      Map<eHeartRhythm, SEElectroCardioGramWaveform> leads = dst.waveforms.get(w.getLead());
      if(leads == null)
      {
        leads = new HashMap<>();
        dst.waveforms.put(w.getLead(), leads);
      }
      leads.put(w.getRhythm(), w);
    }
  }
  public static ElectroCardioGramWaveformListData unload(SEElectroCardioGramWaveformList src)
  {
    ElectroCardioGramWaveformListData.Builder dst =  ElectroCardioGramWaveformListData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEElectroCardioGramWaveformList src, ElectroCardioGramWaveformListData.Builder dst)
  {
    for(Map<eHeartRhythm, SEElectroCardioGramWaveform> leads : src.waveforms.values())
    {     
      for(SEElectroCardioGramWaveform w : leads.values())
        dst.addWaveform(SEElectroCardioGramWaveform.unload(w));
    }
  }
  
  public SEElectroCardioGramWaveform getWaveform(eWaveformLead lead, eHeartRhythm rhythm)
  {
    Map<eHeartRhythm, SEElectroCardioGramWaveform> leads = this.waveforms.get(lead);
    if(leads == null)
    {
      leads = new HashMap<>();
      this.waveforms.put(lead, leads);
    }
    if(!leads.containsKey(rhythm))
    {
    	SEElectroCardioGramWaveform waveform = new SEElectroCardioGramWaveform();
    	waveform.setLead(lead);
    	waveform.setRhythm(rhythm);
      leads.put(rhythm, waveform);
    }
    return leads.get(rhythm);
  }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.cdm.system.equipment.electrocardiogram;

import java.util.*;

import com.google.protobuf.*;
import com.google.protobuf.util.*;
import pulse.cdm.bind.ElectroCardioGram.ElectroCardioGramWaveformData;
import pulse.cdm.bind.ElectroCardioGram.ElectroCardioGramWaveformData.eWaveformLead;
import pulse.cdm.bind.ElectroCardioGram.ElectroCardioGramWaveformListData;
import pulse.cdm.bind.Physiology.eHeartRhythm;
import pulse.utilities.FileUtils;

public class SEElectroCardioGramWaveformList
{
  Map<eWaveformLead,Map<eHeartRhythm,SEElectroCardioGramWaveform>> waveforms = new HashMap<eWaveformLead, Map<eHeartRhythm,SEElectroCardioGramWaveform>>();
 
  public SEElectroCardioGramWaveformList()
  {
    reset();
  }
  
  public void reset()
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
    dst.reset();
    for(ElectroCardioGramWaveformData wData : src.getWaveformList())
    {
      SEElectroCardioGramWaveform w = new SEElectroCardioGramWaveform();
      SEElectroCardioGramWaveform.load(wData,w);
      Map<eHeartRhythm, SEElectroCardioGramWaveform> leads = dst.waveforms.get(w.getLead());
      if(leads == null)
      {
        leads = new HashMap<eHeartRhythm, SEElectroCardioGramWaveform>();
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
      leads = new HashMap<eHeartRhythm, SEElectroCardioGramWaveform>();
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

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.system.equipment.electrocardiogram;

import com.kitware.pulse.cdm.bind.ElectroCardioGram.ElectroCardioGramWaveformData;
import com.kitware.pulse.cdm.bind.Physiology.eHeartRhythm;
import com.kitware.pulse.cdm.properties.SEFunctionElectricPotentialVsTime;
import com.kitware.pulse.cdm.properties.SEScalarTime;

public class SEElectroCardioGramWaveform
{
  protected ElectroCardioGramWaveformData.eWaveformLead   lead;
  protected eHeartRhythm                      rhythm;
  protected SEFunctionElectricPotentialVsTime data;
  protected SEScalarTime                      timeStep;
  
  public SEElectroCardioGramWaveform()
  {
    clear();
  }
  
  public void clear()
  {
    lead = ElectroCardioGramWaveformData.eWaveformLead.NullLead;
    rhythm = null;
    data = null;
    timeStep = null;
  }
  
  public static void load(ElectroCardioGramWaveformData src, SEElectroCardioGramWaveform dst)
  {
    dst.clear();
    if(src.getRhythm()!=eHeartRhythm.UNRECOGNIZED)
      dst.setRhythm(src.getRhythm());
    if(src.getLead()!=ElectroCardioGramWaveformData.eWaveformLead.UNRECOGNIZED)
     dst.setLead(src.getLead());
    if(src.hasData())
      SEFunctionElectricPotentialVsTime.load(src.getData(),dst.getData());
    if(src.hasTimeStep())
      SEScalarTime.load(src.getTimeStep(),dst.getTimeStep()); 
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
    if(src.hasRhythm())
      dst.setRhythm(src.rhythm);
    if(src.hasData())
      dst.setData(SEFunctionElectricPotentialVsTime.unload(src.data));
    if(src.hasTimeStep())
      dst.setTimeStep(SEScalarTime.unload(src.timeStep));
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
  
  public eHeartRhythm getRhythm()
  {
    return rhythm;
  }
  public void setRhythm(eHeartRhythm r)
  {
    this.rhythm = r;
  }
  public boolean hasRhythm()
  {
    return rhythm == null ? false : true;
  }
  
  public boolean hasData()
  {
    return data == null ? false : data.isValid();
  }
  public SEFunctionElectricPotentialVsTime getData()
  {
    if (data == null)
      data = new SEFunctionElectricPotentialVsTime();
    return data;
  }

  public boolean hasTimeStep()
  {
    return timeStep == null ? false : timeStep.isValid();
  }
  public SEScalarTime getTimeStep()
  {
    if (timeStep == null)
      timeStep = new SEScalarTime();
    return timeStep;
  }
}

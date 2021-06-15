/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.SubstanceInfusionData;
import com.kitware.pulse.cdm.properties.SEScalarMassPerVolume;
import com.kitware.pulse.cdm.properties.SEScalarVolume;
import com.kitware.pulse.cdm.properties.SEScalarVolumePerTime;

public class SESubstanceInfusion extends SEPatientAction
{

  private static final long serialVersionUID = 8029916816858227270L;
  protected SEScalarMassPerVolume concentration;
  protected SEScalarVolumePerTime rate;
  protected SEScalarVolume volume;
  protected String substance;
  
  public SESubstanceInfusion()
  {
    this.rate = null;
    this.concentration = null;
    this.volume = null;
    this.substance = "";
  }
  
  public void copy(SESubstanceInfusion other)
  {
    if(this==other)
      return;
    super.copy(other);
    substance = other.substance;
    
    if (other.rate != null)
      getRate().set(other.rate);
    else if (rate != null)
      rate.invalidate();
    
    if (other.concentration != null)
      getConcentration().set(other.concentration);
    else if (concentration != null)
      concentration.invalidate();
    
    if (other.volume != null)
      getVolume().set(other.volume);
    else if (volume != null)
      volume.invalidate();
  }
  
  @Override
  public void reset()
  {
    super.reset();
    if (rate != null)
      rate.invalidate();
    if (concentration != null)
      concentration.invalidate();
    if (volume != null)
      volume.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return hasRate() && hasConcentration() && hasSubstance();
  }
  
  public static void load(SubstanceInfusionData src, SESubstanceInfusion dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    dst.setSubstance(src.getSubstance());
    if(src.hasRate())
      SEScalarVolumePerTime.load(src.getRate(),dst.getRate());
    if(src.hasConcentration())
      SEScalarMassPerVolume.load(src.getConcentration(),dst.getConcentration());
    if(src.hasVolume())
      SEScalarVolume.load(src.getVolume(),dst.getVolume());
  }
  
  public static SubstanceInfusionData unload(SESubstanceInfusion src)
  {
    SubstanceInfusionData.Builder dst = SubstanceInfusionData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SESubstanceInfusion src, SubstanceInfusionData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if (src.hasRate())
      dst.setRate(SEScalarVolumePerTime.unload(src.rate));
    if (src.hasConcentration())
      dst.setConcentration(SEScalarMassPerVolume.unload(src.concentration));
    if (src.hasVolume())
      dst.setVolume(SEScalarVolume.unload(src.volume));
    dst.setSubstance(src.substance);
  }
  
  public boolean hasConcentration()
  {
    return concentration == null ? false : concentration.isValid();
  }
  public SEScalarMassPerVolume getConcentration()
  {
    if (concentration == null)
      concentration = new SEScalarMassPerVolume();
    return concentration;
  }
  
  public boolean hasRate()
  {
    return rate ==  null ? false : rate.isValid();
  }
  public SEScalarVolumePerTime getRate()
  {
    if (rate == null)
      rate = new SEScalarVolumePerTime();
    return rate;
  }
  
  public boolean hasVolume()
  {
    return volume ==  null ? false : volume.isValid();
  }
  public SEScalarVolume getVolume()
  {
    if (volume == null)
      volume = new SEScalarVolume();
    return volume;
  }
  
  public boolean hasSubstance() { return substance != null; }
  public void setSubstance(String name)
  {
    substance = name==null?"":name;
  }
  public String getSubstance()
  {
    return substance;
  }
  
  @Override
  public String toString()
  {
    if (rate != null || concentration != null)  
      return "Substance Infusion"
          + "\n\tRate: " + getRate()
          + "\n\tConcentration: " + getConcentration()
          + "\n\tVolume: " + getVolume()
          + "\n\tSubstance: " + getSubstance();
    else
      return "Action not specified properly";
  }
}

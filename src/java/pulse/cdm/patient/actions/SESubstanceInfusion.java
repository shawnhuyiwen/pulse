/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.patient.actions;

import pulse.cdm.bind.PatientActions.SubstanceInfusionData;
import pulse.cdm.properties.SEScalarMassPerVolume;
import pulse.cdm.properties.SEScalarVolumePerTime;

public class SESubstanceInfusion extends SEPatientAction
{

  private static final long serialVersionUID = 8029916816858227270L;
  protected SEScalarMassPerVolume concentration;
  protected SEScalarVolumePerTime rate;
  protected String substance;
  
  public SESubstanceInfusion()
  {
    this.rate = null;
    this.concentration = null;
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
  }
  
  @Override
  public void reset()
  {
    super.reset();
    if (rate != null)
      rate.invalidate();
    if (concentration != null)
      concentration.invalidate();
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
          + "\n\tSubstance: " + getSubstance();
    else
      return "Action not specified properly";
  }
}

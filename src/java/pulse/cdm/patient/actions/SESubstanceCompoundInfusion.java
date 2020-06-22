/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.patient.actions;

import pulse.cdm.bind.PatientActions.SubstanceCompoundInfusionData;
import pulse.cdm.properties.SEScalarVolume;
import pulse.cdm.properties.SEScalarVolumePerTime;

public class SESubstanceCompoundInfusion extends SEPatientAction
{

  private static final long serialVersionUID = -8462874362131575884L;
  protected SEScalarVolume bagVolume;
  protected SEScalarVolumePerTime rate;
  protected String compound;
  
  public SESubstanceCompoundInfusion()
  {
    this.rate = null;
    this.bagVolume = null;
    this.compound = "";
  }

  public void copy(SESubstanceCompoundInfusion other)
  {
    if(this==other)
      return;
    super.copy(other);
    compound = other.compound;
    
    if (other.rate != null)
      getRate().set(other.rate);
    else if (rate != null)
      rate.invalidate();
    
    if (other.bagVolume != null)
      getBagVolume().set(other.bagVolume);
    else if (bagVolume != null)
      bagVolume.invalidate();
  }
  
  @Override
  public void reset()
  {
    super.reset();
    if (rate != null)
      rate.invalidate();
    if (bagVolume != null)
      bagVolume.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return hasRate() && hasBagVolume() && hasSubstanceCompound();
  }
  
  public static void load(SubstanceCompoundInfusionData src, SESubstanceCompoundInfusion dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    dst.setSubstanceCompound(src.getSubstanceCompound());
    if(src.hasRate())
      SEScalarVolumePerTime.load(src.getRate(),dst.getRate());
    if(src.hasBagVolume())
      SEScalarVolume.load(src.getBagVolume(),dst.getBagVolume());
  }
  
  public static SubstanceCompoundInfusionData unload(SESubstanceCompoundInfusion src)
  {
    SubstanceCompoundInfusionData.Builder dst = SubstanceCompoundInfusionData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SESubstanceCompoundInfusion src, SubstanceCompoundInfusionData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if (src.hasRate())
      dst.setRate(SEScalarVolumePerTime.unload(src.rate));
    if (src.hasBagVolume())
      dst.setBagVolume(SEScalarVolume.unload(src.bagVolume));
    dst.setSubstanceCompound(src.getSubstanceCompound());
  }
  
  public boolean hasBagVolume()
  {
    return bagVolume == null ? false : bagVolume.isValid();
  }
  public SEScalarVolume getBagVolume()
  {
    if (bagVolume == null)
      bagVolume = new SEScalarVolume();
    return bagVolume;
  }
  
  public boolean hasRate()
  {
    return rate == null ? false : rate.isValid();
  }
  public SEScalarVolumePerTime getRate()
  {
    if (rate == null)
      rate = new SEScalarVolumePerTime();
    return rate;
  }
  
  public boolean hasSubstanceCompound(){return !compound.isEmpty();}
  public void setSubstanceCompound(String name)
  {
    compound = name==null?"":name;
  }
  public String getSubstanceCompound()
  {
    return compound;
  }
  
  @Override
  public String toString()
  {
    if (rate != null || compound != null)
      return "Compound Infusion "
          + "\n\tRate: " + getRate()
          + "\n\tBag Volume: " + getBagVolume()
          + "\n\tSubstance Compound: " + getSubstanceCompound();
    else
      return "Action not specified properly";
  }
}

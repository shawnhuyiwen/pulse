/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.system.equipment.inhaler;

import com.kitware.pulse.cdm.bind.Enums.eSwitch;
import com.kitware.pulse.cdm.bind.Inhaler.InhalerData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;
import com.kitware.pulse.cdm.properties.SEScalarMass;
import com.kitware.pulse.cdm.properties.SEScalarVolume;
import com.kitware.pulse.cdm.system.equipment.SEEquipment;

public class SEInhaler extends SEEquipment
{
  protected eSwitch        state;
  protected SEScalarMass   meteredDose;
  protected SEScalar0To1   nozzleLoss;
  protected SEScalarVolume spacerVolume;
  protected String         substance;

  public SEInhaler()
  {
    clear();
  }
  
  public void clear()
  {
    state = eSwitch.Off;
    meteredDose = null;
    nozzleLoss = null;
    spacerVolume = null;
    substance = "";
  }

  @Override
  public void reset()
  {
    state = eSwitch.Off;
    if (meteredDose != null)
      meteredDose.invalidate();
    if (nozzleLoss != null)
      nozzleLoss.invalidate();
    if (spacerVolume != null)
      spacerVolume.invalidate();
    substance = "";
  }

  public void copy(SEInhaler from)
  {
    reset();

    setState(from.getState());
    meteredDose = from.getMeteredDose();
    nozzleLoss = from.getNozzleLoss();
    spacerVolume = from.getSpacerVolume();
    substance = from.substance;
  }

  public static void load(InhalerData src, SEInhaler dst)
  {
    if (src.getState()!=eSwitch.UNRECOGNIZED && src.getState()!=eSwitch.NullSwitch)
      dst.setState(src.getState());
    if (src.hasMeteredDose())
      SEScalarMass.load(src.getMeteredDose(),dst.getMeteredDose());
    if (src.hasNozzleLoss())
      SEScalar0To1.load(src.getNozzleLoss(),dst.getNozzleLoss());
    if (src.hasSpacerVolume())
      SEScalarVolume.load(src.getSpacerVolume(),dst.getSpacerVolume());
    if (src.getSubstance() != null)
      dst.setSubstance(src.getSubstance());
  }
  public static InhalerData unload(SEInhaler src)
  {
    InhalerData.Builder dst = InhalerData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEInhaler src, InhalerData.Builder dst)
  {
    dst.setState(src.state);
    if (src.hasMeteredDose())
      dst.setMeteredDose(SEScalarMass.unload(src.meteredDose));
    if (src.hasNozzleLoss())
      dst.setNozzleLoss(SEScalar0To1.unload(src.nozzleLoss));
    if (src.hasSpacerVolume())
      dst.setSpacerVolume(SEScalarVolume.unload(src.spacerVolume));
    if (src.hasSubstance())
      dst.setSubstance(src.substance);
  }

  public eSwitch getState()
  {
    return state;
  }
  public void setState(eSwitch s)
  {
  	this.state = (s==eSwitch.NullSwitch) ? eSwitch.Off : s;
  }

  public boolean hasMeteredDose()
  {
    return meteredDose == null ? false : meteredDose.isValid();
  }
  public SEScalarMass getMeteredDose()
  {
    if (meteredDose == null)
      meteredDose = new SEScalarMass();
    return meteredDose;
  }

  public boolean hasNozzleLoss()
  {
    return nozzleLoss == null ? false : nozzleLoss.isValid();
  }
  public SEScalar0To1 getNozzleLoss()
  {
    if (nozzleLoss == null)
      nozzleLoss = new SEScalar0To1();
    return nozzleLoss;
  }

  public boolean hasSpacerVolume()
  {
    return spacerVolume == null ? false : spacerVolume.isValid();
  }
  public SEScalarVolume getSpacerVolume()
  {
    if (spacerVolume == null)
      spacerVolume = new SEScalarVolume();
    return spacerVolume;
  }

  public String getSubstance()
  {
    return substance;
  }
  public void setSubstance(String s)
  {
    this.substance = s==null?"":s;
  }
  public boolean hasSubstance() 
  {
    return substance == null ? false : true;
  }
  
  @Override
  public String toString()
  {
    String str = "Inhaler:";
    str += "\n\tState: " + getState();
    str += "\n\tMetered Dose: "; str += this.hasMeteredDose()?this.getMeteredDose():"Not Supplied";
    str += "\n\tNozzle Loss: "; str += this.hasNozzleLoss()?this.getNozzleLoss():"Not Supplied";
    str += "\n\tSpacer Volume: "; str += this.hasSpacerVolume()?this.getSpacerVolume():"Not Supplied";    
    str += "\n\tSubstance: "; str += this.hasSubstance()?this.getSubstance():"Not Supplied";
    return str;
  }
}
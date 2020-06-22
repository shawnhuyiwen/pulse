/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.electrocardiogram;

import com.kitware.pulse.cdm.bind.ElectroCardioGram.ElectroCardioGramData;
import com.kitware.pulse.cdm.properties.SEScalarElectricPotential;
import com.kitware.pulse.cdm.system.SESystem;

public class SEElectroCardioGram implements SESystem
{
  protected SEScalarElectricPotential lead3ElectricPotential;
  
  public SEElectroCardioGram()
  {
    lead3ElectricPotential = null;
  }
  
  @Override
  public void reset()
  {
    if (hasLead3ElectricPotential())
      lead3ElectricPotential.invalidate();
  }
  
  public static void load(ElectroCardioGramData src, SEElectroCardioGram dst)
  {
    if (src.hasLead3ElectricPotential())
      SEScalarElectricPotential.load(src.getLead3ElectricPotential(),dst.getLead3ElectricPotential());
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
  
}

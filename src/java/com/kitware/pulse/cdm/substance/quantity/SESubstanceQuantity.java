/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.substance.quantity;

import com.kitware.pulse.cdm.bind.SubstanceQuantity.SubstanceQuantityData;
import com.kitware.pulse.cdm.substance.SESubstance;

public abstract class SESubstanceQuantity 
{
  protected SESubstance           substance;
  
  public SESubstanceQuantity(SESubstance s)
  {
    substance=s;
  }
  public void reset()
  {
    
  }
  public static void load(SubstanceQuantityData src, SESubstanceQuantity dst)
  {
    dst.reset();
  }
  protected static void unload(SESubstanceQuantity src, SubstanceQuantityData.Builder dst)
  {
    if(src.substance!=null)
      dst.setSubstance(src.getSubstance().getName());
  }
  
  public SESubstance getSubstance()
  {
    return substance;
  }
}

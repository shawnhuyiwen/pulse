/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.compartment;


import java.util.ArrayList;
import java.util.List;

import com.kitware.pulse.cdm.bind.Compartment.GasCompartmentData;
import com.kitware.pulse.cdm.bind.SubstanceQuantity.GasSubstanceQuantityData;
import com.kitware.pulse.cdm.substance.SESubstance;
import com.kitware.pulse.cdm.substance.SESubstanceManager;
import com.kitware.pulse.cdm.substance.quantity.SEGasSubstanceQuantity;

public class SEGasCompartment extends SEFluidCompartment
{
  protected List<SEGasSubstanceQuantity> substanceQuantities;
  
  public SEGasCompartment()
  {
    this.substanceQuantities=new ArrayList<>();
  }
  
  @Override
  public void clear()
  {
    super.clear();
    for (SEGasSubstanceQuantity sq : this.substanceQuantities)
      sq.clear();
  }
  
  public static void load(GasCompartmentData src, SEGasCompartment dst, SESubstanceManager subMgr)
  {
    SEFluidCompartment.load(src.getFluidCompartment(), dst);
  
    for(GasSubstanceQuantityData subQData : src.getSubstanceQuantityList())
    {
      SEGasSubstanceQuantity subQ = dst.getSubstanceQuantity(subMgr.getSubstance(subQData.getSubstanceQuantity().getSubstance()));
      SEGasSubstanceQuantity.load(subQData, subQ);
    }
  }
  public static GasCompartmentData unload(SEGasCompartment src)
  {
    GasCompartmentData.Builder dst = GasCompartmentData.newBuilder();
    unload(src,dst);
    return dst.build();    
  }
  protected static void unload(SEGasCompartment src, GasCompartmentData.Builder dst)
  {
    SEFluidCompartment.unload(src,dst.getFluidCompartmentBuilder());
    for(SEGasSubstanceQuantity subQ : src.getSubstanceQuantities())
      dst.addSubstanceQuantity(SEGasSubstanceQuantity.unload(subQ));
  }
  
  public boolean hasSubstanceQuantities()
  {
    return substanceQuantities.size() > 0 ? true : false;
  }
  public boolean hasSubstanceQuantity(SESubstance substance)
  {
    for (SEGasSubstanceQuantity csq : substanceQuantities)
    {
      if (csq.getSubstance() == substance)
        return true;
    }
    return false;
  }
  public SEGasSubstanceQuantity getSubstanceQuantity(SESubstance substance)
  {
    for (SEGasSubstanceQuantity csq : substanceQuantities)
    {
      if (substance == csq.getSubstance())
        return csq;
    }
    SEGasSubstanceQuantity csq = new SEGasSubstanceQuantity(substance);
    substanceQuantities.add(csq);
    return csq;
  }
  public List<SEGasSubstanceQuantity> getSubstanceQuantities()
  {
    return substanceQuantities;
  }
  public void removeSubstanceQuantity(SESubstance substance)
  {
    for (SEGasSubstanceQuantity csq : substanceQuantities)
    {
      if (csq.getSubstance() == substance)
      {
        substanceQuantities.remove(csq);
        return;
      }
    }
  }
  public void removeSubstanceQuantity(String substanceName)
  {
    for (SEGasSubstanceQuantity csq : substanceQuantities)
    {
      if (csq.getSubstance().getName().equals(substanceName))
      {
        substanceQuantities.remove(csq);
        return;
      }
    }
  }
}

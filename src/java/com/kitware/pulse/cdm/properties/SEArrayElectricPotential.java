/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.properties;

import com.kitware.pulse.cdm.bind.Properties.ArrayElectricPotentialData;
import com.kitware.pulse.cdm.exceptions.InvalidUnitException;
import com.kitware.pulse.cdm.properties.CommonUnits.ElectricPotentialUnit;
import com.kitware.pulse.utilities.UnitConverter;

public class SEArrayElectricPotential extends SEArray
{
  public SEArrayElectricPotential()
  {
    super();
  }
    
  public static void load(ArrayElectricPotentialData src, SEArrayElectricPotential dst)
  {
    SEArray.load(src.getArrayElectricPotential(), dst);
  }
  public static ArrayElectricPotentialData unload(SEArrayElectricPotential src)
  {
    if(!src.isValid())
      return null;
    ArrayElectricPotentialData.Builder dst = ArrayElectricPotentialData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEArrayElectricPotential src, ArrayElectricPotentialData.Builder dst)
  {
    SEArray.unload(src,dst.getArrayElectricPotentialBuilder());
  }
  
  @Override
  public boolean validUnit(String unit)
  {
    return ElectricPotentialUnit.validUnit(unit);
  }
  
  // TODO Add methods with ElectricPotentialUnit unit rather than String unit

}

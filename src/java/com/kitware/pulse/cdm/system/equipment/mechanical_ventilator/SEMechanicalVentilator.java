/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.system.equipment.mechanical_ventilator;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

import com.kitware.pulse.cdm.bind.MechanicalVentilator.MechanicalVentilatorData;
import com.kitware.pulse.cdm.properties.*;
import com.kitware.pulse.cdm.system.equipment.SEEquipment;
import com.kitware.pulse.utilities.Log;

public class SEMechanicalVentilator extends SEEquipment
{

  public SEMechanicalVentilator()
  {
  }

  @Override
  public void reset()
  {
  }

  public void copy(SEMechanicalVentilator from)
  {
    reset();
  }

  public static void load(MechanicalVentilatorData src, SEMechanicalVentilator dst)
  {
    dst.reset();
    
  }
  public static MechanicalVentilatorData unload(SEMechanicalVentilator src)
  {
    MechanicalVentilatorData.Builder dst = MechanicalVentilatorData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEMechanicalVentilator src, MechanicalVentilatorData.Builder dst)
  {
    
  }
  
  @Override
  public String toString()
  {
    String str = "Mechanical Ventilator";
    
    
    return str;
  }
}
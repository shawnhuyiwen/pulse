/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.mechanical_ventilator.actions;

import com.kitware.pulse.cdm.bind.MechanicalVentilatorActions.MechanicalVentilatorActionData;
import com.kitware.pulse.cdm.system.equipment.SEEquipmentAction;

public abstract class SEMechanicalVentilatorAction extends SEEquipmentAction
{

  private static final long serialVersionUID = -7237768261378630458L;

  public SEMechanicalVentilatorAction()
  {
    
  }
  
  public void copy(SEMechanicalVentilatorAction other)
  {
    if (this == other)
      return;
    super.copy(other);
  }
  
  @Override
  public void clear()
  {
    super.clear();
  }
  
  public static void load(MechanicalVentilatorActionData src, SEMechanicalVentilatorAction dst) 
  {
    SEEquipmentAction.load(src.getEquipmentAction(), dst);
  }
  protected static void unload(SEMechanicalVentilatorAction src, MechanicalVentilatorActionData.Builder dst)
  {
    SEEquipmentAction.unload(src, dst.getEquipmentActionBuilder());
  }
  
  @Override
  public abstract String toString();
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.anesthesia_machine.actions;

import com.kitware.pulse.cdm.bind.AnesthesiaMachineActions.AnesthesiaMachineActionData;
import com.kitware.pulse.cdm.system.equipment.SEEquipmentAction;

public abstract class SEAnesthesiaMachineAction extends SEEquipmentAction
{

  private static final long serialVersionUID = 7632437910067529682L;

  public SEAnesthesiaMachineAction()
  {
    
  }
  
  public void copy(SEAnesthesiaMachineAction other)
  {
    if (this == other)
      return;
    super.copy(other);
  }
  
  @Override
  public void reset()
  {
    super.reset();
  }
  
  public static void load(AnesthesiaMachineActionData src, SEAnesthesiaMachineAction dst) 
  {
    SEEquipmentAction.load(src.getEquipmentAction(), dst);
  }
  protected static void unload(SEAnesthesiaMachineAction src, AnesthesiaMachineActionData.Builder dst)
  {
    SEEquipmentAction.unload(src, dst.getEquipmentActionBuilder());
  }
  
  @Override
  public abstract String toString();
}

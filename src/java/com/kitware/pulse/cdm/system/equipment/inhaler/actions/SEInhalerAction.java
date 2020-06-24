/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.inhaler.actions;

import com.kitware.pulse.cdm.bind.InhalerActions.InhalerActionData;
import com.kitware.pulse.cdm.system.equipment.SEEquipmentAction;

public abstract class SEInhalerAction extends SEEquipmentAction
{

  private static final long serialVersionUID = 6841036589043042143L;

  public SEInhalerAction()
  {
    
  }
  
  public void copy(SEInhalerAction other)
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
  
  public static void load(InhalerActionData src, SEInhalerAction dst) 
  {
    SEEquipmentAction.load(src.getEquipmentAction(), dst);
  }
  protected static void unload(SEInhalerAction src, InhalerActionData.Builder dst)
  {
    SEEquipmentAction.unload(src, dst.getEquipmentActionBuilder());
  }
  
  @Override
  public abstract String toString();
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.bag_valve_mask.actions;

import com.kitware.pulse.cdm.bind.BagValveMaskActions.BagValveMaskActionData;
import com.kitware.pulse.cdm.system.equipment.SEEquipmentAction;

public abstract class SEBagValveMaskAction extends SEEquipmentAction
{

  private static final long serialVersionUID = -7237768261378630458L;

  public SEBagValveMaskAction()
  {
    
  }
  
  public void copy(SEBagValveMaskAction other)
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
  
  public static void load(BagValveMaskActionData src, SEBagValveMaskAction dst) 
  {
    SEEquipmentAction.load(src.getEquipmentAction(), dst);
  }
  protected static void unload(SEBagValveMaskAction src, BagValveMaskActionData.Builder dst)
  {
    SEEquipmentAction.unload(src, dst.getEquipmentActionBuilder());
  }
  
  @Override
  public abstract String toString();
}

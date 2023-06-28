/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.ecmo.actions;

import com.kitware.pulse.cdm.bind.ECMOActions.ECMOActionData;
import com.kitware.pulse.cdm.system.equipment.SEEquipmentAction;

public abstract class SEECMOAction extends SEEquipmentAction
{

  private static final long serialVersionUID = -7237768261378630458L;

  public SEECMOAction()
  {
    
  }
  
  public void copy(SEECMOAction other)
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
  
  public static void load(ECMOActionData src, SEECMOAction dst) 
  {
    SEEquipmentAction.load(src.getEquipmentAction(), dst);
  }
  protected static void unload(SEECMOAction src, ECMOActionData.Builder dst)
  {
    SEEquipmentAction.unload(src, dst.getEquipmentActionBuilder());
  }
  
  @Override
  public abstract String toString();
}

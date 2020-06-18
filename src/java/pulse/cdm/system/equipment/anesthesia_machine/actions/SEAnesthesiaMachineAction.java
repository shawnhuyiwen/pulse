/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.system.equipment.anesthesia_machine.actions;

import pulse.cdm.bind.AnesthesiaMachineActions.AnesthesiaMachineActionData;
import pulse.cdm.system.equipment.SEEquipmentAction;

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
  
  public abstract String toString();
}

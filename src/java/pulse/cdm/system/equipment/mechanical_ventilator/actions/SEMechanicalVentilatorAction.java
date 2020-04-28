/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.system.equipment.mechanical_ventilator.actions;

import pulse.cdm.bind.MechanicalVentilatorActions.MechanicalVentilatorActionData;
import pulse.cdm.system.equipment.SEEquipmentAction;

public abstract class SEMechanicalVentilatorAction extends SEEquipmentAction
{
  public SEMechanicalVentilatorAction()
  {
    
  }
  
  public void copy(SEMechanicalVentilatorAction other)
  {
    if (this == other)
      return;
    super.copy(other);
  }
  
  public void reset()
  {
    super.reset();
  }
  
  public static void load(MechanicalVentilatorActionData src, SEMechanicalVentilatorAction dst) 
  {
    SEEquipmentAction.load(src.getEquipmentAction(), dst);
  }
  protected static void unload(SEMechanicalVentilatorAction src, MechanicalVentilatorActionData.Builder dst)
  {
    SEEquipmentAction.unload(src, dst.getEquipmentActionBuilder());
  }
  
  public abstract String toString();
}

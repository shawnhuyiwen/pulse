/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.physiology.datamodel.system.equipment.anesthesia.actions;

import com.kitware.physiology.cdm.AnesthesiaMachineActions.AnesthesiaMachineOxygenTankPressureLossData;
import com.kitware.physiology.cdm.Enums.eSwitch;

public class SEAnesthesiaMachineOxygenTankPressureLoss extends SEAnesthesiaMachineAction
{
  protected eSwitch state;

  public SEAnesthesiaMachineOxygenTankPressureLoss()
  {
    state = eSwitch.Off;
  }

  public void reset()
  {
    super.reset();
    state = eSwitch.Off;
  }

  public boolean isValid()
  {
    return true;
  }

  public static void load(AnesthesiaMachineOxygenTankPressureLossData src, SEAnesthesiaMachineOxygenTankPressureLoss dst)
  {
    SEAnesthesiaMachineAction.load(src.getAnesthesiaMachineAction(),dst);
    if (src.getState()!=eSwitch.UNRECOGNIZED && src.getState()!=eSwitch.NullSwitch)
      dst.setState(src.getState());
  }
  public static AnesthesiaMachineOxygenTankPressureLossData unload(SEAnesthesiaMachineOxygenTankPressureLoss src)
  {
    AnesthesiaMachineOxygenTankPressureLossData.Builder dst = AnesthesiaMachineOxygenTankPressureLossData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEAnesthesiaMachineOxygenTankPressureLoss src, AnesthesiaMachineOxygenTankPressureLossData.Builder dst)
  {
    SEAnesthesiaMachineAction.unload(src, dst.getAnesthesiaMachineActionBuilder());
    dst.setState(src.state);
  }

  /*
   * State
   */
  public eSwitch getState()
  {
    return state;
  }
  public void setState(eSwitch s)
  {
  	this.state = (s==eSwitch.NullSwitch) ? eSwitch.Off : s;
  }

  public String toString()
  {
    return "Oxygen Tank Pressure Loss"
        + "\n\tState: " + getState();
  }
}

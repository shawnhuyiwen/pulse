/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.system.equipment.anesthesia_machine.actions;

import com.kitware.pulse.cdm.bind.AnesthesiaMachineActions.AnesthesiaMachineOxygenTankPressureLossData;
import com.kitware.pulse.cdm.bind.Enums.eSwitch;

public class SEAnesthesiaMachineOxygenTankPressureLoss extends SEAnesthesiaMachineAction
{

  private static final long serialVersionUID = 105202652228873931L;
  protected eSwitch state;

  public SEAnesthesiaMachineOxygenTankPressureLoss()
  {
    state = eSwitch.Off;
  }

  @Override
  public void reset()
  {
    super.reset();
    state = eSwitch.Off;
  }

  @Override
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

  @Override
  public String toString()
  {
    return "Oxygen Tank Pressure Loss"
        + "\n\tState: " + getState();
  }
}

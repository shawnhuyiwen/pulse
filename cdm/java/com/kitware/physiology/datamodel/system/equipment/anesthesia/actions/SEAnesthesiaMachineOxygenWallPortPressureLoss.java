/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.physiology.datamodel.system.equipment.anesthesia.actions;

import com.kitware.physiology.cdm.AnesthesiaMachineActions.AnesthesiaMachineOxygenWallPortPressureLossData;
import com.kitware.physiology.cdm.Enums.eSwitch;

public class SEAnesthesiaMachineOxygenWallPortPressureLoss extends SEAnesthesiaMachineAction
{
  protected eSwitch state;
  
  public SEAnesthesiaMachineOxygenWallPortPressureLoss()
  {
    state = eSwitch.On;
  }
  
  public void reset()
  {
    super.reset();
    state = eSwitch.On;
  }
  
  public boolean isValid()
  {
    return true;
  }
  
  public static void load(AnesthesiaMachineOxygenWallPortPressureLossData src, SEAnesthesiaMachineOxygenWallPortPressureLoss dst)
  {
    SEAnesthesiaMachineAction.load(src.getAnesthesiaMachineAction(),dst);
    if (src.getState()!=eSwitch.UNRECOGNIZED && src.getState()!=eSwitch.NullSwitch)
      dst.setState(src.getState());
  }
  public static AnesthesiaMachineOxygenWallPortPressureLossData unload(SEAnesthesiaMachineOxygenWallPortPressureLoss src)
  {
    AnesthesiaMachineOxygenWallPortPressureLossData.Builder dst = AnesthesiaMachineOxygenWallPortPressureLossData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEAnesthesiaMachineOxygenWallPortPressureLoss src, AnesthesiaMachineOxygenWallPortPressureLossData.Builder dst)
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
    return "Oxygen Wall Port Pressure Loss"
        + "\n\tState: " + getState();
  }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.system.equipment.anesthesia_machine.actions;

import pulse.cdm.bind.AnesthesiaMachineActions.AnesthesiaMachineOxygenWallPortPressureLossData;
import pulse.cdm.bind.Enums.eSwitch;

public class SEAnesthesiaMachineOxygenWallPortPressureLoss extends SEAnesthesiaMachineAction
{

  private static final long serialVersionUID = -7922046988182673148L;
  protected eSwitch state;
  
  public SEAnesthesiaMachineOxygenWallPortPressureLoss()
  {
    state = eSwitch.On;
  }
  
  @Override
  public void reset()
  {
    super.reset();
    state = eSwitch.On;
  }
  
  @Override
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
  
  @Override
  public String toString()
  {
    return "Oxygen Wall Port Pressure Loss"
        + "\n\tState: " + getState();
  }
}

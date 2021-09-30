/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.mechanical_ventilator.actions;

import com.kitware.pulse.cdm.bind.Actions.eAppliedRespiratoryCycle;
import com.kitware.pulse.cdm.bind.Enums.eSwitch;
import com.kitware.pulse.cdm.bind.MechanicalVentilatorActions.MechanicalVentilatorHoldData;

public class SEMechanicalVentilatorHold extends SEMechanicalVentilatorAction
{
  protected eSwitch                  state;
  protected eAppliedRespiratoryCycle appliedRespiratoryCycle;

  public SEMechanicalVentilatorHold()
  {
    clear();
  }

  public SEMechanicalVentilatorHold(SEMechanicalVentilatorHold other)
  {
    copy(other);
  }

  public void copy(SEMechanicalVentilatorHold other)
  {
    super.copy(other);
    state = other.state;
    appliedRespiratoryCycle = other.appliedRespiratoryCycle;
  }

  public void clear()
  {
    appliedRespiratoryCycle = eAppliedRespiratoryCycle.Expiration;
    state = eSwitch.Off;
  }
  
  public static void load(MechanicalVentilatorHoldData src, SEMechanicalVentilatorHold dst)
  {
    dst.clear();
    SEMechanicalVentilatorAction.load(src.getMechanicalVentilatorAction(),dst);
    dst.setState(src.getState());
    dst.setAppliedRespiratoryCycle(src.getAppliedRespiratoryCycle());
  }
  public static MechanicalVentilatorHoldData unload(SEMechanicalVentilatorHold src)
  {
    MechanicalVentilatorHoldData.Builder dst = MechanicalVentilatorHoldData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEMechanicalVentilatorHold src, MechanicalVentilatorHoldData.Builder dst)
  {
    SEMechanicalVentilatorAction.unload(src, dst.getMechanicalVentilatorActionBuilder());
    dst.setState(src.getState());
    dst.setAppliedRespiratoryCycle(src.getAppliedRespiratoryCycle());
  }

  public boolean isValid()
  {
    return true;
  }

  public eSwitch getState()
  {
    return state;
  }

  public void setState(eSwitch s)
  {
    state = s;
  }

  public eAppliedRespiratoryCycle getAppliedRespiratoryCycle()
  {
    return appliedRespiratoryCycle;
  }

  public void setAppliedRespiratoryCycle(eAppliedRespiratoryCycle c)
  {
    appliedRespiratoryCycle = c;
  }

  public String toString()
  {
    String str = "Mechanical Ventilator Hold";
    str += "\n\tState: " + this.state;
    str += "\n\tApplied Respiratory Cycle: " + this.appliedRespiratoryCycle;

    return str;
  }
}

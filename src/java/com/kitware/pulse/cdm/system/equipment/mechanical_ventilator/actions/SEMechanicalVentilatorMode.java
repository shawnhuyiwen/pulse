/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.mechanical_ventilator.actions;

import com.kitware.pulse.cdm.bind.Enums.eSwitch;
import com.kitware.pulse.cdm.bind.MechanicalVentilatorActions.MechanicalVentilatorModeData;
import com.kitware.pulse.cdm.properties.SEScalarPressure;
import com.kitware.pulse.cdm.properties.SEScalarVolumePerTime;

public class SEMechanicalVentilatorMode extends SEMechanicalVentilatorAction
{
  protected eSwitch connection;

  public SEMechanicalVentilatorMode()
  {
    clear();
  }

  public SEMechanicalVentilatorMode(SEMechanicalVentilatorMode other)
  {
    copy(other);
  }

  public void copy(SEMechanicalVentilatorMode other)
  {
    super.copy(other);
    connection = other.connection;
  }

  public void clear()
  {
    connection = eSwitch.Off;
  }
  
  protected static void load(MechanicalVentilatorModeData src, SEMechanicalVentilatorMode dst)
  {
    dst.reset();
    SEMechanicalVentilatorAction.load(src.getMechanicalVentilatorAction(),dst);
    dst.setConnection(src.getConnection());
  }
  protected static void unload(SEMechanicalVentilatorMode src, MechanicalVentilatorModeData.Builder dst)
  {
    SEMechanicalVentilatorAction.unload(src, dst.getMechanicalVentilatorActionBuilder());
    dst.setConnection(src.getConnection());
  }

  public boolean isValid()
  {
    return true;
  }

  public eSwitch getConnection()
  {
    return connection;
  }

  public void setConnection(eSwitch s)
  {
    connection = s;
  }

  public String toString()
  {
    String str = "Mechanical Ventilator Mode";
    str += "\n\tConnection: " + this.connection;

    return str;
  }
}

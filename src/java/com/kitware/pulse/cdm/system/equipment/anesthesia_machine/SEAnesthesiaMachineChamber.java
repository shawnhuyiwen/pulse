/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.system.equipment.anesthesia_machine;

import com.kitware.pulse.cdm.bind.AnesthesiaMachine.AnesthesiaMachineChamberData;
import com.kitware.pulse.cdm.bind.Enums.eSwitch;
import com.kitware.pulse.cdm.properties.SEScalar0To1;

public class SEAnesthesiaMachineChamber 
{
  protected eSwitch state;
  protected String substance;
  protected SEScalar0To1 substanceFraction;

  public SEAnesthesiaMachineChamber()
  {
    this.state = eSwitch.Off;
    this.substance = "";
    this.substanceFraction = null;
  }

  public void clear()
  {
    state = eSwitch.Off;
    substance = "";
    if (substanceFraction != null)
      substanceFraction.invalidate();
  }

  public void copy(SEAnesthesiaMachineChamber from)
  {
    clear();
    this.state=from.state;
    this.substance=from.substance;
    if(from.hasSubstanceFraction())
      this.getSubstanceFraction().set(from.substanceFraction);
  }

  public static void load( AnesthesiaMachineChamberData src, SEAnesthesiaMachineChamber dst)
  {
    dst.clear();
    if (src.getState() != eSwitch.UNRECOGNIZED && src.getState()!=eSwitch.NullSwitch)
      dst.setState(src.getState());
    if (src.getSubstance()!=null)
      dst.setSubstance(src.getSubstance());
    if (src.hasSubstanceFraction())
      SEScalar0To1.load(src.getSubstanceFraction(),dst.getSubstanceFraction());
  }
  public static AnesthesiaMachineChamberData unload(SEAnesthesiaMachineChamber src)
  {
    AnesthesiaMachineChamberData.Builder dst =  AnesthesiaMachineChamberData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEAnesthesiaMachineChamber src, AnesthesiaMachineChamberData.Builder dst)
  {
    if(src.hasSubstance())
      dst.setSubstance(src.substance);
    dst.setState(src.state);
    if (src.hasSubstanceFraction())
      dst.setSubstanceFraction(SEScalar0To1.unload(src.substanceFraction));
  }

  public eSwitch getState()
  {
    return state;
  }
  public void setState(eSwitch s)
  {
  	this.state = (s==eSwitch.NullSwitch) ? eSwitch.Off : s;
  }

  public boolean hasSubstanceFraction()
  {
    return substanceFraction == null ? false : substanceFraction.isValid();
  }
  public SEScalar0To1 getSubstanceFraction()
  {
    if (substanceFraction == null)
      substanceFraction = new SEScalar0To1();
    return substanceFraction;
  }

  public boolean hasSubstance()
  {
    return this.substance!=null;
  }
  public String getSubstance()
  {
    return substance;
  }
  public void setSubstance(String substance)
  {
    this.substance = substance==null?"":substance;
  }

  @Override
  public String toString()
  {
      return "Anesthesia Machine Chamber"
      + "\n\tState: " + getState()
      + "\n\tSubstance Fraction: " + getSubstanceFraction()
      + "\n\tSubstance: " + (hasSubstance()?getSubstance():"NotProvided");
  }
}

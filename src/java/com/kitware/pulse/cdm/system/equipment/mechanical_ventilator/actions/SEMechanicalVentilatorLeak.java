/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.mechanical_ventilator.actions;

import com.kitware.pulse.cdm.bind.MechanicalVentilatorActions.MechanicalVentilatorLeakData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;

public class SEMechanicalVentilatorLeak extends SEMechanicalVentilatorAction
{
  protected SEScalar0To1 severity;

  public SEMechanicalVentilatorLeak()
  {
    severity = null;
  }

  public SEMechanicalVentilatorLeak(SEMechanicalVentilatorLeak other)
  {
    copy(other);
  }

  public void copy(SEMechanicalVentilatorLeak other)
  {
    super.copy(other);
    if (other.hasSeverity())
      getSeverity().set(other.getSeverity());
  }

  public void clear()
  {
    if (severity != null)
      severity.invalidate();
  }
  
  public static void load(MechanicalVentilatorLeakData src, SEMechanicalVentilatorLeak dst)
  {
    dst.reset();
    SEMechanicalVentilatorAction.load(src.getMechanicalVentilatorAction(),dst);
    if (src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(), dst.getSeverity());
  }
  public static MechanicalVentilatorLeakData unload(SEMechanicalVentilatorLeak src)
  {
    MechanicalVentilatorLeakData.Builder dst = MechanicalVentilatorLeakData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEMechanicalVentilatorLeak src, MechanicalVentilatorLeakData.Builder dst)
  {
    SEMechanicalVentilatorAction.unload(src, dst.getMechanicalVentilatorActionBuilder());
    if(src.hasSeverity())
      dst.setSeverity(SEScalar0To1.unload(src.getSeverity()));
  }

  public boolean isValid()
  {
    return hasSeverity();
  }

  public boolean hasSeverity()
  {
    return severity == null ? false : severity.isValid();
  }

  public SEScalar0To1 getSeverity()
  {
    if (severity == null)
      severity = new SEScalar0To1();
    return severity;
  }

  public String toString()
  {
    String str = "Mechanical Ventilator Leak";
    str += "\n\tSeverity: " + (hasSeverity() ? getSeverity().toString() : "Not Provided");

    return str;
  }
}

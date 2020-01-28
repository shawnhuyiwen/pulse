/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.system.equipment.anesthesia.actions;

import pulse.cdm.bind.AnesthesiaMachineActions.AnesthesiaMachineMaskLeakData;

import pulse.cdm.properties.SEScalar0To1;

public class SEAnesthesiaMachineMaskLeak extends SEAnesthesiaMachineAction
{
  protected SEScalar0To1 severity;
  
  public SEAnesthesiaMachineMaskLeak()
  {
    severity = null;
  }
  
  public void reset()
  {
    super.reset();
    if (severity != null)
      severity.invalidate();
  }
  
  public boolean isValid()
  {
    return hasSeverity();
  }
  
  public static void load(AnesthesiaMachineMaskLeakData src, SEAnesthesiaMachineMaskLeak dst)
  {
    SEAnesthesiaMachineAction.load(src.getAnesthesiaMachineAction(),dst);
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  public static AnesthesiaMachineMaskLeakData unload(SEAnesthesiaMachineMaskLeak src)
  {
    AnesthesiaMachineMaskLeakData.Builder dst = AnesthesiaMachineMaskLeakData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEAnesthesiaMachineMaskLeak src, AnesthesiaMachineMaskLeakData.Builder dst)
  {
    SEAnesthesiaMachineAction.unload(src, dst.getAnesthesiaMachineActionBuilder());
    if (src.hasSeverity())
      dst.setSeverity(SEScalar0To1.unload(src.severity));
  }
  
  /*
   * Severity
   */
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
    if (severity != null)
      return "Mask Leak"
          + "\n\tSeverity: " + getSeverity();
    else
      return "Action not specified properly";
  }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.system.equipment.anesthesia_machine.actions;

import pulse.cdm.bind.AnesthesiaMachineActions.AnesthesiaMachineSodaLimeFailureData;
import pulse.cdm.properties.SEScalar0To1;

public class SEAnesthesiaMachineSodaLimeFailure extends SEAnesthesiaMachineAction
{

  private static final long serialVersionUID = -4326536489444380821L;
  protected SEScalar0To1 severity;
  
  public SEAnesthesiaMachineSodaLimeFailure()
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
  
  public static void load(AnesthesiaMachineSodaLimeFailureData src, SEAnesthesiaMachineSodaLimeFailure dst)
  {
    SEAnesthesiaMachineAction.load(src.getAnesthesiaMachineAction(),dst);
    if(src.hasSeverity())
      SEScalar0To1.load(src.getSeverity(),dst.getSeverity());
  }
  public static AnesthesiaMachineSodaLimeFailureData unload(SEAnesthesiaMachineSodaLimeFailure src)
  {
    AnesthesiaMachineSodaLimeFailureData.Builder dst = AnesthesiaMachineSodaLimeFailureData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEAnesthesiaMachineSodaLimeFailure src, AnesthesiaMachineSodaLimeFailureData.Builder dst)
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
      return "Soda Lime Failure"
          + "\n\tSeverity: " + getSeverity();
    else
      return "Action not specified properly";
  }
}

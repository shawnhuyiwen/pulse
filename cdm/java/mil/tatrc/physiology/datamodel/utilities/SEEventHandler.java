/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package mil.tatrc.physiology.datamodel.utilities;

import com.kitware.physiology.cdm.AnesthesiaMachineEnums.eAnesthesiaMachine;
import com.kitware.physiology.cdm.PatientEnums.ePatient;

import mil.tatrc.physiology.datamodel.properties.SEScalarTime;

public interface SEEventHandler
{
  public void handlePatientEvent(ePatient.Event type, boolean active, SEScalarTime time);
  public void handleAnesthesiaMachineEvent(eAnesthesiaMachine.Event type, boolean active, SEScalarTime time);
}

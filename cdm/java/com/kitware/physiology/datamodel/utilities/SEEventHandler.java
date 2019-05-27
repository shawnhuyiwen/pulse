/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.physiology.datamodel.utilities;

import com.kitware.physiology.cdm.AnesthesiaMachine.eAnesthesiaMachine;
import com.kitware.physiology.cdm.Patient.ePatient;
import com.kitware.physiology.datamodel.properties.SEScalarTime;

public interface SEEventHandler
{
  public void handlePatientEvent(ePatient.Event type, boolean active, SEScalarTime time);
  public void handleAnesthesiaMachineEvent(eAnesthesiaMachine.Event type, boolean active, SEScalarTime time);
}

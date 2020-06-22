/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.engine;

import com.kitware.pulse.cdm.bind.Events.eEvent;
import com.kitware.pulse.cdm.properties.SEScalarTime;

public class SEActiveEvent
{
  public eEvent type = eEvent.UNRECOGNIZED;
  public SEScalarTime duration = new SEScalarTime();
}
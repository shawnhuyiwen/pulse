/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.physiology.datamodel.engine;

import com.kitware.physiology.cdm.Events.eEvent;
import com.kitware.physiology.datamodel.properties.SEScalarTime;

public interface SEEventHandler
{
  public void handleEvent(eEvent type, boolean active, SEScalarTime time);
}

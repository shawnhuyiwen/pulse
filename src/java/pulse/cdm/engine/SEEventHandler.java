/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.cdm.engine;

import pulse.cdm.bind.Events.eEvent;
import pulse.cdm.properties.SEScalarTime;

public interface SEEventHandler
{
  public void handleEvent(eEvent type, boolean active, SEScalarTime time);
}

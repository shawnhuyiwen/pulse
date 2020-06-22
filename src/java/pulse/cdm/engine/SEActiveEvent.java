/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.cdm.engine;

import pulse.cdm.bind.Events.eEvent;
import pulse.cdm.properties.SEScalarTime;

public class SEActiveEvent
{
  public eEvent type = eEvent.UNRECOGNIZED;
  public SEScalarTime duration = new SEScalarTime();
}
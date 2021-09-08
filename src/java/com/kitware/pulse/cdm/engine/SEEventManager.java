/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.engine;

import java.util.HashMap;
import java.util.Map;

import com.kitware.pulse.cdm.bind.Events.eEvent;
import com.kitware.pulse.cdm.properties.CommonUnits.TimeUnit;
import com.kitware.pulse.cdm.properties.SEScalarTime;

public class SEEventManager
{
  SEEventHandler handler;
  protected Map<eEvent,Boolean> states = new HashMap<>();
  protected Map<eEvent,Double> durations = new HashMap<>();
  
  public SEEventManager()
  {
  
  }
  
  public void clear()
  {
    handler = null;
    states.clear();
  }
  
  public void setEvent(eEvent e, boolean active, SEScalarTime time)
  {
    this.states.put(e, active);
    this.durations.put(e, time.getValue(TimeUnit.s));
    
    if (this.handler != null)
      this.handler.handleEvent(e,active,time);
  }
  
  public boolean isEventActive(eEvent e)
  {
    if(!this.states.containsKey(e))
      return false;
    return this.states.get(e);
  }
  
  public double getDuration_s(eEvent e, SEScalarTime currentTime)
  {
    if(isEventActive(e))
      return 0;
    return currentTime.getValue(TimeUnit.s) - this.durations.get(e);
  }
  
  public void forwardEvents(SEEventHandler h)
  {
    this.handler = h;
  }
  
  public SEEventHandler getHandler()
  {
    return this.handler;
  }
}

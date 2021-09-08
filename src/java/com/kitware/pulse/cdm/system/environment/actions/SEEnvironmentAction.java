/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.system.environment.actions;

import com.kitware.pulse.cdm.actions.SEAction;
import com.kitware.pulse.cdm.bind.EnvironmentActions.AnyEnvironmentActionData;
import com.kitware.pulse.cdm.bind.EnvironmentActions.EnvironmentActionData;
import com.kitware.pulse.utilities.Log;

public abstract class SEEnvironmentAction extends SEAction
{

  private static final long serialVersionUID = -3161085494098014618L;

  public SEEnvironmentAction()
  {
    
  }
  
  public void copy(SEEnvironmentAction other)
  {
    if (this == other)
      return;
    super.copy(other);
  }
  
  @Override
  public void clear()
  {
    super.clear();
  }
  
  public static void load(EnvironmentActionData src, SEEnvironmentAction dst) 
  {
    SEAction.load(src.getAction(), dst);
  }
  protected static void unload(SEEnvironmentAction src, EnvironmentActionData.Builder dst)
  {
    SEAction.unload(src, dst.getActionBuilder());
  }
  
  public static SEEnvironmentAction ANY2CDM(AnyEnvironmentActionData c) 
  {
    switch(c.getActionCase())
    {
    case CHANGEENVIRONMENTALCONDITIONS:
    {
      SEChangeEnvironmentalConditions dst = new SEChangeEnvironmentalConditions();
      SEChangeEnvironmentalConditions.load(c.getChangeEnvironmentalConditions(), dst);
      return dst;
    }
    case THERMALAPPLICATION:
    {
      SEThermalApplication dst = new SEThermalApplication();
      SEThermalApplication.load(c.getThermalApplication(), dst);
      return dst;
    }
    case ACTION_NOT_SET:
      Log.warn("AnyEnvironmentActionData was empty...was that intended?");
      return null;
    }
    Log.error("Unsupported Environment Action type "+c.getActionCase());
    return null;
  }
  public static AnyEnvironmentActionData CDM2ANY(SEEnvironmentAction c)
  {
    AnyEnvironmentActionData.Builder dst = AnyEnvironmentActionData.newBuilder();
    
    if(c instanceof SEChangeEnvironmentalConditions)
    {
      dst.setChangeEnvironmentalConditions(SEChangeEnvironmentalConditions.unload((SEChangeEnvironmentalConditions)c));
      return dst.build();
    }
    if(c instanceof SEThermalApplication)
    {
      dst.setThermalApplication(SEThermalApplication.unload((SEThermalApplication)c));
      return dst.build();
    }
    Log.error("Unsupported Environment Action type "+c);
    return dst.build();
  }
  
  @Override
  public abstract String toString();
}

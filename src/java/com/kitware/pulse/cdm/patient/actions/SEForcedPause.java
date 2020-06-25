/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.ForcedPauseData;
import com.kitware.pulse.cdm.properties.SEScalarTime;

public class SEForcedPause extends SEConsciousRespirationCommand
{
  protected SEScalarTime period;
  
  public SEForcedPause()
  {
    period = null;
  }
  
  public void copy(SEForcedPause other)
  {
    if(this==other)
      return;

    period = other.period;
  }
  
  @Override
  public SEConsciousRespirationCommand getCopy()
  {
    SEConsciousRespirationCommand command = new SEForcedPause();
    command.copy(this);
    
    return command;
  }
  
  @Override
  public void reset()
  {
    super.reset();

    if (period != null)
      period.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return hasPeriod();
  }
  
  public static void load(ForcedPauseData src, SEForcedPause dst)
  {
    //SEConsciousRespirationCommand.load(src.getConsciousRespirationCommand(), dst);
    if(src.hasPeriod())
      SEScalarTime.load(src.getPeriod(),dst.getPeriod());    
  }
  
  public static ForcedPauseData unload(SEForcedPause src)
  {
    ForcedPauseData.Builder dst = ForcedPauseData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEForcedPause src, ForcedPauseData.Builder dst)
  {
    //SEConsciousRespirationCommand.unload(src,dst.getConsciousRespirationCommand());
    if (src.hasPeriod())
      dst.setPeriod(SEScalarTime.unload(src.period));
  }
  
  public boolean hasPeriod()
  {
    return period != null;
  }
  public SEScalarTime getPeriod()
  {
    if (period == null)
      period = new SEScalarTime();
    return period;
  }
  
  @Override
  public String toString()
  {
    if (period != null)
    {
      return "Breath Hold"
          + "\n\tPeriod: " + getPeriod();
    }
    else
      return "Action not specified properly";
  }
}

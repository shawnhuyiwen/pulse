/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.Enums.eSwitch;
import com.kitware.pulse.cdm.bind.PatientActions.CardiacArrestData;

public class SECardiacArrest extends SEPatientAction
{
  private static final long serialVersionUID = 875699095552953525L;
  
  protected eSwitch state;
  
  public SECardiacArrest()
  {
    state = eSwitch.Off;
  }
  
  public void copy(SECardiacArrest other)
  {
    if(this==other)
      return;
    super.copy(other);
    state = other.state;
  }
  
  @Override
  public void clear()
  {
    super.clear();
    state = eSwitch.Off;
  }
  
  @Override
  public boolean isValid()
  {
    return true;
  }
  
  public static void load(CardiacArrestData src, SECardiacArrest dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.getState()!=eSwitch.UNRECOGNIZED && src.getState()!=eSwitch.NullSwitch)
    	dst.state = src.getState();
  }
  
  public static CardiacArrestData unload(SECardiacArrest src)
  {
    CardiacArrestData.Builder dst = CardiacArrestData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SECardiacArrest src, CardiacArrestData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    dst.setState(src.state);
  }
  
  public eSwitch getState()
  {
    return state;
  }
  public void setState(eSwitch s)
  {
  	this.state = (s==eSwitch.NullSwitch) ? eSwitch.Off : s;
  }
  public boolean hasState()
  {
    return this.state==null || this.state==eSwitch.NullSwitch ? false : true;
  }
  
  @Override
  public String toString()
  {
    return "Cardiac Arrest"
        + "\n\tState: " + getState();
  }
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.Enums.eSide;
import com.kitware.pulse.cdm.bind.Enums.eSwitch;
import com.kitware.pulse.cdm.bind.PatientActions.ChestOcclusiveDressingData;

public class SEChestOcclusiveDressing extends SEPatientAction
{
  private static final long serialVersionUID = -3935497566365734767L;
  
  protected eSide side;
  protected eSwitch state;
  
  public SEChestOcclusiveDressing()
  {
    side = null;
    state = eSwitch.Off;
  }
  
  public void copy(SEChestOcclusiveDressing other)
  {
    if(this==other)
      return;
    super.copy(other);
    this.state = other.state;
    this.side = other.side;
  }
  
  @Override
  public void reset()
  {
    super.reset();
    state = eSwitch.Off;
    side = null;
  }
  
  @Override
  public boolean isValid()
  {
    return hasSide();
  }
  
  public static void load(ChestOcclusiveDressingData src, SEChestOcclusiveDressing dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.getState()!=eSwitch.UNRECOGNIZED && src.getState()!=eSwitch.NullSwitch)
    	dst.state = src.getState();
    if(src.getSide()!=eSide.UNRECOGNIZED)
    	dst.side = src.getSide();
  }
  
  public static ChestOcclusiveDressingData unload(SEChestOcclusiveDressing src)
  {
    ChestOcclusiveDressingData.Builder dst = ChestOcclusiveDressingData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEChestOcclusiveDressing src, ChestOcclusiveDressingData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    dst.setState(src.state);
    if (src.hasSide())
      dst.setSide(src.side);
  }
  
  public eSide getSide()
  {
    return side;
  }
  public void setSide(eSide leftOrRight)
  {
    side = leftOrRight;
  }
  public boolean hasSide()
  {
    return side == null ? false : side!=eSide.NullSide;
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
    return state == null ? false : state!=eSwitch.NullSwitch;
  }
  
  @Override
  public String toString()
  {
    if (side != null)
      return "Chest Occlusive Dressing" 
          + "\n\tState: " + getState()
          + "\n\tSide: " + getSide();
    else
      return "Action not properly specified";
  }
}

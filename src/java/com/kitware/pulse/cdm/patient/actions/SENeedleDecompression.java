/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.Enums.eSide;
import com.kitware.pulse.cdm.bind.Enums.eSwitch;
import com.kitware.pulse.cdm.bind.PatientActions.NeedleDecompressionData;

public class SENeedleDecompression extends SEPatientAction
{

  private static final long serialVersionUID = 67814696179604859L;
  protected eSwitch state;
  protected eSide side;
  
  public SENeedleDecompression()
  {
    state = eSwitch.Off;
    side = null;
  }
  
  public void copy(SENeedleDecompression other)
  {
    if(this==other)
      return;
    super.copy(other);
    state = other.state;
    side = other.side;
  }
  
  @Override
  public void clear()
  {
    super.clear();
    state = eSwitch.Off;
    side = null;
  }
  
  @Override
  public boolean isValid()
  {
    return hasSide();
  }
  
  public static void load(NeedleDecompressionData src, SENeedleDecompression dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.getState()!=eSwitch.UNRECOGNIZED && src.getState()!=eSwitch.NullSwitch)
    	dst.state = src.getState();
    if(src.getSide()!=eSide.UNRECOGNIZED)
    	dst.side = src.getSide();
  }
  
  public static NeedleDecompressionData unload(SENeedleDecompression src)
  {
    NeedleDecompressionData.Builder dst = NeedleDecompressionData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SENeedleDecompression src, NeedleDecompressionData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    dst.setState(src.state);
    if (src.hasSide())
      dst.setSide(src.side);
  }
  
  public eSwitch getState()
  {
    return state;
  }
  public void setState(eSwitch s)
  {
  	this.state = (s==eSwitch.NullSwitch) ? eSwitch.Off : s;
  }
  
  public eSide getSide()
  {
    return side;
  }
  
  public void setSide(eSide rightOrLeft)
  {
    side = rightOrLeft;
  }
  
  public boolean hasSide()
  {
    return side == null ? false : true;
  }
  
  @Override
  public String toString()
  {
    return "Needle Decompression"
        + "\n\tState: " + getState()
        + "\n\tSide: " + getSide();
  }
}

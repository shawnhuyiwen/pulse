/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.ArrhythmiaData;
import com.kitware.pulse.cdm.bind.Physiology.eHeartRhythm;

public class SEArrhythmia extends SEPatientAction
{
  private static final long serialVersionUID = 875699095552953525L;
  
  protected eHeartRhythm rhythm;
  
  public SEArrhythmia()
  {
    rhythm = eHeartRhythm.NormalSinus;
  }
  
  public void copy(SEArrhythmia other)
  {
    if(this==other)
      return;
    super.copy(other);
    rhythm = other.rhythm;
  }
  
  @Override
  public void clear()
  {
    super.clear();
    rhythm = eHeartRhythm.NormalSinus;
  }
  
  @Override
  public boolean isValid()
  {
    return true;
  }
  
  public static void load(ArrhythmiaData src, SEArrhythmia dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.getRhythm()!=eHeartRhythm.UNRECOGNIZED)
    	dst.rhythm = src.getRhythm();
  }
  
  public static ArrhythmiaData unload(SEArrhythmia src)
  {
    ArrhythmiaData.Builder dst = ArrhythmiaData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEArrhythmia src, ArrhythmiaData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    dst.setRhythm(src.rhythm);
  }
  
  public eHeartRhythm getRhythm()
  {
    return rhythm;
  }
  public void setRhythm(eHeartRhythm t)
  {
  	this.rhythm = t;
  }
  public boolean hasRhythm()
  {
    return rhythm != null;
  }
  
  @Override
  public String toString()
  {
    return "Arrhythmia : "
        + "\n\tRhythm: " + getRhythm();
  }
}

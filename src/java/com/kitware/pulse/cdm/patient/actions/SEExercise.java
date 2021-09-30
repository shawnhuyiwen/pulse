/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.ExerciseData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;

public class SEExercise extends SEPatientAction
{

  private static final long serialVersionUID = 7493462620069995893L;
  protected SEScalar0To1 intensity;
  
  public SEExercise()
  {
    intensity=null;
  }
  
  public void copy(SEExercise other)
  {
    if(this==other)
      return;
    super.copy(other);
    if (other.intensity != null)
      getIntensity().set(other.getIntensity());
    else if (intensity != null)
      intensity.invalidate();    
  }
  
  @Override
  public void clear()
  {
    super.clear();
    if (intensity != null)
      intensity.invalidate();
  }
  
  @Override
  public boolean isValid()
  {
    return hasIntensity();
  }
  
  public static void load(ExerciseData src, SEExercise dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    if(src.hasIntensity())
      SEScalar0To1.load(src.getIntensity(),dst.getIntensity());
  }
  
  public static ExerciseData unload(SEExercise src) 
  {
    ExerciseData.Builder dst = ExerciseData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEExercise src, ExerciseData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if (src.hasIntensity())
      dst.setIntensity(SEScalar0To1.unload(src.intensity));
  }
  
  public boolean hasIntensity()
  {
    return intensity == null ? false : intensity.isValid();
  }
  public SEScalar0To1 getIntensity()
  {
    if (intensity == null)
      intensity = new SEScalar0To1();
    return intensity;
  }
  
  @Override
  public String toString() 
  {
    if (hasIntensity())
      return "Exercise" 
          + "\n\tIntensity: " + getIntensity();     
    else
      return "Action not properly specified";
  }
}

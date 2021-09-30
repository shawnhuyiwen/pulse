/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.patient.nutrition;

import com.kitware.pulse.cdm.bind.PatientNutrition.MealData;
import com.kitware.pulse.cdm.properties.SEScalarTime;

public class SEMeal extends SENutrition
{
  protected SEScalarTime  elapsedTime;
  
  public SEMeal()
  {    
    super();
    this.elapsedTime=null;
  }
  
  @Override
  public void clear()
  {
    super.clear();
    if (elapsedTime != null)
      elapsedTime.invalidate();  
  }
  
  public void copy(SEMeal from)
  {
    super.copy(from);
    if(from.hasElapsedTime())
      this.getElapsedTime().set(from.getElapsedTime());
  }
  
  public static void load(MealData src, SEMeal dst)
  {
    dst.clear();
    if(src.hasNutrition())
      SENutrition.load(src.getNutrition(),dst);
    if (src.hasElapsedTime())
      SEScalarTime.load(src.getElapsedTime(),dst.getElapsedTime());
  }
  
  public static MealData unload(SEMeal src)
  {
    MealData.Builder dst = MealData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEMeal src, MealData.Builder dst)
  {
    SENutrition.unload(src,dst.getNutritionBuilder());
    if (src.hasElapsedTime())
      dst.setElapsedTime(SEScalarTime.unload(src.elapsedTime));
  }    
  
  public SEScalarTime getElapsedTime()
  {
    if (elapsedTime == null)
      elapsedTime = new SEScalarTime();
    return elapsedTime;
  }
  public boolean hasElapsedTime()
  {
    return elapsedTime == null ? false : elapsedTime.isValid();
  }
}

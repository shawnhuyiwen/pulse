/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.conditions;

import com.kitware.pulse.cdm.bind.PatientConditions.ConsumeMealData;
import com.kitware.pulse.cdm.patient.nutrition.SEMeal;

public class SEConsumeMeal extends SEPatientCondition
{

  private static final long serialVersionUID = -44596470205969619L;
  protected SEMeal       meal=null;
  protected String       mealFile="";
  
  public SEConsumeMeal()
  {
   
  }
  
  public SEConsumeMeal(SEConsumeMeal other)
  {
    this();
    copy(other);
  }
  
  public void copy(SEConsumeMeal other)
  {
    if(this==other)
      return;
    super.copy(other);
    if(other.meal != null)
      this.getMeal().copy(other.meal);
    this.mealFile=other.mealFile;
  }
  
  @Override
  public void reset()
  {
    super.reset();
    if(this.meal != null)
      this.meal.reset();
    this.mealFile="";
  }
  
  public static void load(ConsumeMealData src, SEConsumeMeal dst)
  {
    SEPatientCondition.load(src.getPatientCondition(), dst);  
    switch(src.getOptionCase())
    {
    case MEAL:
      SEMeal.load(src.getMeal(), dst.getMeal());
      break;
    case MEALFILE:
      dst.setMealFile(src.getMealFile());
    default: // do nothing
    }
  }
  
  public static ConsumeMealData unload(SEConsumeMeal src)
  {
    ConsumeMealData.Builder dst = ConsumeMealData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEConsumeMeal src, ConsumeMealData.Builder dst)
  {
    SEPatientCondition.unload(src, dst.getPatientConditionBuilder()); 
    if(src.hasMealFile())
      dst.setMealFile(src.mealFile);
    else if(src.hasMeal())
      dst.setMeal(SEMeal.unload(src.meal));
  }
  
  public boolean hasMeal()
  {
    return this.meal!=null;
  }
  public SEMeal getMeal()
  {
    if(this.meal == null)
      this.meal=new SEMeal();
    return this.meal;
  }
  
  public boolean hasMealFile()
  {
    return this.mealFile!=null&&!this.mealFile.isEmpty();
  }
  public String getMealFile()
  {
    return this.mealFile;
  }
  public void setMealFile(String s)
  {
    this.mealFile = s;
  }
  
  @Override
  public String toString()
  {
    String str="Consume Meal:";
    if(this.hasMealFile())
      str += "\n\tFile: "+this.mealFile;
    else if (meal != null)
      str +="\n\tElapsedTime: " + (this.meal.hasElapsedTime()?this.meal.getElapsedTime():"None")
          + "\n\tCarbohydrate: " + (this.meal.hasCarbohydrate()?this.meal.getCarbohydrate():"None")
          + "\n\tFat: " + (this.meal.hasFat()?this.meal.getFat():"None")
          + "\n\tProtein: " + (this.meal.hasProtein()?this.meal.getProtein():"None")
          + "\n\tSodium: " + (this.meal.hasSodium()?this.meal.getSodium():"None")
          + "\n\tWater: " + (this.meal.hasWater()?this.meal.getWater():"None");
    
    return str;
  }
}
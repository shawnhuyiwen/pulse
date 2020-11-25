/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.ConsumeNutrientsData;
import com.kitware.pulse.cdm.patient.nutrition.SENutrition;

public class SEConsumeNutrients extends SEPatientAction
{

  private static final long serialVersionUID = 3049429585113617869L;
  protected SENutrition nutrition=null;
  protected String nutritionFile="";
  
  public SEConsumeNutrients()
  {
   
  }
  
  public SEConsumeNutrients(SEConsumeNutrients other)
  {
    this();
    copy(other);
  }
  
  public void copy(SEConsumeNutrients other)
  {
    if(this==other)
      return;
    super.copy(other);
    if(other.nutrition != null)
      this.getNutrition().copy(other.nutrition);
    this.nutritionFile=other.nutritionFile;
  }
  
  @Override
  public void reset()
  {
    super.reset();
    if(this.nutrition != null)
      this.nutrition.reset();
    this.nutritionFile="";
  }
  
  @Override
  public boolean isValid()
  {
    return hasNutrition() || hasNutritionFile();
  }
  
  public static void load(ConsumeNutrientsData src, SEConsumeNutrients dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    switch(src.getOptionCase())
    {
    case NUTRITION:
      SENutrition.load(src.getNutrition(), dst.getNutrition());
      break;
    case NUTRITIONFILE:
      dst.setNutritionFile(src.getNutritionFile());
    default: // do nothing
    }
  }
  
  public static ConsumeNutrientsData unload(SEConsumeNutrients src)
  {
    ConsumeNutrientsData.Builder dst = ConsumeNutrientsData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEConsumeNutrients src, ConsumeNutrientsData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if(src.hasNutritionFile())
      dst.setNutritionFile(src.nutritionFile);
    else if(src.hasNutrition())
      dst.setNutrition(SENutrition.unload(src.nutrition));
  }
  
  public boolean hasNutrition()
  {
    return this.nutrition!=null;
  }
  public SENutrition getNutrition()
  {
    if(this.nutrition==null)
      this.nutrition=new SENutrition();
    return this.nutrition;
  }
  
  public boolean hasNutritionFile()
  {
    return this.nutritionFile!=null&&!this.nutritionFile.isEmpty();
  }
  public String getNutritionFile()
  {
    return this.nutritionFile;
  }
  public void setNutritionFile(String s)
  {
    this.nutritionFile = s;
  }
  
  @Override
  public String toString()
  {
    String str="Consume Nutrients:";
    if(this.hasNutritionFile())
      str +="\n\tFile: "+this.nutritionFile;
    else if (nutrition != null)
      str +="\n\tCarbohydrate: " + (this.nutrition.hasCarbohydrate()?this.nutrition.getCarbohydrate():"None")
          + "\n\tFat: " + (this.nutrition.hasFat()?this.nutrition.getFat():"None")
          + "\n\tProtein: " + (this.nutrition.hasProtein()?this.nutrition.getProtein():"None")
          + "\n\tSodium: " + (this.nutrition.hasSodium()?this.nutrition.getSodium():"None")
          + "\n\tWater: " + (this.nutrition.hasWater()?this.nutrition.getWater():"None");
    return str;
  }
}
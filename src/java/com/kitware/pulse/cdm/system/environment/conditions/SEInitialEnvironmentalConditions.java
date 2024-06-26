/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.environment.conditions;

import com.kitware.pulse.cdm.bind.EnvironmentConditions.InitialEnvironmentalConditionsData;
import com.kitware.pulse.cdm.system.environment.SEEnvironmentalConditions;

public class SEInitialEnvironmentalConditions extends SEEnvironmentCondition
{

  private static final long serialVersionUID = 735720711996975921L;
  protected SEEnvironmentalConditions environmentalConditions=null;
  protected String                    environmentalConditionsFile="";
  
  public SEInitialEnvironmentalConditions()
  {
   
  }
  
  public SEInitialEnvironmentalConditions(SEInitialEnvironmentalConditions other)
  {
    this();
    copy(other);
  }
  
  public void copy(SEInitialEnvironmentalConditions other)
  {
    if(this==other)
      return;
    super.copy(other);
    if(other.environmentalConditions != null)
      this.getEnvironmentalConditions().copy(other.environmentalConditions);
    this.environmentalConditionsFile=other.environmentalConditionsFile;
  }
  
  @Override
  public void clear()
  {
    super.clear();
    if(this.environmentalConditions!=null)
      this.environmentalConditions.clear();
    this.environmentalConditionsFile="";
  }
  
  public boolean isValid()
  {
    return hasEnvironmentalConditions() || hasEnvironmentalConditionsFile();
  }
  
  public static void load(InitialEnvironmentalConditionsData src, SEInitialEnvironmentalConditions dst)
  {
    SEEnvironmentCondition.load(src.getEnvironmentCondition(), dst);
    switch(src.getOptionCase())
    {
    case ENVIRONMENTALCONDITIONSFILE:
      dst.environmentalConditionsFile = src.getEnvironmentalConditionsFile();
      break;
    case ENVIRONMENTALCONDITIONS:
      SEEnvironmentalConditions.load(src.getEnvironmentalConditions(),dst.getEnvironmentalConditions());
      break;
    default: // do nothing
    }
  }
  public static InitialEnvironmentalConditionsData unload(SEInitialEnvironmentalConditions src)
  {
    InitialEnvironmentalConditionsData.Builder dst = InitialEnvironmentalConditionsData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEInitialEnvironmentalConditions src, InitialEnvironmentalConditionsData.Builder dst)
  {
    SEEnvironmentCondition.unload(src, dst.getEnvironmentConditionBuilder());
    if(src.hasEnvironmentalConditionsFile())
      dst.setEnvironmentalConditionsFile(src.environmentalConditionsFile);
    else if(src.hasEnvironmentalConditions())
      dst.setEnvironmentalConditions(SEEnvironmentalConditions.unload(src.environmentalConditions));
  }
  
  public boolean hasEnvironmentalConditions()
  {
    return this.environmentalConditions!=null;
  }
  public SEEnvironmentalConditions getEnvironmentalConditions()
  {
    if(this.environmentalConditions == null)
      this.environmentalConditions=new SEEnvironmentalConditions();
    return this.environmentalConditions;
  }
  
  public boolean hasEnvironmentalConditionsFile()
  {
    return this.environmentalConditionsFile!=null&&!this.environmentalConditionsFile.isEmpty();
  }
  public String getEnvironmentalConditionsFile()
  {
    return this.environmentalConditionsFile;
  }
  public void setEnvironmentalConditionsFile(String s)
  {
    this.environmentalConditionsFile = s;
  }
  
  @Override
  public String toString()
  {
    if(this.hasEnvironmentalConditionsFile())
      return "Initial Envriomental Conditions:"
          + "\n\tEnvironmentalConditionsFile: "+this.environmentalConditionsFile;
    else if (environmentalConditions != null)
      return "Initial Environmental Conditions : "+environmentalConditions.toString();
    else
      return "Condition not specified properly";
  }
}
/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.system.environment.conditions;

import pulse.cdm.bind.EnvironmentConditions.InitialEnvironmentalConditionsData;
import pulse.cdm.substance.SESubstanceManager;
import pulse.cdm.system.environment.SEEnvironmentalConditions;

public class SEInitialEnvironmentalConditions extends SEEnvironmentCondition
{

  private static final long serialVersionUID = 735720711996975921L;
  protected SEEnvironmentalConditions environmentalConditions;
  protected String                    environmentalConditionsFile;
  
  public SEInitialEnvironmentalConditions()
  {
    this.environmentalConditions=new SEEnvironmentalConditions();
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
    this.environmentalConditions.copy(other.environmentalConditions);
    this.environmentalConditionsFile=other.environmentalConditionsFile;
  }
  
  public void reset()
  {
    super.reset();
    this.environmentalConditions.reset();
    this.environmentalConditionsFile="";
  }
  
  public boolean isValid()
  {
    return hasEnvironmentalConditions() || hasEnvironmentalConditionsFile();
  }
  
  public static void load(InitialEnvironmentalConditionsData src, SEInitialEnvironmentalConditions dst, SESubstanceManager subMgr)
  {
    SEEnvironmentCondition.load(src.getEnvironmentCondition(), dst);
    switch(src.getOptionCase())
    {
    case ENVIRONMENTALCONDITIONSFILE:
      dst.environmentalConditionsFile = src.getEnvironmentalConditionsFile();
      break;
    case ENVIRONMENTALCONDITIONS:
      SEEnvironmentalConditions.load(src.getEnvironmentalConditions(),dst.getEnvironmentalConditions(),subMgr);
      break;
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
    if(src.hasEnvironmentalConditions())
      dst.setEnvironmentalConditions(SEEnvironmentalConditions.unload(src.environmentalConditions));
    else if(src.hasEnvironmentalConditionsFile())
      dst.setEnvironmentalConditionsFile(src.environmentalConditionsFile);
  }
  
  public boolean hasEnvironmentalConditions()
  {
    return this.environmentalConditions!=null;
  }
  public SEEnvironmentalConditions getEnvironmentalConditions()
  {
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
  
  public String toString()
  {
    if (environmentalConditions != null)
      return "Initial Environmental Conditions : "+environmentalConditions.toString();
    else if(this.hasEnvironmentalConditionsFile())
      return "Initial Envriomental Conditions:"
          + "\n\tEnvironmentalConditionsFile: "+this.environmentalConditionsFile;
    else
      return "Condition not specified properly";
  }
}
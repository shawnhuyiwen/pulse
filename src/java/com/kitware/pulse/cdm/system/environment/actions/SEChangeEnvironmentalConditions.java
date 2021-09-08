/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.environment.actions;

import com.kitware.pulse.cdm.bind.EnvironmentActions.ChangeEnvironmentalConditionsData;
import com.kitware.pulse.cdm.system.environment.SEEnvironmentalConditions;

public class SEChangeEnvironmentalConditions extends SEEnvironmentAction
{

  private static final long serialVersionUID = -998387113042088499L;
  protected SEEnvironmentalConditions environmentalConditions=null;
  protected String                    environmentalConditionsFile="";
  
  public SEChangeEnvironmentalConditions()
  {
  
  }
  
  public SEChangeEnvironmentalConditions(SEChangeEnvironmentalConditions other)
  {
    this();
    copy(other);
  }
  
  public void copy(SEChangeEnvironmentalConditions other)
  {
    if(this==other)
      return;
    super.copy(other);
    if(this.environmentalConditions != null)
      this.getEnvironmentalConditions().copy(other.environmentalConditions);
    this.environmentalConditionsFile=other.environmentalConditionsFile;
  }
  
  @Override
  public void clear()
  {
    super.clear();
    if(this.environmentalConditions!=null)
      this.environmentalConditions.clear();
    this.environmentalConditionsFile=null;
  }
  
  @Override
  public boolean isValid()
  {
    return hasEnvironmentalConditions() || hasEnvironmentalConditionsFile();
  }
  
  public static void load(ChangeEnvironmentalConditionsData src, SEChangeEnvironmentalConditions dst)
  {
    SEEnvironmentAction.load(src.getEnvironmentAction(), dst);
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
  public static ChangeEnvironmentalConditionsData unload(SEChangeEnvironmentalConditions src)
  {
    ChangeEnvironmentalConditionsData.Builder dst = ChangeEnvironmentalConditionsData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEChangeEnvironmentalConditions src, ChangeEnvironmentalConditionsData.Builder dst)
  {
    SEEnvironmentAction.unload(src, dst.getEnvironmentActionBuilder());
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
      return "Envrioment Configuration:"
          + "\n\tEnvironmentalConditionsFile: "+this.environmentalConditionsFile;
    else if (environmentalConditions != null)
      return "Environment Configuration : "+environmentalConditions.toString();
    else
      return "Action not specified properly";
  }
}
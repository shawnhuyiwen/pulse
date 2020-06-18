/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.system.environment.actions;

import pulse.cdm.bind.EnvironmentActions.ChangeEnvironmentalConditionsData;
import pulse.cdm.substance.SESubstanceManager;
import pulse.cdm.system.environment.SEEnvironmentalConditions;

public class SEChangeEnvironmentalConditions extends SEEnvironmentAction
{

  private static final long serialVersionUID = -998387113042088499L;
  protected SEEnvironmentalConditions environmentalConditions;
  protected String                    environmentalConditionsFile;
  
  public SEChangeEnvironmentalConditions()
  {
    this.environmentalConditions=new SEEnvironmentalConditions();
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
  
  public static void load(ChangeEnvironmentalConditionsData src, SEChangeEnvironmentalConditions dst, SESubstanceManager subMgr)
  {
    SEEnvironmentAction.load(src.getEnvironmentAction(), dst);
    switch(src.getOptionCase())
    {
    case ENVIRONMENTALCONDITIONSFILE:
      dst.environmentalConditionsFile = src.getEnvironmentalConditionsFile();
      break;
    case ENVIRONMENTALCONDITIONS:
      SEEnvironmentalConditions.load(src.getEnvironmentalConditions(),dst.getEnvironmentalConditions(),subMgr);
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
      return "Environment Configuration : "+environmentalConditions.toString();
    else if(this.hasEnvironmentalConditionsFile())
      return "Envrioment Configuration:"
          + "\n\tEnvironmentalConditionsFile: "+this.environmentalConditionsFile;
    else
      return "Action not specified properly";
  }
}
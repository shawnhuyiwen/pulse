/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.bag_valve_mask.actions;

import com.kitware.pulse.cdm.bind.BagValveMaskActions.BagValveMaskConfigurationData;
import com.kitware.pulse.cdm.system.equipment.bag_valve_mask.SEBagValveMask;
import com.kitware.pulse.utilities.Log;

public class SEBagValveMaskConfiguration extends SEBagValveMaskAction
{

  private static final long serialVersionUID = -1487014080271791164L;
  protected SEBagValveMask configuration=null;
  protected String                 configurationFile="";
  
  public SEBagValveMaskConfiguration()
  {
    
  }
  
  public SEBagValveMaskConfiguration(SEBagValveMaskConfiguration other)
  {
    this();
    copy(other);
  }
  
  public void copy(SEBagValveMaskConfiguration other)
  {
    if(this==other)
      return;
    super.copy(other);
    if(other.configuration!=null)
      this.getConfiguration().copy(other.configuration);
    this.configurationFile=other.configurationFile;
  }
  
  @Override
  public void reset()
  {
    super.reset();
    
    if (this.configuration != null)
      this.configuration.reset();
    this.configurationFile="";
  }
  
  @Override
  public boolean isValid()
  {
    return hasConfiguration() || hasConfigurationFile();
  }
  
  public static void load(BagValveMaskConfigurationData src, SEBagValveMaskConfiguration dst)
  {
    SEBagValveMaskAction.load(src.getBagValveMaskAction(),dst);
    switch(src.getOptionCase())
    {
    case CONFIGURATIONFILE:
      dst.configurationFile = src.getConfigurationFile();
      break;
    case CONFIGURATION:
      SEBagValveMask.load(src.getConfiguration(),dst.getConfiguration());
      break;
    default:
    	Log.error("Unknown BagValveMaskConfigurationData Option");
    }
  }
  public static BagValveMaskConfigurationData unload(SEBagValveMaskConfiguration src)
  {
    BagValveMaskConfigurationData.Builder dst = BagValveMaskConfigurationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEBagValveMaskConfiguration src, BagValveMaskConfigurationData.Builder dst)
  {
    SEBagValveMaskAction.unload(src, dst.getBagValveMaskActionBuilder());
    if(src.hasConfigurationFile())
      dst.setConfigurationFile(src.configurationFile);
    else if(src.hasConfiguration())
      dst.setConfiguration(SEBagValveMask.unload(src.configuration));
  }
  
  public boolean hasConfiguration()
  {
    return this.configuration!=null;
  }
  public SEBagValveMask getConfiguration()
  {
    if(this.configuration==null)
      this.configuration=new SEBagValveMask();
    return this.configuration;
  }
  
  public boolean hasConfigurationFile()
  {
    return this.configurationFile!=null&&!this.configurationFile.isEmpty();
  }
  public String getConfigurationFile()
  {
    return this.configurationFile;
  }
  public void setConfigurationFile(String s)
  {
    this.configurationFile = s;
  }
  
  @Override
  public String toString()
  {
    String str = "Mechanical Ventilator Configuration";
    if(this.hasConfigurationFile())
      str +="\n\tConfiguration File: "+this.configurationFile;
    else if(hasConfiguration())
    {
      str += configuration.toString();
    }
    
    return str;
  }
}
/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.inhaler.actions;

import com.kitware.pulse.cdm.bind.InhalerActions.InhalerConfigurationData;
import com.kitware.pulse.cdm.system.equipment.inhaler.SEInhaler;
import com.kitware.pulse.utilities.Log;

public class SEInhalerConfiguration extends SEInhalerAction
{

  private static final long serialVersionUID = 1236980982027471365L;
  protected SEInhaler configuration=null;
  protected String    configurationFile="";
  
  public SEInhalerConfiguration()
  {
  }
  
  public SEInhalerConfiguration(SEInhalerConfiguration other)
  {
    this();
    copy(other);
  }
  
  public void copy(SEInhalerConfiguration other)
  {
    if(this==other)
      return;
    super.copy(other);
    if(other.configuration != null)
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
  
  public static void load(InhalerConfigurationData src, SEInhalerConfiguration dst)
  {
    dst.reset();
    switch(src.getOptionCase())
    {
    case CONFIGURATIONFILE:
      dst.configurationFile = src.getConfigurationFile();
      break;
    case CONFIGURATION:
      SEInhaler.load(src.getConfiguration(),dst.getConfiguration());
      break;
    default:
    	Log.error("Unknown InhalerConfiguationData OptionCase");
    }
  }
  public static InhalerConfigurationData unload(SEInhalerConfiguration src)
  {
    InhalerConfigurationData.Builder dst = InhalerConfigurationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEInhalerConfiguration src, InhalerConfigurationData.Builder dst)
  {
    if(src.hasConfigurationFile())
      dst.setConfigurationFile(src.configurationFile);
    else if(src.hasConfiguration())
      dst.setConfiguration(SEInhaler.unload(src.configuration));
  }
  
  public boolean hasConfiguration()
  {
    return this.configuration!=null;
  }
  public SEInhaler getConfiguration()
  {
    if(this.configuration==null)
      this.configuration=new SEInhaler();
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
    String str = "Inhaler Configuration";
    if(this.hasConfigurationFile())
      str +="\n\tInhaler File: "+this.configurationFile;
    else if(hasConfiguration())
    {
      str += configuration.toString();
    }
    
    return str;
  }
}
/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.mechanical_ventilator.actions;

import com.kitware.pulse.cdm.bind.MechanicalVentilatorActions.MechanicalVentilatorConfigurationData;
import com.kitware.pulse.cdm.system.equipment.mechanical_ventilator.SEMechanicalVentilator;
import com.kitware.pulse.utilities.Log;

public class SEMechanicalVentilatorConfiguration extends SEMechanicalVentilatorAction
{

  private static final long serialVersionUID = -1487014080271791164L;
  protected SEMechanicalVentilator configuration=null;
  protected String                 configurationFile="";
  
  public SEMechanicalVentilatorConfiguration()
  {
    
  }
  
  public SEMechanicalVentilatorConfiguration(SEMechanicalVentilatorConfiguration other)
  {
    this();
    copy(other);
  }
  
  public void copy(SEMechanicalVentilatorConfiguration other)
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
  
  public static void load(MechanicalVentilatorConfigurationData src, SEMechanicalVentilatorConfiguration dst)
  {
    SEMechanicalVentilatorAction.load(src.getMechanicalVentilatorAction(),dst);
    switch(src.getOptionCase())
    {
    case CONFIGURATIONFILE:
      dst.configurationFile = src.getConfigurationFile();
      break;
    case CONFIGURATION:
      SEMechanicalVentilator.load(src.getConfiguration(),dst.getConfiguration());
      break;
    default:
    	Log.error("Unknown MechanicalVentilatorConfigurationData Option");
    }
  }
  public static MechanicalVentilatorConfigurationData unload(SEMechanicalVentilatorConfiguration src)
  {
    MechanicalVentilatorConfigurationData.Builder dst = MechanicalVentilatorConfigurationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEMechanicalVentilatorConfiguration src, MechanicalVentilatorConfigurationData.Builder dst)
  {
    SEMechanicalVentilatorAction.unload(src, dst.getMechanicalVentilatorActionBuilder());
    if(src.hasConfigurationFile())
      dst.setConfigurationFile(src.configurationFile);
    else if(src.hasConfiguration())
      dst.setConfiguration(SEMechanicalVentilator.unload(src.configuration));
  }
  
  public boolean hasConfiguration()
  {
    return this.configuration!=null;
  }
  public SEMechanicalVentilator getConfiguration()
  {
    if(this.configuration==null)
      this.configuration=new SEMechanicalVentilator();
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
/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.anesthesia_machine.actions;
import com.kitware.pulse.cdm.bind.AnesthesiaMachineActions.AnesthesiaMachineConfigurationData;
import com.kitware.pulse.cdm.system.equipment.anesthesia_machine.SEAnesthesiaMachine;
import com.kitware.pulse.utilities.Log;

public class SEAnesthesiaMachineConfiguration extends SEAnesthesiaMachineAction
{

  private static final long serialVersionUID = 7917788116628472277L;
  protected SEAnesthesiaMachine configuration=null;
  protected String              configurationFile="";
  
  public SEAnesthesiaMachineConfiguration()
  {
    
  }
  
  public SEAnesthesiaMachineConfiguration(SEAnesthesiaMachineConfiguration other)
  {
    this();
    copy(other);
  }
  
  public void copy(SEAnesthesiaMachineConfiguration other)
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
  
  public static void load(AnesthesiaMachineConfigurationData src, SEAnesthesiaMachineConfiguration dst)
  {
    SEAnesthesiaMachineAction.load(src.getAnesthesiaMachineAction(),dst);
    switch(src.getOptionCase())
    {
    case CONFIGURATIONFILE:
      dst.configurationFile = src.getConfigurationFile();
      break;
    case CONFIGURATION:
      SEAnesthesiaMachine.load(src.getConfiguration(),dst.getConfiguration());
      break;
    default:
    	Log.error("Unknown AnesthesiaMachineConfigurationData Option");
    }
  }
  public static AnesthesiaMachineConfigurationData unload(SEAnesthesiaMachineConfiguration src)
  {
    AnesthesiaMachineConfigurationData.Builder dst = AnesthesiaMachineConfigurationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEAnesthesiaMachineConfiguration src, AnesthesiaMachineConfigurationData.Builder dst)
  {
    SEAnesthesiaMachineAction.unload(src, dst.getAnesthesiaMachineActionBuilder());
    if(src.hasConfigurationFile())
      dst.setConfigurationFile(src.configurationFile);
    else if(src.hasConfiguration())
      dst.setConfiguration(SEAnesthesiaMachine.unload(src.configuration));
  }
  
  public boolean hasConfiguration()
  {
    return this.configuration!=null;
  }
  public SEAnesthesiaMachine getConfiguration()
  {
    if(this.configuration==null)
      this.configuration=new SEAnesthesiaMachine();
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
    String str = "Anesthesia Machine Configuration";
    if(this.hasConfigurationFile())
      str +="\n\tAnesthesia Machine File: "+this.configurationFile;
    else if(hasConfiguration())
    {
      str += configuration.toString();
    }
    
    return str;
  }
}
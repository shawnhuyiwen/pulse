/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.ecmo.actions;

import com.kitware.pulse.cdm.bind.ECMOActions.ECMOConfigurationData;
import com.kitware.pulse.cdm.system.equipment.ecmo.SEECMOSettings;
import com.kitware.pulse.utilities.Log;

public class SEECMOConfiguration extends SEECMOAction
{

  private static final long serialVersionUID = -1487014080271791164L;
  protected SEECMOSettings settings=null;
  protected String                         settingsFile="";
  
  public SEECMOConfiguration()
  {
    
  }
  
  public SEECMOConfiguration(SEECMOConfiguration other)
  {
    this();
    copy(other);
  }
  
  public void copy(SEECMOConfiguration other)
  {
    if(this==other)
      return;
    super.copy(other);
    if(other.settings!=null)
      this.getSettings().copy(other.settings);
    this.settingsFile=other.settingsFile;
  }
  
  @Override
  public void clear()
  {
    super.clear();
    
    if (this.settings != null)
      this.settings.clear();
    this.settingsFile="";
  }
  
  @Override
  public boolean isValid()
  {
    return hasSettings() || hasSettingsFile();
  }
  
  public static void load(ECMOConfigurationData src, SEECMOConfiguration dst)
  {
    SEECMOAction.load(src.getECMOAction(),dst);
    switch(src.getOptionCase())
    {
    case SETTINGSFILE:
      dst.settingsFile = src.getSettingsFile();
      break;
    case SETTINGS:
      SEECMOSettings.load(src.getSettings(),dst.getSettings());
      break;
    default:
    	Log.error("Unknown ECMOConfigurationData Option");
    }
  }
  public static ECMOConfigurationData unload(SEECMOConfiguration src)
  {
    ECMOConfigurationData.Builder dst = ECMOConfigurationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEECMOConfiguration src, ECMOConfigurationData.Builder dst)
  {
    SEECMOAction.unload(src, dst.getECMOActionBuilder());
    if(src.hasSettingsFile())
      dst.setSettingsFile(src.settingsFile);
    else if(src.hasSettings())
      dst.setSettings(SEECMOSettings.unload(src.settings));
  }
  
  public boolean hasSettings()
  {
    return this.settings!=null;
  }
  public SEECMOSettings getSettings()
  {
    if(this.settings==null)
      this.settings=new SEECMOSettings();
    return this.settings;
  }
  
  public boolean hasSettingsFile()
  {
    return this.settingsFile!=null&&!this.settingsFile.isEmpty();
  }
  public String getSettingsFile()
  {
    return this.settingsFile;
  }
  public void setSettingsFile(String s)
  {
    this.settingsFile = s;
  }
  
  @Override
  public String toString()
  {
    String str = "ECMO Configuration";
    if(this.hasSettingsFile())
      str +="\n\tSettings File: "+this.settingsFile;
    else if(hasSettings())
    {
      str += settings.toString();
    }
    
    return str;
  }
}
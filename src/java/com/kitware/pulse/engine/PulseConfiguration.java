package com.kitware.pulse.engine;

import com.kitware.pulse.engine.bind.Configuration.ConfigurationData;

public class PulseConfiguration
{

  public void reset() 
  {
    
  }
  
  public boolean isValid()
  {
    return true;
  }
  
  public static void load(ConfigurationData src, PulseConfiguration dst)
  {
    
  }

  public static ConfigurationData unload(PulseConfiguration src)
  {
    ConfigurationData.Builder dst = ConfigurationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }

  protected static void unload(PulseConfiguration src, ConfigurationData.Builder dst)
  {
    
  }
}

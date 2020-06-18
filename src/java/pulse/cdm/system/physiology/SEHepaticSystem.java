/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.cdm.system.physiology;

import pulse.cdm.bind.Physiology.HepaticSystemData;
import pulse.cdm.system.SESystem;

public class SEHepaticSystem extends SEPhysiologySystem implements SESystem
{
  public SEHepaticSystem()
  {
    
  }

  @Override
  public void reset()
  {    
    
  }

  public static void load(HepaticSystemData src, SEHepaticSystem dst)
  {        
    
  }
  public static HepaticSystemData unload(SEHepaticSystem src)
  {
    HepaticSystemData.Builder dst = HepaticSystemData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEHepaticSystem src, HepaticSystemData.Builder dst)
  {    
    
  }

  
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.physiology.datamodel.system.equipment.anesthesia;

import com.kitware.physiology.cdm.AnesthesiaMachine.AnesthesiaMachineData;
import com.kitware.physiology.cdm.AnesthesiaMachine.AnesthesiaMachineOxygenBottleData;
import com.kitware.physiology.datamodel.properties.SEScalarVolume;

public class SEAnesthesiaMachineOxygenBottle
{
  protected SEScalarVolume volume;

  public SEAnesthesiaMachineOxygenBottle()
  {
    volume = null;
  }

  public void reset()
  {
    if (volume != null)
      volume.invalidate();
  }

  public void copy(SEAnesthesiaMachineOxygenBottle from)
  {
    reset();
    if(from.hasVolume())
      this.getVolume().set(from.getVolume());
  }

  public static void load(AnesthesiaMachineOxygenBottleData src, SEAnesthesiaMachineOxygenBottle dst)
  {
    if (src.hasVolume())
      SEScalarVolume.load(src.getVolume(),dst.getVolume());        
  }
  public static AnesthesiaMachineOxygenBottleData unload(SEAnesthesiaMachineOxygenBottle src)
  {
    AnesthesiaMachineOxygenBottleData.Builder dst = AnesthesiaMachineOxygenBottleData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEAnesthesiaMachineOxygenBottle src, AnesthesiaMachineOxygenBottleData.Builder dst)
  {
    if (src.hasVolume())
      dst.setVolume(SEScalarVolume.unload(src.volume));
  }

  public boolean hasVolume()
  {
    return volume == null ? false : volume.isValid();
  }

  public SEScalarVolume getVolume()
  {
    if (volume == null)
      volume = new SEScalarVolume();        
    return volume;
  }

  public String toString()
  {
    return "Anesthesia Machine Oxygen Bottle"
        + "\n\tVolume: " + getVolume();
  }
}
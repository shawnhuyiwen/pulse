/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.system.equipment.anesthesia_machine;

import com.kitware.pulse.cdm.bind.AnesthesiaMachine.AnesthesiaMachineOxygenBottleData;
import com.kitware.pulse.cdm.properties.SEScalarVolume;

public class SEAnesthesiaMachineOxygenBottle
{
  protected SEScalarVolume volume;

  public SEAnesthesiaMachineOxygenBottle()
  {
    volume = null;
  }

  public void clear()
  {
    if (volume != null)
      volume.invalidate();
  }

  public void copy(SEAnesthesiaMachineOxygenBottle from)
  {
    clear();
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

  @Override
  public String toString()
  {
    return "Anesthesia Machine Oxygen Bottle"
        + "\n\tVolume: " + getVolume();
  }
}
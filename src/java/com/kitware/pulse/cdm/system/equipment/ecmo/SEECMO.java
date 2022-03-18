/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.ecmo;

import com.kitware.pulse.cdm.properties.*;
import com.kitware.pulse.cdm.system.equipment.SEEquipment;

public class SEECMO implements SEEquipment
{
  protected SEECMOSettings                    settings;

  public SEECMO()
  {
    settings = null;
  }

  @Override
  public void clear()
  {
    if (settings != null)
      settings.clear();
  }
  
  public boolean hasSettings()
  {
    return settings != null;
  }
  public SEECMOSettings getSettings()
  {
    if (settings == null)
      settings = new SEECMOSettings();
    return settings;
  }
}
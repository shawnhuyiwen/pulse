/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.Actions.eMergeType;
import com.kitware.pulse.cdm.bind.PatientActions.RespiratoryMechanicsConfigurationData;
import com.kitware.pulse.cdm.bind.Physiology.RespiratoryMechanicsData;
import com.kitware.pulse.cdm.system.physiology.SERespiratoryMechanics;

public class SERespiratoryMechanicsConfiguration extends SEPatientAction
  {
    protected SERespiratoryMechanics settings = null;
    protected String settingsFile = "";
    protected eMergeType mergeType = eMergeType.Append;

    public SERespiratoryMechanicsConfiguration()
    {
      super();
    }

    public SERespiratoryMechanicsConfiguration(SERespiratoryMechanicsConfiguration other)
    {
      copy(other);
    }

    public void copy(SERespiratoryMechanicsConfiguration other)
    {
      super.copy(other);
      if (other.settings != null)
        this.getSettings().copy(other.settings);
      this.settingsFile = other.settingsFile;
      mergeType = other.mergeType;
    }

    public void clear()
    {
      super.clear();
      if (this.settings != null)
        this.settings.clear();
      this.settingsFile = "";
      mergeType = eMergeType.Append;
    }
    
    public static void load(RespiratoryMechanicsConfigurationData src, SERespiratoryMechanicsConfiguration dst)
    {
      dst.setMergeType(src.getMergeType());
      if (src.hasSettings())
        SERespiratoryMechanics.load(src.getSettings(), dst.getSettings());
      else if (src.getSettingsFile().isEmpty())
        dst.setSettingsFile(src.getSettingsFile());
    }
    public static RespiratoryMechanicsConfigurationData unload(SERespiratoryMechanicsConfiguration src)
    {
      RespiratoryMechanicsConfigurationData.Builder dst = RespiratoryMechanicsConfigurationData.newBuilder();
      unload(src,dst);
      return dst.build();
    }
    protected static void unload(SERespiratoryMechanicsConfiguration src, RespiratoryMechanicsConfigurationData.Builder dst)
    {
      dst.setMergeType(src.getMergeType());
      if(src.hasSettings())
        dst.setSettings(SERespiratoryMechanics.unload(src.getSettings()));
      else if (src.hasSettingsFile())
        dst.setSettingsFile(src.getSettingsFile());
    }

    public boolean isValid()
    {
      return hasSettings() || hasSettingsFile();
    }

    public boolean hasSettings()
    {
      return this.settings != null;
    }
    public SERespiratoryMechanics getSettings()
    {
      if (this.settings == null)
        this.settings = new SERespiratoryMechanics();
      return this.settings;
    }

    public boolean hasSettingsFile()
    {
      return this.settingsFile != null && !this.settingsFile.isEmpty();
    }
    public String getSettingsFile()
    {
      return this.settingsFile;
    }
    public void setSettingsFile(String s)
    {
      this.settingsFile = s;
    }
    public eMergeType getMergeType()
    {
      return mergeType;
    }
    public void setMergeType(eMergeType m)
    {
      mergeType = m;
    }

    public String toString()
    {
      String str = "Respiratory Mechanics Configuration";
      if (this.hasSettingsFile())
        str += "\n\tSettings File: " + this.settingsFile;
      else if (hasSettings())
      {
        str += settings.toString();
      }
      str += "\n\tMerge Type: " + this.mergeType;
      return str;
    }
  }

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEECMOConfiguration : SEECMOAction
  {
    protected SEECMOSettings settings = null;
    protected string settingsFile = "";
    protected eMergeType mergeType = eMergeType.Append;

    public SEECMOConfiguration()
    {

    }

    public SEECMOConfiguration(SEECMOConfiguration other)
    {
      Copy(other);
    }

    public void Copy(SEECMOConfiguration other)
    {
      base.Copy(other);
      if(other.settings != null)
        this.GetSettings().Copy(other.settings);
      this.settingsFile = other.settingsFile;
      mergeType = other.mergeType;
    }

    public override void Clear()
    {
      base.Clear();
      if (this.settings != null)
        this.settings.Clear();
      this.settingsFile = "";
      mergeType = eMergeType.Append;
    }

    public override bool IsValid()
    {
      return HasSettings() || HasSettingsFile();
    }

    public bool HasSettings()
    {
      return this.settings != null;
    }
    public SEECMOSettings GetSettings()
    {
      if (this.settings == null)
        this.settings = new SEECMOSettings();
      return this.settings;
    }

    public bool HasSettingsFile()
    {
      return !string.IsNullOrEmpty(this.settingsFile);
    }
    public string GetSettingsFile()
    {
      return this.settingsFile;
    }
    public void SetSettingsFile(string s)
    {
      this.settingsFile = s;
    }
    public eMergeType GetMergeType()
    {
      return mergeType;
    }
    public void SetMergeType(eMergeType m)
    {
      mergeType = m;
    }

    public override string ToString()
    {
      string str = "ECMO Configuration";
      if (this.HasSettingsFile())
        str += "\n\tSettings File: " + this.settingsFile;
      else if (HasSettings())
      {
        str += settings.ToString();
      }
      str += "\n\tMerge Type: " + this.mergeType;
      return str;
    }
  }
}

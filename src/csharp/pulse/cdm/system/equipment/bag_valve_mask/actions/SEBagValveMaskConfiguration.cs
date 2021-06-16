/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEBagValveMaskConfiguration : SEBagValveMaskAction
  {
    protected SEBagValveMask configuration = null;
    protected string configurationFile = "";

    public SEBagValveMaskConfiguration()
    {

    }

    public SEBagValveMaskConfiguration(SEBagValveMaskConfiguration other)
    {
      Copy(other);
    }

    public void copy(SEBagValveMaskConfiguration other)
    {
      base.Copy(other);
      if(other.configuration != null)
        this.GetConfiguration().Copy(other.configuration);
      this.configurationFile = other.configurationFile;
    }

    public override void Clear()
    {
      base.Clear();
      if (this.configuration != null)
        this.configuration.Clear();
      this.configurationFile = "";
    }

    public override bool IsValid()
    {
      return HasConfiguration() || HasConfigurationFile();
    }


    public bool HasConfiguration()
    {
      return this.configuration != null;
    }
    public SEBagValveMask GetConfiguration()
    {
      if (this.configuration == null)
        this.configuration = new SEBagValveMask();
      return this.configuration;
    }

    public bool HasConfigurationFile()
    {
      return !string.IsNullOrEmpty(this.configurationFile);
    }
    public string GetConfigurationFile()
    {
      return this.configurationFile;
    }
    public void SetConfigurationFile(string s)
    {
      this.configurationFile = s;
    }

    public override string ToString()
    {
      string str = "Bag Valve Mask Configuration";
      if (this.HasConfigurationFile())
        str += "\n\tConfiguration File: " + this.configurationFile;
      else if (HasConfiguration())
      {
        str += configuration.ToString();
      }
      
      return str;
    }
  }
}

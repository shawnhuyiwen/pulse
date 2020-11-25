/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEMechanicalVentilatorConfiguration : SEMechanicalVentilatorAction
  {
    protected SEMechanicalVentilator configuration = null;
    protected string configurationFile = "";

    public SEMechanicalVentilatorConfiguration()
    {

    }

    public SEMechanicalVentilatorConfiguration(SEMechanicalVentilatorConfiguration other)
    {
      Copy(other);
    }

    public void copy(SEMechanicalVentilatorConfiguration other)
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
    public SEMechanicalVentilator GetConfiguration()
    {
      if (this.configuration == null)
        this.configuration = new SEMechanicalVentilator();
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
      string str = "Mechanical Ventilator Configuration";
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

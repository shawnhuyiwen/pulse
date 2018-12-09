/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class SEAnesthesiaMachineConfiguration : SEAnesthesiaMachineAction
{
  protected SEAnesthesiaMachine configuration;
  protected string              configurationFile;
  
  public SEAnesthesiaMachineConfiguration()
  {
    
  }
  
  //public SEAnesthesiaMachineConfiguration(SEAnesthesiaMachineConfiguration other)
  //{
  //  this();
  //  copy(other);    
  //}
  
  //public void copy(SEAnesthesiaMachineConfiguration other)
  //{
  //  if(this==other)
  //    return;
  //  super.copy(other);
  //  this.configuration.copy(other.configuration);
  //  this.configurationFile=other.configurationFile;
  //}
  
  public override void Clear()
  {
    base.Clear();
    
    if (this.configuration != null)
      this.configuration.Clear();
    if (this.configurationFile != null)
      this.configurationFile="";
  }
  
  public override bool IsValid()
  {
    return HasConfiguration() || HasConfigurationFile();
  }
  
  
  public bool HasConfiguration()
  {
    return this.configuration!=null;
  }
  public SEAnesthesiaMachine GetConfiguration()
  {
    if(this.configuration==null)
      this.configuration=new SEAnesthesiaMachine();
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
    string str = "Anesthesia Machine Configuration";
    if(HasConfiguration())
    {
      str += configuration.ToString();
    }
    if(this.HasConfigurationFile())
      str +="\n\tAnesthesia Machine File: "+this.configurationFile;
    return str;
  }
}
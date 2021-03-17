/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Pulse.CDM
{
  public class RunConfiguration
  {
    protected string root_directory;
    protected string data_directory;
    protected string scenario_directory;
    protected string test_config_directory;
    protected string validation_directory;
    protected string verification_directory;

    public string getRootDirectory() { return root_directory; }
    public string getDataDirectory() { return data_directory; }
    public string getScenarioDirectory() { return scenario_directory; }
    public string getTestConfigDirectory() { return test_config_directory; }
    public string getValidationDirectory() { return validation_directory; }
    public string getVerificationDirectory() { return verification_directory; }


    public RunConfiguration()
    {
      // Default values
      root_directory = "./";
      data_directory = "./";
      scenario_directory = "./";
      test_config_directory = "./";
      validation_directory = "./";
      verification_directory = "./";

      // Now let's look for a config file and
      // use any configuration parameters it has
      if (File.Exists("./run.config"))
      {
        var properties = new Dictionary<string, string>();
        foreach (var row in File.ReadAllLines("./run.config"))
          properties.Add(row.Split('=')[0], string.Join("=", row.Split('=').Skip(1).ToArray()));
        SetProperties(properties);
      }
      else
        System.Console.Out.WriteLine("Unable to find run.config");
    }

    protected void SetProperties(Dictionary<string, string> properties)
    {
      if (properties.ContainsKey("root_dir"))
      {
        string val = properties["root_dir"].ToString();
        if (!val.StartsWith("@"))
        {
          if (Directory.Exists(val))
            root_directory = val;
          else
            System.Console.Out.WriteLine("root_dir set to invalid directory " + val + ", ignoring and using default (" + this.root_directory + ")");
        }
      }
      if (properties.ContainsKey("data_dir"))
      {
        string val = properties["data_dir"].ToString();
        if (!val.StartsWith("@"))
        {
          if (Directory.Exists(val))
            data_directory = val;
          else
            System.Console.Out.WriteLine("data_dir set to invalid directory " + val + ", ignoring and using default (" + this.data_directory + ")");
        }
      }
      if (properties.ContainsKey("scenario_dir"))
      {
        string val = properties["scenario_dir"].ToString();
        if (!val.StartsWith("@"))
        {
          if (Directory.Exists(val))
            scenario_directory = val;
          else
            System.Console.Out.WriteLine("scenario_dir set to invalid directory " + val + ", ignoring and using default (" + this.scenario_directory + ")");
        }
      }
      if (properties.ContainsKey("test_config_dir"))
      {
        string val = properties["test_config_dir"].ToString();
        if (!val.StartsWith("@"))
        {
          if (Directory.Exists(val))
            test_config_directory = val;
          else
            System.Console.Out.WriteLine("test_config_dir set to invalid directory " + val + ", ignoring and using default (" + this.test_config_directory + ")");
        }
      }
      if (properties.ContainsKey("validation_dir"))
      {
        string val = properties["validation_dir"].ToString();
        if (!val.StartsWith("@"))
        {
          if (Directory.Exists(val))
            validation_directory = val;
          else
            System.Console.Out.WriteLine("validation_dir set to invalid directory " + val + ", ignoring and using default (" + this.validation_directory + ")");
        }
      }
      if (properties.ContainsKey("verification_dir"))
      {
        string val = properties["verification_dir"].ToString();
        if (!val.StartsWith("@"))
        {
          if (Directory.Exists(val))
            verification_directory = val;
          else
            System.Console.Out.WriteLine("verification_dir set to invalid directory " + val + ", ignoring and using default (" + this.verification_directory + ")");
        }
      }
    }
  }
}

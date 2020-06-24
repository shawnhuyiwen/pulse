package com.kitware.pulse.utilities;

import java.io.File;
import java.io.FileInputStream;
import java.util.Properties;

/**
 * Simple file to load up a config file and
 * provide access to the variables it contains
 * Pretty much a simple I/O wrapped properties file
 * @author aaron.bray
 *
 */
public class RunConfiguration 
{
  protected String rootDirectory;
  protected String dataDirectory;
  protected String scenarioDirectory;
  protected String testConfigDirectory;
  protected String validationDirectory;
  protected String verificationDirectory;
  
  public    String getRootDirectory()         { return rootDirectory;         }
  public    String getDataDirectory()         { return dataDirectory;         }
  public    String getScenarioDirectory()     { return scenarioDirectory;     }
  public    String getTestConfigDirectory()   { return testConfigDirectory;   }
  public    String getValidationDirectory()   { return validationDirectory;   }
  public    String getVerificationDirectory() { return verificationDirectory; }
  
  
  public RunConfiguration()
  { 
    // Default values
  	rootDirectory         = "./";
    dataDirectory         = "./";
    scenarioDirectory     = "./";
    testConfigDirectory   = "./";
    validationDirectory   = "./";
    verificationDirectory = "./";
    
    // Now let's look for a config file and
    // use any configuration parameters it has    
    File file = new File("./run.config");
    if(file.exists())
    {
      try
      {
        FileInputStream fileInput = new FileInputStream(file);
        Properties properties = new Properties();
        properties.load(fileInput);
        setProperties(properties);
        fileInput.close();
      }
      catch(Exception ex)
      {
        Log.error("Could not read run.config file");
      }
    }
    
  }
  
  protected void setProperties(Properties properties)
  {
  	if(properties.containsKey("root_dir"))
    {  
      String val = properties.get("root_dir").toString();
      if(!val.startsWith("@"))
      {
        File dir = new File(val);
        if(dir.exists() && dir.isDirectory())
          rootDirectory = val;
        else
          Log.error("root_dir set to invalid directory "+val+", ignoring and using default ("+this.rootDirectory+")");
      }
    }
    if(properties.containsKey("data_dir"))
    {  
      String val = properties.get("data_dir").toString();
      if(!val.startsWith("@"))
      {
        File dir = new File(val);
        if(dir.exists() && dir.isDirectory())
          dataDirectory = val;
        else
          Log.error("data_dir set to invalid directory "+val+", ignoring and using default ("+this.dataDirectory+")");
      }
    }
    if(properties.containsKey("scenario_dir"))
    {  
      String val = properties.get("scenario_dir").toString();
      if(!val.startsWith("@"))
      {
        File dir = new File(val);
        if(dir.exists() && dir.isDirectory())
          scenarioDirectory = val;
        else
          Log.error("scenario_dir set to invalid directory "+val+", ignoring and using default ("+this.scenarioDirectory+")");
      }
    }
    if(properties.containsKey("test_config_dir"))
    {  
      String val = properties.get("test_config_dir").toString();
      if(!val.startsWith("@"))
      {
        File dir = new File(val);
        if(dir.exists() && dir.isDirectory())
        	testConfigDirectory = val;
        else
          Log.error("test_config_dir set to invalid directory "+val+", ignoring and using default ("+this.testConfigDirectory+")");
      }
    }
    if(properties.containsKey("validation_dir"))
    {  
      String val = properties.get("validation_dir").toString();
      if(!val.startsWith("@"))
      {
        File dir = new File(val);
        if(dir.exists() && dir.isDirectory())
        	validationDirectory = val;
        else
          Log.error("validation_dir set to invalid directory "+val+", ignoring and using default ("+this.validationDirectory+")");
      }
    }
    if(properties.containsKey("verification_dir"))
    {  
      String val = properties.get("verification_dir").toString();
      if(!val.startsWith("@"))
      {
        File dir = new File(val);
        if(dir.exists() && dir.isDirectory())
        	verificationDirectory = val;
        else
          Log.error("verification_dir set to invalid directory "+val+", ignoring and using default ("+this.verificationDirectory+")");
      }
    }
  }
  
}

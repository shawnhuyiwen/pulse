/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package mil.tatrc.physiology.testing;

import java.io.File;
import java.io.IOException;

import mil.tatrc.physiology.utilities.FileUtils;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.RunConfiguration;

public class Rebase
{

  public static void main(String[] args)
  {
    RunConfiguration cfg = new RunConfiguration();
    Log.setFileName("Rebase.log");    
    String toDir = "./test_results/rebase/";
    String configFile = "DebugRun.config";
    switch(args.length)
    {
      case 1:
      {
        toDir = args[0];
        break;
      }
      case 2:
      {
        toDir = args[0];
        configFile = args[1];
      }
    }
    // Replace back slashes with forward slashes
    toDir = toDir.replaceAll("\\\\", "/");

    Log.info("Rebasing to "+toDir);
    Log.info("Rebasing config "+configFile);
    
    // Yeh, this is a bit ugly...
    // /I need to split the configuration parser and the execution from SETestDriver
    SETestDriver config = new SETestDriver();
    config.processConfigFile(new File(cfg.getTestConfigDirectory()+"/"+configFile), cfg.getVerificationDirectory());

    // Make sure from/to dir ends with a slash
    if(!toDir.endsWith("/"))
      toDir = toDir + "/";
    try
    {
      FileUtils.createDirectory(toDir);
    }
    catch(Exception ex)
    {
      Log.error("Could not create to Directory : "+toDir,ex);
      return;
    }
    
    for(SETestDriver.TestJob job : config.jobs)
    {
      for(String rFile : job.computedFiles)
      {
        if(!new File(rFile).exists())
        {
      	  Log.error("Result file does not exist : "+rFile);
      	  continue;
        }
        // Check for a log
        String log = "";
        if(job.name.endsWith(".pba"))
      	  log = rFile.replaceAll("Results.txt", ".log");
        String prepend = rFile.substring(0,rFile.lastIndexOf("/"));
        String newFile = toDir + prepend + ".zip";
        Log.info("Creating new file "+newFile);
        String rPath = "";
        for(String rDir : newFile.split("/"))
        {
          if(rDir.endsWith(".zip"))
          {
            break;
          }
          rPath += rDir + "/";
        }
        try
        {
          FileUtils.createDirectory(rPath);
        } 
        catch (IOException ex)
        {
          Log.error("Could not create directory : "+rPath, ex);
        }
        File oldZip = new File(newFile);
        if(oldZip.exists())
          oldZip.delete();
        String[] files;
        if(!log.isEmpty())
      	  files = new String[]{rFile,log};
        else
      	  files = new String[]{rFile};
        FileUtils.zipFiles(files, newFile);
      }
    }
  }

}

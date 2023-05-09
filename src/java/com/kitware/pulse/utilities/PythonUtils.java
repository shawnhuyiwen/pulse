/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.utilities;

import java.io.BufferedReader;
import java.io.IOException;

public class PythonUtils
{
  protected String python="";
  
  public PythonUtils()
  {
    RunConfiguration cfg = new RunConfiguration();
    this.python = cfg.getPythonCommand();
    
    try
    {
      if(runPython("--version")==0)
        return;
    }catch(Exception ex) {}
    
    try
    {
      this.python = "python3";
      if(runPython("--version")==0)
        return;
    }catch(Exception ex) {}
    
    try
    {
      this.python = "python";
      if(runPython("--version")==0)
        return;
    }catch(Exception ex){}
    
    this.python = "";
    Log.error("Unable to find python");
  }
  
  public boolean isActive()
  {
    return !this.python.isEmpty();
  }
  
  public synchronized int runPython(String pyFilename, String...args) throws InterruptedException, IOException
  {
    if(this.python.isEmpty())
      return 1;
    
    String[] cmds = new String[args.length+2];
    cmds[0] = this.python;
    cmds[1] = pyFilename;
    int i=2;
    for(String arg : args)
      cmds[i++] = arg;
    
    ProcessBuilder processBuilder = new ProcessBuilder(cmds);
    processBuilder.redirectErrorStream(true);
    Process process = processBuilder.start();
    String line;
    BufferedReader input = new BufferedReader(new java.io.InputStreamReader(process.getInputStream()));
    while ((line = input.readLine()) != null)
      Log.info(line);
    return process.waitFor();
  }
}

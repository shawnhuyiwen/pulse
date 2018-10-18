package com.kitware.physiology.utilities;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class jniBridge 
{ 
  public static boolean initialize()
  {
     return initialize(System.getProperty("user.dir"));
  }
  public static boolean initialize(String workingDirectory)
  {
    String location = System.getProperty("user.dir");
    List<String>libs = new ArrayList<String>();
    libs.add("PulseJNI");
    if(!FileUtils.loadLibraries(libs,location))
      throw new RuntimeException("Could not load libraries : " + libs);
    
    File workingDirectoryFile = new File(workingDirectory).getAbsoluteFile();
    File f = new File(workingDirectoryFile.getAbsolutePath() + "/UCEDefs.txt").getAbsoluteFile();
    
    if(!f.exists())
    {
      Log.error("Unable to find "+workingDirectory+"/UCEDefs.txt");
      return false;
    }
    nativeInitialize(workingDirectory);
    return true;
  }
  private static native void nativeInitialize(String workingDirectory);
  
  public static void deinitialize()
  {
    nativeDeinitialize();
  }
  private static native void nativeDeinitialize();
}

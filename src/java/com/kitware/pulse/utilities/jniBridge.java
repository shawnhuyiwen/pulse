package com.kitware.pulse.utilities;

import java.util.ArrayList;
import java.util.Arrays;

public class jniBridge 
{ 
  private static boolean loaded  =  false;
  private static String  libName = "PulseJNI";
  
  public static void initialize()
  {
    initialize(null,false);
  }
  
  public static void initializeDebug()
  {
    initialize(null,true);
  }
  
  // Support looking in a specific location not on the java.library.path
  public static void initialize(String libDir)
  {
    jniBridge.initialize(libDir, false);
  }
  public static void initializeDebug(String libDir)
  {
    jniBridge.initialize(libDir, true);
  }
  
  
  public static void initialize(String libDir, boolean debugMode)
  {
    if(loaded)
      return;
    String lib = debugMode?libName+"d":libName;
    if(!FileUtils.loadLibraries(new ArrayList<>(Arrays.asList(lib)),libDir))
      throw new RuntimeException("Could not load PulseJNI library");
     nativeInitialize();
     loaded = true;
  }
  private static native void nativeInitialize();
  
  public static void deinitialize()
  {
    nativeDeinitialize();
  }
  private static native void nativeDeinitialize();
}

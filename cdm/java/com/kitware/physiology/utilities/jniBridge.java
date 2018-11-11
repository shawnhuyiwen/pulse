package com.kitware.physiology.utilities;

import java.util.*;

public class jniBridge 
{ 
  public static void initialize()
  {
  	String location = System.getProperty("user.dir");
    if(!FileUtils.loadLibraries(new ArrayList<String>(Arrays.asList("PulseJNI")),location))
      throw new RuntimeException("Could not load PulseJNI library");
     nativeInitialize();
  }
  // Specify the location to find the PulseJNI library
  public static void initialize(String lib_dir)
  {
    if(!FileUtils.loadLibraries(new ArrayList<String>(Arrays.asList("PulseJNI")),lib_dir))
      throw new RuntimeException("Could not load PulseJNI library");
     nativeInitialize();
  }
  private static native void nativeInitialize();
  
  public static void deinitialize()
  {
    nativeDeinitialize();
  }
  private static native void nativeDeinitialize();
}

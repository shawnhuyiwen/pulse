package com.kitware.physiology.utilities;

public class jniBridge 
{ 
  public static boolean initialize()
  {
     return initialize();
  }
  private static native void nativeInitialize();
  
  public static void deinitialize()
  {
    nativeDeinitialize();
  }
  private static native void nativeDeinitialize();
}

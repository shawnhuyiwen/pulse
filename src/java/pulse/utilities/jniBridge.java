package pulse.utilities;

import java.util.ArrayList;
import java.util.Arrays;

public class jniBridge 
{ 
  static boolean loaded=false;
  public static void initialize()
  {
    if(loaded)
      return;
  	String location = System.getProperty("user.dir");
    if(!FileUtils.loadLibraries(new ArrayList<>(Arrays.asList("PulseJNI")),location))
      throw new RuntimeException("Could not load PulseJNI library");
     nativeInitialize();
     loaded = true;
  }
  // Specify the location to find the PulseJNI library
  public static void initialize(String lib_dir)
  {
    if(!FileUtils.loadLibraries(new ArrayList<>(Arrays.asList("PulseJNI")),lib_dir))
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

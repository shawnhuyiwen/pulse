/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.physiology.pulse.testing;

import com.kitware.physiology.utilities.jniBridge;
import com.kitware.physiology.utilities.Log;

import com.kitware.physiology.testing.SETestDriver;
import com.kitware.physiology.testing.SETestJob;

public class EngineUnitTestDriver implements SETestDriver.Executor
{
  
  static
  {
    jniBridge.initialize(System.getProperty("user.dir"));
  }
  
  protected long nativeObj;
  protected native long nativeAllocate();
  protected native void nativeDelete(long nativeObj);
  protected native void nativeExecute(long nativeObj, String testName, String outputDir);
  
  public EngineUnitTestDriver()
  {
    this.nativeObj = nativeAllocate();
  }
  
  protected void finalize()
  {
    nativeDelete(this.nativeObj);
  }
  
  public boolean ExecuteTest(SETestJob job)
  {
    nativeExecute(this.nativeObj,job.name,job.computedDirectory);
    Log.info("Completed running "+job.name);
    return true;
  }
  
  public static void main(String[] args)
  {
    SETestDriver.main(args);
  }

}


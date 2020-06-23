/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.engine.testing;

import com.kitware.pulse.cdm.testing.SETestDriver;
import com.kitware.pulse.cdm.testing.SETestJob;
import com.kitware.pulse.utilities.Log;
import com.kitware.pulse.utilities.JNIBridge;

public class EngineUnitTestDriver implements SETestDriver.Executor
{
  static
  {
    JNIBridge.initialize();
  }
  
  protected long nativeObj;
  protected native long nativeAllocate();
  protected native void nativeDelete(long nativeObj);
  protected native void nativeExecute(long nativeObj, String testName, String outputDir);
  
  public EngineUnitTestDriver()
  {
    this.nativeObj = nativeAllocate();
  }
  
  @Override
  protected void finalize()
  {
    nativeDelete(this.nativeObj);
  }
  
  @Override
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


/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.engine.testing;

import pulse.utilities.jniBridge;
import pulse.utilities.Log;

import pulse.cdm.testing.SETestDriver;
import pulse.cdm.testing.SETestJob;

public class EngineUnitTestDriver implements SETestDriver.Executor
{
  static
  {
    jniBridge.initialize();
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


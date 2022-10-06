/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.utilities;

import com.kitware.pulse.utilities.Log.Level;

public abstract class LogListener 
{
  protected Level level;
  protected boolean handleProgress;
  
  public LogListener()
  {
    level=Log.getLevel();
    handleProgress=false;
    listen(true);
  }
  
  public void listen(boolean b)
  {
    if(b)
      Log.addAppender(this);
    else
      Log.removeAppender(this);
  }
  
  public final void setLevel(Level lvl)
  {
    this.level=lvl;
  }
  public final void handleProgress(boolean b)
  {
    this.handleProgress=b;
  }
  
  public final void debug(String msg)
  {
    if(Log.isDebug(this.level))
    {
      handleDebug(msg);
    }
  }
  abstract protected void handleDebug(String msg);
  
  public final void info(String msg)
  {
    if(Log.isInfo(this.level))
    {
      handleInfo(msg);
    }
  }
  abstract protected void handleInfo(String msg);
  
  public final void warn(String msg)
  {
    if(Log.isWarn(this.level))
    {
      handleWarn(msg);
    }
  }
  abstract protected void handleWarn(String msg);
  
  public final void error(String msg)
  {
    if(Log.isError(this.level))
    {
      handleError(msg);
    }
  }
  abstract protected void handleError(String msg);
  
  public final void fatal(String msg)
  {
    if(Log.isFatal(this.level))
    {
      handleFatal(msg);
    }
  }
  abstract protected void handleFatal(String msg);
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.utilities;

import pulse.utilities.Log.Level;

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
  abstract protected void handleDebug(String msg);
  
  public final void debug(String msg, String origin)
  {
    if(Log.isDebug(this.level))
    {
      handleDebug(msg,origin);
    }
  }
  protected void handleDebug(String msg, String origin){handleDebug(origin + " : " + msg);}
  
  public final void debug(String msg, Throwable t)
  {
    if(Log.isDebug(this.level))
    {
      handleDebug(msg,t);
    }
  }
  protected void handleDebug(String msg, Throwable t){handleDebug(msg+" "+t.getMessage());}
  
  public final void info(String msg)
  {
    if(Log.isInfo(this.level))
    {
      handleInfo(msg);
    }
  }
  abstract protected void handleInfo(String msg);
  
  public final void info(String msg, String origin)
  {
    if(Log.isInfo(this.level))
    {
      handleInfo(msg,origin);
    }
  }
  protected void handleInfo(String msg, String origin){handleInfo(origin + " : " + msg);}
  
  public final void info(String msg, Throwable t)
  {
    if(Log.isInfo(this.level))
    {
      handleInfo(msg,t);
    }
  }
  protected void handleInfo(String msg, Throwable t){handleInfo(msg+" "+t.getMessage());}
  
  public final void warn(String msg)
  {
    if(Log.isWarn(this.level))
    {
      handleWarn(msg);
    }
  }
  abstract protected void handleWarn(String msg);
  
  public final void warn(String msg, String origin)
  {
    if(Log.isWarn(this.level))
    {
      handleWarn(msg,origin);
    }
  }
  protected void handleWarn(String msg, String origin){handleWarn(origin + " : " + msg);}
  
  public final void warn(String msg, Throwable t)
  {
    if(Log.isWarn(this.level))
    {
      handleWarn(msg,t);
    }
  }
  protected void handleWarn(String msg, Throwable t){handleWarn(msg+" "+t.getMessage());}
  
  public final void error(String msg)
  {
    if(Log.isError(this.level))
    {
      handleError(msg);
    }
  }
  abstract protected void handleError(String msg);
  
  public final void error(String msg, String origin)
  {
    if(Log.isError(this.level))
    {
      handleError(msg,origin);
    }
  }
  protected void handleError(String msg, String origin){handleError(origin + " : " + msg);}
  
  public final void error(String msg, Throwable t)
  {
    if(Log.isError(this.level))
    {
      handleError(msg,t);
    }
  }
  protected void handleError(String msg, Throwable t){handleError(msg+" "+t.getMessage());}
  
  public final void error(Throwable t)
  {
    if(Log.isError(this.level))
    {
      handleError(t);
    }
  }
  protected void handleError(Throwable t){handleError(t.getMessage());}
  
  public final void fatal(String msg)
  {
    if(Log.isFatal(this.level))
    {
      handleFatal(msg);
    }
  }
  abstract protected void handleFatal(String msg);
  
  public final void fatal(String msg, String origin)
  {
    if(Log.isFatal(this.level))
    {
      handleFatal(msg,origin);
    }
  }
  protected void handleFatal(String msg, String origin){handleFatal(origin + " : " + msg);}
  
  public final void fatal(String msg, Throwable t)
  {
    if(Log.isFatal(this.level))
    {
      handleFatal(msg,t);
    }
  }
  protected void handleFatal(String msg, Throwable t){handleFatal(msg+" "+t.getMessage());}
  
  public final void fatal(Throwable t)
  {
    if(Log.isFatal(this.level))
    {
      handleFatal(t);
    }
  }
  protected void handleFatal(Throwable t){handleFatal(t.getMessage());}
  
  public final void progress(String state, String info, float percent)
  {
    if(this.handleProgress)
    {
      handleProgress(state, info, percent);
    }
  }
  
  // since the default is not to handle progress, leaving this with a default nop implementation
  protected void handleProgress(String status, String info, float percent) {}
}

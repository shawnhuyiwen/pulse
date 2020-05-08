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
  public void handleDebug(String msg){}
  
  public final void debug(String msg, String origin)
  {
    if(Log.isDebug(this.level))
    {
      handleDebug(msg,origin);
    }
  }
  public void handleDebug(String msg, String origin){handleDebug(origin + " : " + msg);}
  
  public final void debug(String msg, Throwable t)
  {
    if(Log.isDebug(this.level))
    {
      handleDebug(msg,t);
    }
  }
  public void handleDebug(String msg, Throwable t){handleDebug(msg+" "+t.getMessage());}
  
  public final void info(String msg)
  {
    if(Log.isInfo(this.level))
    {
      handleInfo(msg);
    }
  }
  public void handleInfo(String msg){}
  
  public final void info(String msg, String origin)
  {
    if(Log.isInfo(this.level))
    {
      handleInfo(msg,origin);
    }
  }
  public void handleInfo(String msg, String origin){handleInfo(origin + " : " + msg);}
  
  public final void info(String msg, Throwable t)
  {
    if(Log.isInfo(this.level))
    {
      handleInfo(msg,t);
    }
  }
  public void handleInfo(String msg, Throwable t){handleInfo(msg+" "+t.getMessage());}
  
  public final void warn(String msg)
  {
    if(Log.isWarn(this.level))
    {
      handleWarn(msg);
    }
  }
  public void handleWarn(String msg){}
  
  public final void warn(String msg, String origin)
  {
    if(Log.isWarn(this.level))
    {
      handleWarn(msg,origin);
    }
  }
  public void handleWarn(String msg, String origin){handleWarn(origin + " : " + msg);}
  
  public final void warn(String msg, Throwable t)
  {
    if(Log.isWarn(this.level))
    {
      handleWarn(msg,t);
    }
  }
  public void handleWarn(String msg, Throwable t){handleWarn(msg+" "+t.getMessage());}
  
  public final void error(String msg)
  {
    if(Log.isError(this.level))
    {
      handleError(msg);
    }
  }
  public void handleError(String msg){}
  
  public final void error(String msg, String origin)
  {
    if(Log.isError(this.level))
    {
      handleError(msg,origin);
    }
  }
  public void handleError(String msg, String origin){handleError(origin + " : " + msg);}
  
  public final void error(String msg, Throwable t)
  {
    if(Log.isError(this.level))
    {
      handleError(msg,t);
    }
  }
  public void handleError(String msg, Throwable t){handleError(msg+" "+t.getMessage());}
  
  public final void error(Throwable t)
  {
    if(Log.isError(this.level))
    {
      handleError(t);
    }
  }
  public void handleError(Throwable t){handleError(t.getMessage());}
  
  public final void fatal(String msg)
  {
    if(Log.isFatal(this.level))
    {
      handleFatal(msg);
    }
  }
  public void handleFatal(String msg){}
  
  public final void fatal(String msg, String origin)
  {
    if(Log.isFatal(this.level))
    {
      handleFatal(msg,origin);
    }
  }
  public void handleFatal(String msg, String origin){handleFatal(origin + " : " + msg);}
  
  public final void fatal(String msg, Throwable t)
  {
    if(Log.isFatal(this.level))
    {
      handleFatal(msg,t);
    }
  }
  public void handleFatal(String msg, Throwable t){handleFatal(msg+" "+t.getMessage());}
  
  public final void fatal(Throwable t)
  {
    if(Log.isFatal(this.level))
    {
      handleFatal(t);
    }
  }
  public void handleFatal(Throwable t){handleFatal(t.getMessage());}
  
  public final void progress(String state, String info, float percent)
  {
    if(this.handleProgress)
    {
      handleProgress(state, info, percent);
    }
  }
  public void handleProgress(String status, String info, float percent){}
}

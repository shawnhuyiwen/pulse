/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.utilities;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * @author abray
 *
 */
public class Log
{
  enum Level
  {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
  }
  
  public static boolean isFatal(Level level)
  {
    return level == Level.FATAL;
  }

  public static boolean isError(Level level)
  {
    return isFatal(level) || level == Level.ERROR;
  }

  public static boolean isWarn(Level level)
  {
    return isError(level) || level == Level.WARN;
  }

  public static boolean isInfo(Level level)
  {
    return isWarn(level) || level == Level.INFO;
  }

  public static boolean isDebug(Level level)
  {
    return isInfo(level) || level == Level.DEBUG;
  }

  
  private static Level level=Level.INFO;
  public static boolean output2Console = true;
  private static Set<String> ignoreClasses=new HashSet<>();
  static
  {
    ignoreClasses.add("Log");
    ignoreClasses.add("TimingProfile");
  }

  /** Key is the thread ID you want this listener to listen to*/
  private static Map<String,Set<LogListener>> appenders=new HashMap<>(); 

  public synchronized static void addAppender(LogListener appender)
  {
    String tName = Thread.currentThread().getName();
    Set<LogListener> listeners = appenders.get(tName);
    if(listeners==null)
    {
      listeners = new HashSet<>();
      appenders.put(tName, listeners);
    }
    listeners.add(appender);
  }

  public synchronized static void addAppender(String threadName, LogListener appender)
  {
    Set<LogListener> listeners = appenders.get(threadName);
    if(listeners==null)
    {
      listeners = new HashSet<>();
      appenders.put(threadName, listeners);
    }
    listeners.add(appender);
  }

  public synchronized static void removeAppender(LogListener appender)
  {
    Set<LogListener> listeners;
    for(String tName : appenders.keySet())
    {
      listeners=appenders.get(tName);
      if(listeners!=null)
        listeners.remove(appender);
    }  
  }
  
  public synchronized static void clearAppenders()
  {
    appenders.clear();
  }

  protected synchronized static String getOrigin()
  {
    return CurrentClass.getTrimmedName(ignoreClasses);
  }

  public synchronized static Level getLevel()
  {
    return level;
  }
  
  public synchronized static void setLevel(Level level)
  {
    Log.level=level;
  }

 
  private Log()
  {

  }

  public synchronized static void ignoreClass(String name)
  {
    ignoreClasses.add(name);
  }

  public synchronized static void debug(String msg)
  {
    if(output2Console && isDebug(level))
      System.out.println("[DEBUG] "+getOrigin()+"::"+msg);
    String tName = Thread.currentThread().getName();
    Set<LogListener> listeners=appenders.get(tName);
    if(listeners!=null)
    {
      for(LogListener a : listeners)
        a.debug(msg);
    }
  }

  public synchronized static void debug(String msg, String origin)
  {
    if(output2Console && isDebug(level))
      System.out.println("[DEBUG] "+origin+"::"+msg);
    String tName = Thread.currentThread().getName();
    Set<LogListener> listeners=appenders.get(tName);
    if(listeners!=null)
    {
      for(LogListener a : listeners)
        a.debug(msg,origin);
    }
  }

  public synchronized static void debug(String msg, Throwable t)
  {
    if(output2Console && isDebug(level))
      System.out.println("[DEBUG] "+getOrigin()+"::"+msg+t);
    String tName = Thread.currentThread().getName();
    Set<LogListener> listeners=appenders.get(tName);
    if(listeners!=null)
    {
      for(LogListener a : listeners)
        a.debug(msg,t);
    }
  }

  public synchronized static void info(String msg)
  {
    if(output2Console && isInfo(level))
      System.out.println("[INFO] "+getOrigin()+"::"+msg);
    String tName = Thread.currentThread().getName();
    Set<LogListener> listeners=appenders.get(tName);
    if(listeners!=null)
    {
      for(LogListener a : listeners)
        a.info(msg);
    }
  }

  public synchronized static void info(String msg, String origin)
  {
    if(output2Console && isInfo(level))
      System.out.println("[INFO] "+origin+"::"+msg);
    String tName = Thread.currentThread().getName();
    Set<LogListener> listeners=appenders.get(tName);
    if(listeners!=null)
    {
      for(LogListener a : listeners)
        a.info(msg,origin);
    }
  }

  public synchronized static void info(String msg, Throwable t)
  {
    if(output2Console && isInfo(level))
      System.out.println("[INFO] "+getOrigin()+"::"+msg+t);
    String tName = Thread.currentThread().getName();
    Set<LogListener> listeners=appenders.get(tName);
    if(listeners!=null)
    {
      for(LogListener a : listeners)
        a.info(msg,t);
    }
  }

  public synchronized static void warn (String msg)
  {
    if(output2Console && isWarn(level))
      System.out.println("[WARN] "+getOrigin()+"::"+msg);
    String tName = Thread.currentThread().getName();
    Set<LogListener> listeners=appenders.get(tName);
    if(listeners!=null)
    {
      for(LogListener a : listeners)
        a.warn(msg);
    }
  }

  public synchronized static void warn(String msg, String origin)
  {
    if(output2Console && isWarn(level))
      System.out.println("[WARN] "+origin+"::"+msg);
    String tName = Thread.currentThread().getName();
    Set<LogListener> listeners=appenders.get(tName);
    if(listeners!=null)
    {
      for(LogListener a : listeners)
        a.warn(msg,origin);
    }
  }

  public synchronized static void warn(String msg, Throwable t)
  {
    if(output2Console && isWarn(level))
      System.out.println("[WARN] "+getOrigin()+"::"+msg+t);
    String tName = Thread.currentThread().getName();
    Set<LogListener> listeners=appenders.get(tName);
    if(listeners!=null)
    {
      for(LogListener a : listeners)
        a.warn(msg,t);
    }
  }

  public synchronized static void error(String msg)
  {
    if(output2Console && isError(level))
      System.out.println("[ERROR] "+getOrigin()+"::"+msg);
    String tName = Thread.currentThread().getName();
    Set<LogListener> listeners=appenders.get(tName);
    if(listeners!=null)
    {
      for(LogListener a : listeners)
        a.error(msg);
    }
  }

  public synchronized static void error(String msg, String origin)
  {
    if(output2Console && isError(level))
      System.out.println("[ERROR] "+origin+"::"+msg);
    String tName = Thread.currentThread().getName();
    Set<LogListener> listeners=appenders.get(tName);
    if(listeners!=null)
    {
      for(LogListener a : listeners)
        a.error(msg,origin);
    }
  }

  public synchronized static void error(String msg, Throwable t)
  {
    if(output2Console && isError(level))
      System.out.println("[ERROR] "+getOrigin()+"::"+msg+t);
    String tName = Thread.currentThread().getName();
    Set<LogListener> listeners=appenders.get(tName);
    if(listeners!=null)
    {
      for(LogListener a : listeners)
        a.error(msg,t);
    }
  }

  public synchronized static void error(Throwable t)
  {
    if(output2Console && isError(level))
      System.out.println("[ERROR] "+getOrigin()+t);
    String tName = Thread.currentThread().getName();
    Set<LogListener> listeners=appenders.get(tName);
    if(listeners!=null)
    {
      for(LogListener a : listeners)
        a.error(t);
    }
  }

  public synchronized static void fatal(String msg)
  {
    if(output2Console && isFatal(level))
      System.out.println("[FATAL] "+getOrigin()+"::"+msg);
    String tName = Thread.currentThread().getName();
    Set<LogListener> listeners=appenders.get(tName);
    if(listeners!=null)
    {
      for(LogListener a : listeners)
        a.fatal(msg);
    }
  }

  public synchronized static void fatal(String msg, String origin)
  {
    if(output2Console && isFatal(level))
      System.out.println("[FATAL] "+origin+"::"+msg);
    String tName = Thread.currentThread().getName();
    Set<LogListener> listeners=appenders.get(tName);
    if(listeners!=null)
    {
      for(LogListener a : listeners)
        a.fatal(msg,origin);
    }
  }

  public synchronized static void fatal(String msg, Throwable t)
  {
    if(output2Console && isFatal(level))
      System.out.println("[FATAL] "+getOrigin()+"::"+msg+t);
    String tName = Thread.currentThread().getName();
    Set<LogListener> listeners=appenders.get(tName);
    if(listeners!=null)
    {
      for(LogListener a : listeners)
        a.fatal(msg,t);
    }
  }

  public synchronized static void fatal(Throwable t)
  {
    if(output2Console && isFatal(level))
      System.out.println("[FATAL] "+getOrigin()+t);
    String tName = Thread.currentThread().getName();
    Set<LogListener> listeners=appenders.get(tName);
    if(listeners!=null)
    {
      for(LogListener a : listeners)
        a.fatal(t);
    }
  }
}

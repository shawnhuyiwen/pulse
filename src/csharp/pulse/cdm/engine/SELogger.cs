/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;

namespace Pulse.CDM
{
  public interface ILogListener
  {
    void Debug(string msg);
    void Info(string msg);
    void Warning(string msg);
    void Error(string msg);
    void Fatal(string msg);
  }

  public class SELogMessages
  {
    public List<string> debug_msgs = new List<string>();
    public List<string> info_msgs = new List<string>();
    public List<string> warning_msgs = new List<string>();
    public List<string> error_msgs = new List<string>();
    public List<string> fatal_msgs = new List<string>();

    public void Clear()
    {
      debug_msgs.Clear();
      info_msgs.Clear();
      warning_msgs.Clear();
      error_msgs.Clear();
      fatal_msgs.Clear();
    }

    public void Forward(ILogListener listener)
    {
      foreach (string msg in debug_msgs)
        listener.Debug(msg);
      foreach (string msg in info_msgs)
        listener.Info(msg);
      foreach (string msg in warning_msgs)
        listener.Warning(msg);
      foreach (string msg in error_msgs)
        listener.Error(msg);
      foreach (string msg in fatal_msgs)
        listener.Fatal(msg);
    }
  }
}

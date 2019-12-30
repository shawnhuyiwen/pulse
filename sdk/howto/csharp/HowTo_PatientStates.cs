using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HowTo_PatientStates
{
  public class MyEventHandler : EventHandler
  {
    public void HandleEvent(SEEventChange change)
    {
      Console.WriteLine(change.ToString());
    }
  }

  public class MyLogListener : LogListener
  {
    public void Debug(string msg) { Console.WriteLine("[DEBUG] " + msg); }
    public void Info(string msg) { Console.WriteLine("[INFO] " + msg); }
    public void Warning(string msg) { Console.WriteLine("[WARN] " + msg); }
    public void Error(string msg) { Console.WriteLine("[ERROR] " + msg); }
    public void Fatal(string msg) { Console.WriteLine("[FATAL] " + msg); }
  }

  class Example
  {
    public static void Run()
    {

    }
  }
}

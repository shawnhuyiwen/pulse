using System;
using System.Threading;
using System.Diagnostics;

namespace HowToCSharp
{
  public class PulseEventHandler : PulseEngineEventCallbacksRef
  {
    public override void HandlePatientEvent(ePatientEvent e, bool active)
    {
      if (!active)
        return;

      switch(e)
      {
        case ePatientEvent.StartOfInhale:
          {
            Console.WriteLine("Inhale");
            break;
          }
      }
    }
  }

  public class PulseLogHandler : PulseLoggerRef
  {
    protected override void OnFatal(string msg) { Console.WriteLine("[Fatal] " + msg); }
    protected override void OnError(string msg) { Console.WriteLine("[Error] " + msg); }
    protected override void OnWarn(string msg) { Console.WriteLine("[Warn] " + msg); }
    protected override void OnInfo(string msg) { Console.WriteLine("[Info] " + msg); }
    protected override void OnDebug(string msg) { Console.WriteLine("[Debug] " + msg); }
  }

  class Program
  {
    static void Main(string[] args)
    {
      PulseEngineRef    Pulse;
      PulseEventHandler Events;
      PulseLogHandler   Logger;

      Pulse = new PulseEngineRef("RealTimePulse.log");
      Pulse.LoadStateFile("./states/StandardMale@0s.pba",0);
      Logger = new PulseLogHandler();
      Logger.SetupCallbacks(Pulse);
      Events = new PulseEventHandler();
      Pulse.SetPulseEventCallback(Events);

      // Simulate 120s and do it in real time
      int seconds = 0;
      int duration_ms;
      Stopwatch stopWatch = new Stopwatch();
      while (seconds < 120)
      {
        stopWatch.Reset();
        stopWatch.Start();
        Pulse.AdvanceModelTime_s(1);
        Console.WriteLine("Total Lung Volume (mL) : "   + Pulse.GetVitalsData().TotalLungVolume_mL + "\n" +
                          "Respiratory Rate (1/min) : " + Pulse.GetVitalsData().RespiratoryRate_per_min + "\n" +
                          "Heart Rate (1/min) : "       + Pulse.GetVitalsData().HeartRate_per_min + "\n");
        // See how long it took us to run a second, and sleep
        duration_ms = (int)stopWatch.Elapsed.TotalMilliseconds;
        if (duration_ms > 1000)
          Console.WriteLine("Pulse is running slower than real time!!! : " + duration_ms);
        else
          Thread.Sleep(1000 - duration_ms);
        stopWatch.Stop();
        seconds++;
      }

      // IF you are going to run pulse in its own thread, use a Thread, and not a Timer!!
      // Create the thread object, passing in the Alpha.Beta method
      // via a ThreadStart delegate. This does not start the thread.
      //Thread oThread = new Thread(new ThreadStart(pulse.run));
      // Start the thread
      //oThread.Start();
      // Spin for a while waiting for the started thread to become alive:
      //while (!oThread.IsAlive);
      // do
      //   Thread.Sleep(1000);
      //   Stuff...
      // Request that oThread be stopped
      //oThread.Abort();
      // Wait until oThread finishes. Join also has overloads
      // that take a millisecond interval or a TimeSpan object.
      //oThread.Join();

      Console.WriteLine();
      Console.WriteLine("Pulse has finished");
    }

  }
}

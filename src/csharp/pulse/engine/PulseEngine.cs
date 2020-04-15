/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Pulse.CDM;

namespace Pulse
{
  // C# class that wraps the PulseC API
  public class PulseEngine
  {
    private const string PulseLib = "PulseC";  // Name of our Pulse library (PulseC or PulseCd)
    readonly IntPtr pulse_cptr;           // Pointer to the pulse engine in C
    private double[] data_values;          // Data coming back from the engine
    private double time_step_s = 0.02;   // Time step (TODO Get this from engine)
    private double extra_time_s = 0;     // Time remainder from spliting requested time into time steps
    private ILogListener log_listener = null;  // Forward log messages to this object
    private SELogMessages log_messages = new SELogMessages();
    private IEventHandler event_handler = null; // Forware events to this object
    private List<SEEventChange> event_changes = new List<SEEventChange>();
    private IntPtr str_addr;             // Used to hold data between C# and C
    private SerializationFormat thunk_as = SerializationFormat.JSON;
    // TODO when we allow binary thunking, add many if/switchs below!

    [DllImport(PulseLib, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    public static extern void Initialize();

    [DllImport(PulseLib, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    public static extern void Deinitialize();

    [DllImport(PulseLib, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern IntPtr Allocate(string log_filename, bool enable_cout, string data_dir);
    PulseEngine() { } // make default constructor private
    public PulseEngine(string log_filename, string data_dir)
    {
      pulse_cptr = Allocate(log_filename, true, data_dir);
    }
    public PulseEngine(string log_filename, bool enable_cout, string data_dir)
    {
      pulse_cptr = Allocate(log_filename, enable_cout, data_dir);
    }

    [DllImport(PulseLib, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern void Deallocate(IntPtr pulse);
    ~PulseEngine()
    {
      Deallocate(pulse_cptr);
    }

    [DllImport(PulseLib, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern bool SerializeFromFile(IntPtr pulse, string filename, string data_mgr, int format, double sim_time_s);
    public bool SerializeFromFile(string filename, SEDataRequestManager data_mgr, double sim_time_s, SerializationFormat format)
    {
      data_values = new double[data_mgr.GetDataRequests().Count + 1];
      string data_mgr_str = PBDataRequest.SerializeToString(data_mgr);
      return SerializeFromFile(pulse_cptr, filename, data_mgr_str, (int)format, sim_time_s);
    }

    [DllImport(PulseLib, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern bool SerializeToFile(IntPtr pulse, string filename, int format);
    public bool SerializeToFile(string filename, SerializationFormat format)
    {
      return SerializeToFile(pulse_cptr, filename, (int)format);
    }

    [DllImport(PulseLib, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern bool SerializeFromString(IntPtr pulse, string state, string data_mgr, int format, double sim_time_s);
    public bool SerializeFromString(string state, SEDataRequestManager data_mgr, double sim_time_s, SerializationFormat format)
    {
      data_values = new double[data_mgr.GetDataRequests().Count + 1];
      string data_mgr_str = PBDataRequest.SerializeToString(data_mgr);
      return SerializeFromString(pulse_cptr, state, data_mgr_str, (int)format, sim_time_s);
    }

    [DllImport(PulseLib, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern bool SerializeToString(IntPtr pulse, int format, out IntPtr state_str);
    public string SerializeToString(SerializationFormat format)
    {
      if (!SerializeToString(pulse_cptr, (int)format, out str_addr))
        return null;
      string state_str = System.Runtime.InteropServices.Marshal.PtrToStringAnsi(str_addr);
      return state_str;
    }

    [DllImport(PulseLib, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern bool InitializeEngine(IntPtr pulse, string patient_configuration, string data_requests, int format);
    public bool InitializeEngine(SEPatientConfiguration patient_configuration, SEDataRequestManager data_mgr)
    {
      data_values = new double[data_mgr.GetDataRequests().Count + 1];
      string patient_configuration_str = PBPatientConfiguration.SerializeToString(patient_configuration);
      string data_mgr_str = PBDataRequest.SerializeToString(data_mgr);
      return InitializeEngine(pulse_cptr, patient_configuration_str, data_mgr_str, (int)thunk_as);
    }

    [DllImport(PulseLib, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern void KeepLogMessages(IntPtr pulse, bool save);// Let the engine know to save log msgs or not
    public void SetLogListener(ILogListener listener)
    {
      log_listener = listener;
      KeepLogMessages(pulse_cptr, log_listener != null);
    }
    [DllImport(PulseLib, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern bool PullLogMessages(IntPtr pulse, int format, out IntPtr event_changes);

    [DllImport(PulseLib, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern void KeepEventChanges(IntPtr pulse, bool keep);// Let the engine know to save events or not
    public void SetEventHandler(IEventHandler handler)
    {
      event_handler = handler;
      KeepEventChanges(pulse_cptr, event_handler != null);
    }
    [DllImport(PulseLib, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern bool PullEvents(IntPtr pulse, int format, out IntPtr event_changes);

    [DllImport(PulseLib, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern bool PullActiveEvents(IntPtr pulse, int format, out IntPtr active_events);
    public bool PullActiveEvents(List<SEActiveEvent> active_events)
    {
      active_events.Clear();
      if (!PullActiveEvents(pulse_cptr, (int)thunk_as, out str_addr))
        return false;
      string activities = System.Runtime.InteropServices.Marshal.PtrToStringAnsi(str_addr);
      if (activities == null)
        return false;
      PBEvents.SerializeFromString(activities, active_events);
      return true;
    }

    [DllImport(PulseLib, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern bool ProcessActions(IntPtr pulse, string any_action_list, int format);
    public bool ProcessAction(SEAction action)
    {
      List<SEAction> actions = new List<SEAction>();
      actions.Add(action);
      return ProcessActions(actions);
    }
    public bool ProcessActions(List<SEAction> actions)
    {
      string any_action_list_str = PBAction.SerializeToString(actions);
      //System.Console.Out.Write(any_action_list_str);
      return ProcessActions(pulse_cptr, any_action_list_str, (int)thunk_as);
    }

    [DllImport(PulseLib, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern bool AdvanceTimeStep(IntPtr pulse);
    public bool AdvanceTimeStep()
    {
      bool b = AdvanceTimeStep(pulse_cptr);
      // Grab events and pass them to handler
      if (event_handler != null)
      {
        event_changes.Clear();
        if (PullEvents(pulse_cptr, (int)thunk_as, out str_addr))
        {
          string event_changes_str = System.Runtime.InteropServices.Marshal.PtrToStringAnsi(str_addr);
          if (event_changes_str != null)
          {
            PBEvents.SerializeFromString(event_changes_str, event_changes);
            foreach (var change in event_changes)
            {
              event_handler.HandleEvent(change);
            }
          }
        }
      }
      // Grab log messages and pass them to handler
      if (log_listener != null)
      {
        log_messages.Clear();
        if (PullLogMessages(pulse_cptr, (int)thunk_as, out str_addr))
        {
          string log_messages_str = System.Runtime.InteropServices.Marshal.PtrToStringAnsi(str_addr);
          if (log_messages_str != null)
          {
            PBLogMessages.SerializeFromString(log_messages_str, log_messages);
            log_messages.Forward(log_listener);
          }
        }
      }

      return b;
    }
    public bool AdvanceTime_s(double duration)
    {
      duration += extra_time_s;
      int steps = (int)Math.Floor(duration / time_step_s);
      extra_time_s = duration - (steps * time_step_s);
      for (int i = 0; i < steps; i++)
      {
        if (!AdvanceTimeStep())
          return false;
      }
      return true;
    }

    [DllImport(PulseLib, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern IntPtr PullData(IntPtr pulse);
    public double[] PullData()
    {
      IntPtr data = PullData(pulse_cptr);
      Marshal.Copy(data, data_values, 0, data_values.Length);
      return data_values;
    }
  }
}

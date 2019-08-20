/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

// C# class that wraps the PulseC API
public class PulseEngine
{
    readonly IntPtr pulseEngineCPtr;        // Pointer to the pulse engine in C

    [DllImport("PulseC", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    public static extern void Initialize();

    [DllImport("PulseC", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    public static extern void Deinitialize();

    [DllImport("PulseC", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern IntPtr Allocate(string logfile, string data_dir);
    PulseEngine() {} // make default constructor private
    public PulseEngine(string logFilePath, string data_dir)
    {
        pulseEngineCPtr = Allocate(logFilePath, data_dir);
    }

    [DllImport("PulseC", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern void Deallocate(IntPtr pulse);
    ~PulseEngine()
    {
        Deallocate(pulseEngineCPtr);
    }

    [DllImport("PulseC", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern bool SerializeFromFile(IntPtr pulse, string filename, string data_requests, int format, double sim_time_s);
    public bool SerializeFromFile(string filename, string data_requests, SerializationFormat format, double sim_time_s)
    {
        return SerializeFromFile(pulseEngineCPtr, filename, data_requests, (int)format, sim_time_s);
    }

    [DllImport("PulseC", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern bool SerializeToFile(IntPtr pulse, string filename, int format);
    public bool SerializeToFile(string filename, SerializationFormat format)
    {
        return SerializeToFile(pulseEngineCPtr, filename, (int)format);
    }

    [DllImport("PulseC", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern bool SerializeFromString(IntPtr pulse, string state, string data_requests, int format, double sim_time_s);
    public bool SerializeFromString(string state, string data_requests, SerializationFormat format, double sim_time_s)
    {
        return SerializeFromString(pulseEngineCPtr, state, data_requests, (int)format, sim_time_s);
    }

    [DllImport("PulseC", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern string SerializeToString(IntPtr pulse, int format);
    public string SerializeToString(SerializationFormat format)
    {
        return SerializeToString(pulseEngineCPtr, (int)format);
    }

    [DllImport("PulseC", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern void InitializeEngine(IntPtr pulse, string patient_configuration, string data_requests, int format);
    public void InitializeEngine(string patient_configuration, string data_requests, SerializationFormat format)
    {
        InitializeEngine(pulseEngineCPtr, patient_configuration, data_requests, (int)format);
    }

    [DllImport("PulseC", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern bool AdvanceTime_s(IntPtr pulse, double time);
    public bool AdvanceTime_s(double timeStep)
    {
        return AdvanceTime_s(pulseEngineCPtr, timeStep);
    }

    [DllImport("PulseC", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern IntPtr PullData(IntPtr pulse);
    public IntPtr PullData()
    {
        return PullData(pulseEngineCPtr);
    }
    
    [DllImport("PulseC", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    static extern bool ProcessActions(IntPtr pulse, string any_action_list, int format);
    public bool ProcessAction(SEAction action)
    {
        List<SEAction> actions = new List<SEAction>();
        actions.Add(action);
        return ProcessActions(actions);
    }
    public bool ProcessActions(List<SEAction> actions)
    {
        var pb = PBAction.Unload(actions);
        string any_action_list_str = pb.ToString();
        Console.WriteLine(any_action_list_str);
        return ProcessActions(pulseEngineCPtr, any_action_list_str, (int)SerializationFormat.JSON);
    }
}

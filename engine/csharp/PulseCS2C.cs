using System;
using System.Runtime.InteropServices;

namespace Pulse
{
    // Below is a subset of methods availible in the generic PhysiologyEngine.h Interface
    // This allows allocating and controling of a Pulse Engine
    // format indicates the serialization format of the string or file content being passed
    // Possible valuse are { BINARY=0, ASCII=1 };
    // NOTE these intefaces do not yet have support for engine configuration object
    // It can be added if needded
    public enum SerializationFormat : int { BINARY=0, ASCII }
    public class PulseCS2C
    {
        [DllImport("PulseC", CharSet=CharSet.Ansi, CallingConvention=CallingConvention.StdCall)]
        public static extern IntPtr Allocate(string logfile);

        [DllImport("PulseC", CharSet=CharSet.Ansi, CallingConvention=CallingConvention.StdCall)]
        public static extern void Deallocate(IntPtr pulse);

        [DllImport("PulseC", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.StdCall)]
        public static extern bool SerializeFromFile(IntPtr pulse, string filename, string data_requests, int format, double sim_time_s);

        [DllImport("PulseC", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.StdCall)]
        public static extern bool SerializeToFile(IntPtr pulse, string filename, int format);

        [DllImport("PulseC", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.StdCall)]
        public static extern bool SerializeFromString(IntPtr pulse, string state, string data_requests, int format, double sim_time_s);

        [DllImport("PulseC", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.StdCall)]
        public static extern string SerializeToString(IntPtr pulse, int format);

        [DllImport("PulseC", CharSet = CharSet.Ansi, CallingConvention=CallingConvention.StdCall)]
        public static extern void InitializeEngine(IntPtr pulse, string patient_configuration, string data_requests, int format);
        
        [DllImport("PulseC", CharSet=CharSet.Ansi, CallingConvention=CallingConvention.StdCall)]
        public static extern bool AdvanceTime_s(IntPtr pulse, double time);

        [DllImport("PulseC", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr PullData(IntPtr pulse);
    }
}

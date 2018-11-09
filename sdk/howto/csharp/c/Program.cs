using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using Pulse;

namespace HowTo_UseC
{
    class Program
    {
        static void Main(string[] args)
        {
            // We are not providing data requests, so this will default to vitals dta
            double[] result = new double[12];
            List<string> headings = new List<string>();
            headings.Add("SimTime(s)");
            headings.Add("Lead3ElectricPotential(mV)");
            headings.Add("HeartRate(1 / min)");
            headings.Add("ArterialPressure(mmHg)");
            headings.Add("MeanArterialPressure(mmHg)");
            headings.Add("SystolicArterialPressure(mmHg)");
            headings.Add("DiastolicArterialPressure(mmHg)");
            headings.Add("OxygenSaturation");
            headings.Add("EndTidalCarbonDioxidePressure(mmHg)");
            headings.Add("RespirationRate(1 / min)");
            headings.Add("SkinTemperature(degC)");
            headings.Add("Carina-CarbonDioxide-PartialPressure(mmHg)");

            // Instatiate a Pulse engine
            IntPtr pulse = PulseCS2C.Allocate("pulse.log",".");
            // Load a state file
            //if(!PulseCS2C.SerializeFromFile(pulse, "./states/Soldier@0s.pba", null, (int)SerializationFormat.ASCII, 0))
            //    Console.WriteLine("Error Initializing Pulse!");
            string file_content = File.ReadAllText("./states/Soldier@0s.pba");
            if (!PulseCS2C.SerializeFromString(pulse, file_content, null, (int)SerializationFormat.ASCII, 0))
                Console.WriteLine("Error Initializing Pulse!");
            // Advance time and print out values
            for (int i = 1; i <= 10; i++)
            {
                if (!PulseCS2C.AdvanceTime_s(pulse, 1))
                    Console.WriteLine("Error Advancing Time!");
                else
                {
                    // Pull data from pulse
                    IntPtr data = PulseCS2C.PullData(pulse);
                    Marshal.Copy(data, result, 0, 12);
                    for (int d = 0; d < 12; d++)
                        Console.WriteLine(headings[d]+" " + result[d]);
                }
            }
        }
    }
}

using System;
using System.Collections.Generic;
using Pulse;
using Pulse.CDM;

namespace HowTo_PatientStates
{
  class Example
  {
    public static void Run()
    {
      // Here is an array to store Active Events
      List<SEActiveEvent> active_events = new List<SEActiveEvent>();
      // Create a list of Data Requests to specify all the data we want from Pulse
      List<SEDataRequest> data_requests = new List<SEDataRequest>
      {
        SEDataRequest.CreatePhysiologyRequest("HeartRate", "1/min"),
        SEDataRequest.CreatePhysiologyRequest("ArterialPressure", "mmHg"),
        SEDataRequest.CreatePhysiologyRequest("MeanArterialPressure", "mmHg"),
        SEDataRequest.CreatePhysiologyRequest("SystolicArterialPressure", "mmHg"),
        SEDataRequest.CreatePhysiologyRequest("DiastolicArterialPressure", "mmHg"),
        SEDataRequest.CreatePhysiologyRequest("OxygenSaturation"),
        SEDataRequest.CreatePhysiologyRequest("EndTidalCarbonDioxidePressure", "mmHg"),
        SEDataRequest.CreatePhysiologyRequest("RespirationRate", "1/min"),
        SEDataRequest.CreatePhysiologyRequest("SkinTemperature", "degC"),
        SEDataRequest.CreateGasCompartmentSubstanceRequest("Carina", "CarbonDioxide", "PartialPressure", "mmHg"),
        SEDataRequest.CreatePhysiologyRequest("BloodVolume", "mL"),
        SEDataRequest.CreateECGRequest("Lead3ElectricPotential", "mV"),
      };
      SEDataRequestManager data_mgr = new SEDataRequestManager(data_requests);
      // Create a reference to a double[] that will contain the data returned from Pulse
      double[] data_values;
      // data_values[0] is ALWAYS the simulation time in seconds
      // The rest of the data values are in order of the data_requests list provided

      PulseEngine pulse = new PulseEngine(null, true, ".");
      double ms_to_s = 0.001;

      // Keep these 2 lists in sync
      List<string> injury_set_names = new List<string>();
      List<List<SEAction>> injury_sets = new List<List<SEAction>>();

      // First Set of Injuries
      injury_set_names.Add("MinorLegBleed");
      List<SEAction> injury_set_1 = new List<SEAction>();
      SEHemorrhage h1 = new SEHemorrhage();
      h1.SetType(SEHemorrhage.eType.External);
      h1.SetCompartment("RightLeg");
      h1.GetRate().SetValue(33, VolumePerTimeUnit.mL_Per_min);
      injury_set_1.Add(h1);
      injury_sets.Add(injury_set_1);

      // Second Set of Injuries
      injury_set_names.Add("ModerateLegBleed");
      List<SEAction> injury_set_2 = new List<SEAction>();
      SEHemorrhage h2 = new SEHemorrhage();
      h2.SetType(SEHemorrhage.eType.External);
      h2.SetCompartment("RightLeg");
      h2.GetRate().SetValue(66, VolumePerTimeUnit.mL_Per_min);
      injury_set_2.Add(h2);
      injury_sets.Add(injury_set_2);

      for (int i = 0; i < injury_sets.Count; i++)
      {
        Console.WriteLine("Creating states for inury set : " + injury_set_names[i]);
        // Loop over how many patients we have and the various injury combinations
        if (!pulse.SerializeFromFile("./states/Soldier@0s.json", data_mgr, 0, SerializationFormat.JSON))
        {
          Console.WriteLine("Error Initializing Pulse!");
          return;
        }
        // Apply the injury
        pulse.ProcessActions(injury_sets[i]);
        // Advance time and create a new state every Xm for Ym
        int advance_min = 2;
        int final_time_min = 20;
        int current_time_min = 0;
        while (current_time_min < final_time_min)
        {
          var watch = System.Diagnostics.Stopwatch.StartNew();

          current_time_min += advance_min;
          if (!pulse.AdvanceTime_s(advance_min * 60))
            Console.WriteLine("Error Advancing Time!");
          else
          {
            string state_filename = "./states/Soldier_w"+ injury_set_names[i]+"@"+current_time_min+"min.json";
            // Pull data from pulse
            data_values = pulse.PullData();
            // And write it out to the console
            data_mgr.ToConsole(data_values);

            // The engine can provide you with all the current
            // active events and how long they have been active for
            // Active events are reevaluated each call to PullActiveEvents
            pulse.PullActiveEvents(active_events);
            foreach (var active_event in active_events)
            {
              Console.WriteLine(active_event.ToString());
            }
            pulse.SerializeToFile(state_filename, SerializationFormat.JSON);

            watch.Stop();
            var elapsed_ms = watch.ElapsedMilliseconds;
            Console.WriteLine("Saving state " + state_filename);
            Console.WriteLine("It took " + elapsed_ms * ms_to_s + "(s) to execute " + advance_min + "(min)");
          }
        }
        Console.WriteLine("\n\n");
      }

    }
  }
}

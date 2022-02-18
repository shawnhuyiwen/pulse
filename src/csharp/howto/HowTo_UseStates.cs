/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;
using System.IO;
using HowTo_DeathState;
using Pulse;
using Pulse.CDM;

/// <summary>
/// When setting up states for your application
/// It's best to setup scenario files that generate your states
/// As Pulse evolves and new versions come out, states will need to be regenerated
/// Rerunning a scenarios are the easiest way to do this.
/// This class will run all scenarios in a given directory
/// Next, its good define the death state for your set of scenarios
/// This class will also use the example death state handler
/// And will run each of the states generated for 20m, listening to death
/// This helps determine states, if left alone, will die gracefully or not
/// It also helps you tune your death check to your generated states
/// </summary>
namespace HowTo_UseStates
{
  class Example
  {
    public static void Run()
    {
      // This assumes we have run the HowTo_PatientStates
      string state_dir = "./test_results/howto/HowTo_PatientStates/";

      Logger log = new Logger("./test_results/howto/HowTo_UseStates.cs.log");
      RunConfiguration cfg = new RunConfiguration();
      SEScenarioExec opts = new SEScenarioExec();
      opts.SetDataRootDirectory("./");
      opts.SetDataRequestCSVFilename("");


      // Now lets run all the states that these scenarios generated
      // for 20m each and see if any of them die and how
      DirectoryInfo d = new DirectoryInfo(state_dir);
      FileInfo[] Files = d.GetFiles("*.json", SearchOption.AllDirectories);

      foreach (FileInfo file in Files)
      {
        log.WriteLine("\n------------------------------------------------------------\n");
        log.WriteLine("Running State " + file.FullName);

        string base_name = System.IO.Path.GetFileNameWithoutExtension(file.Name);

        PulseEngine pulse = new PulseEngine();
        pulse.LogToConsole(true);
        pulse.SetLogFilename(state_dir + base_name + ".ext.log");

        List<SEDataRequest> data_requests = new List<SEDataRequest>
        {
          // Vitals Monitor Data
          SEDataRequest.CreatePhysiologyDataRequest("HeartRate", FrequencyUnit.Per_min),
          SEDataRequest.CreatePhysiologyDataRequest("ArterialPressure", PressureUnit.mmHg),
          SEDataRequest.CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit.mmHg),
          SEDataRequest.CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit.mmHg),
          SEDataRequest.CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit.mmHg),
          SEDataRequest.CreatePhysiologyDataRequest("OxygenSaturation"),
          SEDataRequest.CreatePhysiologyDataRequest("EndTidalCarbonDioxidePressure", PressureUnit.mmHg),
          SEDataRequest.CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit.Per_min),
          SEDataRequest.CreatePhysiologyDataRequest("SkinTemperature", TemperatureUnit.C),
          SEDataRequest.CreateGasCompartmentDataRequest("Carina", "CarbonDioxide", "PartialPressure", PressureUnit.mmHg),
          SEDataRequest.CreatePhysiologyDataRequest("BloodVolume", VolumeUnit.mL),
          SEDataRequest.CreateECGDataRequest("Lead3ElectricPotential", ElectricPotentialUnit.mV),
        };
        SEDataRequestManager data_mgr = new SEDataRequestManager(data_requests);
        // Create a reference to a double[] that will contain the data returned from Pulse
        double[] data_values;
        // data_values[0] is ALWAYS the simulation time in seconds
        // The rest of the data values are in order of the data_requests list provided

        // NOTE: No data requests are being provided, so Pulse will return the default vitals data
        if (!pulse.SerializeFromFile(file.FullName, data_mgr))
        {
          log.WriteLine("Error Initializing Pulse!");
          return;
        }
        data_values = pulse.PullData();
        data_mgr.WriteData(data_values, log);

        DeathCheck death_check = new DeathCheck(pulse, log);
        pulse.SetEventHandler(death_check);

        int time_to_run = 1200 * 50;
        int status = 60 * 50;
        // seconds * Hz = total seconds to run, status time for print

        // Run pulse in a loop, and test for death after each time step
        bool dead = false;
        for (int i = 0; i < time_to_run; i++)
        {
          pulse.AdvanceTimeStep();
          // Get the values of the data you requested at this time
          data_values = pulse.PullData();
          if (death_check.IsDead(data_values))
          {
            dead = true;
            log.WriteLine(file.FullName + " DIED");
            break;
          }
          if ((i % status) == 0)
            data_mgr.WriteData(data_values, log);
        }
        if (!dead)
          log.WriteLine(file.FullName + " did NOT die");
      }
    }
  }
}

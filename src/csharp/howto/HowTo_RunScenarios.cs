/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;
using System.IO;
using Pulse;
using Pulse.CDM;

namespace HowTo_RunScenarios
{
  class Example
  {
    public static void Run(string dir = "")
    {
      PulseScenarioExec execOpts = new PulseScenarioExec();
      // Set the directory where Pulse can find its data files (usally bin/install)
      execOpts.SetDataRootDirectory("./");
      // Where do you want the log, csv, and states file to go?
      // You will always get a log in the OutputRootDirectory
      // If you do not want a csv, clear out the data requests in your scenario
      // For any  SESerialization action:
      //   - If the save path has an absolute path, they will be placed there
      //   - If the save path is relative, that path will be created under the OutputRootDirectory
      execOpts.SetOutputRootDirectory("./test_results/howto/HowTo_RunScenarios.cs/");

      if (dir.Length == 0)// Run a single hard coded scenario
      {
        execOpts.SetLogToConsole(eSwitch.On); // You can turn this off if you want
        execOpts.SetScenarioFilename("InitialPatientState.json");// Part of install/bin

        execOpts.SetOrganizeOutputDirectory(eSwitch.Off);// Default is Off
        // OrganizeOutputDirectory is meant to be used with SetScenarioDirectory of scenarios
        // But if you use it with SetScenarioFilename or SetScenarioContent, Pulse creates a new directory
        // in the OutputRootDirectory named the scenario filename (minus the extension, /InitialPatientState for this example)
        // or the name property in the scenario (if using SetScenarioContent)
        
      }
      else // Assume this is a directory full of scenario json files to run
      {
        // I turn off the console when processing a directory of scenarios
        // Pulse multithreads the execution, which results in a lot of logs
        execOpts.SetLogToConsole(eSwitch.On);
        execOpts.SetScenarioDirectory(dir);

        // Pulse will search for all *.json files under the ScenarioDirectory and assume they are scenario files
        // It will run each of these files with a thread pool
        // You can control the size of the thread pool used with
        execOpts.SetThreadCount(-1); // The default is -1
        // Setting a positive number, will result in a pool with that number of threads
        //  - NOTE If the number requested is greater than the hardware_concurrency, we instead use the hardware_concurrency
        // Setting 0 will result in creating a pool with the hardware_concurrency number of threads
        // Setting a negative number will result in a pool with hardware_concurrency + the negative number requested
        //  - So for -1, and a system where hardware_concurrency==8, we will create a thread pool with 8+(-1)=7 threads

        execOpts.SetOrganizeOutputDirectory(eSwitch.Off);// Default is Off
        // If your scenario directory has a hierarchy, that hierarchy can be recreated under the OutputRootDirectory
        // for example you have these 4 scenarios that have SESerialization actions in them
        // Assuming the SESerialization filenames are just a file name (no directories), like `PatientA1.json'
        //   - dir/scenarioA/PatientA1.json
        //   - dir/scenarioA/PatientA2.json
        //   - dir/scenarioB/PatientB1.json
        //   - dir/scenarioB/PatientB2.json

        // eSwitch.Off will result all files being put together in the OutputRootDirectory
        // So Make sure all scenario filenames as well as the SESerialization filenames are unique
        // OutputRootDirectory/PatientA1.log
        // OutputRootDirectory/PatientA1.json
        // OutputRootDirectory/PatientA2.log
        // OutputRootDirectory/PatientA2.json
        // OutputRootDirectory/PatientB1.log
        // OutputRootDirectory/PatientB1.json
        // OutputRootDirectory/PatientB2.log
        // OutputRootDirectory/PatientB2.json

        // eSwitch.On will result in the following files/structure under your OutputRootDirectory
        // OutputRootDirectory/scenarioA/PatientA1/PatientA1.log
        // OutputRootDirectory/scenarioA/PatientA1/PatientA1.json
        // OutputRootDirectory/scenarioA/PatientA2/PatientA2.log
        // OutputRootDirectory/scenarioA/PatientA2/PatientA2.json
        // OutputRootDirectory/scenarioA/PatientB1/PatientB1.log
        // OutputRootDirectory/scenarioA/PatientB1/PatientB1.json
        // OutputRootDirectory/scenarioA/PatientB2/PatientB2.log
        // OutputRootDirectory/scenarioA/PatientB2/PatientB2.json
      }
      if (!execOpts.Execute())
        System.Console.Out.WriteLine("Error running scenario");
      System.Console.Out.WriteLine("Finished!");
    }
  }
}

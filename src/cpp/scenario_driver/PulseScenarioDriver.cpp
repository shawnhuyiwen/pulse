/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PulseScenarioDriver.h"
#include "PulsePhysiologyEngine.h"
#include "PulseScenarioExec.h"
#include "utils/ConfigParser.h"
#include "utils/FileUtils.h"

#define test_serialization false
int main(int argc, char* argv[])
{
  try
  {
    if (argc <= 1)
    {
      std::cerr << "Must provide a scenario filename\n";
      return 1;
    }
    std::string logFile = argv[1];
    std::string csvFile = argv[1];
    std::string output = argv[1];
    // Try to read our config file to properly place results in a development structure
    std::string scenario_dir;
    ConfigSet* config = ConfigParser::FileToConfigSet("run.config");
    if (config->HasKey("scenario_dir"))
    {
      scenario_dir = config->GetValue("scenario_dir");
      std::replace(output.begin(), output.end(), '\\', '/');
      if (output.find(scenario_dir) != std::string::npos)
      {
        output = output.substr(scenario_dir.length());
        logFile = "./test_results/scenarios" + output;
        csvFile = "./test_results/scenarios" + output;
      }
    }
    delete config;
    logFile = Replace(logFile, ".json", ".log");
    csvFile = Replace(csvFile, ".json", "Results.csv");
    // What are we creating?
    std::cout << "Log File : " << logFile << std::endl;
    std::cout << "Results File : " << csvFile << std::endl;
    // Delete any results file that may be there
    remove(csvFile.c_str());

    SEScenarioExec opts;
    opts.SetLogToConsole(eSwitch::On);
    opts.SetLogFilename(logFile);
    opts.SetDataRequestCSVFilename(csvFile);
    opts.SetScenarioFilename(argv[1]);
    opts.SetSerializationDirectory("./states/");

    if (test_serialization)
    {
      // This block will help debug any serialization issues.
      // A serialization issue is where get different results from running the same scenario where we
      // 1. run we run the scenario as is
      // 2. run the scenario but serialized out and in (after actions, or any time really)
      // The two generated CSV files should match as well as match the baseline
      // If they do not, then we are not maintaining state properly to the file and back into the engine
      // To find serialization issues, uncomment out the AutoSerialization line in the ScenarioVerification.config
      // Then run the `run ScenarioVerification` command to run all scenarios, have then save out and in after each action
      // This test will also compare the resulting CSV file to the baseline CSV that did not save in and out
      // If any scenario fails this comparison... run it through here with this block turned on.
      // To debug the issue, this block will:
      // 1. Run one run where we save out states after we apply an action and after the subsequent time step
      // 2. Run another run where we save out the same times, but we reload the state after the first save
      // This gives us two states at the same point in time, per action, that we can compare to see what is different
      // Both times can have differences depending on the bug...good luck!
      std::string fn, sDir;
      SplitFilenamePath(output, fn);
      opts.SetAutoSerializeFilename(fn);
      opts.SetAutoSerializeAfterActions(eSwitch::On);
      opts.SetTimeStampSerializedStates(eSwitch::On);
      opts.SetAutoSerializePeriod_s(0);

      sDir = "./states/reload_on/" + output;
      sDir = Replace(sDir, ".json", "/");
      opts.SetSerializationDirectory(sDir);
      opts.SetReloadSerializedState(eSwitch::On);
      std::unique_ptr<PhysiologyEngine> PulseReloadOn = CreatePulseEngine();
      return PulseScenarioExec::Execute(*PulseReloadOn, opts) ? 0 : 1;

      sDir = "./states/reload_off/" + output;
      sDir = Replace(sDir, ".json", "/");
      opts.SetSerializationDirectory(sDir);
      opts.SetReloadSerializedState(eSwitch::Off);
      std::unique_ptr<PhysiologyEngine> PulseReloadOff = CreatePulseEngine();
      return PulseScenarioExec::Execute(*PulseReloadOff, opts) ? 0 : 1;
    }
    else
    {
      std::unique_ptr<PhysiologyEngine> Pulse = CreatePulseEngine();
      return PulseScenarioExec::Execute(*Pulse, opts) ? 0 : 1;
    }
  }
  catch (std::exception ex)
  {
    std::cerr << ex.what() << std::endl;
    return 1;
  }
  return 1;
}

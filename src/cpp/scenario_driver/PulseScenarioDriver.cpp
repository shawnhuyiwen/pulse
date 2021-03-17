/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PulseScenarioDriver.h"
#include "PulsePhysiologyEngine.h"
#include "PulseScenarioExec.h"
#include "utils/ConfigParser.h"
#include "utils/FileUtils.h"

#define test_serialization true
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

    if (test_serialization)
    {
      std::string sDir = "./states/auto/" + output;
      sDir = Replace(sDir, ".json", "/");
      opts.SetSerializationDirectory(sDir);
      std::string fn;
      SplitFilenamePath(output, fn);
      opts.SetAutoSerializeFilename(fn);
      opts.SetAutoSerializeAfterActions(eSwitch::On);
      opts.SetTimeStampSerializedStates(eSwitch::On);
      opts.SetAutoSerializePeriod_s(5);
    }

    std::unique_ptr<PhysiologyEngine> Pulse = CreatePulseEngine();
    return PulseScenarioExec::Execute(*Pulse, opts)?0:1;
  }
  catch (std::exception ex)
  {
    std::cerr << ex.what() << std::endl;
    return 1;
  }
  return 1;
}

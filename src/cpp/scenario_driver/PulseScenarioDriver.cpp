/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PulseScenarioDriver.h"
#include "PulseScenarioExec.h"
#include "cdm/scenario/SEScenarioLog.h"
#include "cdm/utils/FileUtils.h"

#define test_serialization false

int main(int argc, char* argv[])
{
  if (argc <= 1)
  {
    std::cerr << "Must provide a scenario filename or a directory\n";
    return 1;
  }
  std::string input = argv[1];
  if (!FileExists(input))
  {
    std::cerr << "File does not exist: "+input+"\n";
    return 1;
  }
  eModelType t = (eModelType)-1;
  bool convertLogs = false;
  if (argc >= 3)
  {
    if (std::string(argv[2]).compare("-c") == 0)
      convertLogs =  true;
    else
      eModelType_ValueOf(argv[2], t);
  }

  std::string outputDir = "./test_results/converted_scenarios";
  if (argc >= 4 && convertLogs)
  {
    outputDir = argv[3];
  }

  if (t == (eModelType)-1)
    t = eModelType::HumanAdultWholeBody;

  Logger logger("PulseScenarioDriver.log");
  logger.LogToConsole(true);
  PulseScenarioExec opts(&logger);
  opts.SetModelType(t);

  if (test_serialization)
  {
    opts.AutoSerializeAfterActions(eSwitch::On);
    opts.TimeStampSerializedStates(eSwitch::On);
    opts.SetAutoSerializePeriod_s(0);
  }

  if (IsDirectory(input))
  {
    opts.LogToConsole(eSwitch::Off);

    if (convertLogs)
    {
      opts.OrganizeOutputDirectory(eSwitch::On);
      opts.SetOutputRootDirectory(outputDir);
      opts.SetScenarioLogDirectory(input);
    }
    else
      opts.SetScenarioDirectory(input);
  }
  else
  {
    opts.LogToConsole(eSwitch::On);

    std::string filename, ext;
    SplitFilenameExt(input, filename, ext);
    if (ext.compare(".log") == 0)
      opts.SetScenarioLogFilename(input);
    else
      opts.SetScenarioFilename(input);
  }
  return !opts.Execute();
}

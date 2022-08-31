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
  if (argc >= 3)
    eModelType_ValueOf(argv[2], t);

  if (t == (eModelType)-1)
    t = eModelType::HumanAdultWholeBody;

  Logger logger("PulseScenarioDriver.log");
  logger.LogToConsole(true);

  // If provided file is a log, convert it and write out a new scenario file
  std::string path, filename, ext;
  SplitPathFilenameExt(input, path, filename, ext);
  if (ext.compare(".log") == 0)
  {
    std::string output = path + filename + ".sce.json";

    SEScenario sce(&logger);
    SEScenarioLog sceL(&logger);

    if (!sceL.Convert(input, sce))
    {
      logger.Error("Unable to convert scenario from log file : " + input);
    }

    return !sce.SerializeToFile(output);
  }

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
    opts.SetScenarioDirectory(input);
  }
  else
  {
    opts.LogToConsole(eSwitch::On);
    opts.SetScenarioFilename(input);
  }
  return !opts.Execute();
}

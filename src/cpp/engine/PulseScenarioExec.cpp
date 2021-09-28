/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PulseConfiguration.h"
#include "PulseScenario.h"
#include "PulseScenarioExec.h"
#include "engine/common/Engine.h"

#include "cdm/engine/SEAction.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/utils/FileUtils.h"
#include "cdm/utils/ConfigParser.h"

bool PulseScenarioExec::Execute(PhysiologyEngine& pe, SEScenarioExec& opts)
{
  pe.GetLogger()->SetLogFile(opts.GetLogFilename());
  pe.GetLogger()->LogToConsole(opts.GetLogToConsole() == eSwitch::On);

  PulseScenario sce(pe.GetLogger(), opts.GetDataRootDirectory());

  if (!opts.GetScenarioContent().empty())
  {
    if (!sce.SerializeFromString(opts.GetScenarioContent(), opts.GetContentFormat()))
      return false;
  }
  else if (!opts.GetScenarioFilename().empty())
  {
    if (!sce.SerializeFromFile(opts.GetScenarioFilename()))
      return false;
  }
  else
  {
    pe.GetLogger()->Error("No scenario file or content provided");
    return false;
  }

  if (!opts.GetEngineConfigurationFilename().empty())
  {
    PulseConfiguration pc(pe.GetLogger());
    if (!pc.SerializeFromFile(opts.GetEngineConfigurationFilename(), sce.GetSubstanceManager()))
      return false;
    sce.GetConfiguration().Merge(pc, sce.GetSubstanceManager());
  }
  else if (!opts.GetEngineConfigurationContent().empty())
  {
    PulseConfiguration pc(pe.GetLogger());
    if (!pc.SerializeFromString(opts.GetEngineConfigurationFilename(), opts.GetContentFormat(), sce.GetSubstanceManager()))
      return false;
    sce.GetConfiguration().Merge(pc, sce.GetSubstanceManager());
  }
  if (sce.HasConfiguration())
    pe.SetConfigurationOverride(&sce.GetConfiguration());

  bool b = opts.Execute(pe, sce);
  pe.SetConfigurationOverride(nullptr);
  return b;
}

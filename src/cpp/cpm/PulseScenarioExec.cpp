/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "PulseScenarioExec.h"
#include "controller/Engine.h"
#include "engine/SEAction.h"
#include "engine/SEPatientConfiguration.h"
#include "PulseScenario.h"
#include "PulseConfiguration.h"
#include "properties/SEScalarTime.h"
#include "utils/FileUtils.h"
#include "utils/ConfigParser.h"

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
  if(sce.HasConfiguration())
    pe.SetConfigurationOverride(&sce.GetConfiguration());

  bool b = opts.Execute(pe, sce);
  pe.SetConfigurationOverride(nullptr);
  return b;
}

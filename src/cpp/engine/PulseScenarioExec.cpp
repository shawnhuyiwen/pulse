/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PulseConfiguration.h"
#include "PulseScenario.h"
#include "PulseScenarioExec.h"
#include "engine/common/Engine.h"
#include "engine/io/protobuf/PBScenario.h"

#include "cdm/engine/SEAction.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/utils/FileUtils.h"
#include "cdm/utils/ConfigParser.h"
#include "cdm/utils/ThreadPool.h"


void PulseScenarioExec::Clear()
{
  SEScenarioExec::Clear();
  m_ModelType = eModelType::HumanAdultWholeBody;
}

void PulseScenarioExec::Copy(const PulseScenarioExec& src)
{
  pulse::PBScenario::Copy(src, *this);
}

bool PulseScenarioExec::SerializeToString(std::string& output, eSerializationFormat m, Logger* logger) const
{
  return pulse::PBScenario::SerializeToString(*this, output, m, logger);
}
bool PulseScenarioExec::SerializeFromString(const std::string& src, eSerializationFormat m, Logger* logger)
{
  return pulse::PBScenario::SerializeFromString(src, *this, m, logger);
}

bool ExecuteOpts(PulseScenarioExec* opts, PulseScenario* sce)
{
  if (!opts->GetScenarioFilename().empty())
  {
    opts->Info("Executing Scenario: " + opts->GetScenarioFilename());
    bool b = opts->Execute(*sce);
    if(b)
      opts->Info("Completed Scenario: " + opts->GetScenarioFilename());
    else
      opts->Error("Error Executing Scenario: " + opts->GetScenarioFilename());
    return b;
  }
  else if (!opts->GetScenarioLogFilename().empty())
  {
    opts->Info("Converting Log: " + opts->GetScenarioLogFilename());
    bool b = opts->ExecuteLog();
    if(b)
      opts->Info("Completed Log Conversion: " + opts->GetScenarioLogFilename());
    else
      opts->Error("Error Executing Log Conversion: " + opts->GetScenarioLogFilename());
    return b;
  }
  opts->Error("No scenario/scenario log provided.");
  return false;
}
bool PulseScenarioExec::Execute()
{
  if (!GetScenarioContent().empty())
  {
    PulseScenario sce(GetLogger(), GetDataRootDirectory());
    if (!sce.SerializeFromString(GetScenarioContent(), GetContentFormat()))
      return false;
    return Execute(sce);
  }
  else if (!GetScenarioFilename().empty())
  {
    PulseScenario sce(GetLogger(), GetDataRootDirectory());
    if (!sce.SerializeFromFile(GetScenarioFilename()))
      return false;
    return Execute(sce);
  }
  else if (!GetScenarioDirectory().empty())
  {
    // Let's get all the scenarios and create a thread pool
    std::vector<std::string> scenarios;
    ListFiles(GetScenarioDirectory(), scenarios, true, ".json");

    size_t numThreadsSupported = std::thread::hardware_concurrency();
    if (numThreadsSupported == 0)
    {
      Fatal("Unable to detect number of processors");
      return false;
    }

    size_t numThreadsToUse;
    if (m_ThreadCount > 0)
      numThreadsToUse = static_cast<size_t>(m_ThreadCount)>numThreadsSupported ? numThreadsSupported : m_ThreadCount;
    else if (m_ThreadCount == 0)
      numThreadsToUse = numThreadsSupported;
    else
      numThreadsToUse = numThreadsSupported + m_ThreadCount;

    // Let's not kick off more threads than we need
    if (numThreadsToUse > scenarios.size())
      numThreadsToUse = scenarios.size();

    ThreadPool pool(numThreadsToUse);
    for (auto filename : scenarios)
    {
      PulseScenarioExec* opts = new PulseScenarioExec(GetLogger());
      opts->Copy(*this);
      opts->m_ScenarioFilename = filename;
      PulseScenario* sce = new PulseScenario(GetLogger(), GetDataRootDirectory());
      sce->SerializeFromFile(filename);
      pool.enqueue(ExecuteOpts, opts, sce);
    }

    /* ThreadPool waits for threads to complete in its
    *  destructor which happens implicitly as we leave
    *  function scope. If we need return values from
    *  ExecuteScenario calls, enqueue returns a std::future
    */

    return true;
  }
  else if (!GetScenarioLogFilename().empty())
  {
    return ExecuteLog();
  }
  else if (!GetScenarioLogDirectory().empty())
  {
    // Let's get all the scenarios and create a thread pool
    std::vector<std::string> logs;
    ListFiles(GetScenarioLogDirectory(), logs, true, ".log", ".cnv.log");

    size_t numThreadsSupported = std::thread::hardware_concurrency();
    if (numThreadsSupported == 0)
    {
      Fatal("Unable to detect number of processors");
      return false;
    }

    size_t numThreadsToUse;
    if (m_ThreadCount > 0)
      numThreadsToUse = static_cast<size_t>(m_ThreadCount)>numThreadsSupported ? numThreadsSupported : m_ThreadCount;
    else if (m_ThreadCount == 0)
      numThreadsToUse = numThreadsSupported;
    else
      numThreadsToUse = numThreadsSupported + m_ThreadCount;

    // Let's not kick off more threads than we need
    if (numThreadsToUse > logs.size())
      numThreadsToUse = logs.size();

    ThreadPool pool(numThreadsToUse);
    std::vector<std::future<bool>> futures;
    std::vector<PulseScenarioExec*> opts;
    for (auto& filename : logs)
    {
      opts.emplace_back(new PulseScenarioExec(GetLogger()));
      opts.back()->Copy(*this);
      opts.back()->m_ScenarioLogFilename = filename;
      futures.emplace_back(pool.enqueue(ExecuteOpts, opts.back(), nullptr));
    }

    bool success = true;
    for (size_t i = 0; i < futures.size(); ++i)
    {
      if (!futures[i].get())
      {
        Error("Failed to convert " + opts[i]->m_ScenarioLogFilename);
        success = false;
      }
    }

    return success;
  }

  Error("No scenario content/log provided");
  return false;
}

bool PulseScenarioExec::Execute(PulseScenario& sce)
{
  auto pe = CreatePulseEngine(m_ModelType);

  if (!GetEngineConfigurationFilename().empty())
  {
    PulseConfiguration pc(pe->GetLogger());
    if (!pc.SerializeFromFile(GetEngineConfigurationFilename(), sce.GetSubstanceManager()))
      return false;
    sce.GetConfiguration().Merge(pc, sce.GetSubstanceManager());
  }
  else if (!GetEngineConfigurationContent().empty())
  {
    PulseConfiguration pc(pe->GetLogger());
    if (!pc.SerializeFromString(GetEngineConfigurationFilename(), GetContentFormat(), sce.GetSubstanceManager()))
      return false;
    sce.GetConfiguration().Merge(pc, sce.GetSubstanceManager());
  }
  if (sce.HasConfiguration())
    pe->SetConfigurationOverride(&sce.GetConfiguration());

  return SEScenarioExec::Execute(*pe, sce);
}

bool PulseScenarioExec::ExecuteLog()
{
  return SEScenarioExec::Execute();
}

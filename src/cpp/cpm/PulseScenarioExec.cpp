/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "PulseScenarioExec.h"
#include "controller/Engine.h"
#include "engine/SEAction.h"
#include "engine/SEAutoSerialization.h"
#include "engine/SEPatientConfiguration.h"
#include "PulseScenario.h"
#include "PulseConfiguration.h"
#include "properties/SEScalarTime.h"
#include "utils/FileUtils.h"
#include "utils/ConfigParser.h"

PulseScenarioExec::PulseScenarioExec(Logger* logger) : SEScenarioExec(logger)
{
  m_PulseScenario = nullptr;
  m_AutoSerializationTime_s = 0;
  m_AutoSerializationPeriod_s = 0;
  m_AutoSerializationTimeStamps = eSwitch::Off;
  m_AutoSerializationAfterActions = eSwitch::Off;
  m_AutoSerializationReload = eSwitch::Off;
  m_AutoSerializationFileName = "";
}

PulseScenarioExec::~PulseScenarioExec()
{
  SAFE_DELETE(m_PulseScenario);
}

void PulseScenarioExec::Run(const std::string& scenarioFile)
{
  std::string logFile=scenarioFile;
  std::string csvFile=scenarioFile;
  // Try to read our config file to properly place results in a development structure
  std::string scenario_dir;
  ConfigSet* config = ConfigParser::FileToConfigSet("run.config");
  if (config->HasKey("scenario_dir"))
  {
    scenario_dir = config->GetValue("scenario_dir");
    std::string output = scenarioFile;
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
  std::unique_ptr<PhysiologyEngine> Pulse = CreatePulseEngine();
  Pulse->GetLogger()->SetLogFile(logFile.c_str());
  if (!Pulse)
  {
    std::cerr << "Unable to create PulseEngine" << std::endl;
    return;
  }
  try
  {
    PulseScenarioExec exec(Pulse->GetLogger());
    exec.ExecuteFile(*Pulse, scenarioFile, csvFile);
  }
  catch (CommonDataModelException ex)
  {
    std::cerr << ex.what() << std::endl;
  }
  catch (std::exception ex)
  {
    std::cerr << ex.what() << std::endl;
  }
  catch (...)
  {
    std::cerr << "Unable to run scenario " << scenarioFile << std::endl;
  }
}

bool PulseScenarioExec::Execute(PhysiologyEngine& engine, const std::string& scenario, SerializationFormat f, const std::string& resultsFile, std::string const& dataDir)
{
  try
  {
    Info(m_ss);
    m_Cancel = false;
    if (m_PulseScenario == nullptr)
      m_PulseScenario = new PulseScenario(GetLogger(), dataDir);

    if (!m_PulseScenario->SerializeFromString(scenario, f))
    {
      Error("Unable to load scenario : " + scenario);
      return false;
    }
    if (m_PulseScenario->HasPatientConfiguration())
      m_PulseScenario->GetPatientConfiguration().SetDataRoot(dataDir);
    return Execute(engine, *m_PulseScenario, resultsFile);
  }
  catch (CommonDataModelException& ex)
  {
    Error(ex.what());
  }
  catch (...)
  {
    Error("Caught unknown exception, ending simulation");
  }
  return false;
}
bool PulseScenarioExec::ExecuteFile(PhysiologyEngine& engine, const std::string& scenarioFile, const std::string& resultsFile, std::string const& dataDir)
{
  try
  {
    m_ss << "Executing scenario file : " << scenarioFile << std::endl;
    Info(m_ss);
    m_Cancel = false;
    if (m_PulseScenario == nullptr)
      m_PulseScenario = new PulseScenario(GetLogger(), dataDir);

    if (!m_PulseScenario->SerializeFromFile(scenarioFile))
    {
        Error("Unable to load scenario file : " + scenarioFile);
        return false;
    }
    std::string rFile = resultsFile;
    if (rFile.empty())
    {
      rFile = scenarioFile;
      rFile += ".csv";
    }
    if (m_PulseScenario->HasPatientConfiguration())
      m_PulseScenario->GetPatientConfiguration().SetDataRoot(dataDir);
    return Execute(engine, *m_PulseScenario, rFile);
  }
  catch (CommonDataModelException& ex)
  {
    Error(ex.what());
  }
  catch (...)
  {
    Error("Caught unknown exception, ending simulation");
  }
  return false;
}
bool PulseScenarioExec::Execute(PhysiologyEngine& engine, const PulseScenario& scenario, const std::string& resultsFile)
{
  // If any configuration parameters were provided, use them over what we had
  if (scenario.HasConfiguration())
  {
    auto* cfg = scenario.GetConfiguration();
    if (cfg->HasAutoSerialization())
    {
      m_AutoSerializationPeriod_s     = cfg->GetAutoSerialization()->GetPeriod(TimeUnit::s);
      m_AutoSerializationTimeStamps   = cfg->GetAutoSerialization()->GetPeriodTimeStamps();
      m_AutoSerializationAfterActions = cfg->GetAutoSerialization()->GetAfterActions();
      m_AutoSerializationReload       = cfg->GetAutoSerialization()->GetReloadState();
      m_AutoSerializationFileName     = cfg->GetAutoSerialization()->GetFileName();
      m_AutoSerializationDirectory    = cfg->GetAutoSerialization()->GetDirectory();
      CreatePath(m_AutoSerializationDirectory);
      Info("Exeucting Scenario with AutoSerialization");
    }
  }
  bool success = SEScenarioExec::Execute(engine, scenario, resultsFile, scenario.GetConfiguration());
  return success;
}

bool PulseScenarioExec::ProcessActions(PulseEngine& engine, const SEScenario& scenario)
{
  return SEScenarioExec::ProcessActions(engine, scenario);
}
bool PulseScenarioExec::ProcessAction(PulseEngine& engine, const SEAction& action)
{
  if (m_AutoSerializationAfterActions == eSwitch::On)
  {
     m_ss << action;
     size_t start = m_ss.str().find(": ") + 2;
     size_t end = m_ss.str().find('\n');
     m_AutoSerializationActions << "-" << m_ss.str().substr(start, end - start);
     m_ss.str("");
  }
  return SEScenarioExec::ProcessAction(engine, action);
}

void PulseScenarioExec::AdvanceEngine(PulseEngine& engine)
{
  if (m_AutoSerializationPeriod_s > 0)
  {
    m_AutoSerializationTime_s += engine.GetTimeStep(TimeUnit::s);
    if (m_AutoSerializationTime_s >= m_AutoSerializationPeriod_s)
    {
      Info("Serializing state after requested period : " + m_AutoSerializationActions.str());
      m_AutoSerializationTime_s = 0;
      m_AutoSerializationOutput.str("");
      m_AutoSerializationOutput << m_AutoSerializationDirectory <<"/"<< m_AutoSerializationFileName;
      if (m_AutoSerializationTimeStamps == eSwitch::On)
        m_AutoSerializationOutput << "@" << engine.GetSimulationTime(TimeUnit::s);
      engine.SerializeToFile(m_AutoSerializationOutput.str() + ".json");
      if (m_AutoSerializationReload == eSwitch::On)
      {
        engine.SerializeFromFile(m_AutoSerializationOutput.str() + ".json");
        engine.SerializeToFile(m_AutoSerializationOutput.str() + ".Reloaded.json");
      }
    }
  }
  engine.AdvanceModelTime();
  if (m_AutoSerializationActions.str().length() > 0)
  {
    Info("Serializing state after actions : " + m_AutoSerializationActions.str());
    m_AutoSerializationOutput.str("");
    m_AutoSerializationOutput << m_AutoSerializationDirectory <<"/"<< m_AutoSerializationFileName<<m_AutoSerializationActions.str();
    if (m_AutoSerializationTimeStamps == eSwitch::On)
      m_AutoSerializationOutput << "@" << engine.GetSimulationTime(TimeUnit::s);
    engine.SerializeToFile(m_AutoSerializationOutput.str() + ".json");
    if (m_AutoSerializationReload == eSwitch::On)
    {
      engine.SerializeFromFile(m_AutoSerializationOutput.str() + ".json");
      engine.SerializeToFile(m_AutoSerializationOutput.str() + ".Reloaded.json");
    }
    m_AutoSerializationActions.str("");
  }
}

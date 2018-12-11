/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "controller/ScenarioExec.h"
#include "controller/Engine.h"
#include "engine/SEAction.h"
#include "engine/SEAutoSerialization.h"
#include "PulseScenario.h"
#include "PulseConfiguration.h"
#include "properties/SEScalarTime.h"
#include "utils/FileUtils.h"

PulseScenarioExec::PulseScenarioExec(PulseEngine& engine) : SEScenarioExec(engine), m_PulseConfiguration(engine.GetSubstanceManager())
{
  m_EngineConfiguration = &m_PulseConfiguration;
  m_AutoSerializationTime_s = 0;
  m_AutoSerializationPeriod_s = 0;
  m_AutoSerializationTimeStamps = eSwitch::Off;
  m_AutoSerializationAfterActions = eSwitch::Off;
  m_AutoSerializationReload = eSwitch::Off;
  m_AutoSerializationFileName = "";
}

PulseScenarioExec::~PulseScenarioExec()
{
  
}

bool PulseScenarioExec::Execute(const std::string& scenarioFile, const std::string& resultsFile)
{
  try
  {
    m_ss << "Executing scenario file : " << scenarioFile << std::endl;
    Info(m_ss);
    m_Cancel = false;

    PulseScenario scenario(m_Engine.GetSubstanceManager());
    if (!scenario.SerializeFromFile(scenarioFile,JSON))
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
    return Execute(scenario, rFile);
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
bool PulseScenarioExec::Execute(const PulseScenario& scenario, const std::string& resultsFile)
{
  // If any configuration parameters were provided, use them over what we had
  if (scenario.HasConfiguration())
  {
    m_PulseConfiguration.Merge(*scenario.GetConfiguration());
    if (m_PulseConfiguration.HasAutoSerialization())
    {
      m_AutoSerializationPeriod_s     = m_PulseConfiguration.GetAutoSerialization().GetPeriod(TimeUnit::s);
      m_AutoSerializationTimeStamps   = m_PulseConfiguration.GetAutoSerialization().GetPeriodTimeStamps();
      m_AutoSerializationAfterActions = m_PulseConfiguration.GetAutoSerialization().GetAfterActions();
      m_AutoSerializationReload       = m_PulseConfiguration.GetAutoSerialization().GetReloadState();
      m_AutoSerializationFileName     = m_PulseConfiguration.GetAutoSerialization().GetFileName();
      m_AutoSerializationDirectory    = m_PulseConfiguration.GetAutoSerialization().GetDirectory();
      CreateFilePath(m_AutoSerializationDirectory);
      Info("Exeucting Scenario with AutoSerialization");
    }
  }
  bool success = SEScenarioExec::Execute(scenario, resultsFile);
  return success;
}

bool PulseScenarioExec::ProcessActions(const SEScenario& scenario)
{
  return SEScenarioExec::ProcessActions(scenario);
}
bool PulseScenarioExec::ProcessAction(const SEAction& action)
{
  if (m_AutoSerializationAfterActions == eSwitch::On)
  {
     m_ss << action;
     size_t start = m_ss.str().find(": ") + 2;
     size_t end = m_ss.str().find('\n');
     m_AutoSerializationActions << "-" << m_ss.str().substr(start, end - start);
     m_ss.str("");
  }
  return SEScenarioExec::ProcessAction(action);
}

void PulseScenarioExec::AdvanceEngine()
{
  if (m_AutoSerializationPeriod_s > 0)
  {
    m_AutoSerializationTime_s += m_Engine.GetTimeStep(TimeUnit::s);
    if (m_AutoSerializationTime_s >= m_AutoSerializationPeriod_s)
    {
      Info("Serializing state after requested period : " + m_AutoSerializationActions.str());
      m_AutoSerializationTime_s = 0;
      m_AutoSerializationOutput.str("");
      m_AutoSerializationOutput << m_AutoSerializationDirectory <<"/"<< m_AutoSerializationFileName;
      if (m_AutoSerializationTimeStamps == eSwitch::On)
        m_AutoSerializationOutput << "@" << m_Engine.GetSimulationTime(TimeUnit::s);
      m_Engine.SerializeToFile(m_AutoSerializationOutput.str() + ".json", SerializationFormat::JSON);
      if (m_AutoSerializationReload == eSwitch::On)
      {
        m_Engine.SerializeFromFile(m_AutoSerializationOutput.str() + ".json", SerializationFormat::JSON);
        m_Engine.SerializeToFile(m_AutoSerializationOutput.str() + ".Reloaded.json", SerializationFormat::JSON);
      }
    }
  }
  m_Engine.AdvanceModelTime();
  if (m_AutoSerializationActions.str().length() > 0)
  {
    Info("Serializing state after actions : " + m_AutoSerializationActions.str());
    m_AutoSerializationOutput.str("");
    m_AutoSerializationOutput << m_AutoSerializationDirectory <<"/"<< m_AutoSerializationFileName<<m_AutoSerializationActions.str();
    if (m_AutoSerializationTimeStamps == eSwitch::On)
      m_AutoSerializationOutput << "@" << m_Engine.GetSimulationTime(TimeUnit::s);
    m_Engine.SerializeToFile(m_AutoSerializationOutput.str() + ".json", SerializationFormat::JSON);
    if (m_AutoSerializationReload == eSwitch::On)
    {
      m_Engine.SerializeFromFile(m_AutoSerializationOutput.str() + ".json", SerializationFormat::JSON);
      m_Engine.SerializeToFile(m_AutoSerializationOutput.str() + ".Reloaded.json", SerializationFormat::JSON);
    }
    m_AutoSerializationActions.str("");
  }
}

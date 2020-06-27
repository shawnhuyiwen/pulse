/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "scenario/SEScenarioExec.h"
#include "scenario/SEScenario.h"
#include "engine/SEAction.h"
#include "engine/SECondition.h"
#include "engine/SEPatientConfiguration.h"
#include "engine/SEAdvanceTime.h"
#include "engine/SEDataRequestManager.h"
#include "engine/SEEventManager.h"
#include "PhysiologyEngine.h"
#include "engine/SEEngineTracker.h"
#include "engine/SEEngineConfiguration.h"
#include "patient/SEPatient.h"
#include "properties/SEScalarTime.h"
#include "utils/TimingProfile.h"
#include "utils/FileUtils.h"

SEScenarioExec::SEScenarioExec(Logger* logger) : Loggable(logger)
{
  m_Cancel    = false;
  m_Scenario = nullptr;
}

SEScenarioExec::~SEScenarioExec()
{
  SAFE_DELETE(m_Scenario);
}

bool SEScenarioExec::ExecuteFile(PhysiologyEngine& pe, const std::string& scenarioFile, const std::string& resultsFile, const SEEngineConfiguration* ec, std::string const& dataDir)
{
  try
  {
    m_ss << "Executing scenario file : " << scenarioFile;
    Info(m_ss);
    m_Cancel = false;
    if (m_Scenario == nullptr)
      m_Scenario = new SEScenario(GetLogger(), dataDir);

    try
    {
      m_Scenario->SerializeFromFile(scenarioFile);
    }
    catch (CommonDataModelException& ex)
    {
      Error("Error reading scenario file " + scenarioFile + " " + ex.what());
      return false;
    }
    std::string rFile = resultsFile;
    if (rFile.empty())
    {
      rFile = scenarioFile;
      rFile += ".out";
    }
    if (m_Scenario->HasPatientConfiguration())
      m_Scenario->GetPatientConfiguration().SetDataRoot(dataDir);
    bool success = Execute(pe, *m_Scenario, rFile, ec);
    return success;
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


bool SEScenarioExec::Execute(PhysiologyEngine& pe, const std::string& scenarioFile, SerializationFormat format, const std::string& resultsFile, const SEEngineConfiguration* ec, std::string const& dataDir)
{
  try
  {
    m_ss << "Executing scenario file : " << scenarioFile;
    Info(m_ss);
    m_Cancel = false;
    if (m_Scenario == nullptr)
      m_Scenario = new SEScenario(GetLogger(), dataDir);

    try
    {
      m_Scenario->SerializeFromString(scenarioFile, format);
    }
    catch (CommonDataModelException& ex)
    {
      Error("Error reading scenario file " + scenarioFile + " " + ex.what());
      return false;
    }
    if (m_Scenario->HasPatientConfiguration())
      m_Scenario->GetPatientConfiguration().SetDataRoot(dataDir);
    bool success = Execute(pe, *m_Scenario, resultsFile, ec);
    return success;
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

bool SEScenarioExec::Execute(PhysiologyEngine& pe, const SEScenario& scenario, const std::string& resultsFile, const SEEngineConfiguration* ec)
{
  if (!scenario.IsValid())
  {
    Error("Invalid Scenario");
    return false;
  }
  
  try
  {
    pe.SetConfigurationOverride(ec);
    // Initialize the engine with a state or initial parameters
    if (scenario.HasEngineStateFile())
    {
      pe.SerializeFromFile(scenario.GetEngineStateFile());
      // WE ARE OVERWRITING ANY DATA REQUESTS IN THE STATE WITH WHATS IN THE SCENARIO!!!
      // Make a copy of the data requests, note this clears out data requests from the engine
      pe.GetEngineTracker()->GetDataRequestManager().Copy(scenario.GetDataRequestManager(), pe.GetSubstanceManager());
      if (!pe.GetEngineTracker()->GetDataRequestManager().HasResultsFilename())
        pe.GetEngineTracker()->GetDataRequestManager().SetResultsFilename(resultsFile);
    }
    else if (scenario.HasPatientConfiguration())
    {
      if (!pe.InitializeEngine(*scenario.GetPatientConfiguration()))
      {
        Error("Unable to initialize engine");
        return false;
      }
      // Make a copy of the data requests, note this clears out data requests from the engine
      pe.GetEngineTracker()->GetDataRequestManager().Copy(scenario.GetDataRequestManager(), pe.GetSubstanceManager());
      if (!pe.GetEngineTracker()->GetDataRequestManager().HasResultsFilename())
        pe.GetEngineTracker()->GetDataRequestManager().SetResultsFilename(resultsFile);
    }
    else
    {
      Error("No initial engine parameters set");
      return false;
    }

    return ProcessActions(pe, scenario);
  }
  catch (CommonDataModelException& ex)
  {
    Error(ex.what());
  }
  catch (std::exception& ex)
  {
    Error(ex.what());
  }
  catch (...)
  {
    Error("Caught unknown exception, ending simulation");
  }
  return false;
}

void SEScenarioExec::Cancel()
{
  m_Cancel=true;
}

bool SEScenarioExec::ProcessActions(PhysiologyEngine& pe, const SEScenario& scenario)
{
  Info("Executing Scenario");

  double dT_s=pe.GetTimeStep(TimeUnit::s);
  double scenarioTime_s = 0;
  double statusTime_s = 0;// Current time of this status cycle
  double statusStep_s = 60;//How long did it take to simulate this much time

  double sampleTime_s = scenario.GetDataRequestManager().GetSamplesPerSecond();
  if (sampleTime_s != 0)
    sampleTime_s = 1 / sampleTime_s;
  double currentSampleTime_s = sampleTime_s;//Sample the first step

  TimingProfile profiler;
  profiler.Start("Total");
  profiler.Start("Status");

  pe.GetEngineTracker()->SetupRequests();

  bool err=false;
  const SEAdvanceTime* adv;
  for (const SEAction* a : scenario.GetActions())
  {
    if(m_Cancel)
      break;
    // We override advance time actions in order to advance and 
    // pull requested data at each time step, all other actions 
    // will be processed by the engine
    adv=dynamic_cast<const SEAdvanceTime*>(a);
    if(adv!=nullptr)
    {
      m_ss << "[Action] " << *a;
      Info(m_ss);

      double time_s = adv->GetTime(TimeUnit::s);
      int count = (int)(time_s/dT_s);
      for(int i=0;i<=count;i++)
      {      
        if(m_Cancel)
          break;

        AdvanceEngine(pe);

        // Pull data from the engine
        scenarioTime_s = pe.GetSimulationTime(TimeUnit::s);
        currentSampleTime_s += dT_s;
        if (currentSampleTime_s >= sampleTime_s)
        {
          currentSampleTime_s = 0;
          pe.GetEngineTracker()->TrackData(scenarioTime_s);
        }
        statusTime_s += dT_s;
        // How are we running?
        if (statusTime_s>statusStep_s)
        {
          statusTime_s = 0;
          m_ss << "Current Time is " << scenarioTime_s << "s, it took " << profiler.GetElapsedTime_s("Status") << "s to simulate the past " << statusStep_s << "s" << std::flush;
          profiler.Reset("Status");
          Info(m_ss);
        }
        if(pe.GetEventManager().IsEventActive(eEvent::IrreversibleState))
          return false;// Patient is for all intents and purposes dead, or out at least out of its methodology bounds, quit running
      }
      continue;
    }

    if(!ProcessAction(pe, *a))
    {
      err=true;
      break;
    }
    if(pe.GetEventManager().IsEventActive(eEvent::IrreversibleState))
      return false;// Patient is for all intents and purposes dead, or out at least out of its methodology bounds, quit running
  }
  m_ss << "It took " << profiler.GetElapsedTime_s("Total") << "s to run this simulation";
  profiler.Clear();
  Info(m_ss);

  return !err;
}

bool SEScenarioExec::ProcessAction(PhysiologyEngine& pe, const SEAction& action)
{
  return pe.ProcessAction(action);
}

void SEScenarioExec::AdvanceEngine(PhysiologyEngine& pe)
{
  pe.AdvanceModelTime();
}

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/scenario/SEScenarioExec.h"
#include "cdm/scenario/SEScenario.h"
#include "cdm/io/protobuf/PBScenario.h"
#include "cdm/engine/SEAction.h"
#include "cdm/engine/SECondition.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/engine/SEAdvanceTime.h"
#include "cdm/engine/SESerializeState.h"
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEEventManager.h"
#include "cdm/PhysiologyEngine.h"
#include "cdm/engine/SEEngineTracker.h"
#include "cdm/engine/SEEngineConfiguration.h"
#include "cdm/patient/SEPatient.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/utils/TimingProfile.h"
#include "cdm/utils/FileUtils.h"

SEScenarioExec::SEScenarioExec()
{
  Clear();
}

SEScenarioExec::~SEScenarioExec()
{
  Clear();
}

void SEScenarioExec::Clear()
{
  m_LogToConsole = eSwitch::On;
  m_LogFilename = "";

  m_DataRequestCSVFilename = "";
  m_DataRootDirectory = "./";

  m_EngineConfigurationContent = "";
  m_EngineConfigurationFilename = "";

  m_ScenarioContent = "";
  m_ScenarioFilename = "";

  m_SaveNextStep = false;
  m_SerializationDirectory = "./states/";
  m_AutoSerializeFilename = "";
  m_AutoSerializeBaseFilename = "";
  m_AutoSerializeFilenameExt = "";
  m_AutoSerializeAfterActions = eSwitch::Off;
  m_AutoSerializePeriod_s = 0;
  m_AutoSerializeTime_s = 0;
  m_TimeStampSerializedStates = eSwitch::Off;
  m_ReloadSerializedState = eSwitch::Off;
  m_SerializationOutput.str("");
  m_SerializationActions.str("");
}

bool SEScenarioExec::SerializeToString(std::string& output, eSerializationFormat m, Logger* logger) const
{
  return PBScenario::SerializeToString(*this, output, m, logger);
}
bool SEScenarioExec::SerializeFromString(const std::string& src, eSerializationFormat m, Logger* logger)
{
  return PBScenario::SerializeFromString(src, *this, m, logger);
}

bool SEScenarioExec::Execute(PhysiologyEngine& pe, SEScenario& sce)
{
  if (!sce.IsValid())
  {
    pe.GetLogger()->Error("Invalid Scenario");
    return false;
  }

  try
  {
    // Initialize the engine with a state or initial parameters
    if (sce.HasEngineStateFile())
    {
      if (!pe.SerializeFromFile(sce.GetEngineStateFile()))
      {
        pe.GetLogger()->Error("Unable to load state file: "+ sce.GetEngineStateFile());
        return false;
      }
      // WE ARE OVERWRITING ANY DATA REQUESTS IN THE STATE WITH WHATS IN THE SCENARIO!!!
      // Make a copy of the data requests, note this clears out data requests from the engine
      pe.GetEngineTracker()->GetDataRequestManager().Copy(sce.GetDataRequestManager());
      if(!m_DataRequestCSVFilename.empty())
        pe.GetEngineTracker()->GetDataRequestManager().SetResultsFilename(m_DataRequestCSVFilename);
    }
    else if (sce.HasPatientConfiguration())
    {
      sce.GetPatientConfiguration().SetDataRoot(m_DataRootDirectory);
      // Make a copy of the data requests, note this clears out data requests from the engine
      pe.GetEngineTracker()->GetDataRequestManager().Copy(sce.GetDataRequestManager());
      if (!m_DataRequestCSVFilename.empty())
        pe.GetEngineTracker()->GetDataRequestManager().SetResultsFilename(m_DataRequestCSVFilename);
      if (!pe.InitializeEngine(sce.GetPatientConfiguration()))
      {
        pe.GetLogger()->Error("Unable to initialize engine");
        return false;
      }
    }
    else
    {
      pe.GetLogger()->Error("No initial engine parameters set");
      return false;
    }

    return ProcessActions(pe, sce);
  }
  catch (CommonDataModelException& ex)
  {
    pe.GetLogger()->Error(ex.what());
  }
  catch (std::exception& ex)
  {
    pe.GetLogger()->Error(ex.what());
  }
  catch (...)
  {
    pe.GetLogger()->Error("Caught unknown exception, ending simulation");
  }
  return false;
}

bool SEScenarioExec::ProcessActions(PhysiologyEngine& pe, SEScenario& sce)
{
  std::stringstream ss;
  pe.GetLogger()->Info("Executing Scenario");

  double dT_s=pe.GetTimeStep(TimeUnit::s);
  double scenarioTime_s = 0;
  double statusTime_s = 0;// Current time of this status cycle
  double statusStep_s = 60;//How long did it take to simulate this much time

  double sampleTime_s = sce.GetDataRequestManager().GetSamplesPerSecond();
  if (sampleTime_s != 0)
    sampleTime_s = 1 / sampleTime_s;
  double currentSampleTime_s = sampleTime_s;//Sample the first step

  TimingProfile profiler;
  profiler.Start("Total");
  profiler.Start("Status");

  pe.GetEngineTracker()->SetupRequests();

  bool err=false;
  const SEAdvanceTime* adv;
  double expectedFinalSimTime_s = 0;
  double spareAdvanceTime_s = 0;
  for (SEAction* a : sce.GetActions())
  {
    // We override advance time actions in order to advance and 
    // pull requested data at each time step, all other actions 
    // will be processed by the engine
    adv=dynamic_cast<const SEAdvanceTime*>(a);
    if(adv!=nullptr)
    {
      ss << "[Action] " << *a;
      pe.GetLogger()->Info(ss);
      expectedFinalSimTime_s += adv->GetTime(TimeUnit::s);

      double time_s = adv->GetTime(TimeUnit::s) + spareAdvanceTime_s;
      int count = (int)(time_s/dT_s);
      spareAdvanceTime_s = time_s - (count * dT_s);
      for(int i=0;i<count;i++)
      {
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
          ss << "Current Time is " << scenarioTime_s << "s, it took " << profiler.GetElapsedTime_s("Status") << "s to simulate the past " << statusStep_s << "s" << std::flush;
          profiler.Reset("Status");
          pe.GetLogger()->Info(ss);
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

  ss << "It took " << profiler.GetElapsedTime_s("Total") << "s to run this simulation";
  profiler.Clear();
  pe.GetLogger()->Info(ss);

  expectedFinalSimTime_s = std::round(expectedFinalSimTime_s * 100.0) / 100.0;
  double simTime_s = std::round(pe.GetSimulationTime(TimeUnit::s) * 100.0) / 100.0;

  pe.GetLogger()->Info("Final SimTime(s) " + pulse::cdm::to_string(simTime_s));
  pe.GetLogger()->Info("Expected Final SimTime(s) " + pulse::cdm::to_string(expectedFinalSimTime_s));
  if (expectedFinalSimTime_s != simTime_s)
  {
    err = true;
    pe.GetLogger()->Error("!!!! Simulation time does not equal expected end time !!!!");
  }

  return err;
}

bool SEScenarioExec::ProcessAction(PhysiologyEngine& pe, SEAction& action)
{
  if (m_AutoSerializeAfterActions == eSwitch::On && m_SerializationActions.str().empty())
  {
    m_SerializationActions << "AfterActions";
  }
  SESerializeState* ss = dynamic_cast<SESerializeState*>(&action);
  if (ss != nullptr && ss->GetType() == eSerialization_Type::Save)
  {
    std::string fn = ss->GetFilename();
    if (fn.find("./") == 0)
      ss->SetFilename(m_SerializationDirectory + fn);
  }
  return pe.ProcessAction(action);
}

void SEScenarioExec::AdvanceEngine(PhysiologyEngine& pe)
{
  if (m_AutoSerializePeriod_s > 0)
  {
    m_AutoSerializeTime_s += pe.GetTimeStep(TimeUnit::s);
    if (m_AutoSerializeTime_s >= m_AutoSerializePeriod_s)
    {
      m_AutoSerializeTime_s = 0;
      m_SerializationOutput.str("");
      m_SerializationOutput << m_SerializationDirectory << "/" << m_AutoSerializeBaseFilename;
      if (m_TimeStampSerializedStates == eSwitch::On)
        m_SerializationOutput << "@" << pe.GetSimulationTime(TimeUnit::s);
      pe.SerializeToFile(m_SerializationOutput.str() + m_AutoSerializeFilenameExt);
      pe.GetLogger()->Info("Serializing state after requested period : " + m_SerializationOutput.str() + m_AutoSerializeFilenameExt);
      if (m_ReloadSerializedState == eSwitch::On)
      {
        pe.SerializeFromFile(m_SerializationOutput.str() + m_AutoSerializeFilenameExt);
        pe.SerializeToFile(m_SerializationOutput.str() + ".Reloaded" + m_AutoSerializeFilenameExt);
      }
    }
  }
  pe.AdvanceModelTime();
  if (m_SaveNextStep)
  {
    m_SaveNextStep = false;
    m_SerializationOutput.str("");
    m_SerializationOutput << m_SerializationDirectory << "/" << m_AutoSerializeBaseFilename << "AfterActionReload";
    if (m_TimeStampSerializedStates == eSwitch::On)
      m_SerializationOutput << "@" << pe.GetSimulationTime(TimeUnit::s);
    pe.SerializeToFile(m_SerializationOutput.str() + m_AutoSerializeFilenameExt);
    pe.GetLogger()->Info("Serializing state again (after the next timestep) : " + m_SerializationOutput.str() + m_AutoSerializeFilenameExt);
  }
  if (m_SerializationActions.str().length() > 0)
  {
    m_SaveNextStep = true;
    m_SerializationOutput.str("");
    m_SerializationOutput << m_SerializationDirectory << "/" << m_AutoSerializeBaseFilename << m_SerializationActions.str();
    if (m_TimeStampSerializedStates == eSwitch::On)
      m_SerializationOutput << "@" << pe.GetSimulationTime(TimeUnit::s);
    pe.SerializeToFile(m_SerializationOutput.str() + m_AutoSerializeFilenameExt);
    pe.GetLogger()->Info("Serializing state after actions : " + m_SerializationOutput.str() + m_AutoSerializeFilenameExt);
    if (m_ReloadSerializedState == eSwitch::On)
    {
      pe.GetLogger()->Info("Reloading and saving reloaded state to : " + m_SerializationOutput.str() + ".Reloaded" + m_AutoSerializeFilenameExt);
      pe.SerializeFromFile(m_SerializationOutput.str() + m_AutoSerializeFilenameExt);
      pe.SerializeToFile(m_SerializationOutput.str() + ".Reloaded" + m_AutoSerializeFilenameExt);
    }
    m_SerializationActions.str("");
  }
}
/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "PulsePhysiologyEnginePool.h"
#include "controller/Engine.h"
#include "engine/SEAdvanceTime.h"
#include "engine/SEPatientConfiguration.h"

namespace
{
  bool GatherResults(std::vector<std::future<bool>>& futures)
  {
    bool result = true;
    for (auto& f : futures)
        result &= f.get();
    return result;
  }
}

SEPhysiologyEnginePoolEngine::SEPhysiologyEnginePoolEngine(Logger* logger) : Loggable(logger), EngineInitialization(logger)
{
  Engine = CreatePulseEngine();
  Engine->GetLogger()->LogToConsole(false);
};

SEPhysiologyEnginePool::SEPhysiologyEnginePool(size_t poolSize, Logger* logger) : Loggable(logger), m_Pool(poolSize)
{
  m_IsActive = false;
}
SEPhysiologyEnginePool::~SEPhysiologyEnginePool()
{
  DELETE_MAP_SECOND(m_Engines);
}

bool SEPhysiologyEnginePool::RemoveEngine(__int32 id)
{
  if (m_Engines.find(id) == m_Engines.end())
    return false;
  m_Engines.erase(id);
  return true;
}
SEPhysiologyEnginePoolEngine* SEPhysiologyEnginePool::GetEngine(__int32 id) const
{
  auto e = m_Engines.find(id);
  if (e != m_Engines.end())
    return e->second;
  return nullptr;
}
const EngineCollection& SEPhysiologyEnginePool::GetEngines() const
{
  return m_Engines;
}
SEPhysiologyEnginePoolEngine* SEPhysiologyEnginePool::CreateEngine(__int32 id)
{
  if (m_IsActive)
    throw CommonDataModelException("Engine pool has already been initialized, dynamic engine support not implemented");

  auto e = m_Engines.find(id);
  if (e != m_Engines.end())
  {
    Warning("ID " + std::to_string(id) + " already has an engine.");
    return e->second;
  }

  SEPhysiologyEnginePoolEngine* epe = new SEPhysiologyEnginePoolEngine(m_Logger);
  m_Engines.insert({ id, epe });
  return epe;
}

bool SEPhysiologyEnginePool::InitializeEngines()
{
  if (m_IsActive)
    throw CommonDataModelException("Engine pool has already been initialized, dynamic engine support not implemented");

  if (m_Engines.empty())
    return false;

  std::vector<std::future<bool>> futures;
  for (auto itr : m_Engines)
  {
    SEPhysiologyEnginePoolEngine* pe = itr.second;
    futures.push_back(m_Pool.enqueue([pe]()
    {
      pe->IsActive = false;
      PhysiologyEngine* engine = pe->Engine.get();
      SEEngineInitialization& init = pe->EngineInitialization;

      if(init.HasLogFilename())
        engine->GetLogger()->SetLogFile(init.GetLogFilename());
      if (init.HasPatientConfiguration())
        pe->IsActive = engine->InitializeEngine(init.GetPatientConfiguration());
      else if (init.HasStateFilename())
        pe->IsActive = engine->SerializeFromFile(init.GetStateFilename());
      else if (init.HasState())
        pe->IsActive = engine->SerializeFromString(init.GetState(), init.GetStateFormat());
      
      if (!pe->IsActive)
        pe->Warning("Engine "+std::to_string(pe->EngineInitialization.GetID())+" was unable to initialize");
      return pe->IsActive;
    }));
  }

  bool b = GatherResults(futures);
  if (!b)
  {// Are any engines ok?
    size_t errors = 0;
    for (auto itr : m_Engines)
    {
      if (!itr.second->IsActive)
        errors++;
    }
    if (errors > 0)
      Warning(std::to_string(errors) + " out of " + std::to_string(m_Engines.size()) + " failed");
    else if (errors == m_Engines.size())
      return false;
  }
  m_IsActive = true;
  return m_IsActive;
}

bool SEPhysiologyEnginePool::AdvanceModelTime(double time, const TimeUnit& unit)
{
  SEAdvanceTime adv;
  adv.GetTime().SetValue(time, unit);
  return ProcessAction(adv);
}
bool SEPhysiologyEnginePool::ProcessAction(const SEAction& action)
{
  for (auto& itr : m_Engines)
    itr.second->Actions.push_back(&action);
  return ProcessActions();
}
bool SEPhysiologyEnginePool::ProcessActions()
{
  if (!m_IsActive)
  {
    Error("Engine pool is not active");
    return false;
  }

  std::vector<std::future<bool>> futures;
  for (auto itr : m_Engines)
  {
    SEPhysiologyEnginePoolEngine* pe = itr.second;
    if (!pe->IsActive)
    {
      pe->Actions.clear();
      continue;
    }

    futures.push_back(m_Pool.enqueue([pe]()
    {
      bool f = true;
      for (const SEAction* a : pe->Actions)
        f &= pe->Engine->ProcessAction(*a);
      pe->Actions.clear();
      return f;
    }));
  }
  return  GatherResults(futures);
}

PhysiologyEnginePoolThunk::PhysiologyEnginePoolThunk()
{

}
PhysiologyEnginePoolThunk::~PhysiologyEnginePoolThunk()
{

}

double PhysiologyEnginePoolThunk::GetTimeStep(std::string const& unit)
{
  return 0.02;
}

bool PhysiologyEnginePoolThunk::InitializeEngines(std::string const& engine_initialization, SerializationFormat format)
{
  return false;
}

bool PhysiologyEnginePoolThunk::RemoveEngine(__int32 id)
{
  return false;
}

bool PhysiologyEnginePoolThunk::ProcessActions(std::string const& actions, SerializationFormat format)
{
  return false;
}

size_t PhysiologyEnginePoolThunk::DataLength() const
{
  return 0;
}
double* PhysiologyEnginePoolThunk::PullDataPtr()
{
  return nullptr;
}
void PhysiologyEnginePoolThunk::PullData(std::map<int, double>& d)
{

}

void PhysiologyEnginePoolThunk::ForwardDebug(const std::string& msg, const std::string& origin)
{

}
void PhysiologyEnginePoolThunk::ForwardInfo(const std::string& msg, const std::string& origin)
{

}
void PhysiologyEnginePoolThunk::ForwardWarning(const std::string& msg, const std::string& origin)
{

}
void PhysiologyEnginePoolThunk::ForwardError(const std::string& msg, const std::string& origin)
{

}
void PhysiologyEnginePoolThunk::ForwardFatal(const std::string& msg, const std::string& origin)
{

}

void PhysiologyEnginePoolThunk::HandleEvent(eEvent type, bool active, const SEScalarTime* time)
{

}

void PhysiologyEnginePoolThunk::SetupDefaultDataRequests()
{

}


/////////////////////////////////////////////////////////////////////////////////
// Simulation Implementation
///

/**
PulseEngineSimPool::PulseEngineSimPool(size_t poolSize) :
    m_pool(poolSize)
{
}

std::shared_ptr<EngineRunner> PulseEngineSimPool::add(std::string patientFile)
{
    auto engine = std::make_shared<PulseEngine>();
    if (engine->SerializeFromFile(patientFile)) {
        return std::make_shared<EngineRunner>(engine);
    }
    return nullptr;
}

EngineRunner::EngineRunner(std::shared_ptr<PulseEngine> engine) :
    m_engine(engine),
    m_advanceTime(1)
{

}

void EngineRunner::run()
{
    while (m_stopped)
    {
        auto start = std::chrono::high_resolution_clock::now();
        m_engine->AdvanceModelTime(static_cast<double>(m_advanceTime.count()), TimeUnit::s);
        processCallbacks();
        auto elapsed = std::chrono::high_resolution_clock::now() - start;

        if ((m_advanceTime - elapsed).count() > 0)
        {
            std::this_thread::sleep_for(m_advanceTime - elapsed);
        }
        else {
            // Warn processing did take too long
        }
    }
}

void EngineRunner::doNext(std::function<bool(PulseEngine*)> f)
{
    std::lock_guard<std::mutex> lock(m_callbackMutex);
    m_callbacks.push_back(f);
}
*/

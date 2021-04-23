/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "PulsePhysiologyEnginePool.h"
#include "SEPatientConfiguration.h"

namespace
{
  bool GatherResults(std::vector<std::future<bool>>& futures, EngineCollection& engines)
  {
    bool result = false;
    int i = 0;
    for (auto& f : futures)
    {
        engines[i].second &= f.get();
        result &= engines[i].second;
        ++i;
    }
    return result;
  }
}

PulsePhysiologyEnginePool::PulsePhysiologyEnginePool(size_t poolSize) :
    m_pool(poolSize)
{
  
}

/// Blocking version of initialization
/// returns when all engines have been initialized
bool PulsePhysiologyEnginePool::CreateEngines(const std::vector<const SEPatientConfiguration*>& configurations)
{
  std::vector<std::string> states;
  return CreateEngines(states, configurations);
}
bool PulsePhysiologyEnginePool::CreateEngines(const std::vector<std::string>& stateFilenames)
{
  std::vector<const SEPatientConfiguration*> configurations;
  return CreateEngines(stateFilenames, configurations);
}
bool PulsePhysiologyEnginePool::CreateEngines(const std::vector<std::string>& stateFilenames,
  const std::vector<const SEPatientConfiguration*>& configurations)
{
  if (!m_engines.empty())
  {
    // We can either
    // a. create engines in a separate 'initializings' collection
    //    Once they have all finished loading/stabilizing, put them in the 'running' collection
    //    This is a dynamic addition of engines to the pool
    // b. Stop and clear out all current engines
    //    This is consistent with PhysiologyEngineInterface, if you call Initialize/Serialize on an existing engine
    //    It is cleared out and started a new with the provided state/configuration
    // I would recommend implementing b. for the initial implementation of this class
    return false;
  }

  for (size_t i = 0; i < stateFilenames.size() + configurations.size(); ++i)
  {
    m_engines.push_back({ std::make_unique<PulseEngine>(), false });
  }

  std::vector<std::future<bool>> futures;
  for (size_t i = 0; i < m_engines.size(); ++i)
  {
    auto engine = m_engines[i].first.get();
    auto file = stateFilenames[i];
    futures.push_back(m_pool.enqueue([engine, file]() {
      return engine->SerializeFromFile(file); }));
  }
  for (size_t i = 0; i < m_engines.size(); ++i)
  {
    auto engine = m_engines[i].first.get();
    auto config = configurations[i];
    futures.push_back(m_pool.enqueue([engine, config]() {
      return engine->InitializeEngine(*config); }));
  }

  return GatherResults(futures, m_engines);
}

bool PulsePhysiologyEnginePool::AdvanceModelTime(double time, const TimeUnit& unit)
{
    return Execute([time, unit](auto engine) {return engine->AdvanceModelTime(time, unit); });
}

bool PulsePhysiologyEnginePool::Execute(std::function<bool(PulseEngine*)> f)
{
  std::vector<std::future<bool>> futures;
  std::for_each(m_engines.begin(), m_engines.end(),
    [&f, &futures, this](const auto& item)
    {
        if (item.second)
          futures.push_back(m_pool.enqueue(f, item.first.get()));
    }
  );

  return GatherResults(futures, m_engines);
}

const EngineCollection& PulsePhysiologyEnginePool::GetEngines()
{
  return m_engines;
}

/////////////////////////////////////////////////////////////////////////////////
// Simulation Implementation
///


//PulseEngineSimPool::PulseEngineSimPool(size_t poolSize) :
//    m_pool(poolSize)
//{
//}
//
//std::shared_ptr<EngineRunner> PulseEngineSimPool::add(std::string patientFile)
//{
//    auto engine = std::make_shared<PulseEngine>();
//    if (engine->SerializeFromFile(patientFile)) {
//        return std::make_shared<EngineRunner>(engine);
//    }
//    return nullptr;
//}



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

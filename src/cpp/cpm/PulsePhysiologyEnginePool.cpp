#include "PulsePhysiologyEnginePool.h"
#include "SEPatientConfiguration.h"

PulsePhysiologyEnginePool::PulsePhysiologyEnginePool(size_t engineNum, size_t poolSize) :
    m_pool(poolSize)
{
    for (size_t i = 0; i < engineNum; ++i)
    {
        m_engines.push_back({ std::make_unique<PulseEngine>(), false });
    }
}

/// Blocking version of initialization
/// returns when all engines have been initialized
std::vector<bool> PulsePhysiologyEnginePool::initAll(std::vector<SEPatientConfiguration*> configurations)
{
    if (configurations.size() != m_engines.size())
    {
        return {};
    }

    std::vector<std::future<bool>> futures;
    for (size_t i = 0; i < m_engines.size(); ++i)
    {
        auto engine = m_engines[i].first.get();
        auto config = configurations[i];
        futures.push_back(m_pool.enqueue([engine, config]() {
            return engine->InitializeEngine(*config); }));
    }

    std::vector<bool> result;
    size_t i = 0;
    for (auto& f : futures)
    {
        m_engines[i].second = f.get();
        result.push_back(m_engines[i].second);
        ++i;
    }
    return result;
}

std::vector<bool> PulsePhysiologyEnginePool::initAll(std::vector<std::string> patientFiles)
{
    if (patientFiles.size() != m_engines.size())
    {
        return {};
    }

    std::vector<std::future<bool>> futures;
    for (size_t i = 0; i < m_engines.size(); ++i)
    {
        auto engine = m_engines[i].first.get();
        auto file = patientFiles[i];
        futures.push_back(m_pool.enqueue([engine, file]() {
            return engine->SerializeFromFile(file); }));
    }

    std::vector<bool> result;
    size_t i = 0;
    for (auto& f : futures)
    {
        m_engines[i].second = f.get();
        result.push_back(m_engines[i].second);
        ++i;
    }
    return result;
}

std::vector<bool> PulsePhysiologyEnginePool::advanceAll(double time, TimeUnit unit)
{
    std::vector<std::future<bool>> futures;
    for (auto& pair : m_engines)
    {
        if (pair.second)
        {
            futures.push_back(m_pool.enqueue([&pair, time, unit]() {return pair.first->AdvanceModelTime(time,unit); }));
        }
    }

    std::vector<bool> result;
    size_t i = 0;
    for (auto& f : futures)
    {
        m_engines[i].second &= f.get();
        result.push_back(m_engines[i].second);
        ++i;
    }
    return result;
}

const std::vector<std::pair<std::unique_ptr<PulseEngine>, bool>>& PulsePhysiologyEnginePool::getEngines()
{
    return m_engines;
}


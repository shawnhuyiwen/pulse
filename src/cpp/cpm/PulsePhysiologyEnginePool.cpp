#include "PulsePhysiologyEnginePool.h"
#include "SEPatientConfiguration.h"

#include <chrono>

namespace {

bool gatherResults(std::vector<std::future<bool>>& futures, EngineCollection& engines)
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
bool PulsePhysiologyEnginePool::init(std::vector<SEPatientConfiguration*> configurations)
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

    return gatherResults(futures, m_engines);
}

bool PulsePhysiologyEnginePool::init(std::vector<std::string> patientFiles)
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

    return gatherResults(futures, m_engines);
}

bool PulsePhysiologyEnginePool::advance(double time, TimeUnit unit)
{
    return execute([time, unit](auto engine) {return engine->AdvanceModelTime(time, unit); });
}

bool PulsePhysiologyEnginePool::execute(std::function<bool(PulseEngine*)> f)
{
    std::vector<std::future<bool>> futures;
    std::for_each(m_engines.begin(), m_engines.end(),
        [&f, &futures, this](const auto& item) {
            if (item.second) futures.push_back(m_pool.enqueue(f, item.first.get()));
        }
    );

    return gatherResults(futures, m_engines);
}

const std::vector<std::pair<std::unique_ptr<PulseEngine>, bool>>& PulsePhysiologyEnginePool::getEngines()
{
    return m_engines;
}

/////////////////////////////////////////////////////////////////////////////////
// Simulation Implementation
///


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

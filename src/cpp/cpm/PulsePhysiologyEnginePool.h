
#include "stdafx.h"

#include <chrono>

#include "CommonDataModel.h"
#include "Engine.h"
#include "ThreadPool.h"


using EngineCollection = std::vector<std::pair<std::unique_ptr<PulseEngine>, bool>>;

class PulsePhysiologyEnginePool
{
public:
    PulsePhysiologyEnginePool(size_t engineNum, size_t poolSize = std::thread::hardware_concurrency());

    bool init(std::vector<SEPatientConfiguration*> configurations);
    bool init(std::vector<std::string> patientFiles);
    bool advance(double time, TimeUnit unit);

    bool execute(std::function<bool(PulseEngine*)> f);

    const std::vector<std::pair<std::unique_ptr<PulseEngine>, bool>>& getEngines();

    const size_t workerCount() const { return m_pool.workerCount(); }

private:
    EngineCollection m_engines;
    ThreadPool m_pool;
};

class EngineRunner {
public:
    EngineRunner(std::shared_ptr<PulseEngine> engine);

    // So here is a tricky decision for each new live engine we can either 
    // keep running the engine on its own thread, as it should be running faster than realtime
    // there should be a good amount of sleeping going on this means that we should be able to 
    // handle a good number of running engines at the same time, 
    // ALTERNATIVELY, we could keep putting new tasks on the threadpool for each AdvanceModelTime
    // but we'd still have sleep the correct amount of time, not sure if that is an improvement

    void setAdvanceTime();
    void setRealTimeRate(); // Determine how many seconds of sim time per second of realtime should be processed

    // Starts calling AdvanceModelTime with the value AdvanceTime repeatedly so that 
    // engine sim time proceeds at the rate determined by setRealtimeRate 
    void run();

    void stop();    // Stops and joins can't call run afterwards

    void pause(bool doPause);

    /// Executes the function f the next time the engine exits from advanceModelTime 
    void doNext(std::function<bool(PulseEngine*)> f);

    /// Returns the amount of simulated time passed 
    std::chrono::duration<std::chrono::seconds> getSimulatedTime();

private:

    void processCallbacks()
    {
        std::lock_guard<std::mutex> lock(m_callbackMutex);
        std::for_each(m_callbacks.begin(), m_callbacks.end(), [this](auto i) {i(m_engine.get()); });
    }


    std::mutex m_callbackMutex;
    std::vector <std::function<bool(PulseEngine*)>> m_callbacks;

    std::chrono::seconds m_advanceTime;
    std::chrono::milliseconds m_elapsed;
    std::atomic<bool> m_stopped = false;

    std::shared_ptr<PulseEngine> m_engine;
};

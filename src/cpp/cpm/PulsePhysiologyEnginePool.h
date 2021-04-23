/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#pragma once

#include "CommonDataModel.h"
#include "controller/Engine.h"
#include "utils/ThreadPool.h"
#include <chrono>

// Engine ID
using EngineCollection = std::map<uint32_t, std::unique_ptr<PulseEngine>>;

class PulsePhysiologyEnginePool
{
public:
    PulsePhysiologyEnginePool(size_t poolSize = std::thread::hardware_concurrency());
    const size_t GetWorkerCount() const { return m_pool.workerCount(); }

    bool SerializeFromFile(const std::map<uint32_t, std::string>& stateFilenames);
    bool SerializeToFile(const std::map<uint32_t, std::string>& stateFilenames);

    bool SerializeFromString(const std::map<uint32_t, std::string>& states);
    bool SerializeToFile(SerializationFormat format, std::map<uint32_t, std::string>& states);

    bool InitializeEngine(const std::map<uint32_t, SEPatientConfiguration>& configurations);

    bool GetInitialPatient(SerializationFormat format, std::map<uint32_t, std::string>& states);
    bool GetConditions(SerializationFormat format, std::map<uint32_t, std::string>& states);

    bool AdvanceModelTime(const std::map<uint32_t, double>& times, const TimeUnit& unit);

    bool AdvanceModelTime(const std::map<uint32_t,double>& times, const TimeUnit& unit);


    const EngineCollection& GetEngines();

private:

  bool Execute(std::function<bool(PulseEngine*)> f);

  EngineCollection m_engines;
  ThreadPool m_pool;
};

class EngineRunner
{
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

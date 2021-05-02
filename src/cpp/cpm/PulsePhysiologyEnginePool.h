/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#pragma once

#include "CommonDataModel.h"
#include "utils/ThreadPool.h"
#include <chrono>

#include "engine/SEEngineInitialization.h"

// TODO Template these class and move to CDM
// (Template is the PhysiologEngine type)

class SEPhysiologyEnginePoolEngine : public Loggable
{
  friend class SEPhysiologyEnginePool;
  friend class SEPhysiologyEnginePoolThunk;
protected:
  SEPhysiologyEnginePoolEngine(Logger* logger = nullptr);

public:
  ~SEPhysiologyEnginePoolEngine() = default;

  bool                              IsActive=false;
  SEEngineInitialization            EngineInitialization;
  std::vector<const SEAction*>      Actions;
  std::unique_ptr<PhysiologyEngine> Engine;
};
using EngineCollection = std::map<__int32, SEPhysiologyEnginePoolEngine*>;

class SEPhysiologyEnginePool : public Loggable
{
public:
  SEPhysiologyEnginePool(size_t poolSize = std::thread::hardware_concurrency(), Logger* logger = nullptr);
  ~SEPhysiologyEnginePool();

  const size_t GetWorkerCount() const { return m_Pool.workerCount(); }

  bool RemoveEngine(__int32 id);
  const EngineCollection& GetEngines() const;
  SEPhysiologyEnginePoolEngine* GetEngine(__int32 id) const;
  SEPhysiologyEnginePoolEngine* CreateEngine(__int32 id);

  bool InitializeEngines();

  // Advance all engines the same amount of time
  // If you want to advance individual engines/different times
  // Add an SEAdvanceTime action to the SEPhysiologyEnginePoolEngine Actions vector
  bool AdvanceModelTime(double time, const TimeUnit& unit);

  // Process the same action on all engines
  bool ProcessAction(const SEAction& action);

  // Process all the actions on each engine pool engine
  bool ProcessActions();


protected:
  bool m_IsActive;
  EngineCollection m_Engines;
  ThreadPool m_Pool;
};

/**
 * An instance of Pulse where the interface is define in stl and base data types.
 * This interface is a thunk layer using serialized cdm objects to drive a Pulse engine.
 */
class PULSE_DECL PhysiologyEnginePoolThunk : public LoggerForward, public SEEventHandler
{
public:
  PhysiologyEnginePoolThunk();
  virtual ~PhysiologyEnginePoolThunk();

  double GetTimeStep(std::string const& unit);

  bool InitializeEngines(std::string const& engine_initialization, SerializationFormat format);

  bool RemoveEngine(__int32 id);

  bool ProcessActions(std::string const& actions, SerializationFormat format);

  size_t DataLength() const;
  double* PullDataPtr();
  void PullData(std::map<__int32, double>& d);

  virtual void ForwardDebug(const std::string& msg, const std::string& origin);
  virtual void ForwardInfo(const std::string& msg, const std::string& origin);
  virtual void ForwardWarning(const std::string& msg, const std::string& origin);
  virtual void ForwardError(const std::string& msg, const std::string& origin);
  virtual void ForwardFatal(const std::string& msg, const std::string& origin);

  void HandleEvent(eEvent type, bool active, const SEScalarTime* time = nullptr);

protected:
  void SetupDefaultDataRequests();

private:
  class pimpl;
  pimpl* data;
};


/**
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
*/

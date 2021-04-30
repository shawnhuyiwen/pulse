/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#pragma once

#include "CommonDataModel.h"
#include "controller/Engine.h"
#include "utils/ThreadPool.h"
#include <chrono>

/**
 * An instance of Pulse where the interface is define in stl and base data types.
 * This interface is a thunk layer using serialized cdm objects to drive a Pulse engine.
 */
class PULSE_DECL PulseEnginePoolThunk : public LoggerForward, public SEEventHandler
{
public:
  PulseEnginePoolThunk();
  virtual ~PulseEnginePoolThunk();

  bool SerializeToFiles(std::string const& filename);
  std::string SerializeToStrings(SerializationFormat format);

  bool InitializeEngines(std::string const& patient_configuration, std::string const& data_requests, SerializationFormat format);

  std::string GetInitialPatients(SerializationFormat format);
  std::string GetConditions(SerializationFormat format);

  void LogToConsole(bool b);
  void KeepLogMessages(bool keep);// Set this to true if you are going to pull messages from the engines
  void SetLogFilenames(std::string const& logfile);// Set to empty if no log file is wanted
  std::string PullLogMessages(SerializationFormat format);

  void KeepEventChanges(bool keep);
  std::string PullEvents(SerializationFormat format);
  std::string PullActiveEvents(SerializationFormat format);
  std::string GetPatientAssessment(int type, SerializationFormat format);

  bool ProcessActions(std::string const& actions, SerializationFormat format);
  std::string PullActiveActions(SerializationFormat format);

  bool AdvanceTime_s(double time);
  double GetTimeStep(std::string const& unit);

  size_t DataLength() const;
  double* PullDataPtr();
  void PullData(std::map<int,double>& data);

  virtual void ForwardDebug(const std::string& msg, const std::string& origin);
  virtual void ForwardInfo(const std::string& msg, const std::string& origin);
  virtual void ForwardWarning(const std::string& msg, const std::string& origin);
  virtual void ForwardError(const std::string& msg, const std::string& origin);
  virtual void ForwardFatal(const std::string& msg, const std::string& origin);

  void HandleEvent(eEvent type, bool active, const SEScalarTime* time = nullptr);


protected:
  void SetupDefaultDataRequests();

  PhysiologyEngine& GetPhysiologyEngine();
private:
  class pimpl;
  pimpl* data;
};

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

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "CommonDataModel.h"
// CDM Features in use
#include "PhysiologyEngine.h"
#include "engine/SEEventManager.h"
#include "utils/ScopedMutex.h"

/**
 * An instance of an engine where the interface is define in stl and base data types.
 * This interface is a thunk layer using serialized cdm objects to drive a Pulse engine.
 */
class PhysiologyEngineThunk : public LoggerForward, public SEEventHandler
{
public:
  PhysiologyEngineThunk(const std::string& dataDir = "./");
  virtual ~PhysiologyEngineThunk();

  //virtual void SetConfigurationOverride(std::string const& config)=0; // Not Implemented

  virtual bool ExecuteScenario(std::string const& sceExecOpts, SerializationFormat format) = 0;

  bool SerializeFromFile(std::string const& filename, std::string const& data_requests, SerializationFormat data_requests_format);
  bool SerializeToFile(std::string const& filename);

  bool SerializeFromString(std::string const& state, std::string const& data_requests, SerializationFormat format);
  std::string SerializeToString(SerializationFormat format);

  bool InitializeEngine(std::string const& patient_configuration, std::string const& data_requests, SerializationFormat format);

  std::string GetInitialPatient(SerializationFormat format);
  std::string GetConditions(SerializationFormat format);

  void LogToConsole(bool b);
  void KeepLogMessages(bool keep);// Set this to true if you are going to pull messages from the engine
  void SetLogFilename(std::string const& logfile);// Set to empty if no log file is wanted
  std::string PullLogMessages(SerializationFormat format);
  void SetLoggerForward(LoggerForward* lf) { m_ForwardLogs = lf; }

  void KeepEventChanges(bool keep);
  std::string PullEvents(SerializationFormat format);
  std::string PullActiveEvents(SerializationFormat format);
  std::string GetPatientAssessment(int type, SerializationFormat format);
  void SetEventHandler(SEEventHandler* eh) { m_ForwardEvents = eh; }

  bool ProcessActions(std::string const& actions, SerializationFormat format);
  std::string PullActiveActions(SerializationFormat format);

  bool AdvanceTimeStep();
  double GetTimeStep(std::string const& unit);

  size_t DataLength() const;
  double* PullDataPtr();
  void PullData(std::vector<double>& data);

  virtual void ForwardDebug(const std::string& msg, const std::string& origin);
  virtual void ForwardInfo(const std::string& msg, const std::string& origin);
  virtual void ForwardWarning(const std::string& msg, const std::string& origin);
  virtual void ForwardError(const std::string& msg, const std::string& origin);
  virtual void ForwardFatal(const std::string& msg, const std::string& origin);

  virtual void HandleEvent(eEvent type, bool active, const SEScalarTime* time = nullptr);


protected:
  virtual void AllocateEngine() = 0;
  virtual void SetupDefaultDataRequests();

  std::unique_ptr<PhysiologyEngine> m_engine;
  SESubstanceManager* m_subMgr = nullptr;

  bool m_keepLogMsgs = false;
  bool m_keepEventChanges = false;
  LoggerForward* m_ForwardLogs=nullptr;
  SEEventHandler* m_ForwardEvents=nullptr;

  LogMessages m_logMsgs;
  std::string m_dataDir;
  std::vector<const SEEventChange*> m_events;
  std::vector<const SEActiveEvent*> m_activeEvents;
  size_t m_length;
  double* m_requestedData = nullptr;
  std::vector<double> m_requestedValues;
};

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include <iostream>
#include "PulsePhysiologyEngine.h"
#include "controller/Engine.h"
#include "scenario/SEScenarioExec.h"
#include "engine/SEEventHandler.h"

#include <memory>

class PulseEngineC : public LoggerForward, public SEEventHandler
{
public:
  PulseEngineC(const std::string& logFile, const std::string& data_dir);
  ~PulseEngineC();

  void ForwardDebug(const std::string&  msg, const std::string&  origin);
  void ForwardInfo(const std::string&  msg, const std::string&  origin);
  void ForwardWarning(const std::string&  msg, const std::string&  origin);
  void ForwardError(const std::string&  msg, const std::string&  origin);
  void ForwardFatal(const std::string&  msg, const std::string&  origin);
  //void PullData(double time_s);

  void HandlePatientEvent(ePatient_Event type, bool active, const SEScalarTime* time = nullptr);
  void HandleAnesthesiaMachineEvent(eAnesthesiaMachine_Event type, bool active, const SEScalarTime* time = nullptr);

  void SetupDefaultDataRequests();

  std::unique_ptr<PulseEngine> eng;
  double* requestedData = nullptr;
};

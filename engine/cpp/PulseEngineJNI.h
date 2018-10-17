/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include <jni.h>
#include <iostream>
#include "PulsePhysiologyEngine.h"
#include "Controller/Engine.h"
#include "scenario/SEScenarioExec.h"
#include "engine/SEEventHandler.h"
#include "engine/SEAdvanceHandler.h"

#include <memory>

class PulseEngineJNI : public LoggerForward, public SEAdvanceHandler, public SEEventHandler
{
public:
  PulseEngineJNI(const std::string& logFile);
  ~PulseEngineJNI();

  void Reset();

  void ForwardDebug(const std::string&  msg, const std::string&  origin);
  void ForwardInfo(const std::string&  msg, const std::string&  origin);
  void ForwardWarning(const std::string&  msg, const std::string&  origin);
  void ForwardError(const std::string&  msg, const std::string&  origin);
  void ForwardFatal(const std::string&  msg, const std::string&  origin);
  void PushData(double time_s);

  void OnAdvance(double time_s, const PhysiologyEngine& engine);
  void HandlePatientEvent(ePatient_Event type, bool active, const SEScalarTime* time = nullptr);
  void HandleAnesthesiaMachineEvent(eAnesthesiaMachine_Event type, bool active, const SEScalarTime* time = nullptr);

  JNIEnv*    jniEnv;
  jobject    jniObj;
  DataTrack* trk;
  PulseScenarioExec* exec;
  bool       firstUpdate;
  int        update_cnt;
  int        updateFrequency_cnt;
  std::unique_ptr<PulseEngine> eng;
  
  jmethodID jniDebugMethodID;
  jmethodID jniInfoMethodID;
  jmethodID jniWarnMethodID;
  jmethodID jniErrorMethodID;
  jmethodID jniFatalMethodID;
};

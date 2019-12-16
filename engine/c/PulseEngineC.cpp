/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include <cstring>
#include <memory>
#include <iostream>
#include "PulsePhysiologyEngine.h"

#if defined (__clang__)
#define C_EXPORT
#define C_CALL __attribute__((stdcall))
#elif defined(__gnu_linux__)
#define C_EXPORT __attribute__ ((visibility ("default")))
#define C_CALL __attribute__((stdcall))
#else
#define C_EXPORT __declspec(dllexport)
#define C_CALL __stdcall
#endif

// Implement a C Style strdup
// So we can still use std-c11
char* c_strdup(const char* s, size_t slen)
{
  char* result = (char*)malloc(slen + 1);
  if (result == NULL)
  {
    return NULL;
  }
  memcpy(result, s, slen + 1);
  return result;
}

extern "C"
C_EXPORT void C_CALL PulseInitialize()
{
  Logger::Initialize();
}

extern "C"
C_EXPORT void C_CALL PulseDeinitialize()
{
  CUnitConversionEngine::DestroyEngine();
  Logger::Deinitialize();// Free up logger before the DllMain quits so we can stop threads on windows
}

extern "C"
C_EXPORT PulseEngineThunk* C_CALL Allocate(const char* logFile="", bool cout_enabled=true, const char* data_dir=".")
{
  return new PulseEngineThunk(logFile, cout_enabled, data_dir);
}

extern "C"
C_EXPORT void C_CALL Deallocate(PulseEngineThunk* thunk)
{
  SAFE_DELETE(thunk);
}

extern "C"
C_EXPORT bool C_CALL SerializeFromFile(PulseEngineThunk* thunk, const char* filename, const char* data_requests, int format, double sim_time_s)
{
  return thunk->SerializeFromFile(filename, data_requests, (SerializationFormat)format, sim_time_s);
}
extern "C"
C_EXPORT bool C_CALL SerializeToFile(PulseEngineThunk* thunk, const char* filename, int format)
{
  return thunk->SerializeToFile(filename, (SerializationFormat)format);
}

extern "C"
C_EXPORT bool C_CALL SerializeFromString(PulseEngineThunk* thunk, const char* state, const char* data_requests, int format, double sim_time_s)
{
  return thunk->SerializeFromString(state, data_requests, (SerializationFormat)format, sim_time_s);
}
extern "C"
C_EXPORT bool C_CALL SerializeToString(PulseEngineThunk* thunk, int format, char** state_str)
{
  std::string state = thunk->SerializeToString((SerializationFormat)format);
  *state_str = c_strdup(state.c_str(), state.length());
  return true;
}

extern "C"
C_EXPORT bool C_CALL InitializeEngine(PulseEngineThunk* thunk, const char* patient_configuration, const char* data_requests, int format)
{
  return thunk->InitializeEngine(patient_configuration, data_requests, (SerializationFormat)format);
}

extern "C"
C_EXPORT void C_CALL KeepLogMessages(PulseEngineThunk* thunk, bool keep)
{
  thunk->KeepLogMessages(keep);
}

extern "C"
C_EXPORT bool C_CALL PullLogMessages(PulseEngineThunk* thunk, int format, char** str_addr)
{
  std::string log_msgs = thunk->PullLogMessages((SerializationFormat)format);
  *str_addr = c_strdup(log_msgs.c_str(), log_msgs.length());
  return true;
}

extern "C"
C_EXPORT void C_CALL KeepEventChanges(PulseEngineThunk* thunk, bool keep)
{
  thunk->KeepEventChanges(true);
}

extern "C"
C_EXPORT bool C_CALL PullEvents(PulseEngineThunk* thunk, int format, char** str_addr)
{
  std::string event_changes = thunk->PullEvents((SerializationFormat)format);
  *str_addr = c_strdup(event_changes.c_str(), event_changes.length());
  return true;
}

extern "C"
C_EXPORT bool C_CALL PullActiveEvents(PulseEngineThunk* thunk, int format, char** active)
{
  std::string active_events = thunk->PullActiveEvents((SerializationFormat)format);
  *active = c_strdup(active_events.c_str(), active_events.length());
  return true;
}

extern "C"
C_EXPORT bool C_CALL ProcessActions(PulseEngineThunk* thunk, const char* actions, int format)
{
  return thunk->ProcessActions(actions, (SerializationFormat)format);
}

extern "C"
C_EXPORT bool C_CALL AdvanceTimeStep(PulseEngineThunk* thunk)
{
  return thunk->AdvanceTimeStep();
}

extern "C"
C_EXPORT double* C_CALL PullData(PulseEngineThunk* thunk)
{
  return thunk->PullData();
}


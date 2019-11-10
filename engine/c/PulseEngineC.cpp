/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PulseEngineC.h"
#include "patient/SEPatient.h"
#include "engine/SEAction.h"
#include "engine/SEDataRequest.h"
#include "engine/SEDataRequestManager.h"
#include "engine/SEActionManager.h"
#include "engine/SEConditionManager.h"
#include "engine/SEEngineTracker.h"
#include "engine/SEPatientConfiguration.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalarElectricPotential.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "utils/DataTrack.h"
#include "utils/Logger.h"

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
char* c_strdup(const char* s)
{
  size_t slen = strlen(s);
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
C_EXPORT PulseEngineC* C_CALL Allocate(const char* logFile, bool cout_enabled, const char* data_dir=".")
{
  std::string str(logFile==nullptr?"": logFile);
  PulseEngineC *pulseC = new PulseEngineC(str,data_dir);
  pulseC->eng->GetLogger()->LogToConsole(cout_enabled);
  return pulseC;
}

extern "C"
C_EXPORT void C_CALL Deallocate(PulseEngineC* pulseC)
{
  SAFE_DELETE(pulseC);
}

extern "C"
C_EXPORT bool C_CALL SerializeFromFile(PulseEngineC* pulseC, const char* filename, const char* data_requests, int format, double sim_time_s)
{
  SEScalarTime simTime;
  simTime.SetValue(sim_time_s, TimeUnit::s);
  if (!pulseC->eng->SerializeFromFile(filename, (SerializationFormat)format, &simTime, nullptr))
    return false;
  pulseC->eng->GetEventManager().ForwardEvents(pulseC);

  // Load up the data requests
  if (data_requests != nullptr)
  {
    if (!pulseC->eng->GetEngineTracker()->GetDataRequestManager().SerializeFromString(data_requests, (SerializationFormat)format, pulseC->eng->GetSubstanceManager()))
    {
      pulseC->eng->GetLogger()->Error("Unable to load data requests string");
      return false;
    }
  }
  else
    pulseC->SetupDefaultDataRequests();

  return true;
}

extern "C"
C_EXPORT void C_CALL SerializeToFile(PulseEngineC* pulseC, const char* filename, int format)
{
  pulseC->eng->SerializeToFile(filename, (SerializationFormat)format);
}

extern "C"
C_EXPORT bool C_CALL SerializeFromString(PulseEngineC* pulseC, const char* state, const char* data_requests, int format, double sim_time_s)
{
  SEScalarTime simTime;
  simTime.SetValue(sim_time_s, TimeUnit::s);
  if (!pulseC->eng->SerializeFromString(state, (SerializationFormat)format, &simTime, nullptr))
    return false;
  pulseC->eng->GetEventManager().ForwardEvents(pulseC);

  // Load up the data requests
  if (data_requests != nullptr)
  {
    if (!pulseC->eng->GetEngineTracker()->GetDataRequestManager().SerializeFromString(data_requests, (SerializationFormat)format, pulseC->eng->GetSubstanceManager()))
    {
      pulseC->eng->GetLogger()->Error("Unable to load data requests string");
      return false;
    }
  }
  else
    pulseC->SetupDefaultDataRequests();

  return true;
}

extern "C"
C_EXPORT bool C_CALL SerializeToString(PulseEngineC* pulseC, int format, char** state_str)
{
  std::string state;
  if (!pulseC->eng->SerializeToString(state, (SerializationFormat)format))
    return false;
  *state_str = c_strdup(state.c_str());
  return true;
}

extern "C"
C_EXPORT bool C_CALL InitializeEngine(PulseEngineC* pulseC, const char* patient_configuration, const char* data_requests, int format)
{
  SEPatientConfiguration pc(pulseC->eng->GetSubstanceManager());
  if (!pc.SerializeFromString(patient_configuration, (SerializationFormat)format))
  {
    pulseC->eng->GetLogger()->Error("Unable to load patient configuration string");
    return false;
  }

  // Load up the data requests
  if (data_requests != nullptr)
  {
    if (!pulseC->eng->GetEngineTracker()->GetDataRequestManager().SerializeFromString(data_requests, (SerializationFormat)format, pulseC->eng->GetSubstanceManager()))
    {
      pulseC->eng->GetLogger()->Error("Unable to load data request string");
      return false;
    }
  }
  else
    pulseC->SetupDefaultDataRequests();

  // Ok, crank 'er up!
  if (!pulseC->eng->InitializeEngine(pc))
    return false;
  pulseC->eng->GetEventManager().ForwardEvents(pulseC);
  return true;
}

extern "C"
C_EXPORT void C_CALL KeepLogMessages(PulseEngineC* pulseC, bool keep)
{
  pulseC->keep_log_msgs = keep;
}

extern "C"
C_EXPORT bool C_CALL PullLogMessages(PulseEngineC* pulseC, char** str_addr)
{
  if (pulseC->log_msgs.IsEmpty())
    return false;
  
  std::string log_msgs_str;
  LogMessages::SerializeToString(pulseC->log_msgs, log_msgs_str, SerializationFormat::JSON, pulseC->eng->GetLogger());
  pulseC->log_msgs.Clear();
  *str_addr = c_strdup(log_msgs_str.c_str());
  return true;
}

extern "C"
C_EXPORT void C_CALL KeepEventChanges(PulseEngineC* pulseC, bool keep)
{
  pulseC->keep_event_changes = keep;
}

extern "C"
C_EXPORT bool C_CALL PullEvents(PulseEngineC* pulseC, char** str_addr)
{
  if (pulseC->events.empty())
    return false;

  std::string event_changes;
  SEEventChange::SerializeToString(pulseC->events, event_changes, SerializationFormat::JSON, pulseC->eng->GetLogger());
  DELETE_VECTOR(pulseC->events);
  *str_addr = c_strdup(event_changes.c_str());
  return true;
}

extern "C"
C_EXPORT bool C_CALL PullActiveEvents(PulseEngineC* pulseC, char** active)
{

  if (!pulseC->eng->GetEventManager().GetActiveEvents(pulseC->active_events))
    return false;
  std::string active_events_json;
  SEActiveEvent::SerializeToString(pulseC->active_events, active_events_json, SerializationFormat::JSON, pulseC->eng->GetLogger());
  DELETE_VECTOR(pulseC->active_events);
  *active = c_strdup(active_events_json.c_str());
  return true;
}

extern "C"
C_EXPORT bool C_CALL ProcessActions(PulseEngineC* pulseC, const char* actions, int format)
{
  bool success = true;
  if (actions == nullptr)
    return success;

  try
  {
    std::vector<SEAction*> vActions;
    if (!SEActionManager::SerializeFromString(actions, vActions, (SerializationFormat)format, pulseC->eng->GetSubstanceManager()))
      return false;

    for (const SEAction* a : vActions)
    {
      if (!pulseC->eng->ProcessAction(*a))
        success = false;
      delete a;
    }
  }
  catch (CommonDataModelException& ex)
  {
    success = false;
    pulseC->eng->GetLogger()->Error(ex.what());
  }
  catch (std::exception& ex)
  {
    success = false;
    pulseC->eng->GetLogger()->Error(ex.what());
  }
  catch (...)
  {
    success = false;
  }

  return success;
}

PulseEngineC::PulseEngineC(const std::string& logFile, const std::string& data_dir) : SEEventHandler()
{// No logger needed for the event handler, at this point
  eng = std::unique_ptr<PulseEngine>((PulseEngine*)CreatePulseEngine(logFile,data_dir).release());
  eng->GetLogger()->SetForward(this);
  eng->GetLogger()->LogToConsole(false);
}

extern "C"
C_EXPORT bool C_CALL AdvanceTimeStep(PulseEngineC* pulseC)
{
  bool success = true;
  try
  {
    pulseC->eng->AdvanceModelTime();
  }
  catch (CommonDataModelException& ex)
  {
    pulseC->eng->GetLogger()->Error(ex.what());
    success = false;
  }
  catch (std::exception& ex)
  {
    pulseC->eng->GetLogger()->Error(ex.what());
    success = false;
  }
  catch (...)
  {
    pulseC->eng->GetLogger()->Error("Caught Unknown Exception");
    success = false;
  }
  return success;
}

extern "C"
C_EXPORT double* C_CALL PullData(PulseEngineC* pulseC)
{
  double currentTime_s = pulseC->eng->GetSimulationTime(TimeUnit::s);
  pulseC->eng->GetEngineTracker()->TrackData(currentTime_s);
  if (pulseC->requestedData == nullptr)
  {
    // +1 for the sim time
    pulseC->requestedData = new double[pulseC->eng->GetEngineTracker()->GetDataTrack().GetHeadings().size() + 1];
  }
  // Always put the sim time in index 0 as seconds
  pulseC->requestedData[0] = currentTime_s;
  // Pull all data we requested and pack into our array for return to the caller
  int i = 0;
  for (std::string& heading : pulseC->eng->GetEngineTracker()->GetDataTrack().GetHeadings())
    pulseC->requestedData[++i] = pulseC->eng->GetEngineTracker()->GetDataTrack().GetProbe(heading);

  return pulseC->requestedData;
}

void PulseEngineC::SetupDefaultDataRequests()
{// Default to vitals data
  //eng->GetLogger()->Info("No data requests provided, setting up default data requests");
  eng->GetEngineTracker()->GetDataRequestManager().CreateECGDataRequest("Lead3ElectricPotential", ElectricPotentialUnit::mV);
  eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("ArterialPressure", PressureUnit::mmHg);
  eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
  eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");
  eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("EndTidalCarbonDioxidePressure", PressureUnit::mmHg);
  eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CoreTemperature", TemperatureUnit::C);
  SESubstance* CO2 = eng->GetSubstanceManager().GetSubstance("CarbonDioxide");
  eng->GetEngineTracker()->GetDataRequestManager().CreateGasCompartmentDataRequest("Carina",*CO2,"PartialPressure", PressureUnit::mmHg);
  eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("BloodVolume", VolumeUnit::mL);
}

void PulseEngineC::ForwardDebug(const std::string&  msg, const std::string&  origin)
{
  if(keep_log_msgs)
    log_msgs.debug_msgs.push_back(msg + origin);
}

void PulseEngineC::ForwardInfo(const std::string&  msg, const std::string&  origin)
{
  if (keep_log_msgs)
    log_msgs.info_msgs.push_back(msg + origin);
}

void PulseEngineC::ForwardWarning(const std::string&  msg, const std::string&  origin)
{
  if (keep_log_msgs)
    log_msgs.warning_msgs.push_back(msg + origin);
}

void PulseEngineC::ForwardError(const std::string&  msg, const std::string&  origin)
{
  if (keep_log_msgs)
    log_msgs.error_msgs.push_back(msg + origin);
}

void PulseEngineC::ForwardFatal(const std::string&  msg, const std::string&  origin)
{
  if(keep_log_msgs)
    log_msgs.fatal_msgs.push_back(msg + origin);
}

void PulseEngineC::HandleEvent(eEvent type, bool active, const SEScalarTime* time)
{
  if (keep_event_changes)
    events.push_back(new SEEventChange(type, active, time));
}

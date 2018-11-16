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
#include "log4cplus/config.hxx"

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

extern "C"
C_EXPORT PulseEngineC* C_CALL Allocate(const char* logFile, const char* data_dir=".")
{
  log4cplus::initialize();
  std::string str(logFile);
  PulseEngineC *pulseC = new PulseEngineC(str,data_dir);
  return pulseC;
}

extern "C"
C_EXPORT void C_CALL Deallocate(PulseEngineC* pulseC)
{
  SAFE_DELETE(pulseC);
  log4cplus::deinitialize();
}

extern "C"
C_EXPORT bool C_CALL SerializeFromFile(PulseEngineC* pulseC, const char* filename, const char* data_requests, int format, double sim_time_s)
{
  SEScalarTime simTime;
  simTime.SetValue(sim_time_s, TimeUnit::s);
  if (!pulseC->eng->SerializeFromFile(filename, (SerializationFormat)format, &simTime, nullptr))
    return false;
  pulseC->eng->SetEventHandler(pulseC);

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
  pulseC->eng->SetEventHandler(pulseC);

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
C_EXPORT const char* C_CALL SerializeToString(PulseEngineC* pulseC, int format)
{
  std::string state;
  pulseC->eng->SerializeToString(state, (SerializationFormat)format);
  return state.c_str();
}

extern "C"
C_EXPORT bool C_CALL InitializeEngine(PulseEngineC* pulseC, const char* patient_configuration, const char* data_requests, int format)
{
  SEPatientConfiguration pc(pulseC->eng->GetLogger());
  if (!pc.SerializeFromString(patient_configuration, (SerializationFormat)format, pulseC->eng->GetSubstanceManager()))
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
  pulseC->eng->SetEventHandler(pulseC);
  return true;
}

extern "C"
C_EXPORT bool C_CALL AdvanceTime_s(PulseEngineC* pulseC, double time_s)
{
  bool success = true;
  try
  {
    pulseC->eng->AdvanceModelTime(time_s, TimeUnit::s);
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

PulseEngineC::~PulseEngineC()
{
  
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
  eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SkinTemperature", TemperatureUnit::C);
  SESubstance* CO2 = eng->GetSubstanceManager().GetSubstance("CarbonDioxide");
  eng->GetEngineTracker()->GetDataRequestManager().CreateGasCompartmentDataRequest("Carina",*CO2,"PartialPressure", PressureUnit::mmHg);
  eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("BloodVolume", VolumeUnit::mL);
}

void PulseEngineC::ForwardDebug(const std::string&  msg, const std::string&  origin)
{
  
}

void PulseEngineC::ForwardInfo(const std::string&  msg, const std::string&  origin)
{
  std::cout << msg << std::endl;
}

void PulseEngineC::ForwardWarning(const std::string&  msg, const std::string&  origin)
{
  
}

void PulseEngineC::ForwardError(const std::string&  msg, const std::string&  origin)
{
  
}

void PulseEngineC::ForwardFatal(const std::string&  msg, const std::string&  origin)
{
  
}

void PulseEngineC::HandlePatientEvent(ePatient_Event type, bool active, const SEScalarTime* time)
{
  
}
void PulseEngineC::HandleAnesthesiaMachineEvent(eAnesthesiaMachine_Event type, bool active, const SEScalarTime* time)
{
  
}

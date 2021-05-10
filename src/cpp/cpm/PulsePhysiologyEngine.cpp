/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "PulsePhysiologyEngine.h"
#include "PulseScenarioExec.h"
#include "controller/Engine.h"

#include "patient/SEPatient.h"
#include "patient/assessments/SECompleteBloodCount.h"
#include "patient/assessments/SEComprehensiveMetabolicPanel.h"
#include "patient/assessments/SEPulmonaryFunctionTest.h"
#include "patient/assessments/SEUrinalysis.h"
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

class PulseEngineThunk::pimpl
{
public:
  std::unique_ptr<PulseEngine> eng;
  SESubstanceManager*          subMgr=nullptr;

  bool keep_log_msgs = false;
  bool keep_event_changes = false;

  LogMessages log_msgs;
  std::string data_dir;
  std::vector<const SEEventChange*> events;
  std::vector<const SEActiveEvent*> active_events;
  size_t length;
  std::vector<double> requestedValues;
  double* requestedData = nullptr;
};

PulseEngineThunk::PulseEngineThunk(const std::string& dataDir) : SEEventHandler()
{
  data = new PulseEngineThunk::pimpl();
  data->eng = std::unique_ptr<PulseEngine>((PulseEngine*)CreatePulseEngine().release());
  data->eng->GetLogger()->LogToConsole(false);
  data->eng->GetLogger()->AddForward(this);
  data->data_dir = dataDir;
}
PulseEngineThunk::~PulseEngineThunk()
{
  delete[] data->requestedData;
  delete data;
}

PhysiologyEngine& PulseEngineThunk::GetPhysiologyEngine()
{
  return *data->eng;
}

bool PulseEngineThunk::ExecuteScenario(std::string const& execOpts, SerializationFormat format)
{
  SEScenarioExec opts;
  if (!opts.SerializeFromString(execOpts, format))
    return false;
  return PulseScenarioExec::Execute(*data->eng, opts);
}

bool PulseEngineThunk::SerializeFromFile(std::string const& filename, std::string const& data_requests, SerializationFormat data_requests_format)
{
  if (!data->eng->SerializeFromFile(filename))
    return false;
  data->eng->GetEventManager().ForwardEvents(this);

  // Load up the data requests
  if (!data_requests.empty())
  {
    if (!data->eng->GetEngineTracker()->GetDataRequestManager().SerializeFromString(data_requests, data_requests_format, data->eng->GetSubstanceManager()))
    {
      data->eng->GetLogger()->Error("Unable to load data requests string");
      return false;
    }
  }
  else
  {
    SetupDefaultDataRequests();
    data->eng->GetLogger()->Info("No data requested, will return default vitals");
  }

  return true;
}

bool PulseEngineThunk::SerializeToFile(std::string const& filename)
{
  return data->eng->SerializeToFile(filename);
}


bool PulseEngineThunk::SerializeFromString(std::string const& state, std::string const& data_requests, SerializationFormat format)
{
  if (!data->eng->SerializeFromString(state, format))
    return false;
  data->eng->GetEventManager().ForwardEvents(this);

  // Load up the data requests
  if (!data_requests.empty())
  {
    if (!data->eng->GetEngineTracker()->GetDataRequestManager().SerializeFromString(data_requests, format, data->eng->GetSubstanceManager()))
    {
      data->eng->GetLogger()->Error("Unable to load data requests string");
      return false;
    }
  }
  else
    SetupDefaultDataRequests();

  return true;
}


std::string PulseEngineThunk::SerializeToString(SerializationFormat format)
{
  std::string state;
  if (!data->eng->SerializeToString(state, format))
    state.clear();
  return state;
}

bool PulseEngineThunk::InitializeEngine(std::string const& patient_configuration, std::string const& data_requests, SerializationFormat format)
{
  const SESubstanceManager* subMgr;
  if (data->eng->GetSubstanceManager().GetSubstances().empty())
  {
    if (data->subMgr == nullptr)
    {
      // We need to use our subMgr here
      data->subMgr = new SESubstanceManager(data->eng->GetLogger());
      data->subMgr->LoadSubstanceDirectory(data->data_dir);
    }
    subMgr = data->subMgr;
  }
  else
    subMgr = &data->eng->GetSubstanceManager();

  SEPatientConfiguration pc(data->eng->GetLogger());
  if (!pc.SerializeFromString(patient_configuration, format, *subMgr))
  {
    data->eng->GetLogger()->Error("Unable to load patient configuration string");
    return false;
  }

  // Load up the data requests
  if (!data_requests.empty())
  {
    if (!data->eng->GetEngineTracker()->GetDataRequestManager().SerializeFromString(data_requests, format, *subMgr))
    {
      data->eng->GetLogger()->Error("Unable to load data request string");
      return false;
    }
  }
  else
    SetupDefaultDataRequests();

  // Ok, crank 'er up!
  if (!data->eng->InitializeEngine(pc))
    return false;
  data->eng->GetEventManager().ForwardEvents(this);
  return true;
}

std::string PulseEngineThunk::GetInitialPatient(SerializationFormat format)
{
  std::string stream;
  data->eng->GetInitialPatient().SerializeToString(stream, format);
  return stream;
}

std::string PulseEngineThunk::GetConditions(SerializationFormat format)
{
  std::string stream;
  data->eng->GetConditionManager().SerializeToString(stream, format);
  return stream;
}

void PulseEngineThunk::LogToConsole(bool b)
{
  data->eng->GetLogger()->LogToConsole(b);
}
void PulseEngineThunk::KeepLogMessages(bool keep)
{
  data->keep_log_msgs = keep;
}
void PulseEngineThunk::SetLogFilename(std::string const& logfile)
{
  data->eng->GetLogger()->SetLogFile(logfile);
}
std::string PulseEngineThunk::PullLogMessages(SerializationFormat format)
{
  std::string log_msgs;
  if (data->log_msgs.IsEmpty())
    return log_msgs;

  LogMessages::SerializeToString(data->log_msgs, log_msgs, format, data->eng->GetLogger());
  data->log_msgs.Clear();
  return log_msgs;
}

void PulseEngineThunk::KeepEventChanges(bool keep)
{
  data->keep_event_changes = keep;
}
std::string PulseEngineThunk::PullEvents(SerializationFormat format)
{
  std::string events;
  if (data->events.empty())
    return events;
  SEEventChange::SerializeToString(data->events, events, format, data->eng->GetLogger());
  DELETE_VECTOR(data->events);
  return events;
}
std::string PulseEngineThunk::PullActiveEvents(SerializationFormat format)
{
  std::string active_events;
  if (!data->eng->GetEventManager().GetActiveEvents(data->active_events))
    active_events.clear();
  SEActiveEvent::SerializeToString(data->active_events, active_events, SerializationFormat::JSON, data->eng->GetLogger());
  DELETE_VECTOR(data->active_events);
  return active_events;
}

std::string PulseEngineThunk::GetPatientAssessment(int type, SerializationFormat format)
{
  std::string stream;
  switch (type)
  {
  case 0: // CBC
  {
    SECompleteBloodCount cbc(data->eng->GetLogger());
    data->eng->GetPatientAssessment(cbc);
    cbc.SerializeToString(stream, format);
    break;
  }
  case 1: // CMP
  {
    SEComprehensiveMetabolicPanel cmp(data->eng->GetLogger());
    data->eng->GetPatientAssessment(cmp);
    cmp.SerializeToString(stream, format);
    break;
  }
  case 2:// PFT
  {
    SEPulmonaryFunctionTest pft(data->eng->GetLogger());
    data->eng->GetPatientAssessment(pft);
    pft.SerializeToString(stream, format);
    break;
  }
  case 3: // U
  {
    SEUrinalysis u(data->eng->GetLogger());
    data->eng->GetPatientAssessment(u);
    u.SerializeToString(stream, format);
    break;
  }
  default:
    stream = "Unsupported assessment type";
  };

  return stream;
}

bool PulseEngineThunk::ProcessActions(std::string const& actions, SerializationFormat format)
{
  bool success = true;
  if (actions.empty())
    return success;

  try
  {
    std::vector<SEAction*> vActions;
    if (!SEActionManager::SerializeFromString(actions, vActions, format, data->eng->GetSubstanceManager()))
      return false;

    for (const SEAction* a : vActions)
    {
      if (!data->eng->ProcessAction(*a))
        success = false;
      delete a;
    }
  }
  catch (CommonDataModelException& ex)
  {
    success = false;
    data->eng->GetLogger()->Error(ex.what());
  }
  catch (std::exception& ex)
  {
    success = false;
    data->eng->GetLogger()->Error(ex.what());
  }
  catch (...)
  {
    success = false;
  }

  return success;
}
std::string PulseEngineThunk::PullActiveActions(SerializationFormat format)
{
  std::string stream;
  data->eng->GetActionManager().SerializeToString(stream, format);
  return stream;
}

double PulseEngineThunk::GetTimeStep(std::string const& unit)
{
  TimeUnit time_unit = TimeUnit::GetCompoundUnit(unit);
  return data->eng->GetTimeStep(time_unit);
}
bool PulseEngineThunk::AdvanceTimeStep()
{
  bool success = true;
  try
  {
    success = data->eng->AdvanceModelTime();
  }
  catch (CommonDataModelException& ex)
  {
    data->eng->GetLogger()->Error(ex.what());
    success = false;
  }
  catch (std::exception& ex)
  {
    data->eng->GetLogger()->Error(ex.what());
    success = false;
  }
  catch (...)
  {
    data->eng->GetLogger()->Error("Caught Unknown Exception");
    success = false;
  }
  return success;
}

double* PulseEngineThunk::PullDataPtr()
{
  double currentTime_s = data->eng->GetSimulationTime(TimeUnit::s);
  data->eng->GetEngineTracker()->TrackData(currentTime_s);
  if (data->requestedData == nullptr)
  {
    // +1 for the sim time
    data->length = data->eng->GetEngineTracker()->GetDataTrack().NumTracks() + 1;
    data->requestedData = new double[data->length];
  }
  // Always put the sim time in index 0 as seconds
  data->requestedData[0] = currentTime_s;
  // Pull all data we requested and pack into our array for return to the caller
  for (size_t i = 1; i<data->length; i++)
    data->requestedData[i] = data->eng->GetEngineTracker()->GetDataTrack().GetProbe(i-1);

  return data->requestedData;
}

size_t PulseEngineThunk::DataLength() const
{
  return data->length;
}
void PulseEngineThunk::PullData(std::vector<double>& d)
{
  std::cout << "Trying to pull data" << std::endl;
  double* v = PullDataPtr();
  std::cout << "Pult " << data->length << " data" << std::endl;
  if (d.size() != data->length)
  {
    std::cout << "Reserving result" << std::endl;
    d.reserve(data->length);
    std::cout << "Ok" << std::endl;
  }
  for (int i = 0; i < data->length; i++)
  {

    std::cout << "Beep " << d[i] << std::endl;
    d[i] = v[i];

    std::cout << "Boop" << d[i] << std::endl;
  }

  std::cout << "Added data to the array? " << data->length << " data" << std::endl;
}

void PulseEngineThunk::SetupDefaultDataRequests()
{// Default to vitals data
  //eng->GetLogger()->Info("No data requests provided, setting up default data requests");
  data->eng->GetEngineTracker()->GetDataRequestManager().CreateECGDataRequest("Lead3ElectricPotential", ElectricPotentialUnit::mV);
  data->eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("HeartRate", FrequencyUnit::Per_min);
  data->eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("ArterialPressure", PressureUnit::mmHg);
  data->eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("MeanArterialPressure", PressureUnit::mmHg);
  data->eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("SystolicArterialPressure", PressureUnit::mmHg);
  data->eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("DiastolicArterialPressure", PressureUnit::mmHg);
  data->eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("OxygenSaturation");
  data->eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("EndTidalCarbonDioxidePressure", PressureUnit::mmHg);
  data->eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("RespirationRate", FrequencyUnit::Per_min);
  data->eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("CoreTemperature", TemperatureUnit::C);
  data->eng->GetEngineTracker()->GetDataRequestManager().CreateGasCompartmentDataRequest("Carina", "CarbonDioxide", "PartialPressure", PressureUnit::mmHg);
  data->eng->GetEngineTracker()->GetDataRequestManager().CreatePhysiologyDataRequest("BloodVolume", VolumeUnit::mL);
}

void PulseEngineThunk::ForwardDebug(const std::string& msg, const std::string& origin)
{
  if (data->keep_log_msgs)
    data->log_msgs.debug_msgs.push_back(msg + origin);
}

void PulseEngineThunk::ForwardInfo(const std::string& msg, const std::string& origin)
{
  if (data->keep_log_msgs)
    data->log_msgs.info_msgs.push_back(msg + origin);
}

void PulseEngineThunk::ForwardWarning(const std::string& msg, const std::string& origin)
{
  if (data->keep_log_msgs)
    data->log_msgs.warning_msgs.push_back(msg + origin);
}

void PulseEngineThunk::ForwardError(const std::string& msg, const std::string& origin)
{
  if (data->keep_log_msgs)
    data->log_msgs.error_msgs.push_back(msg + origin);
}

void PulseEngineThunk::ForwardFatal(const std::string& msg, const std::string& origin)
{
  if (data->keep_log_msgs)
    data->log_msgs.fatal_msgs.push_back(msg + origin);
}

void PulseEngineThunk::HandleEvent(eEvent type, bool active, const SEScalarTime* time)
{
  if (data->keep_event_changes)
    data->events.push_back(new SEEventChange(type, active, time));
}
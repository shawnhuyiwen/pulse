 /* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#include "PulseEngine.h"

#include "properties/SEScalar0To1.h"
#include "properties/SEScalarAmountPerVolume.h"
#include "properties/SEScalarElectricPotential.h"
#include "properties/SEScalarForce.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarFlowCompliance.h"
#include "properties/SEScalarFlowResistance.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarMassPerAmount.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressurePerVolume.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"

#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;
using namespace System::Threading;

// Doing the same thing for the logger;
public ref class PulseLoggerRef abstract
{
  delegate void Fatal(const std::string& msg);
  delegate void Error(const std::string& msg);
  delegate void Warn(const std::string& msg);
  delegate void Info(const std::string& msg);
  delegate void Debug(const std::string& msg);

  System::String^ MarshalString(const std::string& s);
  void MarshalFatal(const std::string& msg) { OnFatal(MarshalString(msg)); }
  void MarshalError(const std::string& msg) { OnError(MarshalString(msg)); }
  void MarshalWarn(const std::string& msg) { OnWarn(MarshalString(msg)); }
  void MarshalInfo(const std::string& msg) { OnInfo(MarshalString(msg)); }
  void MarshalDebug(const std::string& msg) { OnDebug(MarshalString(msg)); }
public:
  void SetupCallbacks(PulseEngineRef^ pulse);// A method to setup the function handshake between C# an C++
protected:
  virtual void OnFatal(System::String^ msg) abstract;// CSharp will override this, and it will be called by the Pulse Logger
  virtual void OnError(System::String^ msg) abstract;// CSharp will override this, and it will be called by the Pulse Logger
  virtual void OnWarn(System::String^ msg) abstract;// CSharp will override this, and it will be called by the Pulse Logger
  virtual void OnInfo(System::String^ msg) abstract;// CSharp will override this, and it will be called by the Pulse Logger
  virtual void OnDebug(System::String^ msg) abstract;// CSharp will override this, and it will be called by the Pulse Logger
private:
  Fatal^ on_fatal;
  Error^ on_error;
  Warn^ on_warn;
  Info^ on_info;
  Debug^ on_debug;
};


PulseEngineRef::PulseEngineRef() : _pulse(CreatePulseEngine("default.log").release())
{
  Init();
  LoadStateFile("./states/StandardMale@0s.json", 0);
}
PulseEngineRef::PulseEngineRef(System::String^ log) : _pulse(CreatePulseEngine(MarshalString(log)).release())
{
  Init();
}
PulseEngineRef::~PulseEngineRef()
{
  delete _pulse;
  delete _lock;
  delete _events;
  delete _logger;
  Clear();
}

void PulseEngineRef::Init()
{
  _logger = new PulseLogger();
  _pulse->GetLogger()->SetForward(_logger);

  _events = new PulseEventHandler(_pulse->GetLogger());
  // I want to call my own method when events happen
  using System::IntPtr;
  using System::Runtime::InteropServices::Marshal;

  on_event = gcnew EventDelegate(
    this,
    &PulseEngineRef::HandleEvent
  );
  IntPtr cbPtr = Marshal::GetFunctionPointerForDelegate(on_event);
  _events->SetEventCallback(static_cast<fpEvent>(cbPtr.ToPointer()));
  _pulse->GetEventManager().ForwardEvents(_events);
}

void PulseEngineRef::Clear()
{

}

bool PulseEngineRef::LoadStateFile(System::String^ filename, double sim_start_time_s)
{
  return LoadStateFile(filename, sim_start_time_s, nullptr);
}
bool PulseEngineRef::LoadStateFile(System::String^ filename, double sim_start_time_s, PatientDataRef^ d)
{
  Monitor::Enter(_lock);
  {
    Clear();

    SEScalarTime sim_start_time;
    if (sim_start_time_s > 0)
      sim_start_time.SetValue(sim_start_time_s, TimeUnit::s);
    else
      sim_start_time.SetValue(0, TimeUnit::s);
    if (!_pulse->SerializeFromFile(MarshalString(filename), JSON, &sim_start_time))
      return false;
    _pulse->GetEventManager().ForwardEvents(_events);

    // Get all the systems so we have access to them
    _bldChem = _pulse->GetBloodChemistrySystem();
    _cv = _pulse->GetCardiovascularSystem();
    _drgs = _pulse->GetDrugSystem();
    _endo = _pulse->GetEndocrineSystem();
    _enrgy = _pulse->GetEnergySystem();
    _gi = _pulse->GetGastrointestinalSystem();
    _hep = _pulse->GetHepaticSystem();
    _nrv = _pulse->GetNervousSystem();
    _renl = _pulse->GetRenalSystem();
    _resp = _pulse->GetRespiratorySystem();
    _tiss = _pulse->GetTissueSystem();

    _env = _pulse->GetEnvironment();

    _am = _pulse->GetAnesthesiaMachine();
    _ecg = _pulse->GetElectroCardioGram();
    _inh = _pulse->GetInhaler();

    if (d != nullptr)
    {
      const SEPatient& p = _pulse->GetPatient();
      // Pull any data you extended the Patient Data class to hold
      d->TotalLungCapacity_mL = p.GetTotalLungCapacity(VolumeUnit::mL);
    }

    
    // Grab any substances you will be interested in
    _O2 = _pulse->GetSubstanceManager().GetSubstance("Oxygen");
    _CO2 = _pulse->GetSubstanceManager().GetSubstance("CarbonDioxide");

    // Grab any compartments and compartment substance quantities you added in the header
    _aorta = _pulse->GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::Aorta);
    _aorta_O2 = _aorta->GetSubstanceQuantity(*_O2);
    _aorta_CO2 = _aorta->GetSubstanceQuantity(*_CO2);

    _carina = _pulse->GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::Carina);
    _carina_O2 = _carina->GetSubstanceQuantity(*_O2);
    _carina_CO2 = _carina->GetSubstanceQuantity(*_CO2);
    
    _leftLung = _pulse->GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::LeftLung);
    _rightLung = _pulse->GetCompartments().GetGasCompartment(pulse::PulmonaryCompartment::RightLung);
  }
  Monitor::Exit(_lock);

  return true;
}

void PulseEngineRef::SaveStateFile(System::String^ filename)
{
  Monitor::Enter(_lock);
  {
    _pulse->SerializeToFile(MarshalString(filename),JSON);
  }
  Monitor::Exit(_lock);
}

void PulseEngineRef::AdvanceModelTime_s(double time_s, PulseDataRef^ output)
{
  Monitor::Enter(_lock);
  {
    _pulse->AdvanceModelTime(time_s, TimeUnit::s);
    // Here is where we pull data from Pulse and put it into our VitalsData class for our application to access
    output->HeartRate_per_min = _pulse->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min);
    output->CardiacOutput_mL_Per_s = _pulse->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_s);
    output->ArterialBloodPressure_mmHg = _pulse->GetCardiovascularSystem()->GetArterialPressure(PressureUnit::mmHg);
    output->CentralVenousPressure_mmHg = _pulse->GetCardiovascularSystem()->GetCentralVenousPressure(PressureUnit::mmHg);
    output->DiastolicArterialPressure_mmHg = _pulse->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg);
    output->SystolicArterialPressure_mmHg = _pulse->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg);
    output->MeanArterialPressure_mmHg = _pulse->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg);

    output->EndTidalCarbonDioxidePressure_cmH2O = _pulse->GetRespiratorySystem()->GetEndTidalCarbonDioxidePressure(PressureUnit::cmH2O);
    output->RespiratoryRate_per_min = _pulse->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min);
    output->TidalVolume_mL = _pulse->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL);
    output->TotalLungVolume_mL = _pulse->GetRespiratorySystem()->GetTotalLungVolume(VolumeUnit::mL);
    output->LeftLungVolume_mL = _leftLung->GetVolume(VolumeUnit::mL);
    output->RightLungVolume_mL = _rightLung->GetVolume(VolumeUnit::mL);

    output->SkinTemperature_F = _pulse->GetEnergySystem()->GetSkinTemperature(TemperatureUnit::F);
    output->CoreTemperature_F = _pulse->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::F);

    output->SpO2 = _pulse->GetBloodChemistrySystem()->GetOxygenSaturation();

    output->ECG_Lead3_mV = _pulse->GetElectroCardioGram()->GetLead3ElectricPotential(ElectricPotentialUnit::mV);

    output->CarinaCO2PartialPressure_mmHg = _carina_CO2->GetPartialPressure(PressureUnit::mmHg);
  }
  Monitor::Exit(_lock);
}
double PulseEngineRef::GetSimTime_s()
{
  double t;
  Monitor::Enter(_lock);
  {
    t = _pulse->GetSimulationTime(TimeUnit::s);
  }
  Monitor::Exit(_lock);
  return t;
}

void PulseEngineRef::AcuteStress(double severity)
{
  Monitor::Enter(_lock);
  {
    SEAcuteStress stress;
    stress.GetSeverity().SetValue(severity);
    _pulse->ProcessAction(stress);
  }
  Monitor::Exit(_lock);
}

void PulseEngineRef::AirwayObstruction(double severity)
{
  Monitor::Enter(_lock);
  {
    SEAirwayObstruction obstruction;
    obstruction.GetSeverity().SetValue(severity);
    _pulse->ProcessAction(obstruction);
  }
  Monitor::Exit(_lock);
}

void PulseEngineRef::Apnea(double severity)
{
  Monitor::Enter(_lock);
  {
    SEApnea apnea;
    apnea.GetSeverity().SetValue(severity);
    _pulse->ProcessAction(apnea);
  }
  Monitor::Exit(_lock);
}

void PulseEngineRef::Asthma(double severity)
{
  Monitor::Enter(_lock);
  {
    SEAsthmaAttack asthmaAttack;
    asthmaAttack.GetSeverity().SetValue(severity);
    _pulse->ProcessAction(asthmaAttack);
  }
  Monitor::Exit(_lock);
}

void PulseEngineRef::CardiacArrest()
{
  Monitor::Enter(_lock);
  {
    SECardiacArrest arrest;
    arrest.SetState(eSwitch::On);
    _pulse->ProcessAction(arrest);
  }
  Monitor::Exit(_lock);
}

void PulseEngineRef::ChestCompression(double compressionForce_Newtons)
{
  Monitor::Enter(_lock);
  {
    SEChestCompressionForce chestCompression;
    chestCompression.GetForce().SetValue(compressionForce_Newtons, ForceUnit::N);
    _pulse->ProcessAction(chestCompression);
  }
  Monitor::Exit(_lock);
}

void PulseEngineRef::Hemorrhage(double flowRate_mL_per_min)
{
  Monitor::Enter(_lock);
  {
    SEHemorrhage hemorrhage;
    // Currently, the model is limited to a single location
    hemorrhage.SetCompartment(pulse::VascularCompartment::VenaCava);//the location of the hemorrhage
    hemorrhage.GetRate().SetValue(flowRate_mL_per_min, VolumePerTimeUnit::mL_Per_min);//the rate of hemorrhage
    _pulse->ProcessAction(hemorrhage);
  }
  Monitor::Exit(_lock);
}

void PulseEngineRef::Intubation(eIntubation_Type type)
{
  Monitor::Enter(_lock);
  {
    SEIntubation Intubate;
    Intubate.SetType(type);
    _pulse->ProcessAction(Intubate);
  }
  Monitor::Exit(_lock);
}

void PulseEngineRef::NeedleDecompression(bool active, eSide side)
{
  Monitor::Enter(_lock);
  {
    SENeedleDecompression needleDecomp;
    needleDecomp.SetActive(active);
    needleDecomp.SetSide(side == eSide::Left ? eSide::Left : eSide::Right);
    _pulse->ProcessAction(needleDecomp);
  }
  Monitor::Exit(_lock);
}

void PulseEngineRef::Pneumothorax(eGate type, eSide side, double severity)
{
  Monitor::Enter(_lock);
  {
    SETensionPneumothorax pneumo;
    pneumo.SetType(type == eGate::Open ? eGate::Open : eGate::Closed);
    pneumo.SetSide(side == eSide::Left ? eSide::Left : eSide::Right);
    pneumo.GetSeverity().SetValue(severity);
    _pulse->ProcessAction(pneumo);
  }
  Monitor::Exit(_lock);
}

///////////////////
// Event Handler //
///////////////////

void PulseEventHandler::HandleEvent(eEvent type, bool active, const SEScalarTime* time)
{
  if (_on_event != nullptr)
    _on_event(type, active);// Call the function pointer, from where ever it came and goes
}

void PulseEngineRef::HandleEvent(eEvent type, bool active)
{
  if (_callback != nullptr)
    _callback->HandleEvent(type, active);
}

////////////////////
// Logger Handler //
////////////////////

void PulseLoggerRef::SetupCallbacks(PulseEngineRef^ pulse)
{
  using System::IntPtr;
  using System::Runtime::InteropServices::Marshal;
  IntPtr cbPtr;

  on_fatal = gcnew Fatal(
    this,
    &PulseLoggerRef::MarshalFatal
  );
  cbPtr = Marshal::GetFunctionPointerForDelegate(on_fatal);
  pulse->_logger->SetFatalCallback(static_cast<fpLog>(cbPtr.ToPointer()));

  on_error = gcnew Error(
    this,
    &PulseLoggerRef::MarshalError
  );
  cbPtr = Marshal::GetFunctionPointerForDelegate(on_error);
  pulse->_logger->SetErrorCallback(static_cast<fpLog>(cbPtr.ToPointer()));

  on_warn = gcnew Warn(
    this,
    &PulseLoggerRef::MarshalWarn
  );
  cbPtr = Marshal::GetFunctionPointerForDelegate(on_warn);
  pulse->_logger->SetWarnCallback(static_cast<fpLog>(cbPtr.ToPointer()));

  on_info = gcnew Info(
    this,
    &PulseLoggerRef::MarshalInfo
  );
  cbPtr = Marshal::GetFunctionPointerForDelegate(on_info);
  pulse->_logger->SetInfoCallback(static_cast<fpLog>(cbPtr.ToPointer()));

  on_debug = gcnew Debug(
    this,
    &PulseLoggerRef::MarshalFatal
  );
  cbPtr = Marshal::GetFunctionPointerForDelegate(on_debug);
  pulse->_logger->SetDebugCallback(static_cast<fpLog>(cbPtr.ToPointer()));
}

std::string PulseEngineRef::MarshalString(System::String^ s)
{
  using namespace System::Runtime::InteropServices;
  const char* chars =
    (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
  std::string os = chars;
  Marshal::FreeHGlobal(System::IntPtr((void*)chars));
  return os;
}

System::String^ PulseLoggerRef::MarshalString(const std::string& s)
{
  return gcnew System::String(s.c_str());
}




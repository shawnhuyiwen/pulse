 /* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#include "PulseEngine.h"

#include "PulsePhysiologyEngine.h"
#include "engine/SEEngineConfiguration.h"

#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SEDrugSystem.h"
#include "system/physiology/SEEndocrineSystem.h"
#include "system/physiology/SEEnergySystem.h"
#include "system/physiology/SEGastrointestinalSystem.h"
#include "system/physiology/SEHepaticSystem.h"
#include "system/physiology/SENervousSystem.h"
#include "system/physiology/SERenalSystem.h"
#include "system/physiology/SERespiratorySystem.h"
#include "system/physiology/SETissueSystem.h"

#include "system/environment/SEEnvironment.h"
#include "system/environment/SEEnvironmentalConditions.h"

#include "system/equipment/anesthesiamachine/SEAnesthesiaMachine.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachineChamber.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachineOxygenBottle.h"

#include "system/equipment/ElectroCardioGram/SEElectroCardioGram.h"

#include "system/equipment/Inhaler/SEInhaler.h"

#include "patient/SEPatient.h"
#include "patient/actions/SEAcuteStress.h"
#include "patient/actions/SEAirwayObstruction.h"
#include "patient/actions/SEApnea.h"
#include "patient/actions/SEAsthmaAttack.h"
#include "patient/actions/SEBrainInjury.h"
#include "patient/actions/SEBronchoconstriction.h"
#include "patient/actions/SECardiacArrest.h"
#include "patient/actions/SEChestCompressionForce.h"
#include "patient/actions/SEChestCompressionForceScale.h"
#include "patient/actions/SEChestOcclusiveDressing.h"
#include "patient/actions/SEConsciousRespiration.h"
#include "patient/actions/SEConsumeNutrients.h"
#include "patient/actions/SEExercise.h"
#include "patient/actions/SEHemorrhage.h"
#include "patient/actions/SEIntubation.h"
#include "patient/actions/SEMechanicalVentilation.h"
#include "patient/actions/SENeedleDecompression.h"
#include "patient/actions/SEPericardialEffusion.h"
#include "patient/actions/SESubstanceBolus.h"
#include "patient/actions/SESubstanceCompoundInfusion.h"
#include "patient/actions/SESubstanceInfusion.h"
#include "patient/actions/SETensionPneumothorax.h"
#include "patient/actions/SEUrinate.h"

#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "compartment/SECompartmentManager.h"
#include "compartment/fluid/SEGasCompartment.h"
#include "compartment/fluid/SELiquidCompartment.h"
#include "compartment/substances/SEGasSubstanceQuantity.h"
#include "compartment/substances/SELiquidSubstanceQuantity.h"

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

#include "utils/SEEventHandler.h"

#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;
using namespace System::Threading;

// Create function pointers to call when we get events from pulse
// For this example, I can check activity in my handler and I don't need to propagate time
// It's up to you what data you provide your system, just know you can!
typedef void(__stdcall *fpPatientEvent)(cdm::ePatient_Event type, bool active);

// This is the class to provide Pulse to call when any events are triggered in the engine
class PulseEventHandler : public SEEventHandler
{
public:
  PulseEventHandler(Logger *logger) : SEEventHandler(logger) {}

public:
  virtual void SetPatientEventCallback(fpPatientEvent callback) { _on_patient_event = callback; }// Make Set methods to set up a function pointer to call for each event we are interested in
protected:
  fpPatientEvent _on_patient_event;// Make a member variables for the function pointers we are to call

                                   //These methods need definitions and should call your callback with what ever data you made it require
public:
  virtual void HandlePatientEvent(cdm::ePatient_Event type, bool active, const SEScalarTime* time = nullptr);
  virtual void HandleAnesthesiaMachineEvent(cdm::eAnesthesiaMachine_Event type, bool active, const SEScalarTime* time = nullptr) { /*Not Expecting these */ }
};

typedef void(__stdcall *fpLog)(const std::string&);
class PulseLogger : public LoggerForward
{
public:
  // Make Set methods to set up a function pointer to call for each event we are interested in
  virtual void SetFatalCallback(fpLog callback) { _fatal = callback; }
  virtual void SetErrorCallback(fpLog callback) { _error = callback; }
  virtual void SetWarnCallback(fpLog callback) { _warn = callback; }
  virtual void SetInfoCallback(fpLog callback) { _info = callback; }
  virtual void SetDebugCallback(fpLog callback) { _debug = callback; }
protected:
  // Make a member variables for the function pointers we are to call
  fpLog _fatal;
  fpLog _error;
  fpLog _warn;
  fpLog _info;
  fpLog _debug;

public:// Note for my example, I am ignoring the origin (what class created this log msg)
  virtual void ForwardFatal(const std::string& msg, const std::string& origin) { _fatal(msg); }
  virtual void ForwardError(const std::string& msg, const std::string& origin) { _error(msg); }
  virtual void ForwardWarning(const std::string& msg, const std::string& origin) { _warn(msg); }
  virtual void ForwardInfo(const std::string& msg, const std::string& origin) { _info(msg); }
  virtual void ForwardDebug(const std::string& msg, const std::string& origin) { _debug(msg); }
};

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


PulseEngineRef::PulseEngineRef() : _pulse(CreatePulseEngine("").release())
{
  Init();
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

  on_event = gcnew PatientEventDelegate(
    this,
    &PulseEngineRef::HandlePatientEvent
  );
  IntPtr cbPtr = Marshal::GetFunctionPointerForDelegate(on_event);
  _events->SetPatientEventCallback(static_cast<fpPatientEvent>(cbPtr.ToPointer()));
  _pulse->SetEventHandler(_events);
}

void PulseEngineRef::Clear()
{
  delete _vitalsData;
  _vitalsData = gcnew VitalsData();
}

bool PulseEngineRef::LoadStateFile(System::String^ filename, double sim_start_time_s)
{
  Monitor::Enter(_lock);
  {
    Clear();

    SEScalarTime sim_start_time;
    if (sim_start_time_s > 0)
      sim_start_time.SetValue(sim_start_time_s, TimeUnit::s);
    else
      sim_start_time.SetValue(0, TimeUnit::s);
    if (!_pulse->LoadStateFile(MarshalString(filename), &sim_start_time))
      return false;
    _pulse->SetEventHandler(_events);

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

    const SEPatient& p = _pulse->GetPatient();
    // Pull any data you extended the Patient Data class to hold
    _patientData->TotalLungCapacity_mL = p.GetTotalLungCapacity(VolumeUnit::mL);

    
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
    _pulse->SaveState(MarshalString(filename));
  }
  Monitor::Exit(_lock);
}

void PulseEngineRef::AdvanceModelTime_s(double time_s)
{
  Monitor::Enter(_lock);
  {
    _pulse->AdvanceModelTime(time_s, TimeUnit::s);
    // Here is where we pull data from Pulse and put it into our VitalsData class for our application to access
    _vitalsData->HeartRate_per_min = _pulse->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min);
    _vitalsData->CardiacOutput_mL_Per_s = _pulse->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_s);
    _vitalsData->ArterialBloodPressure_mmHg = _pulse->GetCardiovascularSystem()->GetArterialPressure(PressureUnit::mmHg);
    _vitalsData->CentralVenousPressure_mmHg = _pulse->GetCardiovascularSystem()->GetCentralVenousPressure(PressureUnit::mmHg);
    _vitalsData->DiastolicArterialPressure_mmHg = _pulse->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg);
    _vitalsData->SystolicArterialPressure_mmHg = _pulse->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg);
    _vitalsData->MeanArterialPressure_mmHg = _pulse->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg);

    _vitalsData->EndTidalCarbonDioxidePressure_cmH2O = _pulse->GetRespiratorySystem()->GetEndTidalCarbonDioxidePressure(PressureUnit::cmH2O);
    _vitalsData->RespiratoryRate_per_min = _pulse->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min);
    _vitalsData->TidalVolume_mL = _pulse->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL);
    _vitalsData->TotalLungVolume_mL = _pulse->GetRespiratorySystem()->GetTotalLungVolume(VolumeUnit::mL);
    _vitalsData->LeftLungVolume_mL = _leftLung->GetVolume(VolumeUnit::mL);
    _vitalsData->RightLungVolume_mL = _rightLung->GetVolume(VolumeUnit::mL);

    _vitalsData->SkinTemperature_F = _pulse->GetEnergySystem()->GetSkinTemperature(TemperatureUnit::F); 
    _vitalsData->CoreTemperature_F = _pulse->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::F);

    _vitalsData->SpO2 = _pulse->GetBloodChemistrySystem()->GetOxygenSaturation();

    _vitalsData->ECG_Lead3_mV = _pulse->GetElectroCardioGram()->GetLead3ElectricPotential(ElectricPotentialUnit::mV);
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
    arrest.SetState(cdm::eSwitch::On);
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

void PulseEngineRef::Intubation(/*cdm::*/eIntubationType type)
{
  Monitor::Enter(_lock);
  {
    SEIntubation Intubate;
    cdm::eIntubation_Type t;
    switch (type)
    {
    case eIntubationType::Off: t = cdm::eIntubation_Type_Off; break;
    case eIntubationType::Esophageal: t = cdm::eIntubation_Type_Esophageal; break;
    case eIntubationType::LeftMainstem: t = cdm::eIntubation_Type_LeftMainstem; break;
    case eIntubationType::RightMainstem: t = cdm::eIntubation_Type_RightMainstem; break;
    case eIntubationType::Tracheal: t = cdm::eIntubation_Type_Tracheal; break;
    }
    Intubate.SetType(t);// type);
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
    needleDecomp.SetSide(side == eSide::Left ? cdm::eSide::Left : cdm::eSide::Right);
    _pulse->ProcessAction(needleDecomp);
  }
  Monitor::Exit(_lock);
}

void PulseEngineRef::Pneumothorax(eGate type, eSide side, double severity)
{
  Monitor::Enter(_lock);
  {
    SETensionPneumothorax pneumo;
    pneumo.SetType(type == eGate::Open ? cdm::eGate::Open : cdm::eGate::Closed);
    pneumo.SetSide(side == eSide::Left ? cdm::eSide::Left : cdm::eSide::Right);
    pneumo.GetSeverity().SetValue(severity);
    _pulse->ProcessAction(pneumo);
  }
  Monitor::Exit(_lock);
}

///////////////////
// Event Handler //
///////////////////

void PulseEventHandler::HandlePatientEvent(cdm::ePatient_Event type, bool active, const SEScalarTime* time)
{
  if (_on_patient_event != nullptr)
    _on_patient_event(type, active);// Call the function pointer, from where ever it came and goes
}

void PulseEngineRef::HandlePatientEvent(ePatientEvent type, bool active)
{
  if(_callback != nullptr)
    _callback->HandlePatientEvent(type, active);
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




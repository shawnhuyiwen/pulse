/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "controller/Controller.h"
#include "controller/BlackBoxes.h"
#include "controller/Circuits.h"
#include "controller/Compartments.h"
#include "controller/Substances.h"
#include "io/protobuf/PBPulseState.h"
#include "physiology/BloodChemistry.h"
#include "physiology/Cardiovascular.h"
#include "physiology/Drugs.h"
#include "physiology/Endocrine.h"
#include "physiology/Energy.h"
#include "physiology/Gastrointestinal.h"
#include "physiology/Hepatic.h"
#include "physiology/Nervous.h"
#include "physiology/Renal.h"
#include "physiology/Respiratory.h"
#include "physiology/Saturation.h"
#include "physiology/Tissue.h"
#include "environment/Environment.h"
#include "equipment/AnesthesiaMachine.h"
#include "equipment/ECG.h"
#include "equipment/Inhaler.h"
#include "equipment/MechanicalVentilator.h"
#include "PulseConfiguration.h"

#include "engine/SEPatientConfiguration.h"
#include "engine/SEConditionManager.h"
#include "engine/SEActionManager.h"
#include "engine/SEEngineTracker.h"
#include "engine/SEDataRequestManager.h"
#include "engine/SEAdvanceTime.h"
#include "engine/SESerializeState.h"
#include "engine/SEOverrides.h"
#include "engine/SEEventManager.h"
#include "engine/SEAdvanceHandler.h"
#include "engine/SEEngineStabilization.h"

#include "patient/SEPatient.h"
#include "patient/actions/SEPatientAssessmentRequest.h"
#include "patient/assessments/SEPulmonaryFunctionTest.h"
#include "patient/assessments/SECompleteBloodCount.h"
#include "patient/assessments/SEComprehensiveMetabolicPanel.h"
#include "patient/assessments/SEUrinalysis.h"

#include "utils/FileUtils.h"

class FatalListner : public LoggerForward
{
public:
  FatalListner(SEEventManager& mgr, SEScalarTime& ct) : m_Events (mgr), m_CurrentTime(ct) {};
  ~FatalListner() = default;

  virtual void ForwardDebug(const std::string& msg, const std::string& origin) { }
  virtual void ForwardInfo(const std::string& msg, const std::string& origin) { }
  virtual void ForwardWarning(const std::string& msg, const std::string& origin) { }
  virtual void ForwardError(const std::string& msg, const std::string& origin) { }
  virtual void ForwardFatal(const std::string& msg, const std::string& origin)
  {
    m_Events.SetEvent(eEvent::IrreversibleState, true, m_CurrentTime);
  }

protected:
  SEEventManager& m_Events;
  SEScalarTime& m_CurrentTime;
};


PulseData::PulseData(Logger* logger) : Loggable(logger)
{
  m_State = EngineState::NotReady;
  m_AdvanceHandler = nullptr;

  m_CurrentTime.SetValue(0, TimeUnit::s);
  m_SimulationTime.SetValue(0, TimeUnit::s);
  m_SpareAdvanceTime_s = 0;

  m_Logger->SetLogTime(&m_SimulationTime);
}
PulseData::~PulseData()
{
  SAFE_DELETE(m_EngineTrack);
}

void PulseData::AdvanceCallback(double time_s)
{
  if (m_AdvanceHandler)
  {
    if (time_s >= 0 || m_AdvanceHandler->OnForStabilization())
      m_AdvanceHandler->OnAdvance(time_s);
  }
}

void PulseData::SetAirwayMode(eAirwayMode mode)
{
  if (mode == m_AirwayMode)
    return;// do nazing!
  if (mode != eAirwayMode::Free && m_AirwayMode != eAirwayMode::Free)
    throw CommonDataModelException("Can only change airway mode from the Free mode, Disable other equipment first.");
  if(!IsAirwayModeSupported(mode))
    throw CommonDataModelException("Unsupported airwaymode : "+eAirwayMode_Name(mode));
  m_Compartments->UpdateAirwayGraph();
  m_AirwayMode = mode;
  std::stringstream ss;
  ss << "Airway Mode : " << eAirwayMode_Name(m_AirwayMode);
  Info(ss);
}

void PulseData::SetIntubation(eSwitch s)
{
  if (s == eSwitch::NullSwitch)
    s = eSwitch::Off;
  if (m_Intubation == s)
    return;// do nazing!
  if (m_AirwayMode == eAirwayMode::Inhaler)
    throw CommonDataModelException("Cannot intubate if the inhaler is active.");
  m_Intubation = s;
}

void PulseData::SetupTracker()
{
  m_EngineTrack = new SEEngineTracker(*m_CurrentPatient, *m_Actions, *m_Substances, *m_Compartments, m_Logger);
  if(m_BloodChemistrySystem)
    m_EngineTrack->AddSystem(*m_BloodChemistrySystem);
  if (m_CardiovascularSystem)
  m_EngineTrack->AddSystem(*m_CardiovascularSystem);
  if (m_EndocrineSystem)
  m_EngineTrack->AddSystem(*m_EndocrineSystem);
  if (m_EnergySystem)
  m_EngineTrack->AddSystem(*m_EnergySystem);
  if (m_GastrointestinalSystem)
  m_EngineTrack->AddSystem(*m_GastrointestinalSystem);
  if (m_HepaticSystem)
  m_EngineTrack->AddSystem(*m_HepaticSystem);
  if (m_NervousSystem)
  m_EngineTrack->AddSystem(*m_NervousSystem);
  if (m_RenalSystem)
  m_EngineTrack->AddSystem(*m_RenalSystem);
  if (m_RespiratorySystem)
  m_EngineTrack->AddSystem(*m_RespiratorySystem);
  if (m_DrugSystem)
  m_EngineTrack->AddSystem(*m_DrugSystem);
  if (m_TissueSystem)
  m_EngineTrack->AddSystem(*m_TissueSystem);
  if (m_Environment)
  m_EngineTrack->AddSystem(*m_Environment);
  if (m_AnesthesiaMachine)
  m_EngineTrack->AddSystem(*m_AnesthesiaMachine);
  if (m_ECG)
  m_EngineTrack->AddSystem(*m_ECG);
  if (m_Inhaler)
  m_EngineTrack->AddSystem(*m_Inhaler);
  if (m_MechanicalVentilator)
  m_EngineTrack->AddSystem(*m_MechanicalVentilator);
}

SEEngineTracker& PulseData::GetEngineTracker() const { return *m_EngineTrack; }
DataTrack& PulseData::GetDataTrack() const { return m_EngineTrack->GetDataTrack(); }
SaturationCalculator& PulseData::GetSaturationCalculator() const { return *m_SaturationCalculator; }

PulseSubstances& PulseData::GetSubstances() const { return *m_Substances; }

const SEPatient& PulseData::GetInitialPatient() const { return *m_InitialPatient; }
SEPatient& PulseData::GetCurrentPatient() const { return *m_CurrentPatient; }

bool PulseData::HasBloodChemistry() const { return m_BloodChemistrySystem!=nullptr; }
SEBloodChemistrySystem& PulseData::GetBloodChemistry() const { return *m_BloodChemistrySystem; }
bool PulseData::HasCardiovascular() const { return m_CardiovascularSystem != nullptr; }
SECardiovascularSystem& PulseData::GetCardiovascular() const { return *m_CardiovascularSystem; }
bool PulseData::HasDrugs() const { return m_DrugSystem != nullptr; }
SEDrugSystem& PulseData::GetDrugs() const { return *m_DrugSystem; }
bool PulseData::HasEndocrine() const { return m_EndocrineSystem != nullptr; }
SEEndocrineSystem& PulseData::GetEndocrine() const { return *m_EndocrineSystem; }
bool PulseData::HasEnergy() const { return m_EnergySystem != nullptr; }
SEEnergySystem& PulseData::GetEnergy() const { return *m_EnergySystem; }
bool PulseData::HasGastrointestinal() const { return m_GastrointestinalSystem != nullptr; }
SEGastrointestinalSystem& PulseData::GetGastrointestinal() const { return *m_GastrointestinalSystem; }
bool PulseData::HasHepatic() const { return m_HepaticSystem != nullptr; }
SEHepaticSystem& PulseData::GetHepatic() const { return *m_HepaticSystem; }
bool PulseData::HasNervous() const { return m_NervousSystem != nullptr; }
SENervousSystem& PulseData::GetNervous() const { return *m_NervousSystem; }
bool PulseData::HasRenal() const { return m_RenalSystem != nullptr; }
SERenalSystem& PulseData::GetRenal() const { return *m_RenalSystem; }
bool PulseData::HasRespiratory() const { return m_RespiratorySystem != nullptr; }
SERespiratorySystem& PulseData::GetRespiratory() const { return *m_RespiratorySystem; }
bool PulseData::HasTissue() const { return m_TissueSystem != nullptr; }
SETissueSystem& PulseData::GetTissue() const { return *m_TissueSystem; }

bool PulseData::HasEnvironment() const { return m_Environment != nullptr; }
SEEnvironment& PulseData::GetEnvironment() const { return *m_Environment; }
bool PulseData::HasAnesthesiaMachine() const { return m_AnesthesiaMachine != nullptr; }
SEAnesthesiaMachine& PulseData::GetAnesthesiaMachine() const { return *m_AnesthesiaMachine; }
bool PulseData::HasECG() const { return m_ECG != nullptr; }
SEElectroCardioGram& PulseData::GetECG() const { return *m_ECG; }
bool PulseData::HasInhaler() const { return m_Inhaler != nullptr; }
SEInhaler& PulseData::GetInhaler() const { return *m_Inhaler; }
bool PulseData::HasMechanicalVentilator() const { return m_MechanicalVentilator != nullptr; }
SEMechanicalVentilator& PulseData::GetMechanicalVentilator() const { return *m_MechanicalVentilator; }

SEActionManager& PulseData::GetActions() const { return *m_Actions; }

SEConditionManager& PulseData::GetConditions() const { return *m_Conditions; }

SEEventManager& PulseData::GetEvents() const { return *m_EventManager; }

PulseCircuits& PulseData::GetCircuits() const { return *m_Circuits; }

PulseCompartments& PulseData::GetCompartments() const { return *m_Compartments; }

PulseBlackBoxes& PulseData::GetBlackBoxes() const { return *m_BlackBoxes; }

const PulseConfiguration& PulseData::GetConfiguration() const { return *m_Config; }

const SEScalarTime& PulseData::GetEngineTime() const { return m_CurrentTime; }
const SEScalarTime& PulseData::GetSimulationTime() const { return m_SimulationTime; }
const SEScalarTime& PulseData::GetTimeStep() const { return m_Config->GetTimeStep(); }
double PulseData::GetTimeStep_s() const { return GetTimeStep().GetValue(TimeUnit::s); }

bool PulseData::HasOverride() const { return m_ScalarOverrides.size() > 0; }
const std::vector<SEScalarProperty>& PulseData::GetOverrides() const { return m_ScalarOverrides; }

PulseController::PulseController(Logger* logger) : PulseData(logger)
{
  m_ConfigOverride = nullptr;
  m_Logger->LogToConsole(true);
}
PulseController::~PulseController()
{
  m_Logger->RemoveForward(m_LogForward);
  SAFE_DELETE(m_LogForward);

  SAFE_DELETE(m_Stabilizer);

  SAFE_DELETE(m_Substances);

  SAFE_DELETE(m_InitialPatient);
  SAFE_DELETE(m_CurrentPatient);

  SAFE_DELETE(m_Config);

  SAFE_DELETE(m_SaturationCalculator);

  SAFE_DELETE(m_Actions);
  SAFE_DELETE(m_Conditions);

  SAFE_DELETE(m_BloodChemistrySystem);
  SAFE_DELETE(m_CardiovascularSystem);
  SAFE_DELETE(m_EndocrineSystem);
  SAFE_DELETE(m_EnergySystem);
  SAFE_DELETE(m_GastrointestinalSystem);
  SAFE_DELETE(m_HepaticSystem);
  SAFE_DELETE(m_NervousSystem);
  SAFE_DELETE(m_RenalSystem);
  SAFE_DELETE(m_RespiratorySystem);
  SAFE_DELETE(m_DrugSystem);
  SAFE_DELETE(m_TissueSystem);

  SAFE_DELETE(m_Environment);

  SAFE_DELETE(m_AnesthesiaMachine);
  SAFE_DELETE(m_ECG);
  SAFE_DELETE(m_Inhaler);
  SAFE_DELETE(m_MechanicalVentilator);

  SAFE_DELETE(m_EventManager);

  SAFE_DELETE(m_Compartments);
  SAFE_DELETE(m_BlackBoxes);

  SAFE_DELETE(m_Circuits);
}

void PulseController::Allocate()
{
  m_Stabilizer = new PulseStabilizationController(*this);

  m_Substances = new PulseSubstances(*this);

  m_InitialPatient = new SEPatient(GetLogger());
  m_CurrentPatient = new SEPatient(GetLogger());

  m_Config = new PulseConfiguration(GetLogger());
  m_Config->Initialize("");//Setup defaults that don't need files on disk

  m_SaturationCalculator = new SaturationCalculator(*this);

  m_Actions = new SEActionManager(*m_Substances);
  m_Conditions = new SEConditionManager(GetLogger());

  m_BloodChemistrySystem = new BloodChemistry(*this);
  m_CardiovascularSystem = new Cardiovascular(*this);
  m_EndocrineSystem = new Endocrine(*this);
  m_EnergySystem = new Energy(*this);
  m_GastrointestinalSystem = new Gastrointestinal(*this);
  m_HepaticSystem = new Hepatic(*this);
  m_NervousSystem = new Nervous(*this);
  m_RenalSystem = new Renal(*this);
  m_RespiratorySystem = new Respiratory(*this);
  m_DrugSystem = new Drugs(*this);
  m_TissueSystem = new Tissue(*this);

  m_Environment = new Environment(*this);

  m_AnesthesiaMachine = new AnesthesiaMachine(*this);
  m_ECG = new ECG(*this);
  m_Inhaler = new Inhaler(*this);
  m_MechanicalVentilator = new MechanicalVentilator(*this);

  m_EventManager = new SEEventManager(GetLogger());

  m_Compartments = new PulseCompartments(*this);
  m_BlackBoxes = new PulseBlackBoxes(*this);

  m_Circuits = new PulseCircuits(*this);

  m_LogForward = new FatalListner(*m_EventManager, m_CurrentTime);
  m_Logger->AddForward(m_LogForward);

  SetupTracker();
}

bool PulseController::SetConfigurationOverride(const SEEngineConfiguration* config)
{
  if (config != nullptr)
  {
    m_ConfigOverride = dynamic_cast<const PulseConfiguration*>(config);
    if (m_ConfigOverride == nullptr)
    {
      Error("Configuration provided is not a Pulse Configuration Object");
      return false;
    }
  }
  else
    m_ConfigOverride = nullptr;
  return true;
}

bool PulseController::SerializeFromFile(const std::string& filename)
{
  return PBPulseState::SerializeFromFile(filename, *this, m_ConfigOverride);
}
bool PulseController::SerializeToFile(const std::string& filename) const
{
  return PBPulseState::SerializeToFile(*this, filename);
}

bool PulseController::SerializeFromString(const std::string& src, SerializationFormat m)
{
  return PBPulseState::SerializeFromString(src, *this, m);
}
bool PulseController::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBPulseState::SerializeToString(*this, output, m);
}

bool PulseController::InitializeEngine(const std::string& patient_configuration, SerializationFormat m)
{
  SEPatientConfiguration pc(GetLogger());
  pc.SerializeFromString(patient_configuration, m, *m_Substances);
  return InitializeEngine(pc);
}

bool PulseController::InitializeEngine(const SEPatientConfiguration& patient_configuration)
{
  m_State = EngineState::NotReady;

  m_SpareAdvanceTime_s = 0;
  m_AirwayMode = eAirwayMode::Free;
  m_Intubation = eSwitch::Off;
  m_CurrentTime.SetValue(0, TimeUnit::s);
  m_SimulationTime.SetValue(0, TimeUnit::s);
  m_Logger->SetLogTime(&m_SimulationTime);

  Info("Looking for files in " + patient_configuration.GetDataRoot());
  m_DataDir = patient_configuration.GetDataRoot();

  m_EngineTrack->ResetFile();
  m_Config->Initialize("");
  m_ScalarOverrides.clear();
  m_State = EngineState::Initialization;
  if(patient_configuration.HasOverride())
  {
    for (auto& so : patient_configuration.GetScalarOverrides())
      m_ScalarOverrides.push_back(so);
  }
  if (patient_configuration.HasPatient())
  {
    if (!PulseController::Initialize(*patient_configuration.GetPatient()))
      return false;
  }
  else if (patient_configuration.HasPatientFile())
  {
    SEPatient patient(m_Logger);
    std::string pFile = patient_configuration.GetPatientFile();
    if (!FileExists(pFile))
    {
      if (FileExists(m_DataDir+pFile))
      {
        pFile = m_DataDir + pFile;
      }
      else if (FileExists(m_DataDir+"/patients/"+pFile))
      {
        pFile = m_DataDir+"/patients/"+pFile;
      }
    }
    if (!patient.SerializeFromFile(pFile))// TODO Support all serialization formats
      return false;
    if (!PulseController::Initialize(patient))
      return false;
  }
  else
    return false;

  // We don't capture events during initialization
  SEEventHandler* event_handler = m_EventManager->GetEventHandler();
  m_EventManager->ForwardEvents(nullptr);

  // Stabilize the engine to a resting state (with a standard meal and environment)
  if (!m_Config->HasStabilization())
  {
    Error("Pulse needs stabilization criteria, none provided in configuration file");
    return false;
  }

  m_State = EngineState::InitialStabilization;
  if (!m_Config->GetStabilization()->StabilizeRestingState(*m_Stabilizer))
    return false;

  // Copy any changes to the current patient to the initial patient
  m_InitialPatient->Copy(*m_CurrentPatient);

  // We need to copy conditions here, so systems can prepare for them in their AtSteadyState method
  if (patient_configuration.HasConditions())
    m_Conditions->Copy(*patient_configuration.GetConditions(), *m_Substances);
  AtSteadyState(EngineState::AtInitialStableState);// This will peek at conditions

  m_State = EngineState::SecondaryStabilization;
  // Apply conditions and anything else to the physiology
  // now that it's steady with provided patient, environment, and feedback
  if (!m_Conditions->IsEmpty())
  {// Now restabilize the patient with any conditions that were applied
   // Push conditions into condition manager
    if (!m_Config->GetStabilization()->StabilizeConditions(*m_Stabilizer, *m_Conditions))
      return false;
  }
  AtSteadyState(EngineState::AtSecondaryStableState);
 
  // Don't allow any changes to Quantity/Potential/Flux values directly
  // Use Quantity/Potential/Flux Sources
  m_Circuits->SetReadOnly(true);

  AtSteadyState(EngineState::Active);
  Info("Finalizing homeostasis...");
  // Run this again to clear out any bumps from systems resetting baselines in the last AtSteadyState call
  // AdvanceModelTime(30, TimeUnit::s); // I would rather run Feedback stablization again, but...
  // This does not work for a few patients, they will not stay stable (???)  
  //if (!m_Config->GetStabilizationCriteria()->StabilizeFeedbackState(*this))
  //  return false;

  if (!m_Config->GetStabilization()->IsTrackingStabilization())
    m_SimulationTime.SetValue(0, TimeUnit::s);
  // Hook up the handlers (Note events will still be in the log)
  m_EventManager->ForwardEvents(event_handler);
  return true;
}

bool PulseController::Initialize(SEPatient const& patient)
{
  Info("Configuring patient");
  if (!SetupPatient(patient))
    return false;

  Info("Resetting Substances");
  m_Substances->LoadSubstanceDirectory(m_DataDir);

  // Clear all substances and reload the original data
  // This clears out all engine specific data stored in the substance
  // This will preserve the pointer to the substance, but not any pointers
  // to any substance child objects, those will need to be fixed up, if they exist

  Info("Initializing Configuration");
  m_Config->Initialize(m_DataDir, m_Substances); // Reset to Defaults

  // Now, Let's see if there is anything to merge into our base configuration
  Info("Merging OnDisk Configuration");
  PulseConfiguration cFile(GetLogger());
  cFile.SerializeFromFile("PulseConfiguration.json", *m_Substances);
  m_Config->Merge(cFile, *m_Substances);

  // Now, override anything with a configuration provided by the user or scenario
  if (m_ConfigOverride != nullptr)
  {
    Info("Merging Provided Configuration");
    m_Config->Merge(*m_ConfigOverride, *m_Substances);
  }

  if (!m_Config->IsPDEnabled())
    Info("PD IS DISABLED!!!!");

  // Now we can check the config
  if (m_Config->IsWritingPatientBaselineFile())
  {
    std::string stableDir = m_DataDir+"/stable/";
    MakeDirectory(stableDir.c_str());
    m_CurrentPatient->SerializeToFile(stableDir + m_CurrentPatient->GetName() + ".json");
  }

  m_Actions->Clear();
  m_Conditions->Clear();
  m_EventManager->Clear();

  // This will also Initialize the environment
  // Due to needing the initial environment values for circuits to construct properly
  Info("Creating Circuits and Compartments");
  CreateCircuitsAndCompartments();

  // Appy any prestabilization overrides
  if (m_Config->HasInitialOverrides())
  {
    ProcessAction(m_Config->GetInitialOverrides());
    OverrideCircuits();// Override any circuit values
  }

  Info("Initializing Substances");
  m_Substances->InitializeSubstances();
  if (m_RespiratorySystem && m_Environment)
  {
    m_Substances->InitializeGasCompartments();
  }
  if (m_SaturationCalculator && m_CardiovascularSystem)
  {
    m_SaturationCalculator->Setup();
    m_Substances->InitializeLiquidCompartmentGases();
    m_Substances->InitializeLiquidCompartmentNonGases();
  }
  InitializeSystems();

  AdvanceCallback(-1);
  return true;
}

void PulseController::InitializeSystems()
{
  // Clear everything before initializing
  m_CardiovascularSystem->Clear();
  m_RespiratorySystem->Clear();
  m_AnesthesiaMachine->Clear();
  m_MechanicalVentilator->Clear();
  m_GastrointestinalSystem->Clear();
  m_HepaticSystem->Clear();
  m_RenalSystem->Clear();
  m_NervousSystem->Clear();
  m_EndocrineSystem->Clear();
  m_DrugSystem->Clear();
  m_EnergySystem->Clear();
  m_BloodChemistrySystem->Clear();
  m_TissueSystem->Clear();
  m_ECG->Clear();
  m_Inhaler->Clear();

  Info("Initializing Systems");
  m_CardiovascularSystem->Initialize();
  m_RespiratorySystem->Initialize();
  m_AnesthesiaMachine->Initialize();
  m_MechanicalVentilator->Initialize();
  m_GastrointestinalSystem->Initialize();
  m_HepaticSystem->Initialize();
  m_RenalSystem->Initialize();
  m_NervousSystem->Initialize();
  m_EndocrineSystem->Initialize();
  m_DrugSystem->Initialize();
  m_EnergySystem->Initialize();
  m_BloodChemistrySystem->Initialize();
  m_TissueSystem->Initialize(); // Depends on some parameters that Blood Chemistry initializes,needs to be after
  m_ECG->Initialize();
  m_Inhaler->Initialize();
}

void PulseController::SetSimulationTime(const SEScalarTime& time)
{
  m_SimulationTime.Set(time);
}

bool PulseController::IsReady() const
{
  if (m_State == EngineState::Fatal)
    return false;
  if (m_State == EngineState::NotReady)
  {
    Error("Engine is not ready to process, Initialize the engine or Load a state.");
    return false;
  }
  return true;
}

bool PulseController::AdvanceModelTime()
{
  if (!IsReady())
    return false;

  PreProcess();
  Process();
  PostProcess();

  if (m_EventManager->IsEventActive(eEvent::IrreversibleState))
  {
    m_State = EngineState::Fatal;
    return false;
  }

  m_EventManager->UpdateEvents(m_Config->GetTimeStep());
  m_CurrentTime.Increment(m_Config->GetTimeStep());
  m_SimulationTime.Increment(m_Config->GetTimeStep());

  if (m_AdvanceHandler)
    m_AdvanceHandler->OnAdvance(m_CurrentTime.GetValue(TimeUnit::s));

  // TODO Figure out a way to track what overrides were used and which were not
  m_ScalarOverrides.clear();

  return true;
}

bool PulseController::AdvanceModelTime(double time, const TimeUnit& unit)
{
  if (m_Config->AllowDynamicTimeStep() == eSwitch::On)
  {
    m_Config->GetTimeStep().SetValue(time, unit);
    return AdvanceModelTime();
  }
  else
  {
    double time_s = Convert(time, unit, TimeUnit::s) + m_SpareAdvanceTime_s;
    int count = (int)(time_s / GetTimeStep_s());
    for (int i = 0; i < count; i++)
      if (!AdvanceModelTime())
        return false;
    m_SpareAdvanceTime_s = time_s - (count * GetTimeStep_s());
  }
  return true;
}

void PulseController::AtSteadyState(EngineState state)
{
  m_State = state;
  m_Environment->AtSteadyState();
  m_CardiovascularSystem->AtSteadyState();
  m_Inhaler->AtSteadyState();
  m_RespiratorySystem->AtSteadyState();
  m_AnesthesiaMachine->AtSteadyState();
  m_MechanicalVentilator->AtSteadyState();
  m_GastrointestinalSystem->AtSteadyState();
  m_HepaticSystem->AtSteadyState();
  m_RenalSystem->AtSteadyState();
  m_NervousSystem->AtSteadyState();
  m_EnergySystem->AtSteadyState();
  m_EndocrineSystem->AtSteadyState();
  m_DrugSystem->AtSteadyState();
  m_TissueSystem->AtSteadyState();
  m_BloodChemistrySystem->AtSteadyState();
  m_ECG->AtSteadyState();
}

void PulseController::PreProcess()
{
  m_Environment->PreProcess();
  m_CardiovascularSystem->PreProcess();
  m_Inhaler->PreProcess();
  m_RespiratorySystem->PreProcess();
  m_AnesthesiaMachine->PreProcess();
  m_MechanicalVentilator->PreProcess();
  m_GastrointestinalSystem->PreProcess();
  m_HepaticSystem->PreProcess();
  m_RenalSystem->PreProcess();
  m_NervousSystem->PreProcess();
  m_EnergySystem->PreProcess();
  m_EndocrineSystem->PreProcess();
  m_DrugSystem->PreProcess();
  m_TissueSystem->PreProcess();
  m_BloodChemistrySystem->PreProcess();
  m_ECG->PreProcess();
}
void PulseController::Process()
{
  m_Environment->Process();
  m_CardiovascularSystem->Process();
  m_Inhaler->Process();
  m_RespiratorySystem->Process();
  m_AnesthesiaMachine->Process();
  m_MechanicalVentilator->Process();
  m_GastrointestinalSystem->Process();
  m_HepaticSystem->Process();
  m_RenalSystem->Process();
  m_NervousSystem->Process();
  m_EnergySystem->Process();
  m_EndocrineSystem->Process();
  m_DrugSystem->Process();
  m_TissueSystem->Process();
  m_BloodChemistrySystem->Process();
  m_ECG->Process();
}
void PulseController::PostProcess()
{
  m_Environment->PostProcess();
  m_CardiovascularSystem->PostProcess();
  m_Inhaler->PostProcess();
  m_RespiratorySystem->PostProcess();
  m_AnesthesiaMachine->PostProcess();
  m_MechanicalVentilator->PostProcess();
  m_GastrointestinalSystem->PostProcess();
  m_HepaticSystem->PostProcess();
  m_RenalSystem->PostProcess();
  m_NervousSystem->PostProcess();
  m_EnergySystem->PostProcess();
  m_EndocrineSystem->PostProcess();
  m_DrugSystem->PostProcess();
  m_TissueSystem->PostProcess();
  m_BloodChemistrySystem->PostProcess();
  m_ECG->PostProcess();
}

bool PulseController::ProcessAction(const SEAction& action)
{
  // We can setup overrides before the engine is ready
  const SEOverrides* overrides = dynamic_cast<const SEOverrides*>(&action);
  if (overrides != nullptr)
  {
    for (auto& so : overrides->GetScalarProperties())
    {
      m_ScalarOverrides.push_back(so);
      Info("Overriding " + so.name + " with " + cdm::to_string(so.value) + " " + so.unit);
    }
    return true;
  }

  if (!IsReady())
    return false;
  m_ss << "[Action] " << m_SimulationTime << ", " << action;
  Info(m_ss);

  const SEAdvanceTime* adv = dynamic_cast<const SEAdvanceTime*>(&action);
  if(adv != nullptr)
    return AdvanceModelTime(adv->GetTime(TimeUnit::s), TimeUnit::s);

  const SESerializeState* serialize = dynamic_cast<const SESerializeState*>(&action);
  if (serialize != nullptr)
  {
    if (serialize->GetType() == eSerialization_Type::Save)
    {
      if (serialize->HasFilename())
      {
        SerializeToFile(serialize->GetFilename());
      }
      else
      {
        std::stringstream ss;
        MakeDirectory("./states");
        ss << "./states/" << m_InitialPatient->GetName() << "@" << GetSimulationTime().GetValue(TimeUnit::s) << "s.pbb";
        Info("Saving " + ss.str());
        SerializeToFile(ss.str());
        ss.str(""); ss.clear();
        ss << "./states/" << m_InitialPatient->GetName() << "@" << GetSimulationTime().GetValue(TimeUnit::s) << "s.json";
        Info("Saving " + ss.str());
        SerializeToFile(ss.str());
        // Debug code to make sure things are consistent
        //SerializeFomFile(ss.str());
        //SerializeToFile("./states/AfterSave.json);
      }
    }
    else
      return SerializeFromFile(serialize->GetFilename());
    return true;
  }

  const SEPatientAssessmentRequest* patientAss = dynamic_cast<const SEPatientAssessmentRequest*>(&action);
  if (patientAss != nullptr)
  {
    switch (patientAss->GetType())
    {
    case ePatientAssessment_Type::PulmonaryFunctionTest:
    {
      SEPulmonaryFunctionTest pft(m_Logger);
      GetPatientAssessment(pft);

      // Write out the Assessement
      std::string pftFile = GetEngineTracker().GetDataRequestManager().GetResultFilename();
      if (pftFile.empty())
        pftFile = "PulmonaryFunctionTest";
      m_ss << "PFT@" << GetSimulationTime().GetValue(TimeUnit::s) << "s";
      pftFile = Replace(pftFile, "Results", m_ss.str());
      pftFile = Replace(pftFile, ".csv", ".json");
      m_ss << "PulmonaryFunctionTest@" << GetSimulationTime().GetValue(TimeUnit::s) << "s.json";
      pft.SerializeToFile(pftFile);
      break;
    }
    case ePatientAssessment_Type::Urinalysis:
    {
      SEUrinalysis upan(m_Logger);
      GetPatientAssessment(upan);

      std::string upanFile = GetEngineTracker().GetDataRequestManager().GetResultFilename();
      if (upanFile.empty())
        upanFile = "Urinalysis";
      m_ss << "Urinalysis@" << GetSimulationTime().GetValue(TimeUnit::s) << "s";
      upanFile = Replace(upanFile, "Results", m_ss.str());
      upanFile = Replace(upanFile, ".csv", ".json");
      m_ss << "Urinalysis@" << GetSimulationTime().GetValue(TimeUnit::s) << "s.json";
      upan.SerializeToFile(upanFile);
      break;
    }

    case ePatientAssessment_Type::CompleteBloodCount:
    {
      SECompleteBloodCount cbc(m_Logger);
      GetPatientAssessment(cbc);
      std::string cbcFile = GetEngineTracker().GetDataRequestManager().GetResultFilename();
      if (cbcFile.empty())
        cbcFile = "CompleteBloodCount";
      m_ss << "CBC@" << GetSimulationTime().GetValue(TimeUnit::s) << "s";
      cbcFile = Replace(cbcFile, "Results", m_ss.str());
      cbcFile = Replace(cbcFile, ".csv", ".json");
      m_ss << "CompleteBloodCount@" << GetSimulationTime().GetValue(TimeUnit::s) << "s.json";
      cbc.SerializeToFile(cbcFile);
      break;
    }

    case ePatientAssessment_Type::ComprehensiveMetabolicPanel:
    {
      SEComprehensiveMetabolicPanel mp(m_Logger);
      GetPatientAssessment(mp);
      std::string mpFile = GetEngineTracker().GetDataRequestManager().GetResultFilename();
      if (mpFile.empty())
        mpFile = "ComprehensiveMetabolicPanel";
      m_ss << "CMP@" << GetSimulationTime().GetValue(TimeUnit::s) << "s";
      mpFile = Replace(mpFile, "Results", m_ss.str());
      mpFile = Replace(mpFile, ".csv", ".json");
      m_ss << "ComprehensiveMetabolicPanel@" << GetSimulationTime().GetValue(TimeUnit::s) << "s.json";
      mp.SerializeToFile(mpFile);
      break;
    }
    default:
    {
      m_ss << "Unsupported assessment request " << ePatientAssessment_Type_Name(patientAss->GetType());
      Error(m_ss);
      return false;
    }
    }
    return true;
  }

  return GetActions().ProcessAction(action);
}

bool PulseController::GetPatientAssessment(SEPatientAssessment& assessment) const
{
  if (!IsReady())
    return false;
  SEPulmonaryFunctionTest* pft = dynamic_cast<SEPulmonaryFunctionTest*>(&assessment);
  if (pft != nullptr)
    return m_RespiratorySystem->CalculatePulmonaryFunctionTest(*pft);

  SECompleteBloodCount* cbc = dynamic_cast<SECompleteBloodCount*>(&assessment);
  if (cbc != nullptr)
    return m_BloodChemistrySystem->CalculateCompleteBloodCount(*cbc);

  SEComprehensiveMetabolicPanel* cmp = dynamic_cast<SEComprehensiveMetabolicPanel*>(&assessment);
  if (cmp != nullptr)
    return m_BloodChemistrySystem->CalculateComprehensiveMetabolicPanel(*cmp);

  SEUrinalysis* u = dynamic_cast<SEUrinalysis*>(&assessment);
  if (u != nullptr)
  {
    return m_RenalSystem->CalculateUrinalysis(*u);
  }

  Error("Unsupported patient assessment");
  return false;
}

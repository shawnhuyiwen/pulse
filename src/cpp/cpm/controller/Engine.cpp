/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "controller/Engine.h"
#include "controller/Circuits.h"
#include "controller/Compartments.h"
#include "controller/Substances.h"
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
#include "PulseConfiguration.h"
#include "patient/SEPatient.h"
#include "patient/actions/SEPatientAssessmentRequest.h"
#include "patient/assessments/SEPulmonaryFunctionTest.h"
#include "patient/assessments/SEUrinalysis.h"
#include "patient/assessments/SECompleteBloodCount.h"
#include "patient/assessments/SEComprehensiveMetabolicPanel.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceCompound.h"
#include "circuit/SECircuitManager.h"
#include "compartment/SECompartmentManager.h"
#include "engine/SEEngineStabilization.h"
#include "engine/SEEngineTracker.h"
#include "engine/SEAdvanceHandler.h"
#include "engine/SEDataRequestManager.h"
#include "engine/SEPatientConfiguration.h"
#include "engine/SEAction.h"
#include "engine/SEActionManager.h"
#include "engine/SECondition.h"
#include "engine/SEConditionManager.h"
#include "engine/SESerializeState.h"
#include "engine/SEEventManager.h"
#include "scenario/SEScenario.h"
#include "properties/SEScalarTime.h"
#include "utils/FileUtils.h"
#include "io/protobuf/PBPulseState.h"
#include <memory>

PULSE_DECL std::unique_ptr<PhysiologyEngine> CreatePulseEngine(const std::string& logfile, const std::string& data_dir)
{
  return std::unique_ptr<PulseEngine>(new PulseEngine(logfile, data_dir));
}

PULSE_DECL std::unique_ptr<PhysiologyEngine> CreatePulseEngine(Logger* logger, const std::string& data_dir)
{
  return std::unique_ptr<PulseEngine>(new PulseEngine(logger, data_dir));
}

PulseEngine::PulseEngine(Logger* logger, const std::string& data_dir) : PulseController(logger, data_dir)
{
  m_State = EngineState::NotReady;
  m_EngineTrack = new SEEngineTracker(*this);
  m_DataTrack = &m_EngineTrack->GetDataTrack();
}

PulseEngine::PulseEngine(const std::string& logFileName, const std::string& data_dir) : PulseController(logFileName, data_dir)
{
  m_State = EngineState::NotReady;
  m_EngineTrack = new SEEngineTracker(*this);
  m_DataTrack = &m_EngineTrack->GetDataTrack();
}

PulseEngine::~PulseEngine()
{  
  
}

bool PulseEngine::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBPulseState::SerializeToString(*this, output, m);
}
bool PulseEngine::SerializeToFile(const std::string& filename, SerializationFormat m) const
{
  return PBPulseState::SerializeToFile(*this, filename, m);
}
bool PulseEngine::SerializeFromString(const std::string& src, SerializationFormat m)
{
  return SerializeFromString(src, m, nullptr, nullptr);
}
bool PulseEngine::SerializeFromString(const std::string& src, SerializationFormat m, const SEScalarTime* simTime, const SEEngineConfiguration* config)
{
  return PBPulseState::SerializeFromString(src, *this, m, simTime, config);
}
bool PulseEngine::SerializeFromFile(const std::string& filename, SerializationFormat m)
{
  return SerializeFromFile(filename, m, nullptr, nullptr);
}
bool PulseEngine::SerializeFromFile(const std::string& filename, SerializationFormat m, const SEScalarTime* simTime, const SEEngineConfiguration* config)
{
  return PBPulseState::SerializeFromFile(filename, *this, m, simTime, config);
}

Logger* PulseEngine::GetLogger() const
{
  return Loggable::GetLogger();
}

SEEngineTracker* PulseEngine::GetEngineTracker() const
{
  return m_EngineTrack;
}

bool PulseEngine::InitializeEngine(const std::string& patient_configuration, SerializationFormat m, const SEEngineConfiguration* config)
{
  SEPatientConfiguration pc(*m_Substances);
  pc.SerializeFromString(patient_configuration, m);
  return InitializeEngine(pc, config);
}

bool PulseEngine::InitializeEngine(const SEPatientConfiguration& patient_configuration, const SEEngineConfiguration* config)
{
  const PulseConfiguration* pConfig = nullptr;
  if (config != nullptr)
  {
    pConfig = dynamic_cast<const PulseConfiguration*>(config);
    if (pConfig == nullptr)
    {
      Error("Configuration provided is not a Pulse Configuration Object");
      return false;
    }
  }
  m_EngineTrack->ResetFile();
  m_State = EngineState::Initialization;
  if (patient_configuration.HasPatient())
  {
    if (!PulseController::Initialize(pConfig, *patient_configuration.GetPatient()))
      return false;
  }
  else if (patient_configuration.HasPatientFile())
  {
    SEPatient patient(m_Logger);
    std::string pFile = patient_configuration.GetPatientFile();
    if (pFile.find("/patients") == std::string::npos)
    {// Prepend the patient directory if it's not there
      pFile = "./patients/";
      pFile += patient_configuration.GetPatientFile();
    }
    if (!patient.SerializeFromFile(pFile, JSON))// TODO Support all serialization formats
      return false;
    if (!PulseController::Initialize(pConfig, patient))
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
  if (!m_Config->GetStabilization()->StabilizeRestingState(*this))
    return false;

  // We need to copy conditions here, so systems can prepare for them in their AtSteadyState method
  if(patient_configuration.HasConditions())
    m_Conditions->Copy(*patient_configuration.GetConditions());
  AtSteadyState(EngineState::AtInitialStableState);// This will peek at conditions

  // Copy any changes to the current patient to the initial patient
  m_InitialPatient->Copy(*m_CurrentPatient);

  m_State = EngineState::SecondaryStabilization;
  // Apply conditions and anything else to the physiology
  // now that it's steady with provided patient, environment, and feedback
  if (!m_Conditions->IsEmpty())
  {// Now restabilize the patient with any conditions that were applied
   // Push conditions into condition manager
    if (!m_Config->GetStabilization()->StabilizeConditions(*this, *m_Conditions))
      return false;
  }
  else
  {
    if (!m_Config->GetStabilization()->StabilizeFeedbackState(*this))
      return false;
  }
  AtSteadyState(EngineState::AtSecondaryStableState);  

  m_State = EngineState::Active;
  // Hook up the handlers (Note events will still be in the log)
  m_EventManager->ForwardEvents(event_handler);
  Info("Finalizing homeostasis");

  // Run this again to clear out any bumps from systems resetting baselines in the last AtSteadyState call
  AdvanceModelTime(30, TimeUnit::s); // I would rather run Feedback stablization again, but...
  // This does not work for a few patients, they will not stay stable (???)  
  //if (!m_Config->GetStabilizationCriteria()->StabilizeFeedbackState(*this))
  //  return false;

  if (!m_Config->GetStabilization()->IsTrackingStabilization())
    m_SimulationTime->SetValue(0, TimeUnit::s);
  // Don't allow any changes to Quantity/Potential/Flux values directly
  // Use Quantity/Potential/Flux Sources
  m_Circuits->SetReadOnly(true);

  return true;
}


const SEConditionManager&  PulseEngine::GetConditionManager() const
{
  return *m_Conditions;
}


double PulseEngine::GetTimeStep(const TimeUnit& unit) const
{
  return m_Config->GetTimeStep(unit);
}

double PulseEngine::GetSimulationTime(const TimeUnit& unit) const
{
  return m_SimulationTime->GetValue(unit);
}

void PulseEngine::AdvanceModelTime()
{
  if (!IsReady())
    return;
  if(m_EventManager->IsEventActive(eEvent::IrreversibleState))
    return;

  PreProcess();
  Process();
  PostProcess();

  m_EventManager->UpdateEvents(m_Config->GetTimeStep());
  m_CurrentTime->Increment(m_Config->GetTimeStep());
  m_SimulationTime->Increment(m_Config->GetTimeStep());

  if(m_AdvanceHandler)
    m_AdvanceHandler->OnAdvance(m_CurrentTime->GetValue(TimeUnit::s), *this);
}

void PulseEngine::AdvanceModelTime(double time, const TimeUnit& unit)
{
  double time_s = Convert(time, unit, TimeUnit::s) + m_spareAdvanceTime_s;
  int count = (int)(time_s / GetTimeStep(TimeUnit::s));
  for (int i = 0; i < count; i++)
    AdvanceModelTime();
  m_spareAdvanceTime_s = time_s - (count * GetTimeStep(TimeUnit::s));
}

void PulseEngine::AdvanceCallback(double time_s)
{
  if (m_AdvanceHandler)
  {
    if(time_s >= 0 || m_AdvanceHandler->OnForStabilization())
    m_AdvanceHandler->OnAdvance(time_s, *this);
  }
}

bool PulseEngine::ProcessAction(const SEAction& action)
{  
  if (!IsReady())
    return false;
  m_ss << "[Action] " << *m_SimulationTime << ", " << action;
  Info(m_ss);

  const SESerializeState* serialize = dynamic_cast<const SESerializeState*>(&action);
  if (serialize != nullptr)
  {
    if (serialize->GetType() == eSerialization_Type::Save)
    {
      if (serialize->HasFilename())
      {
        SerializeToFile(serialize->GetFilename(), JSON);
      }
      else
      {
        std::stringstream ss;
        MakeDirectory("./states");
        ss << "./states/" << m_InitialPatient->GetName() << "@" << GetSimulationTime(TimeUnit::s) << "s.json";
        Info("Saving " + ss.str());
        SerializeToFile(ss.str(), JSON);
        // Debug code to make sure things are consistent
        //SerializeFomFile(ss.str(),JSON);
        //SerializeToFile("./states/AfterSave.json",JSON);
      }     
    }
    else
      return SerializeFromFile(serialize->GetFilename(),JSON);
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
        std::string pftFile = GetEngineTracker()->GetDataRequestManager().GetResultFilename();
        if (pftFile.empty())
          pftFile = "PulmonaryFunctionTest";
        m_ss << "PFT@" << GetSimulationTime(TimeUnit::s) << "s";
        pftFile = Replace(pftFile, "Results", m_ss.str());
        pftFile = Replace(pftFile, ".csv", ".json");
        m_ss << "PulmonaryFunctionTest@" << GetSimulationTime(TimeUnit::s) << "s.json";
        pft.SerializeToFile(pftFile,JSON);
        break;
      }
      case ePatientAssessment_Type::Urinalysis:
      {
        SEUrinalysis upan(m_Logger);
        GetPatientAssessment(upan);

        std::string upanFile = GetEngineTracker()->GetDataRequestManager().GetResultFilename();
        if (upanFile.empty())
          upanFile = "Urinalysis";
        m_ss << "Urinalysis@" << GetSimulationTime(TimeUnit::s) << "s";
        upanFile = Replace(upanFile, "Results", m_ss.str());
        upanFile = Replace(upanFile, ".csv", ".json");
        m_ss << "Urinalysis@" << GetSimulationTime(TimeUnit::s) << "s.json";
        upan.SerializeToFile(upanFile,JSON);
        break;
      }

      case ePatientAssessment_Type::CompleteBloodCount:
      {
        SECompleteBloodCount cbc(m_Logger);
        GetPatientAssessment(cbc);
        std::string cbcFile = GetEngineTracker()->GetDataRequestManager().GetResultFilename();
        if (cbcFile.empty())
          cbcFile = "CompleteBloodCount";
        m_ss << "CBC@" << GetSimulationTime(TimeUnit::s) << "s";
        cbcFile = Replace(cbcFile, "Results", m_ss.str());
        cbcFile = Replace(cbcFile, ".csv", ".json");
        m_ss << "CompleteBloodCount@" << GetSimulationTime(TimeUnit::s) << "s.json";
        cbc.SerializeToFile(cbcFile,JSON);
        break;
      }

      case ePatientAssessment_Type::ComprehensiveMetabolicPanel:
      {
        SEComprehensiveMetabolicPanel mp(m_Logger);
        GetPatientAssessment(mp);
        std::string mpFile = GetEngineTracker()->GetDataRequestManager().GetResultFilename();
        if (mpFile.empty())
          mpFile = "ComprehensiveMetabolicPanel";
        m_ss << "CMP@" << GetSimulationTime(TimeUnit::s) << "s";
        mpFile = Replace(mpFile, "Results", m_ss.str());
        mpFile = Replace(mpFile, ".csv", ".json");
        m_ss << "ComprehensiveMetabolicPanel@" << GetSimulationTime(TimeUnit::s) << "s.json";
        mp.SerializeToFile(mpFile,JSON);
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

const SEActionManager&  PulseEngine::GetActionManager() const
{
  return *m_Actions;
}

bool PulseEngine::IsReady() const
{
  if (m_State == EngineState::NotReady)
  {
    Error("Engine is not ready to process, Initialize the engine or Load a state.");
    return false;
  }  
  return true;
}

const SEEventManager& PulseEngine::GetEventManager() const
{
  return *m_EventManager;
}


void PulseEngine::SetAdvanceHandler(SEAdvanceHandler* handler)
{
  m_AdvanceHandler = handler;
}

const SEEngineConfiguration* PulseEngine::GetConfiguration() const
{
  return PulseController::m_Config.get();
}

const SEPatient& PulseEngine::GetPatient() const
{
  return *PulseController::m_CurrentPatient.get();
}

bool PulseEngine::GetPatientAssessment(SEPatientAssessment& assessment) const
{
  if (!IsReady())
    return false;
  return PulseController::GetPatientAssessment(assessment);
}

const SEEnvironment* PulseEngine::GetEnvironment() const
{
  return PulseController::m_Environment.get();
}

SESubstanceManager& PulseEngine::GetSubstanceManager()
{
  return *m_Substances;
}
const SESubstanceManager& PulseEngine::GetSubstanceManager() const
{
  return *m_Substances;
}

const SEBloodChemistrySystem* PulseEngine::GetBloodChemistrySystem() const
{
  return PulseController::m_BloodChemistrySystem.get();
}

const SECardiovascularSystem* PulseEngine::GetCardiovascularSystem() const
{
  return PulseController::m_CardiovascularSystem.get();
}

const SEDrugSystem* PulseEngine::GetDrugSystem() const
{
  return PulseController::m_DrugSystem.get();
}

const SEEndocrineSystem* PulseEngine::GetEndocrineSystem() const
{
  return PulseController::m_EndocrineSystem.get();
}

const SEEnergySystem* PulseEngine::GetEnergySystem() const
{
  return PulseController::m_EnergySystem.get();
}

const SEGastrointestinalSystem* PulseEngine::GetGastrointestinalSystem() const
{
  return PulseController::m_GastrointestinalSystem.get();
}

const SEHepaticSystem* PulseEngine::GetHepaticSystem() const
{
  return PulseController::m_HepaticSystem.get();
}

const SENervousSystem* PulseEngine::GetNervousSystem() const
{
  return PulseController::m_NervousSystem.get();
}

const SERenalSystem* PulseEngine::GetRenalSystem() const
{
  return PulseController::m_RenalSystem.get();
}

const SERespiratorySystem* PulseEngine::GetRespiratorySystem() const
{
  return PulseController::m_RespiratorySystem.get();
}

const SETissueSystem* PulseEngine::GetTissueSystem() const
{
  return PulseController::m_TissueSystem.get();
}


const SEAnesthesiaMachine* PulseEngine::GetAnesthesiaMachine() const
{
  return PulseController::m_AnesthesiaMachine.get();
}

const SEElectroCardioGram* PulseEngine::GetElectroCardioGram() const
{
  return PulseController::m_ECG.get();
}

const SEInhaler* PulseEngine::GetInhaler() const
{
  return PulseController::m_Inhaler.get();
}

const SECompartmentManager& PulseEngine::GetCompartments() const
{
  return *PulseController::m_Compartments.get();
}

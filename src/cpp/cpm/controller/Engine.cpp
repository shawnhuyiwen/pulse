/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "controller/Engine.h"
#include "PulseConfiguration.h"
#include "Substances.h"
#include "Compartments.h"
#include "BlackBoxes.h"

PULSE_DECL std::unique_ptr<PhysiologyEngine> CreatePulseEngine(Logger* logger)
{
  return std::unique_ptr<PulseEngine>(new PulseEngine(logger));
}

PulseEngine::PulseEngine(Logger* logger)
{
  m_PulseController = new PulseController(logger);
  m_PulseController->Allocate();
}

PulseEngine::~PulseEngine()
{
  SAFE_DELETE(m_PulseController);
}

bool PulseEngine::SerializeFromFile(const std::string& filename)
{
  return m_PulseController->SerializeFromFile(filename);
}
bool PulseEngine::SerializeToFile(const std::string& filename) const
{
  return m_PulseController->SerializeToFile(filename);
}

bool PulseEngine::SerializeFromString(const std::string& src, SerializationFormat m)
{
  return m_PulseController->SerializeFromString(src, m);
}
bool PulseEngine::SerializeToString(std::string& output, SerializationFormat m) const
{
  return m_PulseController->SerializeToString(output, m);
}

bool PulseEngine::InitializeEngine(const std::string& patient_configuration, SerializationFormat m)
{
  return m_PulseController->InitializeEngine(patient_configuration, m);
}
bool PulseEngine::InitializeEngine(const SEPatientConfiguration& patient_configuration)
{
  return m_PulseController->InitializeEngine(patient_configuration);
}

bool PulseEngine::SetConfigurationOverride(const SEEngineConfiguration* config)
{
  return m_PulseController->SetConfigurationOverride(config);
}

Logger* PulseEngine::GetLogger() const
{
  return m_PulseController->GetLogger();
}

SEEngineTracker* PulseEngine::GetEngineTracker() const
{
  return &m_PulseController->GetData().GetEngineTracker();
}


const SEConditionManager&  PulseEngine::GetConditionManager() const
{
  return m_PulseController->GetConditions();
}

double PulseEngine::GetTimeStep(const TimeUnit& unit) const
{
  return m_PulseController->GetData().GetTimeStep().GetValue(unit);
}

double PulseEngine::GetSimulationTime(const TimeUnit& unit) const
{
  return  m_PulseController->GetData().GetSimulationTime().GetValue(unit);
}
void PulseEngine::SetSimulationTime(const SEScalarTime& time)
{
  return  m_PulseController->SetSimulationTime(time);
}

bool PulseEngine::AdvanceModelTime()
{
  return m_PulseController->AdvanceModelTime();
}

bool PulseEngine::AdvanceModelTime(double time, const TimeUnit& unit)
{
  return m_PulseController->AdvanceModelTime(time, unit);
}

bool PulseEngine::ProcessAction(const SEAction& action)
{  
  return m_PulseController->ProcessAction(action);
}

const SEActionManager&  PulseEngine::GetActionManager() const
{
  return m_PulseController->GetData().GetActions();
}

const SEEventManager& PulseEngine::GetEventManager() const
{
  return m_PulseController->GetData().GetEvents();
}

void PulseEngine::SetAdvanceHandler(SEAdvanceHandler* handler)
{
  m_PulseController->SetAdvanceHandler(handler);
}

const SEEngineConfiguration* PulseEngine::GetConfiguration() const
{
  return &m_PulseController->GetData().GetConfiguration();
}

const SEPatient& PulseEngine::GetPatient() const
{
  return m_PulseController->GetData().GetCurrentPatient();
}

const SEPatient& PulseEngine::GetInitialPatient() const
{
  return m_PulseController->GetData().GetInitialPatient();
}

bool PulseEngine::GetPatientAssessment(SEPatientAssessment& assessment) const
{
  
  return m_PulseController->GetPatientAssessment(assessment);
}

const SEEnvironment* PulseEngine::GetEnvironment() const
{
  return &m_PulseController->GetData().GetEnvironment();
}

const SESubstanceManager& PulseEngine::GetSubstanceManager() const
{
  return m_PulseController->GetData().GetSubstances();
}

const SEBloodChemistrySystem* PulseEngine::GetBloodChemistrySystem() const
{
  return &m_PulseController->GetData().GetBloodChemistry();
}

const SECardiovascularSystem* PulseEngine::GetCardiovascularSystem() const
{
  return &m_PulseController->GetData().GetCardiovascular();
}

const SEDrugSystem* PulseEngine::GetDrugSystem() const
{
  return &m_PulseController->GetData().GetDrugs();
}

const SEEndocrineSystem* PulseEngine::GetEndocrineSystem() const
{
  return &m_PulseController->GetData().GetEndocrine();
}

const SEEnergySystem* PulseEngine::GetEnergySystem() const
{
  return &m_PulseController->GetData().GetEnergy();
}

const SEGastrointestinalSystem* PulseEngine::GetGastrointestinalSystem() const
{
  return &m_PulseController->GetData().GetGastrointestinal();
}

const SEHepaticSystem* PulseEngine::GetHepaticSystem() const
{
  return &m_PulseController->GetData().GetHepatic();
}

const SENervousSystem* PulseEngine::GetNervousSystem() const
{
  return &m_PulseController->GetData().GetNervous();
}

const SERenalSystem* PulseEngine::GetRenalSystem() const
{
  return &m_PulseController->GetData().GetRenal();
}

const SERespiratorySystem* PulseEngine::GetRespiratorySystem() const
{
  return &m_PulseController->GetData().GetRespiratory();
}

const SETissueSystem* PulseEngine::GetTissueSystem() const
{
  return &m_PulseController->GetData().GetTissue();
}

const SEAnesthesiaMachine* PulseEngine::GetAnesthesiaMachine() const
{
  return &m_PulseController->GetData().GetAnesthesiaMachine();
}

const SEElectroCardioGram* PulseEngine::GetElectroCardioGram() const
{
  return &m_PulseController->GetData().GetECG();
}

const SEInhaler* PulseEngine::GetInhaler() const
{
  return &m_PulseController->GetData().GetInhaler();
}

const SEMechanicalVentilator* PulseEngine::GetMechanicalVentilator() const
{
  return &m_PulseController->GetData().GetMechanicalVentilator();
}

const SECompartmentManager& PulseEngine::GetCompartments() const
{
  return m_PulseController->GetData().GetCompartments();
}

SEBlackBoxManager& PulseEngine::GetBlackBoxes() const
{
  return m_PulseController->GetData().GetBlackBoxes();
}

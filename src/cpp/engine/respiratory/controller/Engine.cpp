/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/respiratory/controller/Engine.h"
#include "PulseConfiguration.h"

#include "cpm/controller/BlackBoxes.h"
#include "cpm/controller/Compartments.h"
#include "cpm/controller/Substances.h"

std::unique_ptr<PhysiologyEngine> CreateRespiratoryEngine(Logger* logger)
{
  return std::unique_ptr<RespiratoryEngine>(new RespiratoryEngine(logger));
}

RespiratoryEngine::RespiratoryEngine(Logger* logger)
{
  m_Controller = new Controller(logger);
  m_Controller->Allocate();
  m_Controller->SetupTracker();
}

RespiratoryEngine::~RespiratoryEngine()
{
  SAFE_DELETE(m_Controller);
}

bool RespiratoryEngine::SerializeFromFile(const std::string& filename)
{
  m_Controller->Warning("Respiratory Engine does not support serialization");
  return false;
}
bool RespiratoryEngine::SerializeToFile(const std::string& filename) const
{
  m_Controller->Warning("Respiratory Engine does not support serialization");
  return false;
}

bool RespiratoryEngine::SerializeFromString(const std::string& src, SerializationFormat m)
{
  m_Controller->Warning("Respiratory Engine does not support serialization");
  return false;
}
bool RespiratoryEngine::SerializeToString(std::string& output, SerializationFormat m) const
{
  m_Controller->Warning("Respiratory Engine does not support serialization");
  return false;
}

bool RespiratoryEngine::InitializeEngine(const std::string& patient_configuration, SerializationFormat m)
{
  return m_Controller->InitializeEngine(patient_configuration, m);
}
bool RespiratoryEngine::InitializeEngine(const SEPatientConfiguration& patient_configuration)
{
  return m_Controller->InitializeEngine(patient_configuration);
}

bool RespiratoryEngine::SetConfigurationOverride(const SEEngineConfiguration* config)
{ // TODO Aaron, this should immediatly merge provided config into data config object
  return m_Controller->SetConfigurationOverride(config);
}

Logger* RespiratoryEngine::GetLogger() const
{
  return m_Controller->GetLogger();
}

SEEngineTracker* RespiratoryEngine::GetEngineTracker() const
{
  return &m_Controller->GetData().GetEngineTracker();
}


const SEConditionManager& RespiratoryEngine::GetConditionManager() const
{
  return m_Controller->GetConditions();
}

double RespiratoryEngine::GetTimeStep(const TimeUnit& unit) const
{
  return m_Controller->GetData().GetTimeStep().GetValue(unit);
}

double RespiratoryEngine::GetSimulationTime(const TimeUnit& unit) const
{
  return  m_Controller->GetData().GetSimulationTime().GetValue(unit);
}
void RespiratoryEngine::SetSimulationTime(const SEScalarTime& time)
{
  return  m_Controller->SetSimulationTime(time);
}

bool RespiratoryEngine::AdvanceModelTime()
{
  return m_Controller->AdvanceModelTime();
}

bool RespiratoryEngine::AdvanceModelTime(double time, const TimeUnit& unit)
{
  return m_Controller->AdvanceModelTime(time, unit);
}

bool RespiratoryEngine::ProcessAction(const SEAction& action)
{  
  return m_Controller->ProcessAction(action);
}

const SEActionManager& RespiratoryEngine::GetActionManager() const
{
  return m_Controller->GetData().GetActions();
}

const SEEventManager& RespiratoryEngine::GetEventManager() const
{
  return m_Controller->GetData().GetEvents();
}

void RespiratoryEngine::SetAdvanceHandler(SEAdvanceHandler* handler)
{
  m_Controller->SetAdvanceHandler(handler);
}

const SEEngineConfiguration* RespiratoryEngine::GetConfiguration() const
{
  return &m_Controller->GetData().GetConfiguration();
}

const SEPatient& RespiratoryEngine::GetPatient() const
{
  return m_Controller->GetData().GetCurrentPatient();
}

const SEPatient& RespiratoryEngine::GetInitialPatient() const
{
  return m_Controller->GetData().GetInitialPatient();
}

bool RespiratoryEngine::GetPatientAssessment(SEPatientAssessment& assessment) const
{
  
  return m_Controller->GetPatientAssessment(assessment);
}

const SEEnvironment* RespiratoryEngine::GetEnvironment() const
{
  return &m_Controller->GetData().GetEnvironment();
}

const SESubstanceManager& RespiratoryEngine::GetSubstanceManager() const
{
  return m_Controller->GetData().GetSubstances();
}

const SEBloodChemistrySystem* RespiratoryEngine::GetBloodChemistrySystem() const
{
  return nullptr;
}

const SECardiovascularSystem* RespiratoryEngine::GetCardiovascularSystem() const
{
  return nullptr;
}

const SEDrugSystem* RespiratoryEngine::GetDrugSystem() const
{
  return nullptr;
}

const SEEndocrineSystem* RespiratoryEngine::GetEndocrineSystem() const
{
  return nullptr;
}

const SEEnergySystem* RespiratoryEngine::GetEnergySystem() const
{
  return nullptr;
}

const SEGastrointestinalSystem* RespiratoryEngine::GetGastrointestinalSystem() const
{
  return nullptr;
}

const SEHepaticSystem* RespiratoryEngine::GetHepaticSystem() const
{
  return nullptr;
}

const SENervousSystem* RespiratoryEngine::GetNervousSystem() const
{
  return nullptr;
}

const SERenalSystem* RespiratoryEngine::GetRenalSystem() const
{
  return nullptr;
}

const SERespiratorySystem* RespiratoryEngine::GetRespiratorySystem() const
{
  return &m_Controller->GetData().GetRespiratory();
}

const SETissueSystem* RespiratoryEngine::GetTissueSystem() const
{
  return nullptr;
}

const SEAnesthesiaMachine* RespiratoryEngine::GetAnesthesiaMachine() const
{
  return nullptr;
}

const SEBagValveMask* RespiratoryEngine::GetBagValveMask() const
{
  return nullptr;
}

const SEElectroCardioGram* RespiratoryEngine::GetElectroCardioGram() const
{
  return nullptr;
}

const SEInhaler* RespiratoryEngine::GetInhaler() const
{
  return nullptr;
}

const SEMechanicalVentilator* RespiratoryEngine::GetMechanicalVentilator() const
{
  return &m_Controller->GetData().GetMechanicalVentilator();
}

const SECompartmentManager& RespiratoryEngine::GetCompartments() const
{
  return m_Controller->GetData().GetCompartments();
}

SEBlackBoxManager& RespiratoryEngine::GetBlackBoxes() const
{
  return m_Controller->GetData().GetBlackBoxes();
}
